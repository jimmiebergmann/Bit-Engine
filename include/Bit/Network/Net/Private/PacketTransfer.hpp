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

// Documentation about the packet structure, please:
// ... NIL


#ifndef BIT_NETWORK_PACKET_TRANSFER_HPP
#define BIT_NETWORK_PACKET_TRANSFER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/MemoryPool.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/Network/Net/Private/SequenceManager.hpp>
#include <map>
#include <list>
#include <queue>

namespace Bit
{

	namespace Net
	{

		namespace Private
		{
			/*
				Structure of a packet
				-------------------------------------------------------------------------
				|	Size(bytes)	|	Description
				-------------------------------------------------------------------------
				|		1		|	Packet type. bit 0 - 7 = message type, bit 8 = reliable flag.
				|		2		|	Sequence number.
				|		~		|	Data
			*/

			////////////////////////////////////////////////////////////////
			/// Constants
			////////////////////////////////////////////////////////////////
			const SizeType NetBufferSize = 512;
			const SizeType NetPacketTypeSize = 1;
			const SizeType NetSequenceSize = 2;
			const Uint8	   NetReliableFlagMask = 0x80;
			const Uint8	   NetTypeMask = 0x7F;

			/// Type count constants
			const SizeType NetPacketTypeCount = 8;
			const SizeType NetConnectStatusTypeCount = 4;
			const SizeType NetDisconnectTypeCount = 4;
			const SizeType NetEntityMessageTypeCount = 3;

			// Packet size(minimum) constants.
			// * = extra data, required but of unknwon size.
			const SizeType NetHeaderSize = 3;					///< Packet type + Sequence number
			const SizeType NetConnectPacketSize = 4;			///< Header + Identifier length + *Identifier*
			const SizeType NetDisconnectPacketSize = 4;			///< Header + Reason
			const SizeType NetAcceptPacketSize = 12;			///< Header + Connect status + Server time
			const SizeType NetRejectPacketSize = 4;				///< Header + Connect status
			const SizeType NetAcknowledgementPacketSize = 3;	///< Header, sequence in header is the packet to acknowledge.
			
			const SizeType NetAlivePacketSize = 3;				///< Header
			const SizeType NetEntityUpdatePacketSize = 4;
			const SizeType NetEntityDestroyedPacketSize = 6;
			const SizeType NetUserMessagePacketSize = 4;
			const SizeType NetHostMessagePacketSize = 4;
			//const SizeType NetPingPacketSize = 2;

			////////////////////////////////////////////////////////////////
			/// \brief	Packet type.
			///			Supporting up to 127 packet types in total,
			///			the last bit is reserved for reliable type.
			///
			////////////////////////////////////////////////////////////////
			struct PacketType
			{
				enum eType
				{
					///							| Reliable  |	Sender	| 
					/// -------------------------------------------------
					Connect			= 0,	//	|	No		|	Both	|
					Disconnect		= 1,	//	|	No		|	Both	|
					Acknowledgement = 2,	//	|	No		|	Both	|
					Alive			= 3,	//	|	No		|	Both	|
					EntityMessage	= 4,	//	|	Both	|	Server	|
					UserMessage		= 5,	//	|	Both	|	Client	|
					HostMessage		= 6,	//	|	Both	|	Server	|
					Ping			= 7,	//	|	No		|	Both	|
					Unknown			= 8		// Unknow type, received corrupt packet.
					/// -------------------------------------------------
				};
			};

			////////////////////////////////////////////////////////////////
			/// \brief Connect status type
			///
			////////////////////////////////////////////////////////////////
			struct ConnectStatusType
			{
				enum eType
				{
					Accepted = 0,	///< Accepted by server
					Denied = 1,		///< Denied by server.
					Banned = 2,		///< The client is banned.
					Full = 3		///< The server is full.
				};
			};

