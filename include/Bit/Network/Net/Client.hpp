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

#ifndef BIT_NETWORK_NET_CLIENT_HPP
#define BIT_NETWORK_NET_CLIENT_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/Network/Net/HostMessageListener.hpp>
#include <Bit/Network/Net/UserMessage.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Semaphore.hpp>
#include <Bit/System/Timer.hpp>
#include <queue>
#include <map>
#include <list>
#include <set>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Client class.
		///
		/// The network ping is calculated by taking the average ping
		/// from the last 3 received packets.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Client
		{

		public:

			// Friend classes
			friend class UserMessage;
			friend class HostMessageListener;

			// Public enums
			enum eStatus
			{
				Unknown,
				SocketError,
				TimedOut,
				Succeeded,
				Denied,
				Banned
			};

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Client( const Uint16 p_Port = 0,
					const Time & p_InitialPing = Bit::Microseconds( 200000 ) );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Client( );

		protected:

			////////////////////////////////////////////////////////////////
			/// \brief Connect to a UDP server.
			///
			/// \param p_Address The address of the server.
			/// \param p_port The servers port.
			/// \param p_ConnectionTimeout Amount of time until the attempt in connecting fails.
			/// \param p_Identifier Identifier string, the host need to
			///						have the same string in order to be able to connect.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			eStatus Connect(	const Address & p_Address,
								const Uint16 p_Port,
								const Time & p_ConnectionTimeout = Time::Infinite,
								const std::string & p_Identifier = "Bit Engine Network" );

			////////////////////////////////////////////////////////////////
			/// \brief Disconnect from the server.
			///
			////////////////////////////////////////////////////////////////
			void Disconnect( );

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
			/// \brief Add listener to a host message.
			///
			////////////////////////////////////////////////////////////////
			Bool HookHostMessage( HostMessageListener * p_pListener, const std::string & m_MessageName );

			////////////////////////////////////////////////////////////////
			/// \brief Create user message.
			///
			/// Destroy the pointer by yourself, or you will suffer from memoryleaks.
			///
			/// \param p_Name Name of the user message.
			/// \param p_MessageSize Message size. 0 < if dynamically allocated.
			///
			////////////////////////////////////////////////////////////////
			UserMessage * CreateUserMessage( const std::string & p_Name, const Int32 p_MessageSize = -1 );

			////////////////////////////////////////////////////////////////
			/// \brief Get the time since last received packet, including heartbeats.
			///
			////////////////////////////////////////////////////////////////
			Time TimeSinceLastRecvPacket( );

			// Protected variables
			EntityManager		m_EntityManager;

		private:

			////////////////////////////////////////////////////////////////
			/// \brief Received data structure
			///
			////////////////////////////////////////////////////////////////
			struct ReceivedData
			{
				ReceivedData( );
				~ReceivedData( );
				Uint16		Sequence;
				Uint8 *		pData;
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
				Uint8 *		pData;
				SizeType	DataSize;
				Timer		SendTimer;
				Timer		ResendTimer;
				Bool		Resent;
			};

			// Private  typedefs
			typedef std::map<Uint16, ReliablePacket*>					ReliablePacketMap;
			typedef std::pair<Uint16, ReliablePacket*>					ReliablePacketPair;
			typedef std::list<Time>										TimeList;
			typedef std::set<HostMessageListener*>						HostMessageListenerSet;
			typedef std::map<std::string, HostMessageListenerSet *>		HostMessageListenerMap;
			typedef std::pair<std::string, HostMessageListenerSet *>	HostMessageListenerPair;
			typedef std::queue<ReceivedData*>							ReceivedDataQueue;

			// Private functions
			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for disconnecting from the server.
			///
			////////////////////////////////////////////////////////////////
			void InternalDisconnect(	const Bool p_CloseMainThread,
										const Bool p_CloseTriggerThread,
										const Bool p_CloseReliableThread,
										const Bool p_CloseUserMessageThread );


			////////////////////////////////////////////////////////////////
			/// \brief Send unreliable packet to the server.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendUnreliable( void * p_pData, const SizeType p_DataSize );

			////////////////////////////////////////////////////////////////
			/// \brief Send reliable packet to the server.
			///
			/// \param p_pData Pointer to the data to send.
			/// \param p_DataSize Size of the data.
			///
			////////////////////////////////////////////////////////////////
			void SendReliable( void * p_pData, const SizeType p_DataSize );

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

			////////////////////////////////////////////////////////////////
			/// \brief	Function for adding user messages to the function caller queue.
			///
			////////////////////////////////////////////////////////////////
			void AddHostMessage( ReceivedData * p_ReceivedData );

			// Private variables
			UdpSocket							m_Socket;				///< Udp socket.
			Uint16								m_Port;					///< Udp and TCP port.
			Thread								m_Thread;				///< Thread created after the connection is established.
			Thread								m_TriggerThread;		///< Thread for creating specific triggers.
			Thread								m_ReliableThread;		///< Thread for checking reliable packets for resend.
			Thread								m_UserMessageThread;	///< Thread for handling user messages.
			Address								m_ServerAddress;		///< The server's address.
			Uint16								m_ServerPort;			///< The server's port.
			ThreadValue<Bool>					m_Connected;			///< Flag for checking if you are connected.
			ThreadValue<Timer>					m_LastRecvTimer;		///< Time for checking when the last recv packet.
			ThreadValue<Timer>					m_LastSendTimer;		///< Time for checking when the last sent reliable packet.
			ThreadValue<Time>					m_ConnectionTimeout;	///< Ammount of time until the connection timeout.
			ThreadValue<Uint16>					m_Sequence;				///< The sequence of the next packet being sent.
			AcknowledgementData					m_AcknowledgementData;	///< Struct of the ack data.
			ThreadValue<ReliablePacketMap>		m_ReliableMap;			///< Map of reliable packets.
			ThreadValue<Time>					m_Ping;					///< Current network ping.
			TimeList							m_PingList;				///< List of the last pings.
			ThreadValue<HostMessageListenerMap>	m_HostMessageListeners;	///< Map of user message listeners and their message types.
			ThreadValue<ReceivedDataQueue>		m_UserMessages;			///< Queue of user messages
			Semaphore							m_UserMessageSemaphore;	///< Semaphore for executing user message listeners.

		};

	}

}

#endif