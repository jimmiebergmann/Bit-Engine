// Copyright (C) 2013 Jimmie Bergmann - jimmiebergmann@gmail.com
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but
//    is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any
//    source distribution.
// ///////////////////////////////////////////////////////////////////////////

#include <Bit/Network/Net/Private/Connection.hpp>
#include <Bit/Network/Net/Server.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Connection::Connection( const Address & p_Address,
								const Uint16 & p_Port,
								const Uint16 & p_UserId,
								const Time & p_InitialPing ) :
			m_pServer( NULL ),
			m_ConnectionTimeout( Seconds( 3.0f ) ),
			m_Connected( false ),
			m_Address( p_Address ),
			m_Port( p_Port ),
			m_UserId( p_UserId ),
			m_Sequence( 0 ),
			m_Ping( p_InitialPing )
		{
		}

		Connection::~Connection( )
		{
			// Disconnect the client.
			InternalDisconnect( true, true, true, true );
		}

		Bool Connection::IsConnected( )
		{
			m_Connected.Mutex.Lock( );
			Bool connected = m_Connected.Value;
			m_Connected.Mutex.Unlock( );
			return connected;
		}

		Time Connection::GetPing( )
		{
			m_Ping.Mutex.Lock( );
			Time time = m_Ping.Value;
			m_Ping.Mutex.Unlock( );
			return time;
		}

		Uint16 Connection::GetUserId( ) const
		{
			return m_UserId;
		}

		const Address & Connection::GetAddress( ) const
		{
			return m_Address;
		}

		Uint16 Connection::GetPort( ) const
		{
			return m_Port;
		}
		
		Uint64 Connection::GetPackedAddress( ) const
		{
			return	static_cast<Uint64>( m_Address.GetAddress( ) ) * 
					static_cast<Uint64>( m_Port ) +
					static_cast<Uint64>( m_Port ) ;
		}
