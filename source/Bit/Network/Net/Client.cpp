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
#include <iostream>
#include <Memory>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Client::Client(const Uint16 p_Port,
			const Time & p_InitialPing) :
			m_EntityManager(NULL),
			m_Port(p_Port),
			m_Connected(false),
			m_ServerAddress(0),
			m_ServerPort(0),
			m_LosingConnectionTimeout(Seconds(3.0f)),
			m_Sequence(0),
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

		Client::eStatus Client::Connect(const Address & p_Address,
			const Uint16 p_Port,
			const Time & p_ConnectionTimeout,
			const Time & p_LosingConnectionTimeout,
			const std::string & p_Identifier)
		{
			// make sure to be disconnected.
			InternalDisconnect(true, true, true, true);

			// Open the udp socket.
			m_Socket.SetBlocking(true);

			// Open the socket.
			if (m_Socket.Open(m_Port) == false)
			{
				return SocketError;
			}

			// Set blocking and return true.
			m_Socket.SetBlocking(true);

			// Create a data buffer.
			Uint8 buffer[BufferSize];

			// Initialize the ping list
			for (SizeType i = 0; i < 5; i++)
			{
				m_PingList.push_front(m_Ping.Value);
			}

			// Keep on receiving until we get the right packet message from the host
			Int32 recvSize = 0;
			Address recvAddress;
			Uint16 recvPort = 0;
			Time timeout = p_ConnectionTimeout;
			const Time sendTime = Seconds(0.2f); ///< How often to check for received packets.
			const SizeType connectPacketSize = 1 + p_Identifier.size(); ///< Connect packet length
			bool recvSynAck = false;

			// Create a connect packet
			std::string connectionPacket;
			connectionPacket.push_back(PacketType::Connect);
			connectionPacket.append(p_Identifier);

			// Start the timer.
			Timer timer;
			timer.Start();

			// Keep on sending and receiving connect / accept packets.
			while (timeout.AsMicroseconds() > 0)
			{
				// Send SYN packet, tell the server that we would like to connect.
				buffer[0] = PacketType::Connect;

				if (m_Socket.Send(connectionPacket.data(), connectionPacket.size(), p_Address, p_Port) != connectPacketSize)
				{
					// Error sending connection packet.
					return Unknown;
				}

				// Receive message
				recvSize = m_Socket.Receive(buffer, BufferSize, recvAddress, recvPort, sendTime);

				// Decrease the timeout time
				timeout = p_ConnectionTimeout - timer.GetLapsedTime();

				// Check the address and port, ignore packets that's not from the host.
				if (recvAddress != p_Address || recvPort != p_Port)
				{
					continue;
				}

				// Make sure that we received a packet type byte.
				if (recvSize < 1)
				{
					continue;
				}

				// Check if this is a SYN-ACK packet, the server wants us to connect.
				if (buffer[0] == PacketType::Accept)
				{
					// We now connected, we assume that the server receive the UDP_ACCEPT packet,
					// resend the packet in client thread if we get another accept packet later.
					m_ServerAddress = p_Address;
					m_ServerPort = p_Port;
					recvSynAck = true;
					break;

				}
				// The server answered with a disconnect packet, we are denied. 
				else if (buffer[0] == PacketType::Reject)
				{
					// Make sure we get the reject type
					if (recvSize < RejectPacketSize)
					{
						continue;
					}

					// Check the cause of the rejection.
					if (buffer[1] == RejectType::Banned)
					{
						return eStatus::Banned;
					}
					else if (buffer[1] == RejectType::Full)
					{
						return eStatus::Full;
					}
					else if (buffer[1] == RejectType::Denied)
					{
						return eStatus::Denied;
					}
					else
					{
						return eStatus::Unknown;
					}

				}
				else // Unexpected packet from the server.
				{
					// Ignore unexpected oacket from the server.
					continue;
				}
			}


			// Did not receive any valid packet from the server.
			if (recvSynAck == false)
			{
				// The connection timeouted
				return TimedOut;
			}

			// Reset sequences
			m_Sequence.Set(0);
			m_EntityUpdateSequence.Set(0);

			// Set lost connection timeout
			m_LosingConnectionTimeout.Set(p_LosingConnectionTimeout);

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
				m_LastRecvTimer.Mutex.Lock();
				m_LastRecvTimer.Value.Start();
				m_LastRecvTimer.Mutex.Unlock();

				// Start the timer for the last send packet.
				m_LastSendTimer.Mutex.Lock();
				m_LastSendTimer.Value.Start();
				m_LastSendTimer.Mutex.Unlock();

				// Receive packets as long as we are connected.
				while (IsConnected())
				{
					// Receive any packet.
					recvSize = m_Socket.Receive(buffer, bufferSize, address, port, Microseconds(1000));

					// Make sure that the packet is from the server
					if (address != m_ServerAddress || port != m_ServerPort)
					{
						continue;
					}

					// Check the packet size.
					if (recvSize < 1)
					{
						continue;
					}

					// Reset the time for checking last time a packet arrived
					m_LastRecvTimer.Mutex.Lock();
					m_LastRecvTimer.Value.Start();
					m_LastRecvTimer.Mutex.Unlock();

					// Check the packet type
					switch (buffer[0])
					{
						// The server disconnected.
					case PacketType::Disconnect:
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
					case PacketType::Acknowledgement:
					{
						// Ignore "corrupt" ack packet.
						if (recvSize != AcknowledgementPacketSize)
						{
							continue;
						}

						// Get the sequence
						Uint16 sequence = Ntoh16(static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
							static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

						// Find the sequence in the reliable map
						m_ReliableMap.Mutex.Lock();
						ReliablePacketMap::iterator it = m_ReliableMap.Value.find(sequence);
						// Remove the packet from the map if it's found and calculate the ping.
						if (it != m_ReliableMap.Value.end())
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
							m_ReliableMap.Value.erase(it);
						}
						m_ReliableMap.Mutex.Unlock();
					}
					break;
					// Alive packet from server.
					case PacketType::Alive:
					{
						// Ignore "corrupt" alive packet.
						if (recvSize != AlivePacketSize)
						{
							continue;
						}

						Uint16 sequence = Ntoh16(static_cast<Uint16>(static_cast<Uint8>(buffer[1])) |
							static_cast<Uint16>(static_cast<Uint8>(buffer[2]) << 8));

						// Use the already allocated packet, change the type
						buffer[0] = PacketType::Acknowledgement;

						// Send the ack packet
						m_Socket.Send(buffer, AcknowledgementPacketSize, m_ServerAddress, m_ServerPort);
					}
					break;
					case PacketType::EntityUpdate:
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
							m_EntityManager.ParseEntityMessage(buffer + EntityUpdatePacketSize, recvSize - EntityUpdatePacketSize);
						}

					}
					break;
					case PacketType::HostMessage:
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
					break;
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
					if (TimeSinceLastRecvPacket() >= m_LosingConnectionTimeout.Get())
					{
						InternalDisconnect(true, false, true, true);
						return;
					}

					// Get the lapsed time since the last packet was sent.
					m_LastSendTimer.Mutex.Lock();
					Time lapsedTime = m_LastSendTimer.Value.GetLapsedTime();
					m_LastSendTimer.Mutex.Unlock();

					// Send alive packet if requred
					if (lapsedTime >= Seconds(0.5f))
					{
						SendReliable(PacketType::Alive, NULL, 0, false);
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
					m_ReliableMap.Mutex.Lock();

					for (ReliablePacketMap::iterator it = m_ReliableMap.Value.begin();
						it != m_ReliableMap.Value.end();
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
							m_Socket.Send(pPacket->pData, pPacket->DataSize, m_ServerAddress, m_ServerPort);

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
					m_ReliableMap.Mutex.Unlock();

				}
			}
			);

			// Execute user message
			m_UserMessageThread.Execute([this]()
			{
				while (IsConnected())
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
				}
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

		void Client::OnEntityCreation(Entity * p_pEntity)
		{
		}

		/*void Client::OnEntityDestruction(const Uint16 p_EntityId, const std::string p_EntityName)
		{
		}*/

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

		Time Client::TimeSinceLastRecvPacket()
		{
			return m_LastRecvTimer.Get().GetLapsedTime();
		}

		// Received data struct
		Client::ReceivedData::ReceivedData(Uint8 * p_pData, const SizeType p_DataSize, const Uint16 p_Sequence) :
			Sequence(p_Sequence),
			DataSize(p_DataSize)
		{
			pData = new Uint8[p_DataSize];
			memcpy(pData, p_pData, p_DataSize);
		}

		Client::ReceivedData::~ReceivedData()
		{
			if (pData)
			{
				delete[] pData;
			}
		}

		void Client::InternalDisconnect(const Bool p_CloseMainThread,
			const Bool p_CloseTriggerThread,
			const Bool p_CloseReliableThread,
			const Bool p_CloseUserMessageThread)
		{
			// Get status and set connected to false.
			m_Connected.Mutex.Lock();
			Bool connected = m_Connected.Value;
			m_Connected.Value = false;
			m_Connected.Mutex.Unlock();

			if (connected)
			{
				// Send close packet.
				SendUnreliable(PacketType::Disconnect, NULL, 0, false, false);
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
			if (p_CloseUserMessageThread)
			{
				m_UserMessageSemaphore.Release();
				m_UserMessageThread.Finish();
			}

			// Reset the sequence.
		/*	m_Sequence.Mutex.Lock();
			m_Sequence.Value = 0;
			m_Sequence.Mutex.Unlock();
*/
			// Clear the reliable packets
			m_ReliableMap.Mutex.Lock();
			for (ReliablePacketMap::iterator it = m_ReliableMap.Value.begin();
				it != m_ReliableMap.Value.end();
				it++)
			{
				delete[] it->second->pData;
				delete it->second;
			}
			m_ReliableMap.Value.clear();
			m_ReliableMap.Mutex.Unlock();

			// Clear user messages
			m_UserMessages.Mutex.Lock();
			while (m_UserMessages.Value.size())
			{
				ReceivedData * pReceivedData = m_UserMessages.Value.front();
				m_UserMessages.Value.pop();

				delete pReceivedData;
			}
			m_UserMessages.Mutex.Unlock();


			// Clear the ping list.
			m_PingList.clear();
		}

		void Client::SendUnreliable(const PacketType::eType p_PacketType,
			void * p_pData,
			const SizeType p_DataSize,
			const bool p_AddSequence,
			const bool p_AddReliableFlag)
		{


			// Use memory pool here?
			// ...

			// Create the packet. Make space for the sequence in case.
			const Bit::SizeType packetSize = p_DataSize + PacketTypeSize +
											(p_AddSequence ? SequenceSize : 0) +
											(p_AddReliableFlag ? 1 : 0);


			Uint8 * pBuffer = new Uint8[packetSize];
			pBuffer[0] = static_cast<Uint8>(p_PacketType);

			// Should we add the sequence?
			if (p_AddSequence)
			{
				// Get the current sequence, and increment it.
				m_Sequence.Mutex.Lock();
				Bit::Uint16 sequence = Bit::Hton16(m_Sequence.Value);
				m_Sequence.Value++;
				m_Sequence.Mutex.Unlock();

				// Add the sequence to the data buffer.
				memcpy(pBuffer + 1, &sequence, SequenceSize);
			}

			// Add the reliable flag
			if (p_AddReliableFlag)
			{
				pBuffer[PacketTypeSize + SequenceSize] = static_cast<Uint8>(ReliabilityType::Unreliable);
			}

			// Add the data to the new buffer
			memcpy(pBuffer + (packetSize - p_DataSize), p_pData, p_DataSize);

			// Send the packet.
			m_Socket.Send(pBuffer, packetSize, m_ServerAddress, m_ServerPort);

			// Delete the packet
			delete[] pBuffer;
		}

		void Client::SendReliable(const PacketType::eType p_PacketType,
			void * p_pData,
			const SizeType p_DataSize,
			const bool p_AddReliableFlag)
		{
			// Create a new buffer.
			const SizeType packetSize = p_DataSize + PacketTypeSize + SequenceSize + (p_AddReliableFlag ? 1 : 0);
			Uint8 * pBuffer = new Uint8[packetSize];
			pBuffer[0] = static_cast<Uint8>(p_PacketType);

			// Get the current sequence, and increment it.
			m_Sequence.Mutex.Lock();
			Bit::Uint16 currentSequence = m_Sequence.Value;
			m_Sequence.Value++;
			m_Sequence.Mutex.Unlock();

			// Add the sequence to the data buffer.
			Bit::Uint16 sequence = Bit::Hton16(currentSequence);
			memcpy(pBuffer + 1, &sequence, SequenceSize);

			// Add the reliable flag
			if (p_AddReliableFlag)
			{
				pBuffer[PacketTypeSize + SequenceSize] = static_cast<Uint8>(ReliabilityType::Reliable);
			}

			// Add the data to the buffer
			memcpy(pBuffer + (packetSize - p_DataSize), p_pData, p_DataSize);

			// Create the reliable packet.
			ReliablePacket * pReliablePacket = new ReliablePacket;
			pReliablePacket->pData = pBuffer;
			pReliablePacket->DataSize = packetSize;
			pReliablePacket->Sequence = currentSequence;
			pReliablePacket->Resent = false;
			pReliablePacket->SendTimer.Start();
			pReliablePacket->ResendTimer.Start();

			// Add the packet to the reliable map.
			m_ReliableMap.Mutex.Lock();
			m_ReliableMap.Value.insert(ReliablePacketPair(currentSequence, pReliablePacket));
			m_ReliableMap.Mutex.Unlock();

			// Send SYN packet, tell the server that we would like to connect.
			if (m_Socket.Send(pBuffer, packetSize, m_ServerAddress, m_ServerPort) == packetSize)
			{
				// Restart the timer
				m_LastSendTimer.Mutex.Lock();
				m_LastSendTimer.Value.Start();
				m_LastSendTimer.Mutex.Unlock();
			}
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

		void Client::AddHostMessage(ReceivedData * p_pReceivedData)
		{
			m_UserMessages.Mutex.Lock();
			m_UserMessages.Value.push(p_pReceivedData);
			m_UserMessages.Mutex.Unlock();

			m_UserMessageSemaphore.Release();
		}

		bool Client::AddEntityUpdateSequence(const Uint16 p_Sequence)
		{
			// Store the return value for later
			bool ret = false;

			m_EntityUpdateSequence.Mutex.Lock();
			if	(p_Sequence >= m_EntityUpdateSequence.Value ||
				(m_EntityUpdateSequence.Value > 49152 &&  p_Sequence < 16384 ))
			{
				m_EntityUpdateSequence.Value = p_Sequence;
				ret = true;
			}
			m_EntityUpdateSequence.Mutex.Unlock();
			
			// Return the return value.
			return ret;
		}

	}

}