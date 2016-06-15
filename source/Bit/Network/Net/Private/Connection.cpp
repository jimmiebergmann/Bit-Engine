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
								const Time & p_LosingConnectionTimeout,
								const Time & p_InitialPing ) :
			PacketTransfer(p_Address, p_Port),
			m_pServer( NULL ),
			m_Connected( false ),
			m_UserId( p_UserId ),
			m_LosingConnectionTimeout(p_LosingConnectionTimeout),
			m_Ping( p_InitialPing ),
			m_pEntityMessageDataPtr(NULL)
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

		void Connection::AddToGroup(const Uint32 p_GroupIndex)
		{
			m_Groups.Mutex.Lock();
			m_Groups.Value.insert(p_GroupIndex);
			m_Groups.Mutex.Unlock();
		}

		void Connection::RemoveFromGroup(const Uint32 p_GroupIndex)
		{
			m_Groups.Mutex.Lock();
			m_Groups.Value.erase(p_GroupIndex);
			m_Groups.Mutex.Unlock();
		}

		void Connection::AddReceivedData(MemoryPool<Uint8>::Item * p_pReceivedPacket)
		{
			m_ReceivedPacketQueue.Mutex.Lock();
			m_ReceivedPacketQueue.Value.push(p_pReceivedPacket);
			m_ReceivedPacketQueue.Mutex.Unlock();

			m_ReceivedPacketSemaphore.Release();
		}

		MemoryPool<Uint8>::Item * Connection::PollReceivedPackets()
		{
			MemoryPool<Uint8>::Item * pItem = NULL;

			m_ReceivedPacketQueue.Mutex.Lock();
			if (m_ReceivedPacketQueue.Value.size())
			{
				pItem = m_ReceivedPacketQueue.Value.front();
				m_ReceivedPacketQueue.Value.pop();
			}
			m_ReceivedPacketQueue.Mutex.Unlock();

			return pItem;
		}

		void Connection::StartThreads( Server * p_pServer )
		{
			// Set server pointer
			m_pServer = p_pServer;

			// Start the timer for the last recv/sent packet.
			RestartLastSentPacketTimer();
			RestartLastReceivedPacketTimer();

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
					MemoryPool<Uint8>::Item * pItem = NULL;

					// Keep on running as long as the server is running.
					while( IsConnected( ) )
					{
						// Wait for an incoming packet
						m_ReceivedPacketSemaphore.Wait();

						// Go throguh the packets.
						while (IsConnected() && (pItem = PollReceivedPackets()) != NULL)
						{
							Uint8 * pData = pItem->GetData();
							SizeType recvSize = pItem->GetUsedSize();

							// Reset the time for checking last time a packet arrived
							RestartLastReceivedPacketTimer();

							// Check the packet type
							switch (pData[0])
							{
								// Connect packet from client, we should get the packet here if
								// the client resent the connection packet.
								case Private::PacketType::Connect:
								{
									// Make sure that the identifier is right.
									if (recvSize != m_pServer->m_Identifier.size() + 1)
									{
										break;
									}
									if (memcmp(pData + 1, m_pServer->m_Identifier.data(), m_pServer->m_Identifier.size()) != 0)
									{
										break;
									}

									// Answer the client with a SYNACK packet.
									pData[3] = Private::PacketType::Accept;
									m_pServer->m_Socket.Send(pData, 1, m_DstAddress, m_DstPort);
								}
								break;
								// Disconnect packet from client.
								case Private::PacketType::Disconnect:
								{

									// Set the connection flag to false
									m_Connected.Set(false);

									// Add the connection to the cleanup thread.
									m_pServer->AddConnectionForCleanup( this );

									// Increase the semaphore for cleanups
									m_pServer->m_CleanupSemaphore.Release( );

									// Call on disconnect function
									m_pServer->OnDisconnection( m_UserId );

									// break this switch, the connection thread will terminate.
									break;
								}
								break;
								// Alive packet from client.
								case Private::PacketType::Alive:
								{
									// Ignore "corrupt" alive packet.
									if (recvSize != 3)
									{
										break;
									}

									Uint16 sequence = Ntoh16(	static_cast<Uint16>(static_cast<Uint8>(pData[1])) |
																static_cast<Uint16>(static_cast<Uint8>(pData[2]) << 8));

									// Use the already allocated packet, change the type
									pData[0] = Private::PacketType::Acknowledgement;

									// Send the ack packet
									m_pServer->m_Socket.Send(pData, 3, m_DstAddress, m_DstPort);
								}
								break;
								// ACK packet from client.
								case Private::PacketType::Acknowledgement:
								{
									// Ignore "corrupt" ack packet.
									if (recvSize != 3)
									{
										break;
									}

									// Get the sequence
									Uint16 sequence = Ntoh16(	static_cast<Uint16>(static_cast<Uint8>(pData[1])) |
																static_cast<Uint16>(static_cast<Uint8>(pData[2]) << 8));

									// Find the sequence in the reliable map
									m_ReliablePackets.Mutex.Lock( );
									ReliablePacketMap::iterator it = m_ReliablePackets.Value.find( sequence );
									// Remove the packet from the map if it's found and calculate the ping.
									if( it != m_ReliablePackets.Value.end( ) )
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
										m_ReliablePackets.Value.erase( it );
									}
									m_ReliablePackets.Mutex.Unlock( );
								}
								break;
								case Private::PacketType::UserMessage:
								{
									/*
									// Error check the recv size
									if (recvSize <= NetUserMessagePacketSize)
									{
										break;
									}

									// Get the sequence
									const Uint16 sequence = Ntoh16(	static_cast<Uint16>(static_cast<Uint8>(pData[1])) |
																	static_cast<Uint16>(static_cast<Uint8>(pData[2]) << 8));


									// Check the reliable flag
									if (pData[PacketTypeSize + SequenceSize] == ReliabilityType::Reliable)
									{
										// Use the already allocated packet, change the type
										pData[0] = PacketType::Acknowledgement;

										// Send the ack packet
										m_pServer->m_Socket.Send(pData, AcknowledgementPacketSize, m_Address, m_Port);
									}

									// Add the packets sequence to the sequence manager, do not handle the packet
									// if we've already received a packet with the same sequence.
									if (m_SequenceManager.AddSequence(sequence))
									{

										// Add the unreliable packet to the received data queue.	
										ReceivedData * pReceivedData = new ReceivedData(pData + UserMessagePacketSize,
																						recvSize - UserMessagePacketSize,
																						sequence);

										// Add the host message.
										AddUserMessage(pReceivedData);
									}
									*/
								}
								break;
								default:
									break;
							};

							// Destroy the packet.
							m_pServer->m_pPacketMemoryPool->Return(pItem);
						}

					}
				}
			);

			// Execute user message
			m_UserMessageThread.Execute( [ this ] ( )
			{
				while( IsConnected( ) )
				{
					/*
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

						// Check if there is any message left(including empty)
						if( name.size( ) + 2 > pReceivedData->DataSize )
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
					*/
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
						const Bit::Time timesinceLastPacket = GetTimeSinceLastReceivedPacket();
						if (timesinceLastPacket >= m_LosingConnectionTimeout)
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
						Time lapsedTime = GetTimeSinceLastSentPacket();

						// Send alive packet if requred
						if( lapsedTime >= Seconds( 0.5f ) )
						{
							SendReliable( Private::PacketType::Alive, NULL, 0 );
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
						m_ReliablePackets.Mutex.Lock( );
						
						for(	ReliablePacketMap::iterator it = m_ReliablePackets.Value.begin( );
								it != m_ReliablePackets.Value.end( );
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
								m_pServer->m_Socket.Send( pPacket->pData, pPacket->DataSize, m_DstAddress, m_DstPort );

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

						m_ReliablePackets.Mutex.Unlock( );

					}
				}
			);


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
				Uint8 buffer = Private::PacketType::Disconnect;
				m_pServer->m_Socket.Send( &buffer, 1, m_DstAddress, m_DstPort );
			}
				
			// Wait for the threads to finish.
			if( p_CloseMainThread )
			{
				m_ReceivedPacketSemaphore.Release();
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
				//m_UserMessageSemaphore.Release( );
				//m_UserMessageThread.Finish( );
			}

			// Clear the reliable packets
			m_ReliablePackets.Mutex.Lock( );
			for(	ReliablePacketMap::iterator it = m_ReliablePackets.Value.begin( );
					it != m_ReliablePackets.Value.end( );
					it ++ )
			{
				delete [ ] it->second->pData;
				delete it->second;
			}
			m_ReliablePackets.Value.clear( );
			m_ReliablePackets.Mutex.Unlock( );

			// Return all the received data items to the servers memory pool.
			/*m_ReliablePackets.Mutex.Lock();
			while (m_ReliablePackets.Value.size())
			{
				m_pServer->m_pPacketMemoryPool.Value->Return(m_ReliablePackets.Value.front());

				m_ReliablePackets.Value.pop();
			}
			m_ReliablePackets.Mutex.Unlock();*/
			
			// Clear the ping list.
			m_PingList.clear( );
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

		void Connection::AddUserMessage(MemoryPool<Uint8> & p_UserMessageData)
		{
			/*m_UserMessages.Mutex.Lock( );
			m_UserMessages.Value.push( p_pReceivedData );
			m_UserMessages.Mutex.Unlock( );
			
			m_UserMessageSemaphore.Release( );*/
		}

		void Connection::SetTempEntityMessagePtr(void * p_pEntityMessageDataPtr)
		{
			m_pEntityMessageDataPtr = p_pEntityMessageDataPtr;
		}

		void * Connection::GetTempEntityMessagePtr()
		{
			return m_pEntityMessageDataPtr;
		}

	}

}