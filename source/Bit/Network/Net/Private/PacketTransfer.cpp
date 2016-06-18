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

#include <Bit/Network/Net/Private/PacketTransfer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		namespace Private
		{

			Uint16 PacketTransfer::ReadNtoh16FromBuffer(const Uint8 * p_pBuffer)
			{
				Uint16 value = 0;
				memcpy(&value, p_pBuffer, sizeof(Uint16));
				return Ntoh16(value);
			}

			Uint16 PacketTransfer::ReadNtoh64FromBuffer(const Uint8 * p_pBuffer)
			{
				Uint64 value = 0;
				memcpy(&value, p_pBuffer, sizeof(Uint64));
				return Ntoh64(value);
			}

			PacketType::eType PacketTransfer::ParsePacketType(const Uint8 p_Byte)
			{
				const Uint8 typeByte = p_Byte & NetTypeMask;

				if (typeByte >= NetPacketTypeCount)
				{
					return PacketType::Unknown;
				}

				return static_cast<PacketType::eType>(typeByte);
			}

			Bool PacketTransfer::ParseReliableFlag(const Uint8 p_Byte)
			{
				return static_cast<Bool>(p_Byte & NetReliableFlagMask);
			}

			PacketTransfer::PacketTransfer( const Address & p_DestinationAddress,
											const Uint16 p_DestinationPort) :
				m_DstAddress(p_DestinationAddress),
				m_DstPort(p_DestinationPort),
				m_NextSequence(0)
			{
				
			}

			Bool PacketTransfer::SendUnreliable(const PacketType::eType p_PacketType,
												const void * p_pData,
												const SizeType p_DataSize)
			{
				// Create the packet. Make space for the sequence in case.
				const Bit::SizeType packetSize = NetHeaderSize + p_DataSize;

				// Set packet header type byte.
				Uint8 * pBuffer = new Uint8[packetSize];
				pBuffer[0] = static_cast<Uint8>(p_PacketType);

				// Set packet header sequence.
				Bit::Uint16 sequence = Bit::Hton16(GetNextSequence());
				memcpy(pBuffer + 1, &sequence, NetSequenceSize);

				// Add the data to the new buffer
				memcpy(pBuffer + NetHeaderSize, p_pData, p_DataSize);

				// Send the packet.
				Bool status = false;
				if (m_Socket.Send(pBuffer, packetSize, m_DstAddress, m_DstPort) == packetSize)
				{
					RestartLastSentPacketTimer();
					status = true;
				}

				// Delete the packet
				delete[] pBuffer;

				return status;
			}

			
			Bool PacketTransfer::SendReliable(	const PacketType::eType p_PacketType,
												const void * p_pData,
												const SizeType p_DataSize)
			{
				// Create the packet. Make space for the sequence in case.
				const Bit::SizeType packetSize = NetHeaderSize + p_DataSize;

				// Set packet header type byte(also the reliable flag),
				Uint8 * pBuffer = new Uint8[packetSize];
				pBuffer[0] = static_cast<Uint8>(p_PacketType) & NetReliableFlagMask;
			
				// Set packet header sequence.
				Bit::Uint16 sequence = Bit::Hton16(GetNextSequence());
				memcpy(pBuffer + 1, &sequence, NetSequenceSize);

				// Add the data to the new buffer
				memcpy(pBuffer + NetHeaderSize, p_pData, p_DataSize);

				// Add the reliable packet.
				AddReliablePacket(sequence, pBuffer, packetSize);

				// Send the packet.
				if (m_Socket.Send(pBuffer, packetSize, m_DstAddress, m_DstPort) == packetSize)
				{
					RestartLastSentPacketTimer();
					return true;
				}

				// Do not delete the allocated buffer by purpose,
				// since it's stored as a reliable packet and will be deleted later.
				return false;
			}

			Uint16 PacketTransfer::GetNextSequence()
			{
				m_NextSequence.Mutex.Lock();
				Uint16 nextSequence = m_NextSequence.Value;
				m_NextSequence.Value = nextSequence + 1;
				m_NextSequence.Mutex.Unlock();

				return nextSequence;
			}

			Uint16 PacketTransfer::CheckNextSequence()
			{
				return m_NextSequence.Get();
			}

			Address PacketTransfer::GetDestinationAddress() const
			{
				return m_DstAddress;
			}


			Uint64 PacketTransfer::GetDestinationPackedAddress() const
			{
				return	static_cast<Uint64>(m_DstAddress.GetAddress()) *
						static_cast<Uint64>(m_DstPort) +
						static_cast<Uint64>(m_DstPort);
			}

			Uint16 PacketTransfer::GetDestinationPort() const
			{
				return m_DstPort;
			}

			void PacketTransfer::AddReliablePacket(	const Uint16 & p_Sequence,
													Uint8 * p_pData,
													const SizeType & p_DataSize)
			{
				ReliablePacket * pReliablePacket = new ReliablePacket;
				pReliablePacket->Sequence = p_Sequence;
				pReliablePacket->pData = p_pData;
				pReliablePacket->DataSize = p_DataSize;
				pReliablePacket->Resent = false;
				pReliablePacket->SendTimer.Start();
				pReliablePacket->ResendTimer.Start();

				// Add the packet to the reliable map.
				m_ReliablePackets.Mutex.Lock();
				m_ReliablePackets.Value.insert(ReliablePacketPair(p_Sequence, pReliablePacket));
				m_ReliablePackets.Mutex.Unlock();
			}

			Bool PacketTransfer::RemoveReliablePacket(	const Uint16 & p_Sequence,
														Bool & p_WasResent,
														Time & p_TimeSinceSent)
			{
				// Find the sequence in the reliable map
				m_ReliablePackets.Mutex.Lock();
				ReliablePacketMap::iterator it = m_ReliablePackets.Value.find(p_Sequence);
				// Remove the packet from the map if it's found and calculate the ping.
				if (it != m_ReliablePackets.Value.end())
				{
					// Set out parameters
					p_WasResent = it->second->Resent;
					p_TimeSinceSent = it->second->SendTimer.GetLapsedTime();

					// Clean up the data
					delete[] it->second->pData;
					delete it->second;

					// Erase the reliable packet
					m_ReliablePackets.Value.erase(it);

					m_ReliablePackets.Mutex.Unlock();
					return true;
				}

				m_ReliablePackets.Mutex.Unlock();
				return false;
			}

			void PacketTransfer::ClearReliablePackets()
			{
				// Find the sequence in the reliable map
				m_ReliablePackets.Mutex.Lock();

				// Remove the packet from the map if it's found and calculate the ping.
				for (	ReliablePacketMap::iterator it = m_ReliablePackets.Value.begin();
						it != m_ReliablePackets.Value.end();
						it++)
				{
					// Clean up the data
					delete[] it->second->pData;
					delete it->second;

					// Erase the reliable packet
					m_ReliablePackets.Value.erase(it);
				}

				m_ReliablePackets.Value.clear();
				m_ReliablePackets.Mutex.Unlock();

			}

			void PacketTransfer::RestartLastSentPacketTimer()
			{
				m_LastSendTimer.Mutex.Lock();
				m_LastSendTimer.Value.Start();
				m_LastSendTimer.Mutex.Unlock();
			}

			void PacketTransfer::RestartLastReceivedPacketTimer()
			{
				m_LastRecvTimer.Mutex.Lock();
				m_LastRecvTimer.Value.Start();
				m_LastRecvTimer.Mutex.Unlock();
			}
			
			Time PacketTransfer::GetTimeSinceLastSentPacket()
			{
				m_LastSendTimer.Mutex.Lock();
				Time time = m_LastSendTimer.Value.GetLapsedTime();
				m_LastSendTimer.Mutex.Unlock();

				return time;
			}

			Time PacketTransfer::GetTimeSinceLastReceivedPacket()
			{
				m_LastRecvTimer.Mutex.Lock();
				Time time = m_LastRecvTimer.Value.GetLapsedTime();
				m_LastRecvTimer.Mutex.Unlock();

				return time;
			}

		}

	}

}