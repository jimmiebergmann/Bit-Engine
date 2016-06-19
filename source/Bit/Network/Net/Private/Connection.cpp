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
            InternalDisconnect( );
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

        void Connection::AddMessage(MemoryPool<Uint8>::Item * p_pReceivedPacket)
        {
            m_ReceivedPacketQueue.Mutex.Lock();
            m_ReceivedPacketQueue.Value.push(p_pReceivedPacket);
            m_ReceivedPacketQueue.Mutex.Unlock();

            m_ReceivedPacketSemaphore.Release();
        }

		Bool Connection::SendUnreliable(const Private::PacketType::eType p_PacketType,
			const void * p_pData,
			const SizeType p_DataSize)
		{
			// Create the packet. Make space for the sequence in case.
			const Bit::SizeType packetSize = Private::NetHeaderSize + p_DataSize;

			// Set packet header type byte.
			Uint8 * pBuffer = new Uint8[packetSize];
			pBuffer[0] = static_cast<Uint8>(p_PacketType);

			// Set packet header sequence.
			Bit::Uint16 sequence = Bit::Hton16(GetNextSequence());
			memcpy(pBuffer + 1, &sequence, Private::NetSequenceSize);

			// Add the data to the new buffer
			memcpy(pBuffer + Private::NetHeaderSize, p_pData, p_DataSize);

			// Send the packet.
			Bool status = false;
			if (m_pServer->m_Socket.Send(pBuffer, packetSize, m_DstAddress, m_DstPort) == packetSize)
			{
				RestartLastSentPacketTimer();
				status = true;
			}

			// Delete the packet
			delete[] pBuffer;

			return status;
		}


		Bool Connection::SendReliable(const Private::PacketType::eType p_PacketType,
			const void * p_pData,
			const SizeType p_DataSize)
		{
			// Create the packet. Make space for the sequence in case.
			const Bit::SizeType packetSize = Private::NetHeaderSize + p_DataSize;

			// Set packet header type byte(also the reliable flag),
			Uint8 * pBuffer = new Uint8[packetSize];
			pBuffer[0] = static_cast<Uint8>(p_PacketType) | Private::NetReliableFlagMask;

			// Set packet header sequence.
			Bit::Uint16 sequence = Bit::Hton16(GetNextSequence());
			memcpy(pBuffer + 1, &sequence, Private::NetSequenceSize);

			// Add the data to the new buffer
			memcpy(pBuffer + Private::NetHeaderSize, p_pData, p_DataSize);

			// Add the reliable packet.
			AddReliablePacket(sequence, pBuffer, packetSize);

			// Send the packet.
			if (m_pServer->m_Socket.Send(pBuffer, packetSize, m_DstAddress, m_DstPort) == packetSize)
			{
				RestartLastSentPacketTimer();
				return true;
			}

			// Do not delete the allocated buffer by purpose,
			// since it's stored as a reliable packet and will be deleted later.
			return false;
		}


        MemoryPool<Uint8>::Item * Connection::PollMessages()
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
                        while (IsConnected() && (pItem = PollMessages()) != NULL)
                        {
                            Uint8 * pData = pItem->GetData();
                            SizeType recvSize = pItem->GetUsedSize();

                            // Reset the time for checking last time a packet arrived
                            RestartLastReceivedPacketTimer();

							// Get packet type and sequence.
                            const Private::PacketType::eType packetType = Private::PacketTransfer::ParsePacketType(pData[0]);
							const Uint16 sequence = Private::PacketTransfer::ReadNtoh16FromBuffer(pData + 1);

							// Add sequence to sequence manager,
							// ignore the packet if it's already received...
							if (m_SequenceManager.AddSequence(sequence))
							{
								// Check the packet type
								switch (packetType)
								{
									// Connect packet from client, we should get the packet here if
									// the client resent the connection packet.
									case Private::PacketType::Connect:
									{
										SizeType identifierLength = static_cast<SizeType>(pData[3]);

										if (identifierLength)
										{
											// Make sure that the identifier is right.
											if (identifierLength != m_pServer->m_Identifier.size() ||
												memcmp(pData + Private::NetConnectPacketSize, m_pServer->m_Identifier.data(), m_pServer->m_Identifier.size()) != 0)
											{
												// Send denied packet
												pData[3] = Private::ConnectStatusType::Denied;
												m_Socket.Send(pData, Private::NetRejectPacketSize, m_DstAddress, m_DstPort);
												break;
											}

											// Get server time and send accept response
											// We're not using the SendUnreliable function here in order to speed things up.
											const Uint64 serverTime = Hton64(m_pServer->GetServerTime().AsMicroseconds());
											pData[3] = Private::ConnectStatusType::Accepted;
											memcpy(&(pData[4]), &serverTime, sizeof(Uint64));
											m_Socket.Send(pData, Private::NetAcceptPacketSize, m_DstAddress, m_DstPort);
										}
									}
									break;
								   /* // Alive packet from client. WE ARE ALREADY SENDING ACKNOWLEDGEMENT PACKETS.
									case Private::PacketType::Alive:
									{
										if (recvSize != Private::NetAlivePacketSize)
										{
											break;
										}

										// Send acknowledgement packet.
										// We're not using the SendUnreliable function here in order to speed things up.
										pData[0] = Private::PacketType::Acknowledgement;
										m_pServer->m_Socket.Send(pData, Private::NetAcknowledgementPacketSize, m_DstAddress, m_DstPort);
									}
									break;*/
									// ACK packet from client.
									case Private::PacketType::Acknowledgement:
									{
										if (recvSize != Private::NetAcknowledgementPacketSize)
										{
											break;
										}

										// Remove the reliable packet.
										const Uint16 sequence = Private::PacketTransfer::ReadNtoh16FromBuffer(pData + 1);
										Bool wasResent = false;
										Time timeSinceSent;
										if(RemoveReliablePacket(sequence, wasResent, timeSinceSent))
										{
											// Calculate the new ping from the lapsed time if it's not a resent packet
											if (wasResent == false)
											{
												CalculateNewPing(timeSinceSent);
											}
										}
									}
									break;
									case Private::PacketType::UserMessage:
									{
                                    
										if (recvSize <= Private::NetUserMessagePacketSize)
										{
											break;
										}

										// Add user message to queue
										AddUserMessage(pItem);
										continue;
									}
									break;
									default:
										break;
								};
							}

                            // Return the packet to the memory pool.
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
                   
                    // Wait for the semaphore to release
                    m_UserMessageSemaphore.Wait( );

					if (IsConnected() == false)
					{
						return;
					}

                    // Go throguh the user messages.
                    m_UserMessages.Mutex.Lock( );

                    while( m_UserMessages.Value.size( ) )
                    {
                        // Get the current user message
						MemoryPool<Uint8>::Item * pItem = m_UserMessages.Value.front( );
						m_UserMessages.Value.pop();

						// Run a do loop once, to return the data at any breakage.
						do
						{
							const SizeType dataSize = pItem->GetUsedSize();
							Uint8 * data = pItem->GetData();

							SizeType messageLength = 0;


							// Get message name length.
							std::string messageName = "";
							SizeType messageNameLength = static_cast<SizeType>(data[3]);

							// Error check data length
							if (messageNameLength)
							{
								// Make sure there are space for the message name.
								if (dataSize < Private::NetUserMessagePacketSize + messageNameLength)
								{
									break;
								}

								// Get the message name
								messageName.assign(reinterpret_cast<char*>(data + Private::NetUserMessagePacketSize), messageNameLength);
							}


							// Find the listeners for this user message
							m_pServer->m_UserMessageListeners.Mutex.Lock();
							Server::UserMessageListenerMap::iterator it = m_pServer->m_UserMessageListeners.Value.find(messageName);
							if (it == m_pServer->m_UserMessageListeners.Value.end())
							{
								m_pServer->m_UserMessageListeners.Mutex.Unlock();
								break;
							}

							Server::UserMessageListenerSet * pMessageSet = it->second;

							// Go through the listeners and call the listener function
							for (Server::UserMessageListenerSet::iterator it2 = pMessageSet->begin(); it2 != pMessageSet->end(); it2++)
							{
								// Get the listener.
								UserMessageListener * pListener = *it2;

								// Create a message decoder
								Uint8 * pMessageDataPtr = data + Private::NetUserMessagePacketSize + messageNameLength;
								SizeType messageDataSize = dataSize - Private::NetUserMessagePacketSize - messageNameLength;
								UserMessageDecoder messageDecoder(messageName, m_UserId, pMessageDataPtr, dataSize);

								// Use threads????
								// Handle the message.
								pListener->HandleMessage(messageDecoder);
							}

							m_pServer->m_UserMessageListeners.Mutex.Unlock();
						}
						while (false);

						// Return the packet to the memory pool.
						m_pServer->m_pPacketMemoryPool->Return(pItem);
                    }

                    m_UserMessages.Mutex.Unlock( );
                    
                }
            }
            );


            // Execute the event thread
            m_TimeoutThread.Execute( [ this ] ( )
                {
                    while( IsConnected( ) )
                    {
                        // Sleep for some time.
                        Sleep( Milliseconds( 10 ) );

                        // Disconnect you've not heard anything from the server in a while.
                        const Bit::Time timesinceLastPacket = GetTimeSinceLastReceivedPacket();
                        if (timesinceLastPacket >= m_LosingConnectionTimeout)
                        {
                            // Call on disconnect function
                            m_pServer->OnDisconnection(m_UserId);

                            // Set the connection flag to false
							m_Connected.Set(false);

                            // Add the connection to the cleanup thread.
                            m_pServer->AddConnectionForCleanup( this );

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


        void Connection::InternalDisconnect()
        {
            // Get status and set connected to false.
            m_Connected.Set(false);
                
            // Wait for the threads to finish.
            m_ReceivedPacketSemaphore.Release();
            m_Thread.Finish( );
            m_TimeoutThread.Finish( );
            m_ReliableThread.Finish( );
            m_UserMessageSemaphore.Release( );
            m_UserMessageThread.Finish( );

            // Clear the reliable packets
			ClearReliablePackets();
            
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

		void Connection::AddUserMessage(MemoryPool<Uint8>::Item * p_UserMessageData)
		{
			m_UserMessages.Mutex.Lock();
			m_UserMessages.Value.push(p_UserMessageData);
			m_UserMessages.Mutex.Unlock();

			m_UserMessageSemaphore.Release();
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