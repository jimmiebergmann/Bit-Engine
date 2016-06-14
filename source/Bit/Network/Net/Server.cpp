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
			Port(0),
			MaxConnections(255),
			LosingConnectionTimeout(Seconds(3.0f)),
			EntityUpdatesPerSecond(22),
			MaxEntities(2048),
			Identifier("Bit Engine Network")
		{
		}

		Server::Properties::Properties(const Uint16 p_Port,
										const Uint8 p_MaxConnections,
										const Time & p_LosingConnectionTimeout,
										const Uint8 p_EntityUpdatesPerSecond,
										const Uint16 p_MaxEntities,
										const std::string & p_Identifier) :
			Port(p_Port),
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
			m_PacketMemoryPool(NULL),
			m_MaxPacketSize(2048)
		{

		}

		Server::~Server()
		{
			Stop();
			UnhookUserMessages();
		}

		Bool Server::OnPreConnection(	const Address & p_Address,
										const Uint16 p_Port,
										std::vector<Uint32> & p_EntityGroups)
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


			/*
			// Create the entity message
			std::vector<Uint8> message;
			if (m_EntityManager.CreateFullEntityMessage(message, false) == false)
			{
				return;
			}

			// Error check the message
			if (message.size() == 0)
			{
				return;
			}

			// Send realiable message.
			it->second->SendReliable(PacketType::EntityUpdate, reinterpret_cast<Uint8 *>(message.data()), message.size(), true);
			*/
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
			m_BanSet.Value.insert(pConnection->GetAddress());
			m_BanSet.Mutex.Unlock();

			Uint8 reason = DisconnectType::Banned;
			pConnection->SendUnreliable(PacketType::Disconnect, &reason, sizeof(reason), false, false);

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
		}


		Bool Server::Start(const Properties & p_Properties)
		{
			// Open the udp socket.
			if (m_Socket.Open(p_Properties.Port) == false)
			{
				return false;
			}
			m_Socket.SetBlocking(true);

			// Set properties
			m_Port = p_Properties.Port;
			m_MaxConnections = p_Properties.MaxConnections;
			m_EntityUpdatesPerSecond = p_Properties.EntityUpdatesPerSecond;
			m_LosingConnectionTimeout.Set(p_Properties.LosingConnectionTimeout);
			m_Identifier = p_Properties.Identifier;

			// Add the free user IDs to the queue
			for (Uint16 i = 0; i < static_cast<Uint16>(m_MaxConnections); i++)
			{
				m_FreeUserIds.push(i);
			}

			// Create entity id queue.
			m_EntityManager.CreateEntityIdQueue(p_Properties.MaxEntities);

			// Create the packet memory pool
			m_PacketMemoryPool.Set(new MemoryPool<Uint8>(p_Properties.MaxConnections * 64, m_MaxPacketSize, true));

			// Start the server timer.
			m_ServerTimer.Get().Start();

			// Start the server thread.
			m_MainThread.Execute([this]()
			{
				Address address;
				Uint16 port = 0;
				Int16 recvSize = 0;

				// Set the running flag to true.
				m_Running.Mutex.Lock();
				m_Running.Value = true;
				m_Running.Mutex.Unlock();

				// Receive packets as long as the server is running.
				while (IsRunning())
				{
					// Get item from memory pool

					m_PacketMemoryPool.Mutex.Lock();
					MemoryPool<Uint8>::Item *pItem = m_PacketMemoryPool.Get()->Get();
					m_PacketMemoryPool.Mutex.Unlock();


					Uint8 * pBuffer = pItem->GetData();

					// Error check the item and it's data.
					if (pItem == NULL || pBuffer == NULL)
					{
						bitLogNetErr(  "Null memory pool item." );
						continue;
					}

					// Receive a packet.
					// Loop while the server is running and until we receive a valid packet.
					while (IsRunning())
					{
						if ((recvSize = m_Socket.Receive(pBuffer, m_MaxPacketSize, address, port, Milliseconds(5))) > 0)
						{
							break;
						}
					}

					// get the client address as an index.
					Uint64 clientAddress = static_cast<Uint64>(address.GetAddress()) *
						static_cast<Uint64>(port)+
						static_cast<Uint64>(port);

					// Lock the connection mutex.
					m_ConnectionMutex.Lock();

					// Check if we received a packet from a connected client.
					AddressConnectionMap::iterator it = m_AddressConnections.find(clientAddress);
					if (it != m_AddressConnections.end())
					{
						// Send the packet to the client thread.
						pItem->SetUsedSize(recvSize);
						it->second->AddReceivedData(pItem);
						m_ConnectionMutex.Unlock();
						continue;
					}

					// Use a do while loop with false condition in order to 
					// jump over and skip code later.
					do
					{
						// This is not a packet from an already connected client,
						// check if the client is tryin go connect.
						if (pBuffer[0] == PacketType::Connect)
						{
							// Make sure that the identifier is right.
							if (recvSize != m_Identifier.size() + ConnectPacketSize)
							{
								// Go to the return packet label.
								break;
							}
							if (memcmp(pBuffer + ConnectPacketSize, m_Identifier.data(), m_Identifier.size()) != 0)
							{
								// Go to the return packet label.
								break;
							}

							// Check if the address is banned
							m_BanSet.Mutex.Lock();
							if (m_BanSet.Value.find(address.GetAddress()) != m_BanSet.Value.end())
							{
								// Send ban packet
								pBuffer[0] = PacketType::Reject;
								pBuffer[1] = RejectType::Banned;
								m_Socket.Send(pBuffer, RejectPacketSize, address, port);

								// Unlock the ban set mutex.
								m_BanSet.Mutex.Unlock();

								// Go to the return packet label.
								break;
							}
							m_BanSet.Mutex.Unlock();

							// Send Deny packet if the server is full.
							if (m_AddressConnections.size() == m_MaxConnections || m_FreeUserIds.size() == 0)
							{
								pBuffer[0] = PacketType::Reject;
								pBuffer[1] = RejectType::Full;
								m_Socket.Send(pBuffer, RejectPacketSize, address, port);

								// Go to the return packet label.
								break;
							}

							// Run the on pre connection function
							std::vector<Uint32> groupVector;
							if (OnPreConnection(address, port, groupVector) == false)
							{
								// SEND REJECT MESSAGE HERE PLEASE.
								pBuffer[0] = PacketType::Reject;
								pBuffer[1] = RejectType::Full;
								m_Socket.Send(pBuffer, RejectPacketSize, address, port);

								break;
							}


							// Answer the client with an accept packet.
							pBuffer[0] = PacketType::Accept;

							// Get server time
							m_ServerTimer.Mutex.Lock();
							Uint64 serverTime = m_ServerTimer.Value.GetLapsedTime().AsMicroseconds();
							m_ServerTimer.Mutex.Unlock();

							Uint64 nServerTime = Hton64(serverTime);
							memcpy(&(pBuffer[3]), &nServerTime, sizeof(Uint64));

							m_Socket.Send(pBuffer, AcceptPacketSize, address, port);

							// Get a user id for this connection
							const Uint16 userId = m_FreeUserIds.front();
							m_FreeUserIds.pop();

							// Create the connection
							Connection * pConnection = new Connection(address, port, userId, m_LosingConnectionTimeout.Value);

							// Add the client to the address connection map
							m_AddressConnections.insert(AddressConnectionMapPair(clientAddress, pConnection));

							// Add the client to the user connection map
							m_UserConnections.insert(UserConnectionMapPair(userId, pConnection));

							// Start client thread.
							pConnection->StartThreads(this);

							// Run the on post connection function.
							// Release the connection mutex as well to let the user send messages and such.
							m_ConnectionMutex.Unlock();
							OnPostConnection(userId);
							m_ConnectionMutex.Lock();
						}
						// Ping packet.
						else if (pBuffer[0] == PacketType::Ping && recvSize == PingPacketSize)
						{
							// Send back the ping packet.
							m_Socket.Send(pBuffer, PingPacketSize, address, port);
						}

					} while (false);

					// Unlock the connection mutex.
					m_ConnectionMutex.Unlock();

					// Return the item to the memory pool.
					m_PacketMemoryPool.Mutex.Lock();
					m_PacketMemoryPool.Value->Return(pItem);
					m_PacketMemoryPool.Mutex.Unlock();
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
							Thread										FactoryThread;
							std::vector<Connection*>					Connections;
							std::set<Uint32>							Groups;
							ServerEntityManager::EntityMessageVector	UnreliableMessages;
							ServerEntityManager::EntityMessageVector	ReliableMessages;
						};

						std::vector<ClientGroup> clientGroups;

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
							std::list<Connection*>::iterator itFirst = tempConnections.begin();


							clientGroups.push_back(ClientGroup());
							ClientGroup & curClientGroup = clientGroups[clientGroups.size() - 1];
							curClientGroup.Groups = (*itFirst)->m_Groups.Get();
							curClientGroup.Connections.push_back((*itFirst));

							for (std::list<Connection*>::iterator itLast = std::next(itFirst);
								itLast != tempConnections.end();)
							{

								// Found an match, Add to client groups and remove it from the temp connections list.
								if ((*itFirst)->m_Groups.Value == (*itLast)->m_Groups.Value)
								{
									curClientGroup.Connections.push_back((*itLast));
									(*itLast)->SetTempEntityMessagePtr(reinterpret_cast<void*>(&curClientGroup));


									itLast = tempConnections.erase(itLast);
								}
								else
								{
									// Keep on searching.
									itLast++;
								}
							}



						}

						

						// The pre allocations are done...



						// Send the messages to the clients.


						/*
							for(auto i = list.begin(); i != list.end();)
							{
								if(condition)
									i = list.erase(i);
								else
									++i;
							}
						*/



						// Find dublicates of group sets in connections
						/*for (UserConnectionMap::iterator itFirst = m_UserConnections.begin();
							itFirst != m_UserConnections.end();
							itFirst++)
						{

							for (UserConnectionMap::iterator itLast = std::next(itFirst);
								itLast != m_UserConnections.end();
								itLast++)
							{

							}
							
						}*/

						m_ConnectionMutex.Unlock();

						/*

						// Create a semaphore to wait for all threads.
						Semaphore threadSemaphore;
						threadSemaphore.Release(clientCount);

						// Create a temporary struct.
						struct MessageFactoryItem
						{
							Thread				FactoryThread;
							std::set<Uint32>	Groups;
						};


						// Go through all the clients
						for (UserConnectionMap::iterator it = m_UserConnections.begin();
							it != m_UserConnections.end();
							it++)
						{

							//ServerEntityManager::EntityMessageVector vec;

							// Create the entity message
							//m_EntityManager.CreateEntityMessage(it->second->m_Groups.Value, vec, vec);
						

						}

						m_ConnectionMutex.Unlock();


						// Wait for semaphore, all threads needs to terminate.
						threadSemaphore.Wait();
						*/


						/*
						// Create the entity message
						std::vector<Uint8> message;
						if (m_EntityManager.CreateEntityMessage(message, false) == false)
						{
							return;
						}

						// Error check the message
						if (message.size() == 0)
						{
							return;
						}

						// Send the message to all the connections
						m_ConnectionMutex.Lock();

						for (UserConnectionMap::iterator it = m_UserConnections.begin();
							it != m_UserConnections.end();
							it++)
						{
							// Should we send entity messages to this client?
							if (it->second->m_SendEntityMessages.Get())
							{
								// Send unrealiable message.
								it->second->SendReliable(PacketType::EntityUpdate, reinterpret_cast<Uint8 *>(message.data()), message.size(), true);
							}
						}

						m_ConnectionMutex.Unlock();
						*/
					}
					);

				}
			}
			);

			// Start the cleanup thread.
			m_CleanupThread.Execute([this]()
			{
				while (IsRunning())
				{
					// Wait for semaphore to trigger
					m_CleanupSemaphore.Wait();

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
					const Uint64 packetAddress = pConnection->GetPackedAddress();
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
					m_FreeUserIds.push(userId);

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
				// Set the running flag to false.
				m_Running.Mutex.Lock();
				m_Running.Value = false;
				m_Running.Mutex.Unlock();

				// Wait for the entity thread to finish
				m_EntityThread.Finish();

				// Wait for the cleanup thread to finish
				m_CleanupSemaphore.Release();
				m_CleanupThread.Finish();

				// Clean up the cleanup connections
				m_CleanupConnections.Mutex.Lock();
				m_CleanupConnections.Value.clear();
				m_CleanupConnections.Mutex.Unlock();


				// Wait for the main thread to finish
				m_MainThread.Finish();

				// Disconnect all the connections
				m_ConnectionMutex.Lock();
				AddressConnectionMap::iterator it = m_AddressConnections.begin();
				while (it != m_AddressConnections.end())
				{
					// Disconnect and delete the connection.
					Connection * pConnection = it->second;
					delete pConnection;

					// Erase the connection
					m_AddressConnections.erase(it);

					// Get the beginning again.
					it = m_AddressConnections.begin();
				}

				// Remove all the user connections
				m_UserConnections.clear();

				// Unlock the mutex
				m_ConnectionMutex.Unlock();

				// Delete the packet memory pool
				if (m_PacketMemoryPool.Get())
				{
					delete m_PacketMemoryPool.Get();
				}

				// Close the sockets
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
			Time serverTime;

			m_ServerTimer.Mutex.Lock();
			serverTime = m_ServerTimer.Value.GetLapsedTime();
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
			return m_Port;
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
					return;
				}
			}

			m_CleanupConnections.Value.push_back( p_pConnection );
			m_CleanupConnections.Mutex.Unlock( );

			// Increase the semaphore for cleanups
			m_CleanupSemaphore.Release( );
		}

	}

}