/*


		Bit::Bool Connection::Receive( Packet & p_Packet )
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
		// Raw packet struct
		Connection::RawPacket::RawPacket( Uint8 * p_pData, const SizeType p_DataSize )
		{
			DataSize = p_DataSize;
			pData = new Uint8[ p_DataSize ];
			memcpy( pData, p_pData, p_DataSize );
		}

		Connection::RawPacket::~RawPacket( )
		{
			delete [ ] pData;
		}

		// Acknowledgement data struct
		Connection::AcknowledgementData::AcknowledgementData( )
		{
			memset( m_SequenceBits, 0, m_SequenceArraySize * 4 );
			m_CurrentBlocks[ 0 ] = 0;
			m_CurrentBlocks[ 1 ] = 1;
		}

		Bool Connection::AcknowledgementData::AddAcknowledgement( const Uint16 p_Sequence )
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

		void Connection::StartThreads( Server * p_pServer )
		{
			// Set server pointer
			m_pServer = p_pServer;

			// Start the timer for the last recv packet.
			m_LastRecvTimer.Mutex.Lock( );
			m_LastRecvTimer.Value.Start( );
			m_LastRecvTimer.Mutex.Unlock( );

			// Start the timer for the last send packet.
			m_LastSendTimer.Mutex.Lock( );
			m_LastSendTimer.Value.Start( );
			m_LastSendTimer.Mutex.Unlock( );

			// Set the connection flag to true.
			m_Connected.Mutex.Lock( );
			m_Connected.Value = true;
			m_Connected.Mutex.Unlock( );

			// Initialize the ping list
			for( SizeType i = 0; i < 5; i++ )
			{
				m_PingList.push_front( m_Ping.Value );
			}

			// Start the thread.
			m_Thread.Execute( [ this ] ( )
			 {
					// Pointer to the raw packet to handle.
					RawPacket * pPacket = NULL;

					// Keep on running as long as the server is running.
					while( IsConnected( ) )
					{
						// Wait for an event
						m_EventSemaphore.Wait( );

						// Go throguh the packets.
						while( ( pPacket = PollRawPacket( ) ) != NULL )
						{
							// Reset the time for checking last time a packet arrived
							m_LastRecvTimer.Mutex.Lock( );
							m_LastRecvTimer.Value.Start( );
							m_LastRecvTimer.Mutex.Unlock( );

							// Check the packet type
							switch( pPacket->pData[ 0 ] )
							{
								// Disconnect packet from client.
								case ePacketType::Close:
								{
									// Destroy the packet.
									delete pPacket;

									// Set the connection flag to false
									m_Connected.Mutex.Lock( );
									m_Connected.Value = false;
									m_Connected.Mutex.Unlock( );

									// Add the connection to the cleanup thread.
									m_pServer->AddConnectionForCleanup( this );

									// Increase the semaphore for cleanups
									m_pServer->m_CleanupSemaphore.Release( );

									// Call on disconnect function
									m_pServer->OnDisconnection( m_UserId );

									// Return, exit the thread
									return;
								}
								break;
								// Alive packet from client.
								case ePacketType::Alive:
								{
									// Ignore "corrupt" alive packet.
									if( pPacket->DataSize != 3 )
									{
										delete pPacket;
										continue;
									}

									Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 1 ] ) ) |
																static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 2 ] ) << 8 ) );

									// Use the already allocated packet, change the type
									pPacket->pData[ 0 ] = ePacketType::Ack;

									// Send the ack packet
									m_pServer->m_Socket.Send( pPacket->pData, 3, m_Address, m_Port );
								}
								break;
								// ACK packet from client.
								case ePacketType::Ack:
								{
									// Ignore "corrupt" ack packet.
									if( pPacket->DataSize != 3 )
									{
										continue;
									}

									// Get the sequence
									Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 1 ] ) ) |
																static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 2 ] ) << 8 ) );

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
								// Unreliable packet from client.
								case ePacketType::UnreliablePacket:
								{
									if( pPacket->DataSize <= HeaderSize + 1 )
									{
										delete pPacket;
										continue;
									}

									// Add the unreliable packet to the received data queue.	
									ReceivedData * pReceivedData = new ReceivedData;
									pReceivedData->DataSize = pPacket->DataSize - HeaderSize;
									pReceivedData->pData = new Uint8[ pReceivedData->DataSize ];
									memcpy( pReceivedData->pData, pPacket->pData + HeaderSize, pReceivedData->DataSize );

									// Get the sequence
									pReceivedData->Sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 1 ] ) ) |
																		static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 2 ] ) << 8 ) );
														
									switch( pReceivedData->pData[ 0 ] )
									{
										case eMessageType::UserMessageType:
										{
											AddUserMessage( pReceivedData );
										}
										break;
										default:
											break;
									}
								}
								break;
								case ePacketType::ReliablePacket:
								{
									// Use the already allocated packet, change the type
									pPacket->pData[ 0 ] = ePacketType::Ack;

									// Send the ack packet
									m_pServer->m_Socket.Send( pPacket->pData, 3, m_Address, m_Port );

									// Get the sequence.
									Uint16 sequence = Ntoh16(	static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 1 ] ) ) |
																static_cast<Uint16>( static_cast<Uint8>( pPacket->pData[ 2 ] ) << 8 ) );

									// Add the unreliable packet to the received data queue if it's not a resent packet.
									if( m_AcknowledgementData.AddAcknowledgement( sequence ) )
									{
										ReceivedData * pReceivedData = new ReceivedData;
										pReceivedData->Sequence = sequence;
										pReceivedData->DataSize = pPacket->DataSize - HeaderSize;
										pReceivedData->pData = new Uint8[ pReceivedData->DataSize ];
										memcpy( pReceivedData->pData, pPacket->pData + HeaderSize, pReceivedData->DataSize );

										switch( pReceivedData->pData[ 0 ] )
										{
											case eMessageType::UserMessageType:
											{
												AddUserMessage( pReceivedData );
											}
											break;
											default:
												break;
										}
									}
								}
								break;
								default:
									break;
							};

							// Destroy the packet.
							delete pPacket;
						}

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
						m_pServer->m_UserMessageListeners.Mutex.Lock( );
						Server::UserMessageListenerMap::iterator it = m_pServer->m_UserMessageListeners.Value.find( name );
						if( it == m_pServer->m_UserMessageListeners.Value.end( ) )
						{
							// Delete the received data pointer
							delete pReceivedData;
							continue;
						}

						Server::UserMessageListenerSet * pMessageSet = it->second;

						// Go through the listeners and call the listener function
						for( Server::UserMessageListenerSet::iterator it2 = pMessageSet->begin( ); it2 != pMessageSet->end( ); it2++ )
						{
							// Get the listener.
							UserMessageListener * pListener = *it2;

							// Create a message decoder
							Uint8 * pDataPointer =  pReceivedData->pData + name.size( ) + 2;
							const SizeType dataSize = pReceivedData->DataSize - name.size( ) - 2;
							UserMessageDecoder messageDecoder( name, m_UserId, pDataPointer, dataSize ) ;

							// Use threads????
							// Handle the message.
							pListener->HandleMessage( messageDecoder );
						}

						m_pServer->m_UserMessageListeners.Mutex.Unlock( );

						// Delete the received data pointer
						delete pReceivedData;

					}

					m_UserMessages.Mutex.Unlock( );
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
						if( TimeSinceLastRecvPacket( ) >= m_ConnectionTimeout )
						{
							// Set the connection flag to false
							m_Connected.Mutex.Lock( );
							m_Connected.Value = false;
							m_Connected.Mutex.Unlock( );

							// Add the connection to the cleanup thread.
							m_pServer->AddConnectionForCleanup( this );

							// Increase the semaphore for cleanups
							m_pServer->m_CleanupSemaphore.Release( );

							// Call on disconnect function
							m_pServer->OnDisconnection( m_UserId );

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
								m_pServer->m_Socket.Send( pPacket->pData, pPacket->DataSize, m_Address, m_Port );

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


		}

		void Connection::AddRawPacket( Uint8 * p_pData, const SizeType p_DataSize )
		{
			if( p_DataSize == 0 )
			{
				return;
			}

			// Lock the raw packet mutex.
			m_RawPacketQueue.Mutex.Lock( );

			// Create the queue packet.
			RawPacket * pPacket = new RawPacket( p_pData, p_DataSize );

			// Push the packet to the raw packet queue.
			m_RawPacketQueue.Value.push( pPacket );
	
			// Release the semaphore if this is the very first packet in a while.
			if(m_RawPacketQueue.Value.size( ) == 1 )
			{
				m_EventSemaphore.Release( );
			}

			m_RawPacketQueue.Mutex.Unlock( );
		}

		Connection::RawPacket * Connection::PollRawPacket( )
		{
			RawPacket * pPacket = NULL;

			m_RawPacketQueue.Mutex.Lock( );
			if( m_RawPacketQueue.Value.size( ) )
			{
				pPacket = m_RawPacketQueue.Value.front( );
				m_RawPacketQueue.Value.pop( );
			}
			m_RawPacketQueue.Mutex.Unlock( );

			return pPacket;
		}

		Time Connection::TimeSinceLastRecvPacket( )
		{
			m_LastRecvTimer.Mutex.Lock( );
			Time time = m_LastRecvTimer.Value.GetLapsedTime( );
			m_LastRecvTimer.Mutex.Unlock( );
			return time;
		}

		void Connection::InternalDisconnect(	const Bool p_CloseMainThread,
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
				m_pServer->m_Socket.Send( &buffer, 1, m_Address, m_Port );
			}
				
			// Wait for the threads to finish.
			if( p_CloseMainThread )
			{
				m_EventSemaphore.Release( );
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

			// Clear all the raw packets.
			m_RawPacketQueue.Mutex.Lock( );
			while( m_RawPacketQueue.Value.size( ) )
			{
				RawPacket * pPacket = m_RawPacketQueue.Value.front( );
				delete pPacket;
				m_RawPacketQueue.Value.pop( );
			}
			m_RawPacketQueue.Mutex.Unlock( );
			
			// Clear the ping list.
			m_PingList.clear( );
		}

		void Connection::SendUnreliable( void * p_pData, const Bit::SizeType p_DataSize )
		{
			// Use memory pool here?

			// Create the packet.
			const Bit::SizeType packetSize = p_DataSize + HeaderSize;
			Uint8 * pData = new Uint8[ packetSize ];
			pData[ 0 ] = ePacketType::UnreliablePacket;

			// Get the current sequence
			m_Sequence.Mutex.Lock( );
			Bit::Uint16 sequence = Bit::Hton16( m_Sequence.Value );
			// Increment the sequence
			m_Sequence.Value++;
			m_Sequence.Mutex.Unlock( );

			// Add the sequence to the data buffer.
			memcpy( pData + 1, &sequence, 2 );

			// Add the data to the new buffer
			memcpy( pData + HeaderSize, p_pData, p_DataSize );

			// Send the packet.
			m_pServer->m_Socket.Send( pData, packetSize, m_Address, m_Port );

			// Delete the packet
			delete [ ] pData;
		}

		void Connection::SendReliable( void * p_pData, const Bit::SizeType p_DataSize )
		{
			InternalSendReliable( ePacketType::ReliablePacket, p_pData, p_DataSize );
		}

		void Connection::InternalSendReliable( const ePacketType & p_PacketType, void * p_pData, const SizeType p_DataSize )
		{
			// Create a new buffer.
			const SizeType packetSize = p_DataSize + HeaderSize;
			Uint8 * pData = new Uint8[ packetSize ];
			pData[ 0 ] = static_cast<Uint8>( p_PacketType );

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
			if( m_pServer->m_Socket.Send( pData, packetSize, m_Address, m_Port ) == packetSize )
			{
				// Restart the timer
				m_LastSendTimer.Mutex.Lock( );
				m_LastSendTimer.Value.Start( );
				m_LastSendTimer.Mutex.Unlock( );
			}
		}

		void Connection::CalculateNewPing( const Time & p_LapsedTime )
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

		void Connection::AddUserMessage( ReceivedData * p_pReceivedData )
		{
			m_UserMessages.Mutex.Lock( );
			m_UserMessages.Value.push( p_pReceivedData );
			m_UserMessages.Mutex.Unlock( );

			m_UserMessageSemaphore.Release( );
		}

	}

}