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

#include <Bit/Network/Net/Client.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/Randomizer.hpp>
#include <Bit/System/Log.hpp>
#include <Memory>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Client::Client(	const Uint16 p_SourcePort,
						const Time & p_InitialPing) :
			PacketTransfer(0, 0),
			m_EntityManager(this),
			m_SrcPort(p_SourcePort),
			m_Connected(false),
			m_LosingConnectionTimeout(Seconds(3.0f)),
			m_Ping(p_InitialPing),
			m_pPacketMemoryPool(NULL)
		{

		}

		Client::~Client()
		{
			// Disconnect the client.
			InternalDisconnect();

			// Clear the user message listeners
			m_HostMessageListeners.Mutex.Lock();
			for (HostMessageListenerMap::iterator it = m_HostMessageListeners.Value.begin();
				it != m_HostMessageListeners.Value.end();
				it++)
			{
				delete it->second;
			}
			m_HostMessageListeners.Value.clear();
			m_HostMessageListeners.Mutex.Unlock();

		}

/*		Bool Client::PingServer(	const Address & p_ServerAddress,
									const Uint16 p_ServerPort,
									Time & p_PingTime,
									const Time & p_Timeout)
		{
			// Open the ping socket
			UdpSocket udp;
			if (udp.Open() == false)
			{
				return false;
			}

			// create the ping data.
			Uint8 pingData[Private::NetPingPacketSize];
			const Uint8 sequence = static_cast<Uint8>(RandomizeNumber(0, 255));	// Create an random sequence.
			pingData[0] = Private::PacketType::Ping;
			pingData[1] = sequence;

			// Send the ping data
			if (udp.Send(pingData, Private::NetPingPacketSize, p_ServerAddress, p_ServerPort) != Private::NetPingPacketSize)
			{
				p_PingTime = Time::Zero;
				return false;
			}

			// Start a ping timer
			Timer timer;
			timer.Start();

			// Receive data.
			Bool received = false;
			Time timeout = p_Timeout;
			while (received == false && timeout != Time::Zero)
			{

				Address fromAddress;
				Uint16 fromPort;
				if (udp.Receive(pingData, Private::NetPingPacketSize, fromAddress, fromPort, timeout) != Private::NetPingPacketSize)
				{
					timeout = p_Timeout - timer.GetLapsedTime();
					continue;
				}

				// Error check port and
				if (fromAddress != p_ServerAddress || fromPort != p_ServerPort ||
					pingData[0] != Private::PacketType::Ping || pingData[1] != sequence)
				{
					timeout = p_Timeout - timer.GetLapsedTime();
					continue;
				}

				// We received our response.
				received = true;

				// Break the loop, we got out ping.
				break;
			}

			// Set the ping time.
			timer.Stop();
			p_PingTime = timer.GetTime();

			// Succeeded.
			return received;
		}
*/

		void Client::OnDisconnection(DisconnectReason::eReason p_Reason)
		{
		}

		Client::eStatus Client::Connect(const Address & p_Address,
			const Uint16 p_Port,
			const Time & p_ConnectionTimeout,
			const Time & p_LosingConnectionTimeout,
			const Time & p_InterpolationTime,
			const Time & p_ExtrapolationTime,
			const std::string & p_Identifier)
		{
			// Do not try to connect again if we already are connected to a server.
			if (m_Connected.Get())
			{
				return AlreadyConnected;
			}

			// Open the socket.
			if (m_Socket.Open(m_SrcPort) == false)
			{
				return SocketError;
			}

			// Set blocking and return true.
			m_Socket.SetBlocking(true);

			// Internally try to connect
			m_DstAddress = p_Address;
			m_DstPort = p_Port;
			m_ServerIdentifier = p_Identifier;
			eStatus connectStatus = eStatus::Unknown;
			if (InternalConnect(p_ConnectionTimeout, connectStatus) == false)
			{
				return connectStatus;
			}

			// Initialize the ping list
			for (SizeType i = 0; i < 5; i++)
			{
				m_PingList.push_front(m_Ping.Value);
			}

			// Set lost connection timeout
			m_LosingConnectionTimeout.Set(p_LosingConnectionTimeout);

			// Set interpolation and extrapolation time.
			m_EntityManager.SetInterpolationTime(p_InterpolationTime);
			m_EntityManager.SetExtrapolationTime(p_ExtrapolationTime);

			m_DisconnectReason.Set(DisconnectReason::DisconnectByClient);

			// Create the packet memory pool
			m_pPacketMemoryPool = new MemoryPool<Uint8>(128, Private::NetBufferSize, true);

			// Set the connected flag to true.
			m_Connected.Set(true);

			// Start the client thread.
			m_Thread.Execute([this]()
			{
				MemoryPool<Uint8>::Item * pItem = NULL;
				Address recvAddress;
				Uint16 recvPort = 0;
				Int32 recvSize = 0;


				// Start the timer for the last recv packet.
				RestartLastSentPacketTimer();
				RestartLastReceivedPacketTimer();

				// Receive packets as long as we are connected.
				while (IsConnected())
				{
					if (pItem == NULL)
					{
						pItem = m_pPacketMemoryPool->Get();
					}
					Uint8 * pBuffer = pItem->GetData();

					// Receive any packet.
					// Ignore too small packets.
					if ((recvSize = m_Socket.Receive(pBuffer, Private::NetBufferSize, recvAddress, recvPort, Microseconds(1000))) < Private::NetHeaderSize)
					{
						continue;
					}

					// Set used data size.
					pItem->SetUsedSize(recvSize);

					// Make sure that the packet is from the server
					if (recvAddress != m_DstAddress || recvPort != m_DstPort)
					{
						continue;
					}

					// Reset the time for checking last time a packet arrived
					RestartLastReceivedPacketTimer();

					// Get the packet type
					const Private::PacketType::eType packetType = Private::PacketTransfer::ParsePacketType(pBuffer[0]);

					// Error check the packet type, ignore unknown packets.
					if (packetType == Private::PacketType::Unknown)
					{
						continue;
					}

					// Get reliable flag and send acknowledgement if needed.
					const Bool reliableType = Private::PacketTransfer::ParseReliableFlag(pBuffer[0]);
					if (reliableType)
					{
						const Uint8 oldTypeByte = pBuffer[0];
						pBuffer[0] = Private::PacketType::Acknowledgement;
						m_Socket.Send(pBuffer, Private::NetAcknowledgementPacketSize, recvAddress, recvPort);
						pBuffer[0] = oldTypeByte;
					}

					// Add sequence to sequence manager,
					// ignore the packet if it's already received...
					const Uint16 sequence = Private::PacketTransfer::ReadNtoh16FromBuffer(pBuffer + 1);
					if (m_SequenceManager.AddSequence(sequence) == false)
					{
						continue;
					}

					// Check the packet type
					switch (packetType)
					{
						// The server disconnected.
						case Private::PacketType::Disconnect:
						{
							if (recvSize != Private::NetDisconnectPacketSize)
							{
								break;
							}


							const Uint8 reasonByte = pBuffer[3];
							if (reasonByte >= Private::NetDisconnectTypeCount)
							{
								m_DisconnectReason.Set(DisconnectReason::Unknown);
								break;
							}
							else
							{
								const Private::DisconnectType::eType disconnectType = static_cast<Private::DisconnectType::eType>(reasonByte);

								switch (disconnectType)
								{
								case Private::DisconnectType::Closed:
									m_DisconnectReason.Set(DisconnectReason::Closed);
									break;
								case Private::DisconnectType::Kicked:
									m_DisconnectReason.Set(DisconnectReason::Kicked);
									break;
								case Private::DisconnectType::LostConnection:
									m_DisconnectReason.Set(DisconnectReason::LostConnection);
									break;
								case Private::DisconnectType::Banned:
									m_DisconnectReason.Set(DisconnectReason::Banned);
									break;
								default:
									break;
								};
							}

							// Set connected to false.
							m_Connected.Set(false);
						}
						break;
/*						// Alive packet from server. WE ARE ALREADY SENDING ACKNOWLEDGEMENT PACKETS.
						case Private::PacketType::Alive:
						{
							if (recvSize != Private::NetAlivePacketSize)
							{
								break;
							}

							// Send acknowledgement packet.
							// We're not using the SendUnreliable function here in order to speed things up.
							buffer[0] = Private::PacketType::Acknowledgement;
							m_Socket.Send(buffer, Private::NetAcknowledgementPacketSize, m_DstAddress, m_DstPort);
						}
						break;*/
						// ACK packet from server.
						case Private::PacketType::Acknowledgement:
						{
							if (recvSize != Private::NetAcknowledgementPacketSize)
							{
								break;
							}

							// Remove the reliable packet.
							Bool wasResent = false;
							Time timeSinceSent;
							if (RemoveReliablePacket(sequence, wasResent, timeSinceSent))
							{
								// Calculate the new ping from the lapsed time if it's not a resent packet
								if (wasResent == false)
								{
									CalculateNewPing(timeSinceSent);
								}
							}
						}
						break;
						
						/*case PacketType::EntityUpdate:
						{
							// Error check the recv size
							if (recvSize <= EntityUpdatePacketSize)
							{
								continue;
							}

							// Check the reliable flag
							if (buffer[PacketTypeSize + SequenceSize] == ReliabilityType::Reliable)
							{
								// Use the already allocated packet, change the type
								buffer[0] = PacketType::Acknowledgement;

								// Send the ack packet
								m_Socket.Send(buffer, AcknowledgementPacketSize, m_ServerAddress, m_ServerPort);
							}

							// Get the sequence
							const Uint16 sequence = Ntoh16(static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
								static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

							// Add the packets sequence to the sequence manager, do not handle the packet
							// if we've already received a packet with the same sequence.
							if (m_SequenceManager.AddSequence(sequence) && AddEntityUpdateSequence(sequence))
							{
								m_EntityManager.ParseEntityMessage(sequence, buffer + EntityUpdatePacketSize, recvSize - EntityUpdatePacketSize);
							}

						}
						break;
						case PacketType::EntityDestroyed:
						{
							// Error check the recv size
							/*if (recvSize < EntityDestroyedPacketSize)
							{
								continue;
							}

							if (buffer[PacketTypeSize + SequenceSize] == ReliabilityType::Reliable)
							{
								// Use the already allocated packet, change the type
								buffer[0] = PacketType::Acknowledgement;

								// Send the ack packet
								m_Socket.Send(buffer, AcknowledgementPacketSize, m_ServerAddress, m_ServerPort);
							}

							// Get the sequence
							const Uint16 sequence = Ntoh16(	static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
															static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

							// Error check the sequence and add it to the sequence manager.
							if (m_SequenceManager.AddSequence(sequence) == false)
							{
								continue;
							}


							// Get entity id
							Uint16 entityId = Ntoh16(	static_cast<Uint16>(static_cast<Uint8>(buffer[4])) |
														static_cast<Uint16>(static_cast<Uint8>(buffer[5]) << 8));

							// Get entity
							Entity * pEntity = m_EntityManager.GetEntity(entityId);
							if (pEntity == NULL)
							{
								bitLogNetErr(  "Trying to destroy NULL entity." );
								continue;
							}

							// Call entity destroyed function.
							OnEntityDestroyed(pEntity);

							// Destroy the entity.
							m_EntityManager.DestroyEntity(pEntity, true);


						}
						break;*/
						case Private::PacketType::HostMessage:
						{
							if (recvSize <= Private::NetHostMessagePacketSize)
							{
								break;
							}

							// Add user message to queue
							AddHostMessage(pItem);
							pItem = NULL; ///< Indicate to get a new item.
							continue;
						}
						break;
						default:
							break;
					}

				}

				// Internally disconnect.
				InternalDisconnect();
			}

			);

			// Execute the event thread
			m_TriggerThread.Execute([this]()
			{
				while (IsConnected())
				{
					// Sleep for some time.
					Sleep(Milliseconds(10));

					// Disconnect you've not heard anything from the server in a while.
					if (GetTimeSinceLastReceivedPacket() >= m_LosingConnectionTimeout.Get())
					{
						m_DisconnectReason.Set(DisconnectReason::LostConnection);
						m_Connected.Set(false);
						return;
					}

					// Get the lapsed time since the last packet was sent.
					Time lapsedTime = GetTimeSinceLastSentPacket();


					// Send alive packet if requred
					if (lapsedTime >= Seconds(0.5f))
					{
						SendReliable(Private::PacketType::Alive, NULL, 0);
					}

				}
			}
			);

			// Execute the reliable thread
			m_ReliableThread.Execute([this]()
			{
				ReliablePacket * pPacket = NULL;

				while (IsConnected())
				{
					// Sleep for some time.
					Sleep(Milliseconds(1));

					// Check if we should resend any packet.
					m_ReliablePackets.Mutex.Lock();

					for (ReliablePacketMap::iterator it = m_ReliablePackets.Value.begin();
						it != m_ReliablePackets.Value.end();
						it++)
					{
						pPacket = it->second;

						// Resend packet after ping * 3.0 seconds.
						// Calculte the resend time.
						m_Ping.Mutex.Lock();
						Time resendTime = Microseconds(static_cast<Uint64>(static_cast<Float64>(m_Ping.Value.AsMicroseconds()) * 3.0f));
						m_Ping.Mutex.Unlock();

						if (pPacket->ResendTimer.GetLapsedTime() >= resendTime)
						{
							// Send packet.
							m_Socket.Send(pPacket->pData, pPacket->DataSize, m_DstAddress, m_DstPort);

							// Restart the resend timer
							pPacket->ResendTimer.Start();

							// Mark the packet as resent.
							pPacket->Resent = true;

							// Increase the ping if we lose packets
							m_Ping.Mutex.Lock();
							m_Ping.Value = Microseconds(static_cast<Uint64>(static_cast<Float64>(m_Ping.Value.AsMicroseconds()) * 1.2f));
							m_Ping.Mutex.Unlock();
						}
					}
					m_ReliablePackets.Mutex.Unlock();

				}
			}
			);

			// Execute user message
			m_HostMessageHandleThread.Execute([this]()
			{
				while (IsConnected())
				{

					// Wait for the semaphore to release
					m_HostMessageSemaphore.Wait();

					if (IsConnected() == false)
					{
						return;
					}

					// Go throguh the host messages.
					m_HostMessages.Mutex.Lock();

					while (m_HostMessages.Value.size())
					{
						// Get the current host message
						MemoryPool<Uint8>::Item * pItem = m_HostMessages.Value.front();
						m_HostMessages.Value.pop();

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


							// Find the listeners for this host message
							m_HostMessageListeners.Mutex.Lock();
							HostMessageListenerMap::iterator it = m_HostMessageListeners.Value.find(messageName);
							if (it == m_HostMessageListeners.Value.end())
							{
								m_HostMessageListeners.Mutex.Unlock();
								break;
							}

							HostMessageListenerSet * pMessageSet = it->second;

							// Go through the listeners and call the listener function
							for (HostMessageListenerSet::iterator it2 = pMessageSet->begin(); it2 != pMessageSet->end(); it2++)
							{
								// Get the listener.
								HostMessageListener * pListener = *it2;

								// Create a message decoder
								Uint8 * pMessageDataPtr = data + Private::NetUserMessagePacketSize + messageNameLength;
								SizeType messageDataSize = dataSize - Private::NetUserMessagePacketSize - messageNameLength;
								HostMessageDecoder messageDecoder(messageName, pMessageDataPtr, dataSize);

								// Use threads????
								// Handle the message.
								pListener->HandleMessage(messageDecoder);
							}

							m_HostMessageListeners.Mutex.Unlock();
						} while (false);

						// Return the packet to the memory pool.
						m_pPacketMemoryPool->Return(pItem);
					}

					m_HostMessages.Mutex.Unlock();

				}

				

			}
			);

			// The connection succeeded
			return Succeeded;
		}

		void Client::Disconnect()
		{
			m_DisconnectReason.Set(DisconnectReason::DisconnectByClient);
			m_Connected.Set(false);
		}

		Bool Client::IsConnected()
		{
			return m_Connected.Get();
		}

		Time Client::GetPing()
		{
			return m_Ping.Get();
		}

		Time Client::GetServerTime()
		{
			Time serverTime;

			m_ServerTimer.Mutex.Lock();
			serverTime = m_ServerTimer.Value.GetLapsedTime();
			m_ServerTimer.Mutex.Unlock();

			serverTime += m_ServerStartTime.Get();

			return serverTime;
		}

		void Client::OnEntityCreation(Entity * p_pEntity)
		{
		}

		void Client::OnEntityDestroyed(Entity * p_pEntity)
		{
		}

		Bool Client::HookHostMessage(HostMessageListener * p_pListener, const std::string & m_MessageName)
		{
			// Error check the parameters
			if (p_pListener == NULL || m_MessageName.size() == 0)
			{
				return false;
			}

			m_HostMessageListeners.Mutex.Lock();

			// Find the user mesage set pointer.
			HostMessageListenerSet * pMessageSet = NULL;

			// Find the message
			HostMessageListenerMap::iterator it = m_HostMessageListeners.Value.find(m_MessageName);
			if (it == m_HostMessageListeners.Value.end())
			{
				// Create a new message
				pMessageSet = new HostMessageListenerSet;
				m_HostMessageListeners.Value.insert(HostMessageListenerPair(m_MessageName, pMessageSet));
			}
			else
			{
				pMessageSet = it->second;
			}

			// Add the listener to the message set
			pMessageSet->insert(p_pListener);

			m_HostMessageListeners.Mutex.Unlock();

			return true;
		}

		UserMessage * Client::CreateUserMessage(const std::string & p_Name, const Int32 p_MessageSize)
		{
			return new UserMessage(p_Name, this, p_MessageSize);
		}

		Bool Client::InternalConnect(const Time & p_ConnectionTimeout, eStatus & p_Status)
		{
			// Keep on receiving until we get the right packet message from the host
			Uint8 buffer[Private::NetBufferSize];
			Int32 recvSize = 0;
			Address recvAddress;
			Uint16 recvPort = 0;
			Time timeout = p_ConnectionTimeout;
			const Time recvCheckTime = Seconds(0.2f); ///< How often to check for received packets.
			bool recvConnectionPacket = false;

			// Create identifier data
			const SizeType identifierDataSize = m_ServerIdentifier.size();
			std::unique_ptr<Uint8[]> identifierData(new Uint8[identifierDataSize + 1]);
			identifierData[0] = static_cast<Uint8>(m_ServerIdentifier.size());
			memcpy((&identifierData[1]), m_ServerIdentifier.data(), identifierDataSize);

			// Create a timer vector for checking roundtime
			// Store each timer for each try to connect, sequence number as key.
			typedef std::map<Uint16, Timer> TimerMap;
			TimerMap timerMap;
			Timer tempTimer;

			// Start the timer for timeout.
			Timer timeoutTimer;
			timeoutTimer.Start();

			// Keep on sending and receiving connect / accept packets.
			while (timeout.AsMicroseconds() > 0)
			{
				// Add new timer to the map and start the timer.
				Uint16 nextSequence = CheckNextSequence();
				TimerMap::iterator itNewTimer = timerMap.insert(std::pair<Uint16, Timer>(nextSequence, tempTimer)).first;
				itNewTimer->second.Start();

				// Send connect packet, tell the server that we would like to connect.
				if (SendUnreliable(Private::PacketType::Connect, reinterpret_cast<const void*>(identifierData.get()), identifierDataSize) == false)
				{
					// Error sending connection packet.
					p_Status = SocketError;
					return false;
				}

				// Receive message.
				// Expect maximum NetAcceptPacketSize size to receive. Minimum NetRejectPacketSize
				recvSize = m_Socket.Receive(buffer, Private::NetAcceptPacketSize, recvAddress, recvPort, recvCheckTime);

				// Decrease the timeout time
				timeout = p_ConnectionTimeout - timeoutTimer.GetLapsedTime();


				// Check the recv size
				if (recvSize <= 0)
				{
					continue;
				}

				// Error check the packet.
				if (recvSize < Private::NetRejectPacketSize || buffer[0] != Private::PacketType::Connect ||
					buffer[3] >= Private::NetConnectStatusTypeCount ||
					recvAddress != m_DstAddress || recvPort != m_DstPort)
				{
					continue;
				}

				// Get the sequence answered by the server.
				const Uint16 answerSequence = Private::PacketTransfer::ReadNtoh16FromBuffer(buffer + 1);

				// Find the answered sequence in the timer map, make sure that this is a valid sequence.
				TimerMap::iterator itCurTimer = timerMap.find(answerSequence);
				if (itCurTimer == timerMap.end())
				{
					continue;
				}

				// Stop round timer
				itCurTimer->second.Stop();

				// Get the accept status
				Private::ConnectStatusType::eType acceptStatus = static_cast<Private::ConnectStatusType::eType>(buffer[3]);

				// Check the connect status from the server.
				if (acceptStatus == Private::ConnectStatusType::Accepted)
				{
					// Check for accept packet size
					if (recvSize != Private::NetAcceptPacketSize)
					{
						timerMap.erase(itCurTimer);
						continue;
					}

					// Get half round time
					Uint64 roundTimeHalf = itCurTimer->second.GetTime().AsMicroseconds() / 2;

					// Get server time
					Uint64 serverTime = Private::PacketTransfer::ReadNtoh64FromBuffer(buffer + 4);

					// Set server time and setart server timer.
					m_ServerStartTime.Set(Microseconds(serverTime));
					m_ServerTimer.Mutex.Lock();
					m_ServerTimer.Value.Start();
					m_ServerTimer.Mutex.Unlock();

					// We are connected.
					p_Status = Succeeded;
					return true;

				}
				// The server denied us for some reason.
				else
				{
					switch (acceptStatus)
					{
					case Private::ConnectStatusType::Denied:
						p_Status = Denied;
						return false;
						break;
					case Private::ConnectStatusType::Banned:
						p_Status = Banned;
						return false;
						break;
					case Private::ConnectStatusType::Full:
						p_Status = Full;
						return false;
						break;
					default:
						break;
					};
				}

			}


			// Did not receive any valid packet from the server.
			// The connection timeouted
			p_Status = TimedOut;
			return false;
		}

		void Client::InternalDisconnect()
		{
			m_DisconnectMutex.Lock();
			
			m_Connected.Set(false);
			
			// Wait for threads to finish.
			m_TriggerThread.Finish();
			m_ReliableThread.Finish();
			m_HostMessageSemaphore.Release();
			m_HostMessageHandleThread.Finish();

			// Send disconnect packet if client disconnect by purpose or timed out.
			if (m_DisconnectReason.Get() == DisconnectReason::DisconnectByClient)
			{
				const Uint8 reason = Private::DisconnectType::Closed;
				SendUnreliable(Private::PacketType::Disconnect, &reason, 1);
			}

			// Cleanup data.
			ClearReliablePackets();
			m_PingList.clear();

			m_DisconnectReason.Set(DisconnectReason::Unknown);

			if (m_pPacketMemoryPool)
			{
				delete m_pPacketMemoryPool;
				m_pPacketMemoryPool = NULL;
			}

			m_DisconnectMutex.Unlock();
		}

		void Client::CalculateNewPing(const Time & p_LapsedTime)
		{
			// Add the new ping to the ping list
			m_PingList.pop_front();
			m_PingList.push_back(p_LapsedTime / 2ULL);

			// Calculate the new ping
			Uint64 newPing = 0ULL;
			for (TimeList::iterator it = m_PingList.begin(); it != m_PingList.end(); it++)
			{
				newPing += it->AsMicroseconds();
			}
			newPing /= static_cast<Uint64>(m_PingList.size());

			m_Ping.Mutex.Lock();
			m_Ping.Value = Microseconds(newPing);
			m_Ping.Mutex.Unlock();
		}

		void Client::AddHostMessage(MemoryPool<Uint8>::Item * p_pItem)
		{
			m_HostMessages.Mutex.Lock();
			m_HostMessages.Value.push(p_pItem);
			m_HostMessages.Mutex.Unlock();
			
			m_HostMessageSemaphore.Release();
		}

	}

}