			////////////////////////////////////////////////////////////////
			/// \brief Disconnect type
			///
			////////////////////////////////////////////////////////////////
			struct DisconnectType
			{
				enum eType
				{
					Closed = 0,				///< The client/server closed the connection.
					Banned = 1,				///< The server banned the client.
					Kicked = 2,				///< The server kicked the client.
					LostConnection = 3		///< The connection was lost.
				};
			};

			////////////////////////////////////////////////////////////////
			/// \brief Entity message type
			///
			////////////////////////////////////////////////////////////////
			struct EntityMessageType
			{
				enum eType
				{
					Create = 0,		///< Entity creation message type.
					Update = 1,		///< Entity variable update message type.
					Destory = 2,	///< Entity destruction message type.
				};
			};


			////////////////////////////////////////////////////////////////
			/// \brief Packet transfer helper function
			///
			////////////////////////////////////////////////////////////////
			class BIT_API PacketTransfer
			{

			public:

				// Static functions

				////////////////////////////////////////////////////////////////
				/// \brief Read network order 16 bit value from buffer and convert it to host order.
				///
				////////////////////////////////////////////////////////////////
				static Uint16 ReadNtoh16FromBuffer(const Uint8 * p_pBuffer);

				////////////////////////////////////////////////////////////////
				/// \brief Read network order 64 bit value from buffer and convert it to host order.
				///
				////////////////////////////////////////////////////////////////
				static Uint16 ReadNtoh64FromBuffer(const Uint8 * p_pBuffer);

				////////////////////////////////////////////////////////////////
				/// \brief Parse packet type from byte, the first byte of a packet
				///		   contains both the type and reliable flag.
				///
				////////////////////////////////////////////////////////////////
				static PacketType::eType ParsePacketType(const Uint8 p_Byte);

				////////////////////////////////////////////////////////////////
				/// \brief Parse packet reliable frag from byte, the first byte of a packet
				///		   contains both the type and reliable flag.
				///
				////////////////////////////////////////////////////////////////
				static Bool ParseReliableFlag(const Uint8 p_Byte);


				////////////////////////////////////////////////////////////////
				/// \brief Constructor
				///
				////////////////////////////////////////////////////////////////
				PacketTransfer(	const Address & p_DestinationAddress,			
								const Uint16 p_DestinationPort);

				////////////////////////////////////////////////////////////////
				/// \brief Send unreliable packet to the server.
				///
				/// \param p_PacketType Type of the packet.
				/// \param p_pData Pointer to the data to send.
				/// \param p_DataSize Size of the data.
				///
				////////////////////////////////////////////////////////////////
				Bool SendUnreliable(const PacketType::eType p_PacketType,
									const void * p_pData,
									const SizeType p_DataSize);

				////////////////////////////////////////////////////////////////
				/// \brief Send reliable packet to the server.
				///
				/// \param p_PacketType Type of the packet.
				/// \param p_pData Pointer to the data to send.
				/// \param p_DataSize Size of the data.
				///
				////////////////////////////////////////////////////////////////
				Bool SendReliable(	const PacketType::eType p_PacketType,
									const void * p_pData,
									const SizeType p_DataSize);

			protected:

				////////////////////////////////////////////////////////////////
				/// \brief	Structure for reliable packets.
				///			Store reliable packets for resend at a later time if they are lost.
				///
				////////////////////////////////////////////////////////////////
				struct ReliablePacket
				{
					Uint16		Sequence;
					Uint8 *		pData;
					SizeType	DataSize;
					Timer		SendTimer;
					Timer		ResendTimer;
					Bool		Resent;
				};

				// Protected types
				typedef std::map<Uint16, ReliablePacket*>		ReliablePacketMap;
				typedef std::pair<Uint16, ReliablePacket*>		ReliablePacketPair;
				typedef std::list<Time>							TimeList;


				////////////////////////////////////////////////////////////////
				/// \brief Get the next sequence number. Increment to the next as well.
				///
				////////////////////////////////////////////////////////////////
				Uint16 GetNextSequence();

