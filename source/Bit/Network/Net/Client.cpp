#include <Bit/Network/Net/Client.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Client::Client( const Uint16 p_Port,
						const Time & p_InitialPing ) :
			m_EntityManager( NULL ),
			m_Port( p_Port ),
			m_Connected( false ),
			m_ServerAddress( 0 ),
			m_ServerPort( 0 ),
			m_ConnectionTimeout( Seconds( 3.0f ) ),
			m_Sequence( 0 ),
			m_Ping( p_InitialPing )
		{
	
		}

		Client::~Client( )
		{
			// Disconnect the client.
			InternalDisconnect( true, true, true, true );

			// Clear the user message listeners
			m_UserMessageListeners.Mutex.Lock( );
			for( UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.begin( );
				 it != m_UserMessageListeners.Value.end( );
				 it++ )
			{
				delete it->second;
			}
			m_UserMessageListeners.Value.clear( );
			m_UserMessageListeners.Mutex.Unlock( );
		}


		Bool Client::Connect(	const Address & p_Address, const Uint16 p_Port,
								const Time & p_ConnectionTimeout )
		{
			// make sure to be disconnected.
			InternalDisconnect( true, true, true, true );

			// Open the udp socket.
			m_Socket.SetBlocking( true );

			// Open the socket.
			if( m_Socket.Open( m_Port ) == false )
			{
				return false;
			}

			// Set blocking and return true.
			m_Socket.SetBlocking( true );

			// Create a data buffer.
			const SizeType bufferSize = 128;
			Uint8 buffer[ bufferSize ];

			// Initialize the ping list
			for( SizeType i = 0; i < 5; i++ )
			{
				m_PingList.push_front( m_Ping.Value );
			}

			// Keep on sending the connect packet until the server answer.
			Timer timer;
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
				SizeType recvSize = 0;
				Address recvAddress;
				Uint16 recvPort = 0;
 
				recvSize = m_Socket.Receive( buffer, bufferSize, recvAddress, recvPort, Seconds( 1 ) );
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
							const SizeType bufferSize = 128;
							Uint8 buffer[ bufferSize ];
							Address address;
							Uint16 port = 0;
							Int16 recvSize = 0;

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
								recvSize = m_Socket.Receive( buffer, bufferSize, address, port, Microseconds( 1000 ) );

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

											Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( buffer[ 1 ] ) ) |
																		static_cast<Uint16>( static_cast<Uint8>( buffer[ 2 ] ) << 8 ) );

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
											Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( buffer[ 1 ] ) ) |
																		static_cast<Uint16>( static_cast<Uint8>( buffer[ 2 ] ) << 8 ) );

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
											if( recvSize <= HeaderSize + 2 )
											{
												continue;
											}
									
											ReceivedData * pReceivedData = new ReceivedData;
											pReceivedData->DataSize = recvSize - HeaderSize;
											pReceivedData->pData = new Uint8[ pReceivedData->DataSize ];
											memcpy( pReceivedData->pData, buffer + HeaderSize, pReceivedData->DataSize );

											// Get the sequence
											pReceivedData->Sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( buffer[ 1 ] ) ) |
																				static_cast<Uint16>( static_cast<Uint8>( buffer[ 2 ] ) << 8 ) );
								
											if( pReceivedData->pData[ 0 ] == static_cast<Uint8>( eMessageType::UserMessageType ) )
											{
												AddUserMessage( pReceivedData );
											}
										}
										break;
										case ePacketType::ReliablePacket:
										{
											// Use the already allocated packet, change the type
											buffer[ 0 ] = ePacketType::Ack;

											// Send the ack packet
											m_Socket.Send( buffer, 3, m_ServerAddress, m_ServerPort );

											// Get the sequence.
											Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( buffer[ 1 ] ) ) |
																		static_cast<Uint16>( static_cast<Uint8>( buffer[ 2 ] ) << 8 ) );

											// Add the unreliable packet to the received data queue if it's not a resent packet.
											if( m_AcknowledgementData.AddAcknowledgement( sequence ) )
											{
												ReceivedData * pReceivedData = new ReceivedData;
												pReceivedData->Sequence = sequence;
												pReceivedData->DataSize = recvSize - HeaderSize;
												pReceivedData->pData = new Uint8[ pReceivedData->DataSize ];
												memcpy( pReceivedData->pData, buffer + HeaderSize, pReceivedData->DataSize );

												if( pReceivedData->pData[ 0 ] == static_cast<Uint8>( eMessageType::UserMessageType ) )
												{
													AddUserMessage( pReceivedData );
												}

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
								Sleep( Milliseconds( 10 ) );

								// Disconnect you've not heard anything from the server in a while.
								if( TimeSinceLastRecvPacket( ) >= m_ConnectionTimeout.Value )
								{
									InternalDisconnect( true, false, true, true );
									return;
								}

								// Get the lapsed time since the last packet was sent.
								m_LastSendTimer.Mutex.Lock( );
								Time lapsedTime =  m_LastSendTimer.Value.GetLapsedTime( );
								m_LastSendTimer.Mutex.Unlock( );

								// Send alive packet if requred
								if( lapsedTime >= Seconds( 0.5f ) )
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
								Sleep( Milliseconds( 1 ) );

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
									Time resendTime = Microseconds( static_cast<Uint64>(static_cast<Float64>( m_Ping.Value.AsMicroseconds( ) ) * 3.0f ));
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
										m_Ping.Value = Microseconds( static_cast<Uint64>(static_cast<Float64>( m_Ping.Value.AsMicroseconds( ) ) * 1.2f ));
										m_Ping.Mutex.Unlock( );
									}
								}
								m_ReliableMap.Mutex.Unlock( );

							}
						}
					);

					// Execute user message
					m_UserMessageThread.Execute( [ this ] ( )
					{
						while( IsConnected( ) )
						{
							// Wait for the semaphore to release
							m_UserMessageSemaphore.Wait( );

							// Go throguh the user messages.
							m_UserMessages.Mutex.Lock( );

							while( m_UserMessages.Value.size( ) )
							{
								// Ge the received data
								ReceivedData * pReceivedData = m_UserMessages.Value.front( );

								// Pop the message
								m_UserMessages.Value.pop( );

								// Get the message name
								SizeType nameEnd = 1;
								for( SizeType i = 1; i < pReceivedData->DataSize; i++ )
								{
									if( pReceivedData->pData[ i ] == 0 )
									{
										nameEnd = i;
										break;
									}
								}

								// Error check the name length
								if( nameEnd == 1 )
								{
									// Delete the received data pointer
									delete pReceivedData;
									continue;
								}

								// Copy the name.
								std::string name;
								name.assign( reinterpret_cast<char*>(pReceivedData->pData + 1), nameEnd - 1 );

								// Check if there is any message left
								if( name.size( ) + 2 >= pReceivedData->DataSize )
								{
									// Delete the received data pointer
									delete pReceivedData;
									continue;
								}

								// Find the listeners for this user message
								m_UserMessageListeners.Mutex.Lock( );
								UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.find( name );
								if( it == m_UserMessageListeners.Value.end( ) )
								{
									// Delete the received data pointer
									delete pReceivedData;
									continue;
								}

								UserMessageListenerSet * pMessageSet = it->second;

								// Go through the listeners and call the listener function
								for( UserMessageListenerSet::iterator it2 = pMessageSet->begin( ); it2 != pMessageSet->end( ); it2++ )
								{
									// Get the listener.
									UserMessageListener * pListener = *it2;

									// Create a message decoder
									Uint8 * pDataPointer =  pReceivedData->pData + name.size( ) + 2;
									const SizeType dataSize = pReceivedData->DataSize - name.size( ) - 2;
									UserMessageDecoder messageDecoder( name, pDataPointer, dataSize ) ;

									// Use threads????
									// Handle the message.
									pListener->HandleMessage( messageDecoder );
								}

								m_UserMessageListeners.Mutex.Unlock( );

								// Delete the received data pointer
								delete pReceivedData;

							}

							m_UserMessages.Mutex.Unlock( );
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
			InternalDisconnect( true, true, true, true );
		}

		Bool Client::IsConnected( )
		{
			m_Connected.Mutex.Lock( );
			Bool connected = m_Connected.Value;
			m_Connected.Mutex.Unlock( );
			return connected;
		}

		Time Client::GetPing( )
		{
			m_Ping.Mutex.Lock( );
			Time time = m_Ping.Value;
			m_Ping.Mutex.Unlock( );
			return time;
		}

		Bool Client::HookUserMessage( UserMessageListener * p_pListener, const std::string & m_MessageName )
		{
			// Error check the parameters
			if( p_pListener == NULL || m_MessageName.size( ) == 0 )
			{
				return false;
			}

			m_UserMessageListeners.Mutex.Lock( );

			// Find the user mesage set pointer.
			UserMessageListenerSet * pMessageSet = NULL;

			// Find the message
			UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.find( m_MessageName );
			if( it == m_UserMessageListeners.Value.end( ) )
			{
				// Create a new message
				pMessageSet = new UserMessageListenerSet;
				m_UserMessageListeners.Value.insert( UserMessageListenerPair( m_MessageName, pMessageSet ) );
			}
			else
			{
				pMessageSet = it->second;
			}

			// Add the listener to the message set
			pMessageSet->insert( p_pListener );

			m_UserMessageListeners.Mutex.Unlock( );

			return true;
		}


/*
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
		*/
		

		Time Client::TimeSinceLastRecvPacket( )
		{
			m_LastRecvTimer.Mutex.Lock( );
			Time time = m_LastRecvTimer.Value.GetLapsedTime( );
			m_LastRecvTimer.Mutex.Unlock( );
			return time;
		}

		// Received data struct
		Client::ReceivedData::ReceivedData( ) :
			pData( NULL )
		{
		}
		Client::ReceivedData::~ReceivedData( )
		{
			if( pData )
			{
				delete [ ] pData;
			}
		}

		// Acknowledgement data class
		Client::AcknowledgementData::AcknowledgementData( )
		{
			memset( m_SequenceBits, 0, m_SequenceArraySize * 4 );
			m_CurrentBlocks[ 0 ] = 0;
			m_CurrentBlocks[ 1 ] = 1;
		}

		Bool Client::AcknowledgementData::AddAcknowledgement( const Uint16 p_Sequence )
		{
			const static Uint32 blockSize = 16384;
			const static Uint32 blockCount = 4;

			// Find the right array and bit index.
			Uint32 arrayIndex	= static_cast<Uint32>(	p_Sequence ) / 32;
			Uint32 bitIndex		= static_cast<Uint32>( p_Sequence ) % 32;

			// Check if the bit already is set
			if( ( m_SequenceBits[ arrayIndex ] >> bitIndex ) & 0x01 )
			{
				return false;
			}

			// Check the block bounding, if we are going to change blocks
			Uint8 sequenceBlock = static_cast<Uint8>( p_Sequence / blockSize );
			if( sequenceBlock != m_CurrentBlocks[ 0 ] &&
				sequenceBlock != m_CurrentBlocks[ 1 ] &&
				sequenceBlock != ( ( m_CurrentBlocks[ 1 ] + 2 ) % 4 ) )
			{
				// Clear the last block.
				Uint32 * pClearSequence = m_SequenceBits + ( 512 * m_CurrentBlocks[ 0 ]);
				memset( pClearSequence, 0, m_SequenceArraySize );

				// Move the block index forward.
				m_CurrentBlocks[ 0 ] = m_CurrentBlocks[ 1 ];
				m_CurrentBlocks[ 1 ] = ( m_CurrentBlocks[ 1 ] + 1 ) % 4;
			}

			// Set the current bit to 1.
			Uint32 bitMask = 1 << bitIndex;
			m_SequenceBits[ arrayIndex ] = m_SequenceBits[ arrayIndex ] | bitMask;
			return true;
		}

		void Client::InternalDisconnect(	const Bool p_CloseMainThread,
											const Bool p_CloseEventThread,
											const Bool p_CloseReliableThread,
											const Bool p_CloseUserMessageThread )
		{
			// Get status and set connected to false.
			m_Connected.Mutex.Lock( );
			Bool connected = m_Connected.Value;
			m_Connected.Value = false;
			m_Connected.Mutex.Unlock( );

			if( connected )
			{
				// Send close packet.
				Uint8 buffer = ePacketType::Close;
				m_Socket.Send( &buffer, 1, m_ServerAddress, m_ServerPort );
			}

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
			if( p_CloseUserMessageThread )
			{
				m_UserMessageSemaphore.Release( );
				m_UserMessageThread.Finish( );
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

			// Clear user messages
			m_UserMessages.Mutex.Lock( );
			while( m_UserMessages.Value.size( ) )
			{
				ReceivedData * pReceivedData = m_UserMessages.Value.front( );
				m_UserMessages.Value.pop( );

				delete pReceivedData;
			}
			m_UserMessages.Mutex.Unlock( );


			// Clear the ping list.
			m_PingList.clear( );
		}

		void Client::InternalSendReliable( const ePacketType & p_PacketType, void * p_pData, const SizeType p_DataSize )
		{
			// Create a new buffer.
			const SizeType packetSize = p_DataSize + HeaderSize;
			Uint8 * pData = new Uint8[ packetSize ];
			pData[ 0 ] = p_PacketType;

			// Add the sequence to the data buffer.
			m_Sequence.Mutex.Lock( );
			Uint16 currentSequence = m_Sequence.Value;
			m_Sequence.Mutex.Unlock( );
							
			// Add the sequence to the data buffer.
			Uint16 sequence = Hton16( currentSequence );
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

		void Client::CalculateNewPing( const Time & p_LapsedTime )
		{
			// Add the new ping to the ping list
			m_PingList.pop_front( );
			m_PingList.push_back( p_LapsedTime / 2ULL );

			// Calculate the new ping
			Uint64 newPing = 0ULL;
			for( TimeList::iterator it = m_PingList.begin( ); it != m_PingList.end( ); it++ )
			{
				newPing += it->AsMicroseconds( );
			}
			newPing /= static_cast<Uint64>( m_PingList.size( ) );
										
			m_Ping.Mutex.Lock( );
			m_Ping.Value = Microseconds( newPing );
			m_Ping.Mutex.Unlock( ); 
		}

		void Client::AddUserMessage( ReceivedData * p_pReceivedData )
		{
			m_UserMessages.Mutex.Lock( );
			m_UserMessages.Value.push( p_pReceivedData );
			m_UserMessages.Mutex.Unlock( );

			m_UserMessageSemaphore.Release( );
		}

	}

}