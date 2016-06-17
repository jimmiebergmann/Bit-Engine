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
#include <Bit/Network/Net/Private/PacketTransfer.hpp>
#include <Bit/System/MemoryPool.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/Semaphore.hpp>
#include <set>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \brief	Connection class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Connection : private Private::PacketTransfer
		{

		public:

			// Public friend classes
			friend class Server;
			friend class EntityManager;
			friend class HostMessage;
			friend class Event;

			// Public typdefs
			typedef std::set<Uint32> GroupSet;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			Connection(	const Address & p_Address, 
						const Uint16 & p_Port,
						const Uint16 & p_UserId,
						const Time & p_LosingConnectionTimeout,
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
			/// \brief Add entity to group.
			///
			////////////////////////////////////////////////////////////////
			void AddToGroup(const Uint32 p_GroupIndex);

			////////////////////////////////////////////////////////////////
			/// \brief Remove entity from group.
			///
			////////////////////////////////////////////////////////////////
			void RemoveFromGroup(const Uint32 p_GroupIndex);

			////////////////////////////////////////////////////////////////
			/// \brief Add received data to container.
			///
			////////////////////////////////////////////////////////////////
			void AddMessage(MemoryPool<Uint8>::Item * p_pReceivedPacket);

			////////////////////////////////////////////////////////////////
			/// \brief Poll raw packet from queue.
			///
			////////////////////////////////////////////////////////////////
			MemoryPool<Uint8>::Item * PollMessages();

		private:

			// Private typedefs
			typedef std::queue<MemoryPool<Uint8>::Item *> PacketPoolItemQueue;

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Start the threads
			///
			////////////////////////////////////////////////////////////////
			void StartThreads( Server * p_pServer );

			////////////////////////////////////////////////////////////////
			/// \brief	Internal function for disconnecting from the server.
			///
			////////////////////////////////////////////////////////////////
			void InternalDisconnect(	const Bool p_CloseMainThread,
										const Bool p_CloseTimeoutThread,
										const Bool p_CloseReliableThread,
										const Bool p_CloseUserMessageThread );

			////////////////////////////////////////////////////////////////
			/// \brief	Calculate the new ping.
			///
			////////////////////////////////////////////////////////////////
			void CalculateNewPing( const Time & p_LapsedTime );

			////////////////////////////////////////////////////////////////
			/// \brief	Function for adding user messages to the user message data container.
			///
			////////////////////////////////////////////////////////////////
			void AddUserMessage(MemoryPool<Uint8> & p_UserMessageData);
			
			////////////////////////////////////////////////////////////////
			/// \brief	Set temporary pointer to entity message data handled by the server.
			///
			////////////////////////////////////////////////////////////////
			void SetTempEntityMessagePtr(void * p_pEntityMessageDataPtr);

			////////////////////////////////////////////////////////////////
			/// \brief	Get temporary pointer to entity message data handled by the server.
			///
			////////////////////////////////////////////////////////////////
			void * GetTempEntityMessagePtr();

			// Private variables
			Server *						m_pServer;					///< Pointer to the server.
			Thread							m_Thread;					///< Thread for handling raw packets.
			Thread							m_UserMessageThread;		///< Thread for handling user messages.
			Thread							m_TimeoutThread;				///< Thread for creating specific events.
			Thread							m_ReliableThread;			///< Thread for checking reliable packets for resend.
			
			const Uint16					m_UserId;					///< The client's user id.
			

			ThreadValue<PacketPoolItemQueue>	m_ReceivedPacketQueue;				///< Queue of received packets from parent(server).
			Semaphore							m_ReceivedPacketSemaphore;		///< Semaphore for executing user message listeners.

			
			//Semaphore						m_ReceivedDataSemaphore;	///< Semaphore for received data.
			ThreadValue<Bool>				m_Connected;				///< Flag for checking if you are connected.
			ThreadValue<Time>				m_Ping;						///< Current network ping.
			Time							m_LosingConnectionTimeout;	///< Ammount of time without any packets before losing the connection.
			TimeList						m_PingList;					///< List of the last pings.
			ThreadValue<GroupSet>			m_Groups;					///< Set of entity groups.
			void *							m_pEntityMessageDataPtr;	///< Temporary pointer to entity message data handled by the server.
		};

	}

}
#endif