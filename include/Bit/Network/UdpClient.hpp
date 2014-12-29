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

#ifndef BIT_NETWORK_UDP_CLIENT_HPP
#define BIT_NETWORK_UDP_CLIENT_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/UdpEvent.hpp>
#include <Bit/Network/UdpPacket.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Timer.hpp>
#include <queue>
#include <map>
#include <list>

namespace Bit
{

	namespace Udp
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Udp client class.
		///
		/// The network ping is calculated by taking the average ping
		/// from the last 3 received packets.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Client
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Client( const Bit::Time & p_InitialPing = Bit::Microseconds( 200000 ) );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Client( );

			////////////////////////////////////////////////////////////////
			/// \brief	Open the Udp client socket,
			///			you need to do this before connecting.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool Open( const Bit::Uint16 p_Port = 0 );

			////////////////////////////////////////////////////////////////
			/// \brief Connect to a UDP server.
			///
			/// \param p_Address The address of the server.
			/// \param p_port The servers port.
			/// \param p_ConnectionTimeout Amount of time until the attempt in connecting fails.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool Connect(	const Bit::Address & p_Address,
								const Bit::Uint16 p_Port,
								const Bit::Time & p_ConnectionTimeout = Bit::Time::Infinite );

			////////////////////////////////////////////////////////////////
			/// \brief Disconnect from the server.
			///
			////////////////////////////////////////////////////////////////
			void Disconnect( );

			////////////////////////////////////////////////////////////////
			/// \brief Check if the client is connected.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool IsConnected( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the current ping.
			///
			////////////////////////////////////////////////////////////////
			Bit::Time GetPing( );

			////////////////////////////////////////////////////////////////
			/// \brief Send unreliable packet to the server.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendUnreliable( void * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Send reliable packet to the server.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendReliable( void * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Receive data from server
			///
			/// You have to delete the data in the udp packet by yourself to prevent memory leaks.
			///
			/// \param p_Packet Udp packet.
			///
			/// \return True if received packet, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool Receive( Packet & p_Packet );

			////////////////////////////////////////////////////////////////
			/// \brief Poll events from the server.
			///
			/// \return True if there is any event to poll, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool PollEvent( Event & p_Event );

			////////////////////////////////////////////////////////////////
			/// \brief Get the time since last received packet, including heartbeats.
			///
			////////////////////////////////////////////////////////////////
			Bit::Time TimeSinceLastRecvPacket( );

		private:

			////////////////////////////////////////////////////////////////
			/// \brief Received data structure
			///
			////////////////////////////////////////////////////////////////
			struct ReceivedData
			{
				Bit::Uint16		Sequence;
				char *			pData;
				Bit::SizeType	DataSize;
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
				Bit::Bool AddAcknowledgement( const Bit::Uint16 p_Sequence );

			private:

				// Private variables
				static const Bit::Uint32	m_SequenceArraySize = 2048;
				Bit::Uint32					m_SequenceBits[ m_SequenceArraySize ];
				Bit::Uint8					m_CurrentBlocks[ 2 ];
				Bit::Mutex					m_Mutex;
			};

			////////////////////////////////////////////////////////////////
			/// \brief	Structure for reliable packets,
			///			save them for later resend if needed.
			///
			////////////////////////////////////////////////////////////////
			struct ReliablePacket
			{
				Bit::Uint16		Sequence;
				char *			pData;
				Bit::SizeType	DataSize;
				Bit::Timer		SendTimer;
				Bit::Timer		ResendTimer;
				Bit::Bool		Resent;
			};

			// Private  typedefs
			typedef std::queue<Event*>	EventQueue;
			typedef std::queue<ReceivedData*> ReceivedDataQueue;
			typedef std::map<Bit::Uint16, ReliablePacket*> ReliablePacketMap;
			typedef std::pair<Bit::Uint16, ReliablePacket*> ReliablePacketPair;
			typedef std::list<Bit::Time> TimeList;

			// Private functions
			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for disconnecting from the server.
			///
			////////////////////////////////////////////////////////////////
			void InternalDisconnect(	const Bit::Bool p_CloseMainThread,
										const Bit::Bool p_CloseEventThread,
										const Bit::Bool p_CloseReliableThread );

			////////////////////////////////////////////////////////////////
			/// \brief Send reliable packet to the server.
			///
			/// \param p_PacketType Should be UDP_ALIVE or UDP_RELIABLE_PACKET
			/// \param p_pData Pointer to the data to send.
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void InternalSendReliable( const ePacketType & p_PacketType, void * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief	Calculate the new ping.
			///
			////////////////////////////////////////////////////////////////
			void CalculateNewPing( const Bit::Time & p_LapsedTime );

			// Private variables
			Bit::UdpSocket						m_Socket;				///< Udp socket.
			Bit::Thread							m_Thread;				///< Thread created after the connection is established.
			Bit::Thread							m_EventThread;			///< Thread for creating specific events.
			Bit::Thread							m_ReliableThread;		///< Thread for checking reliable packets for resend.
			Bit::Address						m_ServerAddress;		///< The server's address.
			Bit::Uint16							m_ServerPort;			///< The server's port.
			Bit::ThreadValue<EventQueue>		m_Events;				///< Queue of events ready to get polled by the user.
			Bit::ThreadValue<ReceivedDataQueue>	m_ReceivedData;			///< Queue of data ready to get polled by the user.
			Bit::ThreadValue<Bit::Bool>			m_Connected;			///< Flag for checking if you are connected.
			Bit::ThreadValue<Bit::Timer>		m_LastRecvTimer;		///< Time for checking when the last recv packet.
			Bit::ThreadValue<Bit::Timer>		m_LastSendTimer;		///< Time for checking when the last sent reliable packet.
			Bit::ThreadValue<Bit::Time>			m_ConnectionTimeout;	///< Ammount of time until the connection timeout.
			Bit::ThreadValue<Bit::Uint16>		m_Sequence;				///< The sequence of the next packet being sent.
			AcknowledgementData					m_AcknowledgementData;	///< Struct of the ack data.
			Bit::ThreadValue<ReliablePacketMap>	m_ReliableMap;			///< Map of reliable packets.
			Bit::ThreadValue<Bit::Time>			m_Ping;					///< Current network ping.
			TimeList							m_PingList;				///< List of the last pings.
	
		};

	}

}

#endif