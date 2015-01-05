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

#include <Bit/Network/Net/Server.hpp>
#include <Bit/Network/Net/Private/ServerEntityChanger.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Server::Server( ) :
			m_EntityManager( new ServerEntityChanger( &m_EntityManager ) ),
			m_MaxConnections( 0 )
		{
	
		}

		Server::~Server( )
		{
			Stop( );
		}

		Event * Server::CreateEvent( const std::string & p_Name )
		{
			return new Event( p_Name, this );
		}

		UserMessage * Server::CreateUserMessage( const std::string & p_Name )
		{
			return new UserMessage( p_Name, this );
		}
			
		void Server::DisconnectUser( const Uint16 p_UserId )
		{
		}

		void Server::BanUser( const Uint16 p_UserId )
		{
		}

		void Server::BanIp( const Address & p_Address )
		{
		}

		Bool Server::Start( const Uint16 p_Port, Uint8 p_MaxConnections )
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
					const SizeType bufferSize = 128;
					char buffer[ bufferSize ];
					Address address;
					Uint16 port = 0;
					Int16 recvSize = 0;

					// Set the running flag to true.
					m_Running.Mutex.Lock( );
					m_Running.Value = true;
					m_Running.Mutex.Unlock( );

					// Receive packets as long as the server is running.
					while( IsRunning( ) )
					{
						// Receive any packet.
						recvSize = m_Socket.Receive( buffer, bufferSize, address, port, Time::Infinite );

						// Ignore empty packets.
						if( recvSize <= 0 )
						{
							continue;
						}

						// Check if the packet is from any known client
						Uint64 clientAddress =	static_cast<Uint64>( address.GetAddress( ) ) * 
												static_cast<Uint64>( port ) +
												static_cast<Uint64>( port ) ;

						m_Connections.Mutex.Lock( );
						ConnectionMap::iterator it = m_Connections.Value.find( clientAddress );
						if( it != m_Connections.Value.end( ) )
						{
							// Send the packet to the client thread.
							it->second->AddRawPacket( buffer, recvSize );
							m_Connections.Mutex.Unlock( );
						}
						/*else
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
								//AddEvent( eEventType::Connect, pConnection );
							}
						}*/
				
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

				// Disconnect all the connections
				m_Connections.Mutex.Lock( );
				ConnectionMap::iterator it = m_Connections.Value.begin( );
				while( it != m_Connections.Value.end( ) )
				{
					// Disconnect and delete the connection.
					Connection * pConnection = it->second;
					delete pConnection;

					// Erase the connection
					m_Connections.Value.erase( it );

					// Get the beginning again.
					it = m_Connections.Value.begin( );
				}
				m_Connections.Mutex.Unlock( );

			}
		}

		Bool Server::IsRunning( )
		{
			m_Running.Mutex.Lock( );
			Bool running = m_Running.Value;
			m_Running.Mutex.Unlock( );
			return running;
		}

	}

}