				////////////////////////////////////////////////////////////////
				/// \brief Check what the next sequence number is.
				///
				////////////////////////////////////////////////////////////////
				Uint16 CheckNextSequence();

				////////////////////////////////////////////////////////////////
				/// \brief Get destination address.
				///
				////////////////////////////////////////////////////////////////
				Address GetDestinationAddress() const;

				////////////////////////////////////////////////////////////////
				/// \brief Get destination packed address. Address * port + port.
				///
				////////////////////////////////////////////////////////////////
				Uint64 GetDestinationPackedAddress() const;

				////////////////////////////////////////////////////////////////
				/// \brief Get destination port.
				///
				////////////////////////////////////////////////////////////////
				Uint16 GetDestinationPort() const;

				////////////////////////////////////////////////////////////////
				/// \brief Add a reliable packet to the reliable packet container.
				///
				/// \param p_Sequence Sequence of the reliable packet.
				/// \param p_pData Pointer to data of the reliable packet.
				/// \param p_DataSize Size of the data.
				///
				////////////////////////////////////////////////////////////////
				void AddReliablePacket(	const Uint16 & p_Sequence,
										Uint8 * p_pData,
										const SizeType & p_DataSize);

				////////////////////////////////////////////////////////////////
				/// \brief Remove reliable packet from container.
				///
				/// \param p_Sequence Sequence of the reliable packet to remove.
				/// \param p_WasResent Flag for checking if packet has been resent.
				/// \param p_TimeSinceSent Time since the original packet was sent.
				///
				/// \return true if packet is removed(found), else false.
				///
				////////////////////////////////////////////////////////////////
				Bool RemoveReliablePacket(	const Uint16 & p_Sequence,
											Bool & p_WasResent,
											Time & p_TimeSinceSent);

				////////////////////////////////////////////////////////////////
				/// \brief Clear all reliable packets.
				///
				////////////////////////////////////////////////////////////////
				void ClearReliablePackets();

				////////////////////////////////////////////////////////////////
				/// \brief Restart the last sent packet timer.
				///
				////////////////////////////////////////////////////////////////
				void RestartLastSentPacketTimer();

				////////////////////////////////////////////////////////////////
				/// \brief Restart the last sent packet timer.
				///
				////////////////////////////////////////////////////////////////
				void RestartLastReceivedPacketTimer();

				////////////////////////////////////////////////////////////////
				/// \brief Get the time since last sent packet.
				///
				///////////////////////////////////////////////////////////////
				Time GetTimeSinceLastSentPacket();

				////////////////////////////////////////////////////////////////
				/// \brief Get the time since last received packet.
				///
				///////////////////////////////////////////////////////////////
				Time GetTimeSinceLastReceivedPacket();


				// Protected variables
				UdpSocket							m_Socket;			///< Socket of the data link.
				Bit::Address						m_DstAddress;		///< Address of the data link destination.
				Uint16								m_DstPort;			///< Port of the data link destination.
				ThreadValue<ReliablePacketMap>		m_ReliablePackets;	///< Reliable packets waiting for resending if needed.
				SequenceManager						m_SequenceManager;	///< Sequence manager.
				TimeList							m_PingList;			///< List of the last ping values.

			private:

				// Private variables
				ThreadValue<Uint16>		m_NextSequence;		///< The sequence of the next packet being sent.
				ThreadValue<Timer>		m_LastSendTimer;	///< Timer for tracking when the last packet was sent.
				ThreadValue<Timer>		m_LastRecvTimer;	///< Timer for tracking when the last packet was received.

			};




















			////////////////////////////////////////////////////////////////
			/// \brief Message type enumerator (OLD) REMOVE SOON PLEASE! NOT NEEDED ANYMORE!
			///
			////////////////////////////////////////////////////////////////
			enum eMessageType
			{
				EntityMessageTypeOLD = 0,
				UserMessageType = 1
			};

		}

	}

}

#endif