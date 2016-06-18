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
#include <Bit/Network/Net/ClientEntityManager.hpp>
#include <Bit/Network/Net/Private/PacketTransfer.hpp>
#include <Bit/Network/Net/HostMessageListener.hpp>
#include <Bit/Network/Net/UserMessage.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Semaphore.hpp>
#include <set>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Client class.
		///
		/// Technical details.
		///		- UDP Packets
		///		- Sending user and host messages(client <---> server communication).
		///		- Removing dublicate packets.
		///		- Replicating entity variables from server to client.
		///		- Resending lost reliable packets.
		///		- Banning clients
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Client : private Private::PacketTransfer
		{

		public:

			// Friend classes
			friend class UserMessage;
			friend class HostMessageListener;
			friend class EntityManager;

			// Public enums
			enum eStatus
			{
				Unknown,
				SocketError,
				TimedOut,
				AlreadyConnected,
				Succeeded,
				Denied,
				Banned,
				Full,
			};

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			/// \param p_SourcePort Source port, if 0, then the OS will pick one for you.
			/// \param p_InitialPing DO WE EVEN NEED THIS?? Inital ping value.
			///
			////////////////////////////////////////////////////////////////
			Client( const Uint16 p_SourcePort = 0,
					const Time & p_InitialPing = Microseconds( 200000 ) );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			virtual ~Client();

			////////////////////////////////////////////////////////////////
			/// \brief Static function for pinging a server.
			///
			/// \param p_ServerAddress	Server address.
			/// \param p_ServerPort		Server port.
			/// \param p_PingTime		The returned ping time.
			/// \param p_Timeout		Time until the function timeout.
			///
			/// \return true if successfully pinged, else false.
			///
			////////////////////////////////////////////////////////////////
/*			static Bool PingServer(	const Address & p_ServerAddress,
									const Uint16 p_ServerPort,
									Time & p_PingTime,
									const Time & p_Timeout = Seconds( 3.0f ));
*/

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
								const Time & p_LosingConnectionTimeout = Seconds(3.0f),
								const Time & p_InterpolationTime = Milliseconds(100),
								const Time & p_ExtrapolationTime = Milliseconds(250),
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
			Time GetPing();

			////////////////////////////////////////////////////////////////
			/// \brief Get the current time of the server.
			///
			////////////////////////////////////////////////////////////////
			Time GetServerTime();

			////////////////////////////////////////////////////////////////
			/// \brief Function to execute when the server created a new entity.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnEntityCreation(Entity * p_pEntity);

			////////////////////////////////////////////////////////////////
			/// \brief Function to execute when the server destroy an entity.
			///
			/// This is the last time you can access the entity pointer before it gets destroyed.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnEntityDestroyed(Entity * p_pEntity);

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

			// Protected variables
			ClientEntityManager		m_EntityManager;

		private:

			// Private structs
			/*struct ReceivedUserMessage
			{
				Uint16 
				MemoryPool<Uint8>::Item PoolData;
			};*/

			// Private  typedefs
			typedef std::list<Time>										TimeList;
			typedef std::set<HostMessageListener*>						HostMessageListenerSet;
			typedef std::map<std::string, HostMessageListenerSet *>		HostMessageListenerMap;
			typedef std::pair<std::string, HostMessageListenerSet *>	HostMessageListenerPair;
			//typedef std::queue<ReceivedData*>							ReceivedDataQueue;

			// Private functions
			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for connecting to the server.
			///
			////////////////////////////////////////////////////////////////
			Bool InternalConnect(const Time & p_ConnectionTimeout, eStatus & p_Status);

			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for disconnecting from the server.
			///
			////////////////////////////////////////////////////////////////
			void InternalDisconnect(const Bool p_CloseMainThread,
									const Bool p_CloseTriggerThread,
									const Bool p_CloseReliableThread,
									const Bool p_CloseHostMessageThread);

			////////////////////////////////////////////////////////////////
			/// \brief	Calculate the new ping.
			///
			////////////////////////////////////////////////////////////////
			void CalculateNewPing( const Time & p_LapsedTime );

			////////////////////////////////////////////////////////////////
			/// \brief	Function for adding user messages to the function caller queue.
			///
			////////////////////////////////////////////////////////////////
			//void AddHostMessage(ReceivedData * p_ReceivedData);


			// Private variables
			Uint16								m_SrcPort;					///< Source port of the data link.
			std::string							m_ServerIdentifier;			///< Identifier string of server.
			Thread								m_Thread;					///< Thread created after the connection is established.
			Thread								m_TriggerThread;			///< Thread for creating specific triggers.
			Thread								m_ReliableThread;			///< Thread for checking reliable packets for resend.
			Thread								m_HostMessageHandleThread;	///< Thread for handling host messages.
			ThreadValue<Time>					m_ServerStartTime;			///< The server's time at the state of connection.
			ThreadValue<Timer>					m_ServerTimer;				///< Timer for checking how long time ago we connected to the server.
			ThreadValue<Bool>					m_Connected;				///< Flag for checking if you are connected.
			ThreadValue<Time>					m_LosingConnectionTimeout;	///< Ammount of time without any packets before losing the connection.
			ThreadValue<Time>					m_Ping;						///< Current network ping.
			TimeList							m_PingList;					///< List of the last pings.
			ThreadValue<HostMessageListenerMap>	m_HostMessageListeners;		///< Map of user message listeners and their message types.
			//ThreadValue<ReceivedDataQueue>	m_UserMessages;				///< Queue of user messages
			Semaphore							m_UserMessageSemaphore;		///< Semaphore for executing user message listeners.

		};

	}

}

#endif