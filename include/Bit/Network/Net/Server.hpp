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
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <queue>
#include <map>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Server class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Server
		{

		public:

			// friend classes
			friend class Connection;

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

			////////////////////////////////////////////////////////////////
			/// \brief Start the server.
			///
			/// \param p_Port Server port.
			/// \param p_MaxConnections Maximum number of connections.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool Start( const Uint16 p_Port, Uint8 p_MaxConnections = 255 );

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

			// Protected variables
			EntityManager		m_EntityManager;

		private:

			// Private  typedefs
			typedef std::map<Uint64,	Connection*>	ConnectionMap;
			typedef std::pair<Uint64,	 Connection*>	ConnectionMapPair;

			// Private variables
			UdpSocket					m_Socket;			///< Udp socket.
			Thread						m_Thread;			///< Thread for handling incoming packets.
			Uint8						m_MaxConnections;	///< Maximum amount of connections.
			ThreadValue<ConnectionMap>	m_Connections;		///< Map of all the connections
			ThreadValue<Bool>			m_Running;			///< Flag for checking if the server is running.
	
		};

	}

}

#endif