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
			m_Ping(p_InitialPing)
		{

		}

		Client::~Client()
		{
			// Disconnect the client.
			InternalDisconnect(true, true, true, true);

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

		Bool Client::PingServer(	const Address & p_ServerAddress,
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

		Client::eStatus Client::Connect(const Address & p_Address,
			const Uint16 p_Port,
			const Time & p_ConnectionTimeout,
			const Time & p_LosingConnectionTimeout,
			const Time & p_InterpolationTime,
			const Time & p_ExtrapolationTime,
			const std::string & p_Identifier)
		{
			// make sure to be disconnected.
			InternalDisconnect(true, true, true, true);

			// Open the udp socket.
			m_Socket.SetBlocking(true);

			// Open the socket.
			if (m_Socket.Open(p_Port) == false)
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

			// Create a data buffer.
			Uint8 buffer[Private::NetBufferSize];

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

			// Set the connected flag to true.
			m_Connected.Set(true);

			// Start the client thread.
			m_Thread.Execute([this]()
			{
				const SizeType bufferSize = 2048;
				Uint8 buffer[bufferSize];
				Address address;
				Uint16 port = 0;
				Int32 recvSize = 0;

				// Start the timer for the last recv packet.
				RestartLastSentPacketTimer();
				RestartLastReceivedPacketTimer();

				// Receive packets as long as we are connected.
				while (IsConnected())
				{
					// Receive any packet.
					recvSize = m_Socket.Receive(buffer, bufferSize, address, port, Microseconds(1000));

					// Make sure that the packet is from the server
					if (address != m_DstAddress || port != m_DstPort)
					{
						continue;
					}

					// Check the packet size.
					if (recvSize < 1)
					{
						continue;
					}

					// Reset the time for checking last time a packet arrived
					RestartLastSentPacketTimer();

					// Check the packet type
					switch (buffer[0])
					{
						// The server disconnected.
					case Private::PacketType::Disconnect:
					{
						// Set connected to false.
						m_Connected.Mutex.Lock();
						m_Connected.Value = false;
						m_Connected.Mutex.Unlock();

						// Wait for the threads to finish.
						m_TriggerThread.Finish();
						m_ReliableThread.Finish();

						// Reset the sequence.
						/*m_Sequence.Mutex.Lock();
						m_Sequence.Value = 0;
						m_Sequence.Mutex.Unlock();
						*/
						return;
					}
					break;
					// ACK packet from server.
					case Private::PacketType::Acknowledgement:
					{
						// Ignore "corrupt" ack packet.
						if (recvSize != Private::NetAcknowledgementPacketSize)
						{
							continue;
						}

						// Get the sequence
						Uint16 sequence = Ntoh16(static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
							static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

						// Find the sequence in the reliable map
						m_ReliablePackets.Mutex.Lock();
						ReliablePacketMap::iterator it = m_ReliablePackets.Value.find(sequence);
						// Remove the packet from the map if it's found and calculate the ping.
						if (it != m_ReliablePackets.Value.end())
						{
							// Calculate the new ping from the lapsed time if it's not a resent packet
							if (it->second->Resent == false)
							{
								CalculateNewPing(it->second->SendTimer.GetLapsedTime());
							}

							// Clean up the data
							delete[] it->second->pData;
							delete it->second;

							// Erase the reliable packet
							m_ReliablePackets.Value.erase(it);
						}
						m_ReliablePackets.Mutex.Unlock();
					}
					break;
					// Alive packet from server.
					case Private::PacketType::Alive:
					{
						// Ignore "corrupt" alive packet.
						if (recvSize != Private::NetAlivePacketSize)
						{
							continue;
						}

						Uint16 sequence = Ntoh16(static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
							static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

						// Use the already allocated packet, change the type
						buffer[0] = Private::PacketType::Acknowledgement;

						// Send the ack packet
						m_Socket.Send(buffer, Private::NetAcknowledgementPacketSize, m_DstAddress, m_DstPort);
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
					break;
					case Private::PacketType::HostMessage:
					{
						// Error check the recv size
						if (recvSize <= HostMessagePacketSize)
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
						if (m_SequenceManager.AddSequence(sequence))
						{

							// Add the unreliable packet to the received data queue.	
							ReceivedData * pReceivedData = new ReceivedData(buffer + UserMessagePacketSize,
								recvSize - UserMessagePacketSize,
								sequence);

							// Add the host message.
							AddHostMessage(pReceivedData);
						}
					}
					break;*/
					default:
						break;
					}

				}
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
						InternalDisconnect(true, false, true, true);
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
				/*while (IsConnected())
				{
					// Wait for the semaphore to release
					m_UserMessageSemaphore.Wait();

					// Go throguh the user messages.
					m_UserMessages.Mutex.Lock();

					while (m_UserMessages.Value.size())
					{
						// Ge the received data
						ReceivedData * pReceivedData = m_UserMessages.Value.front();

						// Pop the message
						m_UserMessages.Value.pop();

						// Get the message name
						SizeType nameEnd = 1;
						for (SizeType i = 1; i < pReceivedData->DataSize; i++)
						{
							if (pReceivedData->pData[i] == 0)
							{
								nameEnd = i;
								break;
							}
						}

						// Error check the name length
						if (nameEnd == 1)
						{
							// Delete the received data pointer
							delete pReceivedData;
							continue;
						}

						// Copy the name.
						std::string name;
						name.assign(reinterpret_cast<char*>(pReceivedData->pData + 1), nameEnd - 1);

						// Check if there is any message left(including empty)
						if (name.size() + 2 > pReceivedData->DataSize)
						{
							// Delete the received data pointer
							delete pReceivedData;
							continue;
						}

						// Find the listeners for this user message
						m_HostMessageListeners.Mutex.Lock();
						HostMessageListenerMap::iterator it = m_HostMessageListeners.Value.find(name);
						if (it == m_HostMessageListeners.Value.end())
						{
							// Delete the received data pointer
							delete pReceivedData;
							continue;
						}

						HostMessageListenerSet * pMessageSet = it->second;

						// Go through the listeners and call the listener function
						for (HostMessageListenerSet::iterator it2 = pMessageSet->begin(); it2 != pMessageSet->end(); it2++)
						{
							// Get the listener.
							HostMessageListener * pListener = *it2;

							// Create a message decoder
							Uint8 * pDataPointer = pReceivedData->pData + name.size() + 2;
							const SizeType dataSize = pReceivedData->DataSize - name.size() - 2;
							HostMessageDecoder messageDecoder(name, pDataPointer, dataSize);

							// Use threads????
							// Handle the message.
							pListener->HandleMessage(messageDecoder);
						}

						m_HostMessageListeners.Mutex.Unlock();

						// Delete the received data pointer
						delete pReceivedData;

					}

					m_UserMessages.Mutex.Unlock();
				}*/
			}
			);

			// The connection succeeded
			return Succeeded;
		}

		void Client::Disconnect()
		{
			InternalDisconnect(true, true, true, true);
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
			const SizeType identifierDataSize = m_ServerIdentifier.size() + 1;
			std::unique_ptr<Uint8[]> identifierData(new Uint8[identifierDataSize]);
			identifierData[0] = static_cast<Uint8>(m_ServerIdentifier.size());
			memcpy((&identifierData[0]) + 1, m_ServerIdentifier.data(), identifierDataSize);

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
				if (SendUnreliable(Private::PacketType::Connect, reinterpret_cast<const void*>(identifierData.get()), identifierDataSize))
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

		void Client::InternalDisconnect(const Bool p_CloseMainThread,
			const Bool p_CloseTriggerThread,
			const Bool p_CloseReliableThread,
			const Bool p_CloseHostMessageThread)
		{
			// Get status and set connected to false.
			m_Connected.Mutex.Lock();
			Bool connected = m_Connected.Value;
			m_Connected.Value = false;
			m_Connected.Mutex.Unlock();

			if (connected)
			{
				// Send close packet.
				SendUnreliable(Private::PacketType::Disconnect, NULL, 0);
			}

			// Wait for the threads to finish.
			if (p_CloseMainThread)
			{
				m_Thread.Finish();
			}
			if (p_CloseTriggerThread)
			{
				m_TriggerThread.Finish();
			}
			if (p_CloseReliableThread)
			{
				m_ReliableThread.Finish();
			}
			if (p_CloseHostMessageThread)
			{
				m_UserMessageSemaphore.Release();
				m_HostMessageHandleThread.Finish();
			}

			// Clear the reliable packets
			m_ReliablePackets.Mutex.Lock();
			for (ReliablePacketMap::iterator it = m_ReliablePackets.Value.begin();
				it != m_ReliablePackets.Value.end();
				it++)
			{
				delete[] it->second->pData;
				delete it->second;
			}
			m_ReliablePackets.Value.clear();
			m_ReliablePackets.Mutex.Unlock();

			// Clear user messages
			/*m_UserMessages.Mutex.Lock();
			while (m_UserMessages.Value.size())
			{
				ReceivedData * pReceivedData = m_UserMessages.Value.front();
				m_UserMessages.Value.pop();

				delete pReceivedData;
			}
			m_UserMessages.Mutex.Unlock();
			*/

			// Clear the ping list.
			m_PingList.clear();
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

		/*void Client::AddHostMessage(ReceivedData * p_pReceivedData)
		{
			m_UserMessages.Mutex.Lock();
			m_UserMessages.Value.push(p_pReceivedData);
			m_UserMessages.Mutex.Unlock();

			m_UserMessageSemaphore.Release();
		}*/

	}

}