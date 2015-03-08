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

#ifndef BIT_NETWORK_NET_SERVER_HPP
#define BIT_NETWORK_NET_SERVER_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <Bit/Network/Net/Private/Connection.hpp>
#include <Bit/Network/Net/UserMessageListener.hpp>
#include <Bit/Network/Net/Event.hpp>
#include <Bit/Network/Net/HostMessage.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/Network/TcpListener.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Semaphore.hpp>
#include <queue>
#include <map>

#ifdef BIT_PLATFORM_WINDOWS
#undef CreateEvent
#endif

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Server class.
		///
		/// The server system uses both TCP and
		///	UDP( TCP for connecting and UDP for data).
		/// This means that you have to open both
		///	TCP and UDP ports for the port that you are hosting on.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Server
		{

		public:

			// friend classes
			friend class Connection;
			friend class HostMessage;
			friend class HostRecipientFilter;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			Server( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~Server( );

		protected:
/*
			// Protected typdefs
			////////////////////////////////////////////////////////////////
			/// \brief Pointer to user message function.
			///	
			/// Parameters:
			///		- Server pointer.
			///		- User id.
			///		- Message pointer.
			///		- Message size.
			///
			////////////////////////////////////////////////////////////////
			typedef void (* UserMessageFunction)(Server *, Uint16, const void *, SizeType );
*/
			// Function to be overloaded.
			////////////////////////////////////////////////////////////////
			/// \brief Function to execute when a user connects.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnConnection( const Uint16 p_UserId );
			
			////////////////////////////////////////////////////////////////
			/// \brief Function to execute when a user disconnects.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnDisconnection( const Uint16 p_UserId );

			////////////////////////////////////////////////////////////////
			/// \brief Create recipient filter
			///
			/// Destroy the pointer by yourself, or you will suffer from memoryleaks.
			///
			////////////////////////////////////////////////////////////////
			HostRecipientFilter * CreateRecipientFilter( const Bool p_Reliable = true );

			////////////////////////////////////////////////////////////////
			/// \brief Create host message.
			///
			/// Destroy the pointer by yourself, or you will suffer from memoryleaks.
			///
			/// \param p_Name Name of the host message.
			/// \param p_MessageSize Message size. 0 < if dynamically allocated.
			///
			////////////////////////////////////////////////////////////////
			HostMessage * CreateHostMessage( const std::string & p_Name, const Int32 p_MessageSize = -1 );

			////////////////////////////////////////////////////////////////
			/// \brief Disconnect a user from the server.
			///
			////////////////////////////////////////////////////////////////
			Bool DisconnectUser( const Uint16 p_UserId );

			////////////////////////////////////////////////////////////////
			/// \brief Ban a user from the server.
			///
			////////////////////////////////////////////////////////////////
			Bool BanUser( const Uint16 p_UserId, const Time p_Time = Microseconds( 0 ) );

			////////////////////////////////////////////////////////////////
			/// \brief Remove a ip ban.
			///
			////////////////////////////////////////////////////////////////
			Bool RemoveBan( const Address & p_Address );

			////////////////////////////////////////////////////////////////
			/// \brief Ban an ip address from the server.
			///
			////////////////////////////////////////////////////////////////
			void BanIp( const Address & p_Address );

			////////////////////////////////////////////////////////////////
			/// \brief Start the server.
			///
			/// \param p_Port Server port.
			/// \param p_MaxConnections Maximum number of connections.
			/// \param p_EntityUpdatesPerSecond Entity updates per second.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool Start( const Uint16 p_Port, 
						const Uint8 p_MaxConnections = 255,
						const Uint8 p_EntityUpdatesPerSecond = 20 );

			////////////////////////////////////////////////////////////////
			/// \brief Stop the server.
			///
			////////////////////////////////////////////////////////////////
			void Stop( );

			////////////////////////////////////////////////////////////////
			/// \brief Check if the server is running
			///
			////////////////////////////////////////////////////////////////
			Bool IsRunning( );

			////////////////////////////////////////////////////////////////
			/// \brief Add listener to a user message.
			///
			////////////////////////////////////////////////////////////////
			Bool HookUserMessage( UserMessageListener * p_pListener, const std::string & m_MessageName );

			////////////////////////////////////////////////////////////////
			/// \brief Unhook all the user messages
			///
			////////////////////////////////////////////////////////////////
			void UnhookUserMessages( );

			// Protected variables
			EntityManager		m_EntityManager;

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Add a client to the cleanup vector.
			///
			////////////////////////////////////////////////////////////////
			void AddConnectionForCleanup( Connection * p_pConnection );

			// Private  typedefs
			typedef std::map<Uint64,	Connection*>					AddressConnectionMap;
			typedef std::pair<Uint64,	Connection*>					AddressConnectionMapPair;
			typedef std::map<Uint16,	Connection*>					UserConnectionMap;
			typedef std::pair<Uint16,	Connection*>					UserConnectionMapPair;
			typedef std::list<Connection*>								ConnectionList;
			typedef std::queue<Uint16>									FreeUserIdMap;
			typedef std::set<Address>									AddressSet;
			typedef std::set<UserMessageListener*>						UserMessageListenerSet;
			typedef std::map<std::string, UserMessageListenerSet *>		UserMessageListenerMap;
			typedef std::pair<std::string, UserMessageListenerSet *>	UserMessageListenerPair;
			

			// Private variables
			UdpSocket							m_Socket;					///< Udp socket.
			Thread								m_MainThread;				///< Thread for handling incoming packets.
			Thread								m_EntityThread;				///< Thread for sending entity states to users.
			Thread								m_CleanupThread;			///< Thread for cleaning up connections.
			Semaphore							m_CleanupSemaphore;			///< Semaphore for cleanups.
			ThreadValue<ConnectionList>			m_CleanupConnections;		///< Queue of connections to cleanup.
			Uint8								m_MaxConnections;			///< Maximum amount of connections.
			Uint8								m_EntityUpdatesPerSecond;	///< Number of updates per second for the entities.
			FreeUserIdMap						m_FreeUserIds;				///< Queue of free user Ids.
			AddressConnectionMap				m_AddressConnections;		///< Map of all the connections via their addresses.
			UserConnectionMap					m_UserConnections;			///< Map of all the connections via their user IDs.
			Mutex								m_ConnectionMutex;			///< Mutex for the address and user connections.
			ThreadValue<Bool>					m_Running;					///< Flag for checking if the server is running.
			ThreadValue<AddressSet>				m_BanSet;					///< Set of banned addresses.
			ThreadValue<UserMessageListenerMap>	m_UserMessageListeners;		///< Map of user message listeners and their message types.
			
		};

	}

}

#endif