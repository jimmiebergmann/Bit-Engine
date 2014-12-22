#ifndef BIT_NETWORK_UDP_SERVER_HPP
#define BIT_NETWORK_UDP_SERVER_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Udp/Event.hpp>
#include <Bit/Network/Udp/Packet.hpp>
#include <Bit/Network/Udp/Connection.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <Bit/System/Thread.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <queue>
#include <map>

namespace Bit
{

	namespace Udp
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Udp server class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Server
		{

		public:

			// friend classes
			friend class Connection;

			// Public typedefs
			typedef std::map<Bit::Uint64, Connection*>::const_iterator ConnectionIterator;

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

			////////////////////////////////////////////////////////////////
			/// \brief Start the server.
			///
			/// \param p_Port Server port.
			/// \param p_MaxConnections Maximum number of connections.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool Start( const Bit::Uint16 p_Port, Bit::Uint8 p_MaxConnections = 255 );

			////////////////////////////////////////////////////////////////
			/// \brief Stop the server.
			///
			////////////////////////////////////////////////////////////////
			void Stop( );

			////////////////////////////////////////////////////////////////
			/// \brief Poll events from the server.
			///
			/// \return True if there is any event to poll, else false.
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool PollEvent( Event & p_Event );

			////////////////////////////////////////////////////////////////
			/// \brief Check if the server is running
			///
			////////////////////////////////////////////////////////////////
			Bit::Bool IsRunning( );

			////////////////////////////////////////////////////////////////
			/// \brief Get begin connection iterator.
			///
			////////////////////////////////////////////////////////////////
			ConnectionIterator GetConnectionIteratorBegin( );

			////////////////////////////////////////////////////////////////
			/// \brief Get end connection iterator.
			///
			////////////////////////////////////////////////////////////////
			ConnectionIterator GetConnectionIteratorEnd( );

		private:

			// Private  typedefs
			typedef std::queue<Event*>						EventQueue;
			typedef std::map<Bit::Uint64,	Connection*>	ConnectionMap;
			typedef std::pair<Bit::Uint64,	 Connection*>	ConnectionMapPair;

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Add event to the event queue
			///
			///////////////////////////////////////////////////////////////
			void AddEvent( const eEventType & p_Type, Connection * pConnection = NULL );

			// Private variables
			Bit::UdpSocket					m_Socket;			///< Udp socket.
			Bit::Thread						m_Thread;			///< Thread for handling incoming packets.
			Bit::Uint8						m_MaxConnections;	///< Maximum amount of connections.
			Bit::ThreadValue<EventQueue>	m_Events;			///< Queue of events.
			Bit::ThreadValue<ConnectionMap>	m_Connections;		///< Map of all the connections
			Bit::ThreadValue<Bit::Bool>		m_Running;			///< Flag for checking if the server is running.
	
		};

	}

}

#endif