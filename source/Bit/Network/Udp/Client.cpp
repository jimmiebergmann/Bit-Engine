#include <Bit/Network/Udp/Client.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Udp
	{

		Client::Client( const Bit::Time & p_InitialPing ) :
			m_Connected( false ),
			m_ServerAddress( 0 ),
			m_ServerPort( 0 ),
			m_ConnectionTimeout( Bit::Seconds( 3.0f ) ),
			m_Sequence( 0 ),
			m_Ping( p_InitialPing )
		{
	
		}

		Client::~Client( )
		{
			// Disconnect the client.
			InternalDisconnect( true, true, true );

			// Remove all the events
			m_Events.Mutex.Lock( );
			while( m_Events.Value.size( ) )
			{
				Event * pEvent = m_Events.Value.front( );
				delete pEvent;
				m_Events.Value.pop( );
			}
			m_Events.Mutex.Unlock( );
		}

		Bit::Bool Client::Open( const Bit::Uint16 p_Port )
		{
			// Open the udp socket.
			m_Socket.SetBlocking( true );

			// Open the socket.
			if( m_Socket.Open( p_Port ) )
			{
				// Set blocking and return true.
				m_Socket.SetBlocking( true );
				return true;
			}

			// Error
			return false;
		}

		Bit::Bool Client::Connect(	const Bit::Address & p_Address, const Bit::Uint16 p_Port,
									const Bit::Time & p_ConnectionTimeout )
		{
			// make sure to be disconnected.
			InternalDisconnect( true, true, true );

			// Create a data buffer.
			const Bit::SizeType bufferSize = 128;
			char buffer[ bufferSize ];

			// Initialize the ping list
			for( Bit::SizeType i = 0; i < 5; i++ )
			{
				m_PingList.push_front( m_Ping.Value );
			}

			// Keep on sending the connect packet until the server answer.
			Bit::Timer timer;
			timer.Start( );
			while( timer.GetLapsedTime( ).AsMicroseconds( ) < p_ConnectionTimeout.AsMicroseconds( ) )
			{
				// Send SYN packet, tell the server that we would like to connect.
				buffer[ 0 ] = ePacketType::Syn;
				if( m_Socket.Send( buffer, 1, p_Address, p_Port ) != 1 )
				{
					// Error sending connection packet.
					continue;
				}

				// Wait for the reply from the server
				Bit::SizeType recvSize = 0;
				Bit::Address recvAddress;
				Bit::Uint16 recvPort = 0;
 
				recvSize = m_Socket.Receive( buffer, bufferSize, recvAddress, recvPort, Bit::Seconds( 1 ) );
				if( recvSize <= 0 )
				{
					// Error receiving the server answer.
					continue;
				}

				// Check if this is a SYN-ACK packet, the server wants us to connect.
				if( buffer[ 0 ] == ePacketType::SynAck )
				{
					// Set the connected flag to true.
					m_Connected.Mutex.Lock( );
					m_Connected.Value = true;
					m_Connected.Mutex.Unlock( );

					// We now connected, we assume that the server receive the UDP_ACCEPT packet,
					// resend the packet in client thread if we get another accept packet later.
					m_ServerAddress = p_Address;
					m_ServerPort = p_Port;

					// Start the client thread.
					m_Thread.Execute( [ this ] ( )
					{
							const Bit::SizeType bufferSize = 128;
							char buffer[ bufferSize ];
							Bit::Address address;
							Bit::Uint16 port = 0;
							Bit::Int16 recvSize = 0;

							// Start the timer for the last recv packet.
							m_LastRecvTimer.Mutex.Lock( );
							m_LastRecvTimer.Value.Start( );
							m_LastRecvTimer.Mutex.Unlock( );

							// Start the timer for the last send packet.
							m_LastSendTimer.Mutex.Lock( );
							m_LastSendTimer.Value.Start( );
							m_LastSendTimer.Mutex.Unlock( );

							// Receive packets as long as we are connected.
							while( IsConnected( ) )
							{
								// Receive any packet.
								recvSize = m_Socket.Receive( buffer, bufferSize, address, port, Bit::Microseconds( 1000 ) );

								// Make sure that the packet is from the server
								if( address != m_ServerAddress || port != m_ServerPort )
								{
									continue;
								}

								// Parse the packet.
								if( recvSize > 0 )
								{
									// Reset the time for checking last time a packet arrived
									m_LastRecvTimer.Mutex.Lock( );
									m_LastRecvTimer.Value.Start( );
									m_LastRecvTimer.Mutex.Unlock( );

									// Check the packet type
									switch( buffer[ 0 ] )
									{
										// The server disconnected.
										case ePacketType::Close:
										{
											// Set connected to false.
											m_Connected.Mutex.Lock( );
											m_Connected.Value = false;
											m_Connected.Mutex.Unlock( );

											// Wait for the threads to finish.
											m_EventThread.Finish( );
											m_ReliableThread.Finish( );

											// Reset the sequence.
											m_Sequence.Mutex.Lock( );
											m_Sequence.Value = 0;
											m_Sequence.Mutex.Unlock( );

											// Add disconnect event
											Event * pEvent = new Event;
											pEvent->Type = eEventType::Disconnect;
											m_Events.Mutex.Lock( );
											m_Events.Value.push( pEvent );
											m_Events.Mutex.Unlock( );
										}
										break;
										// Alive packet from server.
										case ePacketType::Alive:
										{
											// Ignore "corrupt" alive packet.
											if( recvSize != 3 )
											{
												continue;
											}

											Bit::Uint16 sequence = Bit::Ntoh16(	static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 1 ] ) ) |
																				static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 2 ] ) << 8 ) );

											// Use the already allocated packet, change the type
											buffer[ 0 ] = ePacketType::Ack;

											// Send the ack packet
											m_Socket.Send( buffer, 3, m_ServerAddress, m_ServerPort );
										}
										break;
										// ACK packet from server.
										case ePacketType::Ack:
										{
											// Ignore "corrupt" ack packet.
											if( recvSize != 3 )
											{
												continue;
											}

											// Get the sequence
											Bit::Uint16 sequence = Bit::Ntoh16(	static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 1 ] ) ) |
																				static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 2 ] ) << 8 ) );

											// Find the sequence in the reliable map
											m_ReliableMap.Mutex.Lock( );
											ReliablePacketMap::iterator it = m_ReliableMap.Value.find( sequence );
											// Remove the packet from the map if it's found and calculate the ping.
											if( it != m_ReliableMap.Value.end( ) )
											{
												// Calculate the new ping from the lapsed time if it's not a resent packet
												if( it->second->Resent == false )
												{
													CalculateNewPing( it->second->SendTimer.GetLapsedTime( ) );
												}

												// Clean up the data
												delete [ ] it->second->pData;
												delete it->second;

												// Erase the reliable packet
												m_ReliableMap.Value.erase( it );
											}
											m_ReliableMap.Mutex.Unlock( );
										}
										break;
										// The server request another ACK packet,
										// the first one was lost.
										case ePacketType::SynAck:
										{
											buffer[ 0 ] = ePacketType::Ack;
											if( m_Socket.Send( buffer, 1, address, port ) != 1 )
											{
												break;
											}
										}
										break;
										// Received unreliable packet.
										case ePacketType::UnreliablePacket:
										{
											if( recvSize <= HeaderSize + 1 )
											{
												continue;
											}

											// Add the unreliable packet to the received data queue.
											m_ReceivedData.Mutex.Lock( );
									
											ReceivedData * pReceivedData = new ReceivedData;
											pReceivedData->DataSize = recvSize - HeaderSize;
											pReceivedData->pData = new char[ pReceivedData->DataSize ];
											memcpy( pReceivedData->pData, buffer + HeaderSize, pReceivedData->DataSize );

											// Get the sequence
											pReceivedData->Sequence = Bit::Ntoh16(	static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 1 ] ) ) |
																					static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 2 ] ) << 8 ) );
								
											// Push the packet
											m_ReceivedData.Value.push( pReceivedData );

											m_ReceivedData.Mutex.Unlock( );

											// Add the event
											Event * pEvent = new Event;
											pEvent->Type = eEventType::Receive;
											m_Events.Mutex.Lock( );
											m_Events.Value.push( pEvent );
											m_Events.Mutex.Unlock( );
										}
										break;
										case ePacketType::ReliablePacket:
										{
											// Use the already allocated packet, change the type
											buffer[ 0 ] = ePacketType::Ack;

											// Send the ack packet
											m_Socket.Send( buffer, 3, m_ServerAddress, m_ServerPort );

											// Get the sequence.
											Bit::Uint16 sequence = Bit::Ntoh16(	static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 1 ] ) ) |
																				static_cast<Bit::Uint16>( static_cast<Bit::Uint8>( buffer[ 2 ] ) << 8 ) );

											// Add the unreliable packet to the received data queue if it's not a resent packet.
											if( m_AcknowledgementData.AddAcknowledgement( sequence ) )
											{
												ReceivedData * pReceivedData = new ReceivedData;
												pReceivedData->Sequence = sequence;
												pReceivedData->DataSize = recvSize - HeaderSize;
												pReceivedData->pData = new char[ pReceivedData->DataSize ];
												memcpy( pReceivedData->pData, buffer + HeaderSize, pReceivedData->DataSize );

												// Push the packet
												m_ReceivedData.Mutex.Lock( );
												m_ReceivedData.Value.push( pReceivedData );
												m_ReceivedData.Mutex.Unlock( );

												// Add the event
												Event * pEvent = new Event;
												pEvent->Type = eEventType::Receive;
												m_Events.Mutex.Lock( );
												m_Events.Value.push( pEvent );
												m_Events.Mutex.Unlock( );
											}
										}
										break;
										default:
											break;
									}
								}

							}
						}

					);

					// Execute the event thread
					m_EventThread.Execute( [ this ] ( )
						{
							while( IsConnected( ) )
							{
								// Sleep for some time.
								Bit::Sleep( Bit::Milliseconds( 10 ) );

								// Disconnect you've not heard anything from the server in a while.
								if( TimeSinceLastRecvPacket( ) >= m_ConnectionTimeout.Value )
								{
									InternalDisconnect( true, false, true );
									return;
								}

								// Get the lapsed time since the last packet was sent.
								m_LastSendTimer.Mutex.Lock( );
								Bit::Time lapsedTime =  m_LastSendTimer.Value.GetLapsedTime( );
								m_LastSendTimer.Mutex.Unlock( );

								// Send alive packet if requred
								if( lapsedTime >= Bit::Seconds( 0.5f ) )
								{
									InternalSendReliable( ePacketType::Alive, NULL, 0 );
								}

							}
						}
					);

					// Execute the reliable thread
					m_ReliableThread.Execute( [ this ] ( )
						{
							ReliablePacket * pPacket = NULL;

							while( IsConnected( ) )
							{
								// Sleep for some time.
								Bit::Sleep( Bit::Milliseconds( 1 ) );

								// Check if we should resend any packet.
								m_ReliableMap.Mutex.Lock( );
						
								for(	ReliablePacketMap::iterator it = m_ReliableMap.Value.begin( );
										it != m_ReliableMap.Value.end( );
										it++ ) 
								{
									pPacket = it->second;

									// Resend packet after ping * 3.0 seconds.
									// Calculte the resend time.
									m_Ping.Mutex.Lock( );
									Bit::Time resendTime = Bit::Microseconds( static_cast<Bit::Uint64>(static_cast<Bit::Float64>( m_Ping.Value.AsMicroseconds( ) ) * 3.0f ));
									m_Ping.Mutex.Unlock( );

									if( pPacket->ResendTimer.GetLapsedTime( ) >= resendTime )
									{
										// Send packet.
										m_Socket.Send( pPacket->pData, pPacket->DataSize, m_ServerAddress, m_ServerPort );

										// Restart the resend timer
										pPacket->ResendTimer.Start( );

										// Mark the packet as resent.
										pPacket->Resent = true;

										// Increase the ping if we lose packets
										m_Ping.Mutex.Lock( );
										m_Ping.Value = Bit::Microseconds( static_cast<Bit::Uint64>(static_cast<Bit::Float64>( m_Ping.Value.AsMicroseconds( ) ) * 1.2f ));
										m_Ping.Mutex.Unlock( );
									}
								}
								m_ReliableMap.Mutex.Unlock( );

							}
						}
					);
					
			
					return true;
				}
				// The server answered with a disconnect packet, we are denied. 
				else if( buffer[ 0 ] == ePacketType::Close )
				{
					// The server denied us.
					return false;
				}
				// Unknown packet from the server.
				else
				{
					// Error in the server packet, try again.
					continue;
				}
			}

			// We timed out.
			return false;
		}

		void Client::Disconnect( )
		{
			InternalDisconnect( true, true, true );
		}

		Bit::Bool Client::IsConnected( )
		{
			m_Connected.Mutex.Lock( );
			Bit::Bool connected = m_Connected.Value;
			m_Connected.Mutex.Unlock( );
			return connected;
		}

		Bit::Time Client::GetPing( )
		{
			m_Ping.Mutex.Lock( );
			Bit::Time time = m_Ping.Value;
			m_Ping.Mutex.Unlock( );
			return time;
		}

		void Client::SendUnreliable( void * p_pData, const Bit::SizeType p_DataSize )
		{
			// Create the packet.
			const Bit::SizeType packetSize = p_DataSize + HeaderSize;
			char * pData = new char[ packetSize ];
			pData[ 0 ] = static_cast<char>( ePacketType::UnreliablePacket );

			// Add the sequence to the data buffer.
			m_Sequence.Mutex.Lock( );
			Bit::Uint16 sequence = Bit::Hton16( m_Sequence.Value );
			memcpy( pData + 1, &sequence, 2 );
	
			// Increment the sequence
			m_Sequence.Value++;
			m_Sequence.Mutex.Unlock( );

			// Add the data to the new buffer
			memcpy( pData + HeaderSize, p_pData, p_DataSize );

			// Send the packet.
			m_Socket.Send( pData, packetSize, m_ServerAddress, m_ServerPort );

			// Delete the packet
			delete [ ] pData;
		}

		void Client::SendReliable( void * p_pData, const Bit::SizeType p_DataSize )
		{
			InternalSendReliable( ePacketType::ReliablePacket, p_pData, p_DataSize );
		}

		Bit::Bool Client::Receive( Packet & p_Packet )
		{
			// Check if there's data to read, lock the mutex.
			m_ReceivedData.Mutex.Lock( );
			if( m_ReceivedData.Value.size( ) == 0 )
			{
				// Unlock the mutex and return false.
				m_ReceivedData.Mutex.Unlock( );
				return false;
			}

			// Get the received data
			ReceivedData * pReceivedData = m_ReceivedData.Value.front( );

			// Unlock the mutex
			m_ReceivedData.Mutex.Unlock( );

			/// Calculate the data size
			Bit::SizeType dataSize = pReceivedData->DataSize;

			// Set the parameter
			p_Packet.pData = pReceivedData->pData;
			p_Packet.DataSize = pReceivedData->DataSize;
			p_Packet.Sequence = pReceivedData->Sequence;

			// Remove the received data from the queue.
			delete pReceivedData;

			// Pop the data in mutex lock.
			m_ReceivedData.Mutex.Lock( );
			m_ReceivedData.Value.pop( );
			m_ReceivedData.Mutex.Unlock( );

			// Return true.
			return true;
		}

		Bit::Bool Client::PollEvent( Event & p_Event )
		{
			// Lock the event mutex
			m_Events.Mutex.Lock( );

			// Error check the event queue size.
			if( m_Events.Value.size( ) == 0 )
			{
				// Unlock mutex and return error.
				m_Events.Mutex.Unlock( );
				return false;
			}

			// Set the paramter
			Event * pEvent = m_Events.Value.front( );
			p_Event.Type = pEvent->Type;

			// Delete the event
			delete pEvent;

			// Pop the event
			m_Events.Value.pop( );

			// Unlock mutex and return true.
			m_Events.Mutex.Unlock( );
			return true;
		}

		Bit::Time Client::TimeSinceLastRecvPacket( )
		{
			m_LastRecvTimer.Mutex.Lock( );
			Bit::Time time = m_LastRecvTimer.Value.GetLapsedTime( );
			m_LastRecvTimer.Mutex.Unlock( );
			return time;
		}

		// Acknowledgement data struct
		Client::AcknowledgementData::AcknowledgementData( )
		{
			memset( m_SequenceBits, 0, m_SequenceArraySize * 4 );
			m_CurrentBlocks[ 0 ] = 0;
			m_CurrentBlocks[ 1 ] = 1;
		}

		Bit::Bool Client::AcknowledgementData::AddAcknowledgement( const Bit::Uint16 p_Sequence )
		{
			const static Bit::Uint32 blockSize = 16384;
			const static Bit::Uint32 blockCount = 4;

			// Find the right array and bit index.
			Bit::Uint32 arrayIndex  = static_cast<Bit::Uint32>(	p_Sequence ) / 32;
			Bit::Uint32 bitIndex	= static_cast<Bit::Uint32>( p_Sequence ) % 32;

			// Check if the bit already is set
			if( ( m_SequenceBits[ arrayIndex ] >> bitIndex ) & 0x01 )
			{
				return false;
			}

			// Check the block bounding, if we are going to change blocks
			Bit::Uint8 sequenceBlock = static_cast<Bit::Uint8>( p_Sequence / blockSize );
			if( sequenceBlock != m_CurrentBlocks[ 0 ] &&
				sequenceBlock != m_CurrentBlocks[ 1 ] &&
				sequenceBlock != ( ( m_CurrentBlocks[ 1 ] + 2 ) % 4 ) )
			{
				// Clear the last block.
				Bit::Uint32 * pClearSequence = m_SequenceBits + ( 512 * m_CurrentBlocks[ 0 ]);
				memset( pClearSequence, 0, m_SequenceArraySize );

				// Move the block index forward.
				m_CurrentBlocks[ 0 ] = m_CurrentBlocks[ 1 ];
				m_CurrentBlocks[ 1 ] = ( m_CurrentBlocks[ 1 ] + 1 ) % 4;
			}

			// Set the current bit to 1.
			Bit::Uint32 bitMask = 1 << bitIndex;
			m_SequenceBits[ arrayIndex ] = m_SequenceBits[ arrayIndex ] | bitMask;
			return true;
		}

		void Client::InternalDisconnect(	const Bit::Bool p_CloseMainThread,
											const Bit::Bool p_CloseEventThread,
											const Bit::Bool p_CloseReliableThread )
		{
			// Get status and set connected to false.
			m_Connected.Mutex.Lock( );
			Bit::Bool connected = m_Connected.Value;
			m_Connected.Value = false;
			m_Connected.Mutex.Unlock( );

			if( connected )
			{
				// Send connection packet.
				char buffer = ePacketType::Close;
				m_Socket.Send( &buffer, 1, m_ServerAddress, m_ServerPort );

				// Wait for the threads to finish.
				if( p_CloseMainThread )
				{
 					m_Thread.Finish( );
				}
				if( p_CloseEventThread )
				{
					m_EventThread.Finish( );
				}
				if( p_CloseReliableThread )
				{
					m_ReliableThread.Finish( );
				}

				// Reset the sequence.
				m_Sequence.Mutex.Lock( );
				m_Sequence.Value = 0;
				m_Sequence.Mutex.Unlock( );

				// Clear the reliable packets
				m_ReliableMap.Mutex.Lock( );
				for(	ReliablePacketMap::iterator it = m_ReliableMap.Value.begin( );
						it != m_ReliableMap.Value.end( );
						it ++ )
				{
					delete [ ] it->second->pData;
					delete it->second;
				}
				m_ReliableMap.Value.clear( );
				m_ReliableMap.Mutex.Unlock( );

				// Add disconnect event
				Event * pEvent = new Event;
				pEvent->Type = eEventType::Disconnect;
				m_Events.Mutex.Lock( );
				m_Events.Value.push( pEvent );
				m_Events.Mutex.Unlock( );
			}

			// Clear the ping list.
			m_PingList.clear( );
		}

		void Client::InternalSendReliable( const ePacketType & p_PacketType, void * p_pData, const Bit::SizeType p_DataSize )
		{
			// Create a new buffer.
			const Bit::SizeType packetSize = p_DataSize + HeaderSize;
			char * pData = new char[ packetSize ];
			pData[ 0 ] = p_PacketType;

			// Add the sequence to the data buffer.
			m_Sequence.Mutex.Lock( );
			Bit::Uint16 currentSequence = m_Sequence.Value;
			m_Sequence.Mutex.Unlock( );
							
			// Add the sequence to the data buffer.
			Bit::Uint16 sequence = Bit::Hton16( currentSequence );
			memcpy( pData + 1, &sequence, 2 );
	
			// Increment the sequence
			m_Sequence.Mutex.Lock( );
			m_Sequence.Value++;
			m_Sequence.Mutex.Unlock( );

			// Add the data to the new 
			if( p_DataSize )
			{
				memcpy( pData + HeaderSize, p_pData, p_DataSize );
			}

			// Create the reliable packet.
			ReliablePacket * pReliablePacket = new ReliablePacket;
			pReliablePacket->pData = pData;
			pReliablePacket->DataSize = packetSize;
			pReliablePacket->Sequence = currentSequence;
			pReliablePacket->Resent = false;
			pReliablePacket->SendTimer.Start( );
			pReliablePacket->ResendTimer.Start( );

			// Add the packet to the reliable map.
			m_ReliableMap.Mutex.Lock( );
			m_ReliableMap.Value.insert( ReliablePacketPair( currentSequence, pReliablePacket ) );
			m_ReliableMap.Mutex.Unlock( );

			// Send SYN packet, tell the server that we would like to connect.
			if( m_Socket.Send( pData, packetSize, m_ServerAddress, m_ServerPort ) == packetSize )
			{
				// Restart the timer
				m_LastSendTimer.Mutex.Lock( );
				m_LastSendTimer.Value.Start( );
				m_LastSendTimer.Mutex.Unlock( );
			}
		}

		void Client::CalculateNewPing( const Bit::Time & p_LapsedTime )
		{
			// Add the new ping to the ping list
			m_PingList.pop_front( );
			m_PingList.push_back( p_LapsedTime / 2ULL );

			// Calculate the new ping
			Bit::Uint64 newPing = 0ULL;
			for( TimeList::iterator it = m_PingList.begin( ); it != m_PingList.end( ); it++ )
			{
				newPing += it->AsMicroseconds( );
			}
			newPing /= static_cast<Bit::Uint64>( m_PingList.size( ) );
										
			m_Ping.Mutex.Lock( );
			m_Ping.Value = Bit::Microseconds( newPing );
			m_Ping.Mutex.Unlock( ); 
		}

	}

}