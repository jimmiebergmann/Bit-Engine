#include <Bit/Network/Udp/Server.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Udp
	{

		Server::Server( ) :
			m_MaxConnections( 0 )
		{
	
		}

		Server::~Server( )
		{
			for( ConnectionMap::iterator it = m_Connections.Value.begin( );
				 it != m_Connections.Value.end( );
				 it++ )
			{
				delete it->second;
			}
		}

		Bit::Bool Server::Start( const Bit::Uint16 p_Port, Bit::Uint8 p_MaxConnections )
		{
			// Open the udp socket.
			if( m_Socket.Open( p_Port ) == false )
			{
				return false;
			}
			m_Socket.SetBlocking( true );

			// Set max connections.
			m_MaxConnections = p_MaxConnections;

			// Start the server thread.
			m_Thread.Execute( [ this ] ( )
			{
					const Bit::SizeType bufferSize = 128;
					char buffer[ bufferSize ];
					Bit::Address address;
					Bit::Uint16 port = 0;
					Bit::Int16 recvSize = 0;

					// Set the running flag to true.
					m_Running.Mutex.Lock( );
					m_Running.Value = true;
					m_Running.Mutex.Unlock( );

					// Receive packets as long as the server is running.
					while( IsRunning( ) )
					{
						// Receive any packet.
						recvSize = m_Socket.Receive( buffer, bufferSize, address, port, Bit::Time::Infinite );

						// Ignore empty packets.
						if( recvSize <= 0 )
						{
							continue;
						}

						// Check if the packet is from any known client
						Bit::Uint64 clientId =	static_cast<Bit::Uint64>( address.GetAddress( ) ) * 
												static_cast<Bit::Uint64>( port ) +
												static_cast<Bit::Uint64>( port ) ;

						m_Connections.Mutex.Lock( );
						ConnectionMap::iterator it = m_Connections.Value.find( clientId );
						if( it != m_Connections.Value.end( ) )
						{
							// Send the packet to the client thread.
							it->second->AddRawPacket( buffer, recvSize );
							m_Connections.Mutex.Unlock( );
						}
						else
						{
							// This is an unknown client, maybe it's trying to connect.
							if( buffer[ 0 ] == ePacketType::Syn )
							{
								// Send Deny packet if the server is full.
								if( m_Connections.Value.size( ) == m_MaxConnections )
								{
									buffer[ 0 ] = ePacketType::Close;
									m_Socket.Send( buffer, 1, address, port );
									continue;
								}

								// Add the client to the client map
								Connection * pConnection = new Connection( address, port );
								m_Connections.Value.insert( ConnectionMapPair( clientId, pConnection ) );
								m_Connections.Mutex.Unlock( );
						
								// Start client thread.
								pConnection->StartThreads( this );

								// Answer the client with a SYNACK packet.
								buffer[ 0 ] = ePacketType::SynAck;
								m_Socket.Send( buffer, 1, address, port );

								// Add connect event
								AddEvent( eEventType::Connect, pConnection );
							}
						}
				
					}
				}
			);

			// Server is now running
			return true;
		}

		void Server::Stop( )
		{
			if( IsRunning( ) )
			{
				// Set the running flag to false.
				m_Running.Mutex.Lock( );
				m_Running.Value = false;
				m_Running.Mutex.Unlock( );

				// Remove all the events
				m_Events.Mutex.Lock( );
				while( m_Events.Value.size( ) )
				{
					Event * pEvent = m_Events.Value.front( );
					delete pEvent;
					m_Events.Value.pop( );
				}
				m_Events.Mutex.Unlock( );

				// Disconnect all the connections
				m_Connections.Mutex.Lock( );
				ConnectionMap::iterator it = m_Connections.Value.begin( );
				while( it != m_Connections.Value.end( ) )
				{
					// Disconnect and delete the connection.
					Connection * pConnection = it->second;
					pConnection->Disconnect( );
					delete pConnection;
			
					// Get the beginning again.
					it = m_Connections.Value.begin( );
				}
				m_Connections.Mutex.Unlock( );

			}
		}

		Bit::Bool Server::PollEvent( Event & p_Event )
		{
			// Lock the event mutex
			m_Events.Mutex.Lock( );

			// Error check the event queue size.
			if( m_Events.Value.size( ) == 0 )
			{
				// Unlock mutex and return error.
				m_Events.Mutex.Unlock( );
				return false;
			}

			// Set the paramter
			Event * pEvent = m_Events.Value.front( );
			p_Event.Type = pEvent->Type;
			p_Event.pConnection = pEvent->pConnection;

			// Delete the event
			delete pEvent;

			// Pop the event
			m_Events.Value.pop( );

			// Unlock mutex and return true.
			m_Events.Mutex.Unlock( );
			return true;
		}

		Bit::Bool Server::IsRunning( )
		{
			m_Running.Mutex.Lock( );
			Bit::Bool running = m_Running.Value;
			m_Running.Mutex.Unlock( );
			return running;
		}

		Server::ConnectionIterator Server::GetConnectionIteratorBegin( )
		{
			m_Connections.Mutex.Lock( );
			ConnectionIterator it = m_Connections.Value.begin( );
			m_Connections.Mutex.Unlock( );
			return it;
		}

		Server::ConnectionIterator Server::GetConnectionIteratorEnd( )
		{
			m_Connections.Mutex.Lock( );
			ConnectionIterator it = m_Connections.Value.end( );
			m_Connections.Mutex.Unlock( );
			return it;
		}

		void Server::AddEvent( const eEventType & p_Type, Connection * pConnection )
		{
			Event * pEvent = new Event;
			pEvent->Type = p_Type;
			pEvent->pConnection = pConnection;
			m_Events.Mutex.Lock( );
			m_Events.Value.push( pEvent );
			m_Events.Mutex.Unlock( );
		}

	}

}