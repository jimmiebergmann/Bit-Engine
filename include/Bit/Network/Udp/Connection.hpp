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

#ifndef BIT_NETWORK_UDP_CONNECTION_HPP
#define BIT_NETWORK_UDP_CONNECTION_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Udp/Event.hpp>
#include <Bit/Network/Udp/Packet.hpp>
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

	namespace Udp
	{

		////////////////////////////////////////////////////////////////
		/// \brief	Udp connection class.
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
			Connection(		const Bit::Address & p_Address, const Bit::Uint16 & p_Port,
							const Bit::Time & p_InitialPing = Bit::Microseconds( 200000 ) );
		
			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~Connection( );

			////////////////////////////////////////////////////////////////
			/// \brief Disconnect the client from the server.
			///
			/// The client is released from the server,
			/// which means that you have to delete the pointer by yourself. 
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
			/// \brief Send unreliable packet to the client.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendUnreliable( void * p_pData, const Bit::SizeType p_DataSize );

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
			Bit::Bool Receive( Packet & p_Packet );

		private:

			// Private structs

			////////////////////////////////////////////////////////////////
			/// \brief Raw packet queue for sending data from server to connection.
			///
			////////////////////////////////////////////////////////////////
			struct RawPacket
			{
				RawPacket( char * p_pData, const Bit::SizeType p_DataSize );
				~RawPacket( );

				char * pData;
				Bit::SizeType DataSize;
			};

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
			typedef std::queue<Event*>		EventQueue;
			typedef std::queue<ReceivedData*>	ReceivedDataQueue;
			typedef std::queue<RawPacket*>		RawPacketQueue;
			typedef std::map<Bit::Uint16, ReliablePacket*> ReliablePacketMap;
			typedef std::pair<Bit::Uint16, ReliablePacket*> ReliablePacketPair;
			typedef std::list<Bit::Time> TimeList;

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
			void AddRawPacket( char * p_pData, const Bit::SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Poll raw packet from queue.
			///
			////////////////////////////////////////////////////////////////
			RawPacket * PollRawPacket( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the time since last received packet, including heartbeats.
			///
			////////////////////////////////////////////////////////////////
			Bit::Time TimeSinceLastRecvPacket( );

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
			Bit::Thread							m_Thread;				///< Thread for handling raw packets.
			Bit::Thread							m_EventThread;			///< Thread for creating specific events.
			Bit::Thread							m_ReliableThread;		///< Thread for checking reliable packets for resend.
			Server *							m_pServer;				///< Pointer to the server.
			Bit::Address						m_Address;				///< The clients's address.
			Bit::Uint16							m_Port;					///< The client's port.
			Bit::Time							m_ConnectionTimeout;	///< Ammount of time until the connection timeout.
			Bit::Semaphore						m_EventSemaphore;		///< Semaphore for the events.
			Bit::ThreadValue<ReceivedDataQueue>	m_ReceivedData;			///< Queue of data ready to get polled by the user.
			Bit::ThreadValue<RawPacketQueue>	m_RawPacketQueue;		///< Queue of raw packets.
			Bit::ThreadValue<Bit::Bool>			m_Connected;			///< Flag for checking if you are connected.
			Bit::ThreadValue<Bit::Timer>		m_LastRecvTimer;		///< Time for checking when the last recv packet.
			Bit::ThreadValue<Bit::Timer>		m_LastSendTimer;		///< Time for checking when the last sent packet.
			Bit::ThreadValue<Bit::Uint16>		m_Sequence;				///< The sequence of the next packet being sent.
			AcknowledgementData					m_AcknowledgementData;	///< Struct of the ack data.
			Bit::ThreadValue<ReliablePacketMap>	m_ReliableMap;			///< Map of reliable packets.
			Bit::ThreadValue<Bit::Time>			m_Ping;					///< Current network ping.
			TimeList							m_PingList;				///< List of the last pings.
		};

	}

}
#endif