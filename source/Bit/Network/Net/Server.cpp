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

#include <Bit/Network/Net/Server.hpp>
#include <Bit/Network/Net/Private/ServerEntityChanger.hpp>
#include <Bit/Network/TcpSocket.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/Timestep.hpp>
#include <Bit/System/SmartMutex.hpp>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		// Server properites class
		Server::Properties::Properties() :
			HostPort(0),
			MaxConnections(255),
			LosingConnectionTimeout(Seconds(3.0f)),
			EntityUpdatesPerSecond(22),
			MaxEntities(2048),
			Identifier("Bit Engine Network")
		{
		}

		Server::Properties::Properties(const Uint16 p_HostPort,
										const Uint8 p_MaxConnections,
										const Time & p_LosingConnectionTimeout,
										const Uint8 p_EntityUpdatesPerSecond,
										const Uint16 p_MaxEntities,
										const std::string & p_Identifier) :
			HostPort(p_HostPort),
			MaxConnections(p_MaxConnections),
			LosingConnectionTimeout(p_LosingConnectionTimeout),
			EntityUpdatesPerSecond(p_EntityUpdatesPerSecond),
			Identifier(p_Identifier)
		{
		}

		// Server class
		Server::Server() :
			m_EntityManager(this),
			m_MaxConnections(0),
			m_EntityUpdatesPerSecond(0),
			m_pPacketMemoryPool(NULL)
		{

		}

		Server::~Server()
		{
			Stop();
			UnhookUserMessages();
		}

		Bool Server::OnPreConnection(	const Address & p_Address,
										const Uint16 p_Port)
		{
			return true;
		}

		void Server::OnPostConnection(const Uint16 p_UserId)
		{
		}

		void Server::OnDisconnection(const Uint16 p_UserId)
		{
		}

		HostRecipientFilter * Server::CreateRecipientFilter(const Bool p_Reliable)
		{
			return new HostRecipientFilter(this, p_Reliable);
		}

		HostMessage * Server::CreateHostMessage(const std::string & p_Name, const Int32 p_MessageSize)
		{
			return new HostMessage(p_Name, this, p_MessageSize);
		}

		Bool Server::DisconnectUser(const Uint16 p_UserId)
		{
			// Find the connection via user id.
			m_ConnectionMutex.Lock();
			UserConnectionMap::iterator it = m_UserConnections.find(p_UserId);
			if (it == m_UserConnections.end())
			{
				// Unlock the connection mutex.
				m_ConnectionMutex.Unlock();
				return false;
			}

			// Get the connection
			Connection * pConnection = it->second;

			// Unlock the connection mutex.
			m_ConnectionMutex.Unlock();

			// Add the connection for cleanup
			AddConnectionForCleanup(pConnection);

			return true;
		}

		void Server::AddUserToGroup(const Uint16 p_UserId, const Uint32 p_GroupIndex)
		{
			SmartMutex mutex(m_ConnectionMutex);
			mutex.Lock();

			// Find the user.
			UserConnectionMap::iterator it = m_UserConnections.find(p_UserId);
			if (it == m_UserConnections.end())
			{
				return;
			}

			// Add the entity index to the connection.
			it->second->AddToGroup(p_GroupIndex);
		}

		void Server::RemoveUserFromGroup(const Uint16 p_UserId, const Uint32 p_GroupIndex)
		{
			SmartMutex mutex(m_ConnectionMutex);
			mutex.Lock();

			// Find the user.
			UserConnectionMap::iterator it = m_UserConnections.find(p_UserId);
			if (it == m_UserConnections.end())
			{
				return;
			}

			// Add the entity index to the connection.
			it->second->RemoveFromGroup(p_GroupIndex);
		}

		Bool Server::BanUser(const Uint16 p_UserId, const Time p_Time)
		{
			// Find the connection via user id.
			m_ConnectionMutex.Lock();
			UserConnectionMap::iterator it = m_UserConnections.find(p_UserId);
			if (it == m_UserConnections.end())
			{
				// Unlock the connection mutex.
				m_ConnectionMutex.Unlock();
				return false;
			}

			// Get the connection
			Connection * pConnection = it->second;

			// Unlock the connection mutex.
			m_ConnectionMutex.Unlock();

			// Add addresss to ban set
			m_BanSet.Mutex.Lock();
			m_BanSet.Value.insert(pConnection->GetDestinationAddress());
			m_BanSet.Mutex.Unlock();

			Uint8 reason = Private::DisconnectType::Banned;
			pConnection->SendUnreliable(Private::PacketType::Disconnect, &reason, sizeof(reason));

			// Add the connection for cleanup
			AddConnectionForCleanup(pConnection);

			return true;
		}

		Bool Server::RemoveBan(const Address & p_Address)
		{
			// Check if the address is banned
			m_BanSet.Mutex.Lock();
			AddressSet::iterator it = m_BanSet.Value.find(p_Address);
			if (it == m_BanSet.Value.end())
			{
				// Unlock the ban set mutex.
				m_BanSet.Mutex.Unlock();
				return false;
			}

			// Remove the address from the set.
			m_BanSet.Value.erase(it);

			// Unlock the ban set mutex.
			m_BanSet.Mutex.Unlock();

			// Succeeded.
			return true;
		}

		void Server::BanIp(const Address & p_Address)
		{
			// Add addresss to ban set
			m_BanSet.Mutex.Lock();
			m_BanSet.Value.insert(p_Address);
			m_BanSet.Mutex.Unlock();

			// Go through users with this address.
		}


		Bool Server::Start(const Properties & p_Properties)
		{
			// Open the udp socket.
			if (m_Socket.Open(p_Properties.HostPort) == false)
			{
				return false;
			}
			m_Socket.SetBlocking(true);

			// Set properties
			m_HostPort = p_Properties.HostPort;
			m_MaxConnections = p_Properties.MaxConnections;
			m_EntityUpdatesPerSecond = p_Properties.EntityUpdatesPerSecond;
			m_LosingConnectionTimeout.Set(p_Properties.LosingConnectionTimeout);
			m_Identifier = p_Properties.Identifier;

			// Add the free user IDs to the queue
			m_FreeUserIds.Mutex.Lock();
			for (Uint16 i = 0; i < static_cast<Uint16>(m_MaxConnections); i++)
			{
				m_FreeUserIds.Value.push(i);
			}
			m_FreeUserIds.Mutex.Unlock();

			// Create entity id queue.
			m_EntityManager.CreateEntityIdQueue(p_Properties.MaxEntities);

			// Create the packet memory pool
			m_pPacketMemoryPool = new MemoryPool<Uint8>(p_Properties.MaxConnections * 64, Private::NetBufferSize, true);

			// Start the server timer.
			m_ServerTimer.Get().Start();

			// Start the server thread.
			m_MainThread.Execute([this]()
			{
				Bool running = true;
				Address recvAddress;
				Uint64 recvPackedAddress = 0;
				Uint16 recvPort = 0;
				Int32 recvSize = 0;
				MemoryPool<Uint8>::Item * pItem = NULL;

				// Set the running flag to true.
				m_Running.Set(true);


				// Receive packets as long as the server is running.
				while (IsRunning())
				{
					// Get item from memory pool if needed.
					if (pItem == NULL)
					{
						if ((pItem = m_pPacketMemoryPool->Get()) == NULL)
						{
							bitLogNetErr("Null memory pool item.");
							continue;
						}
					}

					Uint8 * pBuffer = pItem->GetData();

					// Error check the item and it's data.
					if (pBuffer == NULL)
					{
						bitLogNetErr( "Null memory pool item data." );
						m_pPacketMemoryPool->Return(pItem);
						pItem = NULL; // Indicate to generate get a new item next loop.
						continue;
					}

					// Receive a packet.
					// Loop while the server is running and until we receive a valid packet.
					while (running = IsRunning())
					{
						if ((recvSize = m_Socket.Receive(pBuffer, Private::NetBufferSize, recvAddress, recvPort, Milliseconds(5))) >= Private::NetHeaderSize)
						{
							break;
						}
					}

					// Break if the server stopped running.
					if (running == false)
					{
						break;
					}

					// Get the packet type
					const Private::PacketType::eType packetType = Private::PacketTransfer::ParsePacketType(pBuffer[0]);

					// Error check the packet type, ignore unknown packets.
					if (packetType == Private::PacketType::Unknown)
					{
						continue;
					}

					// Get the packed address.
					recvPackedAddress = static_cast<Uint64>(recvAddress.GetAddress()) *
						static_cast<Uint64>(recvPort) +
						static_cast<Uint64>(recvPort);

					// Lock the connection mutex.
					m_ConnectionMutex.Lock();

					// Check if we received a packet from a connected client.
					AddressConnectionMap::iterator it = m_AddressConnections.find(recvPackedAddress);
					if (it != m_AddressConnections.end() && 
						packetType != Private::PacketType::Disconnect)
					{
						// Send acknowledgement if needed
						if (Private::PacketTransfer::ParseReliableFlag(pBuffer[0]))
						{
							const Uint8 oldTypeByte = pBuffer[0];
							pBuffer[0] = Private::PacketType::Acknowledgement;
							m_Socket.Send(pBuffer, Private::NetAcknowledgementPacketSize, recvAddress, recvPort);
							pBuffer[0] = oldTypeByte;
						}

						// Move the packet to the client thread.
						pItem->SetUsedSize(recvSize);
						it->second->AddMessage(pItem);
						m_ConnectionMutex.Unlock();

						pItem = NULL; // Indicate to generate get a new item next loop.
						continue; // Continue to next loop.
					}
					m_ConnectionMutex.Unlock();

					// Handle connections and disconnection messages.
					if (packetType == Private::PacketType::Connect ||
						packetType == Private::PacketType::Disconnect)
					{
						// We got a connection message. Add it to the queue.
						ConnectionMessage * pConnectionMessage = new ConnectionMessage;
						pConnectionMessage->pDataItem = pItem;
						pConnectionMessage->Address = recvAddress;
						pConnectionMessage->PackedAddress = recvPackedAddress;
						pConnectionMessage->Port = recvPort;
						pConnectionMessage->PacketType = packetType;

						AddConnectionMessage(pConnectionMessage);
						pItem = NULL;
						continue;
					}

					// The server's main loop is done here...
				}
			}
			);


			// Start the connection thread
			m_ConnectionThread.Execute([this]()
			{
				ConnectionMessage * pConnectionMessage = NULL;
				

				// Run as long the server is running.
				while (IsRunning())
				{
					
					// Poll message.
					if ((pConnectionMessage = PollConnectionMessage()) == NULL)
					{
						continue;
					}

					// Function for internally handling con/dis-connection messages.
					// We do this in order to make sure to return the memory pool item and delete message.
					std::function<void()> InternalHandleConnection = [this, pConnectionMessage]()
					{
						MemoryPool<Uint8>::Item * pItem = pConnectionMessage->pDataItem;
						Uint8 * pBuffer = pItem->GetData();
						Int32 recvSize = pItem->GetDataSize();
						Private::PacketType::eType packetType = pConnectionMessage->PacketType;
						Address recvAddress = pConnectionMessage->Address;
						Uint64 recvPackedAddress = pConnectionMessage->PackedAddress;
						Uint16 recvPort = pConnectionMessage->Port;

						// Handle connection packet.
						if (packetType == Private::PacketType::Connect)
						{
							// Error check the recv size, see if it's a connect packet.
							if (recvSize < Private::NetConnectPacketSize)
							{
								return;
							}

							// Check identifier.
							if (m_Identifier.size())
							{
								SizeType identifierLength = static_cast<SizeType>(pBuffer[3]);

								if (identifierLength)
								{
									// Make sure that the identifier is right.
									if (identifierLength != m_Identifier.size() &&
										memcmp(pBuffer + Private::NetConnectPacketSize, m_Identifier.data(), m_Identifier.size()) != 0)
									{
										// Send denied packet
										pBuffer[3] = Private::ConnectStatusType::Denied;
										m_Socket.Send(pBuffer, Private::NetRejectPacketSize, recvAddress, recvPort);
										
										return;
									}
								}
							}


							// Check if the address is banned
							m_BanSet.Mutex.Lock();
							if (m_BanSet.Value.find(recvAddress.GetAddress()) != m_BanSet.Value.end())
							{
								// Send ban packet
								pBuffer[3] = Private::ConnectStatusType::Banned;
								m_Socket.Send(pBuffer, Private::NetRejectPacketSize, recvAddress, recvPort);

								// Unlock the ban set mutex.
								m_BanSet.Mutex.Unlock();

								return;
							}
							m_BanSet.Mutex.Unlock();


							// Send Deny packet if the server is full.
							m_FreeUserIds.Mutex.Lock();
							SizeType freeUserIdsCount = m_FreeUserIds.Value.size();
							m_FreeUserIds.Mutex.Unlock();

							m_ConnectionMutex.Lock();
							const SizeType connectionCount = static_cast<SizeType>(m_AddressConnections.size());
							m_ConnectionMutex.Unlock();

							if (connectionCount == m_MaxConnections || freeUserIdsCount == 0)
							{
								pBuffer[3] = Private::ConnectStatusType::Full;
								m_Socket.Send(pBuffer, Private::NetRejectPacketSize, recvAddress, recvPort);

								return;
							}


							// Run the on pre connection function
							if (OnPreConnection(recvAddress, recvPort) == false)
							{
								pBuffer[3] = Private::ConnectStatusType::Denied;
								m_Socket.Send(pBuffer, Private::NetRejectPacketSize, recvAddress, recvPort);

								return;
							}


							// Get server time and send accept response
							const Uint64 serverTime = Hton64(GetServerTime().AsMicroseconds());
							pBuffer[3] = Private::ConnectStatusType::Accepted;
							memcpy(&(pBuffer[4]), &serverTime, sizeof(Uint64));
							m_Socket.Send(pBuffer, Private::NetAcceptPacketSize, recvAddress, recvPort);
							
							// Add the client to the connection maps
							m_ConnectionMutex.Lock();
							
							// Ignore already created connections.
							AddressConnectionMap::iterator conFindIt = m_AddressConnections.find(recvPackedAddress);
							if (conFindIt != m_AddressConnections.end())
							{
								m_ConnectionMutex.Unlock();
								return;
							}
							
							// Get a user id for this connection
							m_FreeUserIds.Mutex.Lock();
							const Uint16 userId = m_FreeUserIds.Value.front();
							m_FreeUserIds.Value.pop();
							m_FreeUserIds.Mutex.Unlock();

							// Create the connection
							Connection * pConnection = new Connection(recvAddress, recvPort, userId, m_LosingConnectionTimeout.Value);
							m_AddressConnections.insert(AddressConnectionMapPair(recvPackedAddress, pConnection));
							m_UserConnections.insert(UserConnectionMapPair(userId, pConnection));
							m_ConnectionMutex.Unlock();

							// Start client thread.
						///	pConnection->StartThreads(this);

							// Run the on post connection function.				
							OnPostConnection(userId);
						}
						// Handle disconnect packet.
						else
						{
							// Error check the recv size, see if it's a disconnect packet.
							if (recvSize < Private::NetDisconnectPacketSize)
							{
								return;
							}

							// Find the connection via user id.
							m_ConnectionMutex.Lock();
							AddressConnectionMap::iterator it = m_AddressConnections.find(recvPackedAddress);
							if (it == m_AddressConnections.end())
							{
								// Unlock the connection mutex.
								m_ConnectionMutex.Unlock();
								return;
							}

							// Get the connection
							Connection * pConnection = it->second;

							// Unlock the connection mutex.
							m_ConnectionMutex.Unlock();

							// Run the on disconnection function
							OnDisconnection(pConnection->GetUserId());

							// Add the connection for cleanup
							AddConnectionForCleanup(pConnection);
						}

					};


					// Handle the connection.
					InternalHandleConnection();

					// Cleanup
					m_pPacketMemoryPool->Return(pConnectionMessage->pDataItem);
					delete pConnectionMessage;
				}
			
			}
			);
			


			// Start the entity thread
			m_EntityThread.Execute([this]()
			{
				// Create an instance of a timestep
				Timestep timestep;

				// Run the 
				while (IsRunning())
				{
					if (m_EntityUpdatesPerSecond == 0)
					{
						Sleep(Microseconds(1000));
						continue;
					}

					// Calculate the timestep time
					Time time = Seconds(1.0f / static_cast<Float32>(m_EntityUpdatesPerSecond));

					// Execute the timestep
					timestep.Execute(time, [this]()
					{
						// Get client count.
						m_ConnectionMutex.Lock();
						SizeType clientCount = m_UserConnections.size();

						// Exit the timestep.
						if (clientCount == 0)
						{
							m_ConnectionMutex.Unlock();
							return;
						}

						// Do pre entity message allocations
						// Create a temporary client group struct.
						struct ClientGroup
						{
							std::set<Uint32>							Groups;
							ServerEntityManager::EntityMessageVector	UnreliableMessages;
							ServerEntityManager::EntityMessageVector	ReliableMessages;
						};

						typedef std::vector<ClientGroup*> ClientGroupVector;
						ClientGroupVector clientGroups;

						// Get temporary connection list
						std::list<Connection*> tempConnections;
						for (UserConnectionMap::iterator itFirst = m_UserConnections.begin();
							itFirst != m_UserConnections.end();
							itFirst++)
						{
							if (itFirst->second->m_Groups.Get().size())
							{
								tempConnections.push_back(itFirst->second);
							}
						}

						if (tempConnections.size() == 0)
						{
							m_ConnectionMutex.Unlock();
							return;
						}

						// Continue until the temp connections are empty.
						while(tempConnections.size())
						{
							Connection * curConnection = (*tempConnections.begin());
							tempConnections.erase(tempConnections.begin());

							clientGroups.push_back( new ClientGroup);
							ClientGroup * pCurClientGroup = clientGroups[clientGroups.size() - 1];
							pCurClientGroup->Groups = curConnection->m_Groups.Get();
							curConnection->SetTempEntityMessagePtr(reinterpret_cast<void*>(pCurClientGroup));

							for (std::list<Connection*>::iterator itLast = tempConnections.begin();
								itLast != tempConnections.end();)
							{

								// Found an match, Add to client groups and remove it from the temp connections list.
								if (pCurClientGroup->Groups == (*itLast)->m_Groups.Get())
								{
									(*itLast)->SetTempEntityMessagePtr(reinterpret_cast<void*>(pCurClientGroup));
									itLast = tempConnections.erase(itLast);
								}
								else
								{
									// Keep on searching.
									itLast++;
								}
							}
						}
						
						m_ConnectionMutex.Unlock();
						


						// The pre allocations are done...
						// Create the messages. Multithread.
						const SizeType threadCount = clientGroups.size();
						Thread * pMessageCreationThreads = new Thread[threadCount];
						for (SizeType i = 0; i < threadCount; i++)
						{
							// Run thread.
							ClientGroup * pCurClientGroup = clientGroups[i];
							pMessageCreationThreads->Execute([this, pCurClientGroup]()
							{
								// Create the entity message.
								m_EntityManager.CreateEntityMessage(pCurClientGroup->Groups, pCurClientGroup->ReliableMessages, pCurClientGroup->UnreliableMessages);
							});
						}

						// Wait until all messages are created... then delete all threads as well.
						for (SizeType i = 0; i < threadCount; i++)
						{
							pMessageCreationThreads[i].Finish();
						}
						delete[] pMessageCreationThreads;


						// Send the messages to the clients.

						m_ConnectionMutex.Lock();

						for (UserConnectionMap::iterator it = m_UserConnections.begin();
						it != m_UserConnections.end();
							it++)
						{
							// Get current client group
							ClientGroup * pCurClientGroup = reinterpret_cast<ClientGroup *>(it->second->m_pEntityMessageDataPtr);
							if (pCurClientGroup == NULL)
							{
								// Skip this client, no data were found to send.
								continue;
							}

							// Set the temp entity message pointer to NULL.
							it->second->SetTempEntityMessagePtr(NULL);

							// Send unrealiable message.
							for (ServerEntityManager::EntityMessageVector::size_type i = 0; i < pCurClientGroup->UnreliableMessages.size(); i++)
							{
								ServerEntityManager::EntityMessage message = pCurClientGroup->UnreliableMessages[i];
								it->second->SendUnreliable(Private::PacketType::EntityMessage, reinterpret_cast<Uint8 *>(message.pData), message.Size);
							}

							// Send Realiable message.
							for (ServerEntityManager::EntityMessageVector::size_type i = 0; i < pCurClientGroup->UnreliableMessages.size(); i++)
							{
								ServerEntityManager::EntityMessage message = pCurClientGroup->UnreliableMessages[i];
								it->second->SendReliable(Private::PacketType::EntityMessage, reinterpret_cast<Uint8 *>(message.pData), message.Size);
							}
						}

						m_ConnectionMutex.Unlock();
						


						// Clean up client groups
						for (SizeType i = 0; i < threadCount; i++)
						{
							delete clientGroups[i];
						}
						clientGroups.clear();
					}
					);

				}
			}
			);

			// Start the cleanup thread.
			m_CleanupConnectionThread.Execute([this]()
			{
				while (IsRunning())
				{
					// Wait for semaphore to trigger
					m_CleanupConnectionSemaphore.Wait();

					// Lock the cleanup connections mutex
					m_CleanupConnections.Mutex.Lock();

					// Error check the queue
					if (m_CleanupConnections.Value.size() == 0)
					{
						m_CleanupConnections.Mutex.Unlock();
						continue;
					}

					// Get the connection
					Connection * pConnection = m_CleanupConnections.Value.front();

					// Error check the connection pointer
					if (pConnection == NULL)
					{
						m_CleanupConnections.Mutex.Unlock();
						continue;
					}

					// Get the packet address and user id.
					const Uint64 packetAddress = pConnection->GetDestinationPackedAddress();
					const Uint16 userId = pConnection->GetUserId();

					// Delete the connection
					delete pConnection;

					// Pop the connection from the list
					m_CleanupConnections.Value.pop_front();

					// Unlock mutex
					m_CleanupConnections.Mutex.Unlock();

					// Remove the connection and wait for the connection to thread to finish
					m_ConnectionMutex.Lock();

					// Erase the connection from the address connections
					AddressConnectionMap::iterator it1 = m_AddressConnections.find(packetAddress);
					if (it1 != m_AddressConnections.end())
					{
						m_AddressConnections.erase(it1);
					}

					// Erase the connection from the user id connections
					UserConnectionMap::iterator it2 = m_UserConnections.find(userId);
					if (it2 != m_UserConnections.end())
					{
						m_UserConnections.erase(it2);
					}

					// Restore the user id to the free user id queue
					m_FreeUserIds.Mutex.Lock();
					m_FreeUserIds.Value.push(userId);
					m_FreeUserIds.Mutex.Unlock();

					// Unlock the connection mutex
					m_ConnectionMutex.Unlock();
				}

			}
			);


			// Server is now running
			return true;
		}

		void Server::Stop()
		{
			if (IsRunning())
			{
				/// Set the running flag to false.
				m_Running.Mutex.Lock();
				m_Running.Value = false;
				m_Running.Mutex.Unlock();

				/// Wait For the threads to finish...
				m_MainThread.Finish();
				m_EntityThread.Finish();

				m_ConnectionSemaphore.Release();
				m_ConnectionThread.Finish();

				m_CleanupConnectionSemaphore.Release();
				m_CleanupConnectionThread.Finish();

				// Connection message queue.
				m_ConnectionMessages.Mutex.Lock();
				while (m_ConnectionMessages.Value.size())
				{
					ConnectionMessage * pMessage = m_ConnectionMessages.Value.front();
					m_pPacketMemoryPool->Return(pMessage->pDataItem);
					delete pMessage;
					m_ConnectionMessages.Value.pop();
				}
				m_ConnectionMessages.Mutex.Unlock();

				// Cleanup connection queue.
				m_CleanupConnections.Mutex.Lock();
				m_CleanupConnections.Value.clear();
				m_CleanupConnections.Mutex.Unlock();

				// Disconnect all the connections
				Uint8 closedMessage = static_cast<Uint8>(Private::DisconnectType::Closed);
				m_ConnectionMutex.Lock();
				for(AddressConnectionMap::iterator it = m_AddressConnections.begin();
					it != m_AddressConnections.end();
					it++)
				{
					// Send disconnect message.
					it->second->SendUnreliable(Private::PacketType::Disconnect, &closedMessage, 1);

					// Run the on disconnection function
					const Uint16 userId = it->second->GetUserId();
					m_ConnectionMutex.Unlock(); // Unlock the connection mutex to prevent locks.
					OnDisconnection(userId);
					m_ConnectionMutex.Lock();// Lock the connection mutex again

					// Delete the connection pointer.
					// The destructor will cleanup and return all the data for us.
					delete it->second;
				}

				// Clean connection connections
				m_AddressConnections.clear();
				m_UserConnections.clear();

				// Unlock the mutex
				m_ConnectionMutex.Unlock();

				// Delete the packet memory pool
				delete m_pPacketMemoryPool;

				// Close the host socket
				m_Socket.Close();
			}
		}

		Bool Server::IsRunning()
		{
			m_Running.Mutex.Lock();
			Bool running = m_Running.Value;
			m_Running.Mutex.Unlock();

			return running;
		}

		Time Server::GetServerTime()
		{
			m_ServerTimer.Mutex.Lock();
			Time serverTime = m_ServerTimer.Value.GetLapsedTime();
			m_ServerTimer.Mutex.Unlock();

			return serverTime;
		}

		Bool Server::HookUserMessage(UserMessageListener * p_pListener, const std::string & m_MessageName)
		{
			// Error check the parameters
			if (p_pListener == NULL || m_MessageName.size() == 0)
			{
				return false;
			}

			m_UserMessageListeners.Mutex.Lock();

			// Find the user mesage set pointer.
			UserMessageListenerSet * pMessageSet = NULL;

			// Find the message
			UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.find(m_MessageName);
			if (it == m_UserMessageListeners.Value.end())
			{
				// Create a new message
				pMessageSet = new UserMessageListenerSet;
				m_UserMessageListeners.Value.insert(UserMessageListenerPair(m_MessageName, pMessageSet));
			}
			else
			{
				pMessageSet = it->second;
			}

			// Add the listener to the message set
			pMessageSet->insert(p_pListener);

			m_UserMessageListeners.Mutex.Unlock();

			return true;
		}

		void Server::UnhookUserMessages()
		{
			m_UserMessageListeners.Mutex.Lock();

			for (UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.begin();
				it != m_UserMessageListeners.Value.end();
				it++)
			{
				delete it->second;
			}
			m_UserMessageListeners.Value.clear();


			m_UserMessageListeners.Mutex.Unlock();
		}

		Uint16 Server::GetHostPort()
		{
			return m_HostPort;
		}

		SizeType Server::GetMaxConnectionsCount()
		{
			return m_MaxConnections;
		}

		SizeType Server::GetConnectionCount()
		{
			// Get the count.
			m_ConnectionMutex.Lock();
			SizeType count = m_AddressConnections.size();
			m_ConnectionMutex.Unlock();

			// Return the count.
			return count;
		}

		void Server::AddConnectionForCleanup( Connection * p_pConnection )
		{
			// Add the connection to the cleanup thread.
			m_CleanupConnections.Mutex.Lock( );

			// Go throguh the cleanup connection and make sure that the connection is unique
			for( ConnectionList::iterator it = m_CleanupConnections.Value.begin( );
				 it != m_CleanupConnections.Value.end( );
				 it++ )
			{
				if( p_pConnection == (*it) )
				{
					m_CleanupConnections.Mutex.Unlock();
					return;
				}
			}

			m_CleanupConnections.Value.push_back( p_pConnection );
			m_CleanupConnections.Mutex.Unlock( );

			// Increase the semaphore for cleanups
			m_CleanupConnectionSemaphore.Release( );
		}

		void Server::AddConnectionMessage(ConnectionMessage * p_pConnectionMessage)
		{
			m_ConnectionMessages.Mutex.Lock();
			m_ConnectionMessages.Value.push(p_pConnectionMessage);
			m_ConnectionMessages.Mutex.Unlock();
			m_ConnectionSemaphore.Release();
		}

		Server::ConnectionMessage * Server::PollConnectionMessage()
		{
			m_ConnectionSemaphore.Wait();

			m_ConnectionMessages.Mutex.Lock();

			if (m_ConnectionMessages.Value.size() == 0)
			{
				m_ConnectionMessages.Mutex.Unlock();
				return NULL;
			}

			ConnectionMessage * pMessage = m_ConnectionMessages.Value.front();
			m_ConnectionMessages.Value.pop();

			m_ConnectionMessages.Mutex.Unlock();
			return pMessage;
		}

	}

}