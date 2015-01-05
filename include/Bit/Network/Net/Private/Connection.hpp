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

#ifndef BIT_NETWORK_NET_CONNECTION_HPP
#define BIT_NETWORK_NET_CONNECTION_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Semaphore.hpp>
#include <Bit/System/Timer.hpp>
#include <queue>
#include <map>
#include <list>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \brief	Connection class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Connection
		{

		public:

			// Public friend classes
			friend class Server;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			Connection(	const Address & p_Address, 
						const Uint16 & p_Port,
						const Uint16 & p_UserId,
						const Time & p_InitialPing = Microseconds( 200000 ) );
		
			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~Connection( );

			////////////////////////////////////////////////////////////////
			/// \brief Check if the client is connected.
			///
			////////////////////////////////////////////////////////////////
			Bool IsConnected( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the current ping.
			///
			////////////////////////////////////////////////////////////////
			Time GetPing( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the user id
			///
			////////////////////////////////////////////////////////////////
			Uint16 GetUserId( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get the packed address (ip * port + port )
			///
			////////////////////////////////////////////////////////////////
			Uint64 GetPackedAddress( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Send unreliable packet to the client.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
		/*	void SendUnreliable( void * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Send reliable packet to the client.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendReliable( void * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Receive data from client
			///
			/// You have to delete the data in the udp packet by yourself to prevent memory leaks.
			///
			/// \param p_Packet Udp packet.
			///
			/// \return True if received packet, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool Receive( Packet & p_Packet );*/

		private:

			// Private structs

			////////////////////////////////////////////////////////////////
			/// \brief Raw packet queue for sending data from server to connection.
			///
			////////////////////////////////////////////////////////////////
			struct RawPacket
			{
				RawPacket( char * p_pData, const SizeType p_DataSize );
				~RawPacket( );

				char * pData;
				SizeType DataSize;
			};

			////////////////////////////////////////////////////////////////
			/// \brief Received data structure
			///
			////////////////////////////////////////////////////////////////
			struct ReceivedData
			{
				Uint16		Sequence;
				char *			pData;
				SizeType	DataSize;
			};

			////////////////////////////////////////////////////////////////
			/// \brief Struct of the acknowledgement data.
			///
			////////////////////////////////////////////////////////////////
			class AcknowledgementData
			{

			public:

				////////////////////////////////////////////////////////////////
				/// \brief Default constructor
				///
				////////////////////////////////////////////////////////////////
				AcknowledgementData( );

				////////////////////////////////////////////////////////////////
				/// \brief Default constructor
				///
				/// \return False if the sequence already is added, else true.
				///
				////////////////////////////////////////////////////////////////
				Bool AddAcknowledgement( const Uint16 p_Sequence );

			private:

				// Private variables
				static const Uint32		m_SequenceArraySize = 2048;
				Uint32					m_SequenceBits[ m_SequenceArraySize ];
				Uint8					m_CurrentBlocks[ 2 ];
				Mutex					m_Mutex;
			};

			////////////////////////////////////////////////////////////////
			/// \brief	Structure for reliable packets,
			///			save them for later resend if needed.
			///
			////////////////////////////////////////////////////////////////
			struct ReliablePacket
			{
				Uint16		Sequence;
				char *			pData;
				SizeType	DataSize;
				Timer		SendTimer;
				Timer		ResendTimer;
				Bool		Resent;
			};

			// Private  typedefs
			typedef std::queue<ReceivedData*>	ReceivedDataQueue;
			typedef std::queue<RawPacket*>		RawPacketQueue;
			typedef std::map<Uint16, ReliablePacket*> ReliablePacketMap;
			typedef std::pair<Uint16, ReliablePacket*> ReliablePacketPair;
			typedef std::list<Time> TimeList;

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Start the connection thread
			///
			////////////////////////////////////////////////////////////////
			void StartThreads( Server * p_pServer );

			////////////////////////////////////////////////////////////////
			/// \brief Add raw packet to queue.
			///
			////////////////////////////////////////////////////////////////
			void AddRawPacket( char * p_pData, const SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Poll raw packet from queue.
			///
			////////////////////////////////////////////////////////////////
			RawPacket * PollRawPacket( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the time since last received packet, including heartbeats.
			///
			////////////////////////////////////////////////////////////////
			Time TimeSinceLastRecvPacket( );

			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for disconnecting from the server.
			///
			////////////////////////////////////////////////////////////////
			void InternalDisconnect(	const Bool p_CloseMainThread,
										const Bool p_CloseEventThread,
										const Bool p_CloseReliableThread );

			////////////////////////////////////////////////////////////////
			/// \brief Send reliable packet to the server.
			///
			/// \param p_PacketType Should be UDP_ALIVE or UDP_RELIABLE_PACKET
			/// \param p_pData Pointer to the data to send.
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void InternalSendReliable( const ePacketType & p_PacketType, void * p_pData, const SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief	Calculate the new ping.
			///
			////////////////////////////////////////////////////////////////
			void CalculateNewPing( const Time & p_LapsedTime );

			// Private variables
			Thread							m_Thread;				///< Thread for handling raw packets.
			Thread							m_EventThread;			///< Thread for creating specific events.
			Thread							m_ReliableThread;		///< Thread for checking reliable packets for resend.
			Server *						m_pServer;				///< Pointer to the server.
			const Address					m_Address;				///< The clients's address.
			const Uint16					m_Port;					///< The client's port.
			const Uint16					m_UserId;				///< The client's user id.
			Time							m_ConnectionTimeout;	///< Ammount of time until the connection timeout.
			Semaphore						m_EventSemaphore;		///< Semaphore for the events.
			ThreadValue<ReceivedDataQueue>	m_ReceivedData;			///< Queue of data ready to get polled by the user.
			ThreadValue<RawPacketQueue>		m_RawPacketQueue;		///< Queue of raw packets.
			ThreadValue<Bool>				m_Connected;			///< Flag for checking if you are connected.
			ThreadValue<Timer>				m_LastRecvTimer;		///< Time for checking when the last recv packet.
			ThreadValue<Timer>				m_LastSendTimer;		///< Time for checking when the last sent packet.
			ThreadValue<Uint16>				m_Sequence;				///< The sequence of the next packet being sent.
			AcknowledgementData				m_AcknowledgementData;	///< Struct of the ack data.
			ThreadValue<ReliablePacketMap>	m_ReliableMap;			///< Map of reliable packets.
			ThreadValue<Time>				m_Ping;					///< Current network ping.
			TimeList						m_PingList;				///< List of the last pings.
		};

	}

}
#endif