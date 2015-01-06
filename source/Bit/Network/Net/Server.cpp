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
#include <Bit/Network/TcpSocket.hpp>
#include <Bit/System/Sleep.hpp>
#include <Bit/System/Timer.hpp>
#include <iostream>
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

		void Server::OnConnection( const Uint16 p_UserId )
		{
		}
			
		void Server::OnDisconnection( const Uint16 p_UserId )
		{
		}

		UserRecipientFilter * Server::CreateRecipientFilter( const Bool p_Reliable )
		{
			return new UserRecipientFilter( this, p_Reliable );
		}

		UserMessage * Server::CreateUserMessage( const std::string & p_Name, const Int32 p_MessageSize )
		{
			return new UserMessage( p_Name, this, p_MessageSize );
		}

		Event * Server::CreateEvent( const std::string & p_Name )
		{
			return new Event( p_Name, this );
		}
			
		Bool Server::DisconnectUser( const Uint16 p_UserId )
		{
			// Find the connection via user id.
			m_ConnectionMutex.Lock( );
			UserConnectionMap::iterator it = m_UserConnections.find( p_UserId );
			if( it == m_UserConnections.end( ) )
			{
				return false;
			}

			// Get the connection
			Connection * pConnection = it->second;

			// Unlock the connection mutex.
			m_ConnectionMutex.Unlock( );

			// Add the connection to the cleanup thread.
			m_CleanupConnections.Mutex.Lock( );
			m_CleanupConnections.Value.push( pConnection );
			m_CleanupConnections.Mutex.Unlock( );

			// Increase the semaphore for cleanups
			m_CleanupSemaphore.Release( );

			return true;
		}

		Bool Server::BanUser( const Uint16 p_UserId )
		{
			return false;
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

			// Add the free user IDs to the queue
			for( Uint16 i = 1; i <= static_cast<Uint16>(m_MaxConnections); i++ )
			{
				m_FreeUserIds.push( i );
			}

			// Start the server thread.
			m_MainThread.Execute( [ this ] ( )
			{
					const SizeType bufferSize = 2048;
					Uint8 buffer[ bufferSize ];
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
						recvSize = m_Socket.Receive( buffer, bufferSize, address, port, Milliseconds( 5 ) );

						// Ignore empty packets.
						if( recvSize <= 0 )
						{
							continue;
						}

						// Check if the packet is from any known client
						Uint64 clientAddress =	static_cast<Uint64>( address.GetAddress( ) ) * 
												static_cast<Uint64>( port ) +
												static_cast<Uint64>( port ) ;

						m_ConnectionMutex.Lock( );
						AddressConnectionMap::iterator it = m_AddressConnections.find( clientAddress );
						if( it != m_AddressConnections.end( ) )
						{
							// Send the packet to the client thread.
							it->second->AddRawPacket( buffer, recvSize );
							m_ConnectionMutex.Unlock( );
						}
						else
						{
							// This is an unknown client, maybe it's trying to connect.
							if( buffer[ 0 ] == ePacketType::Syn )
							{
								// Send Deny packet if the server is full.
								if( m_AddressConnections.size( ) == m_MaxConnections || m_FreeUserIds.size( ) == 0 )
								{
									buffer[ 0 ] = ePacketType::Close;
									m_Socket.Send( buffer, 1, address, port );
									continue;
								}

								// Get a user id for this connection
								const Uint16 userId = m_FreeUserIds.front( );
								m_FreeUserIds.pop( );

								// Create the connection
								Connection * pConnection = new Connection( address, port, userId );

								// Add the client to the address connection map
								m_AddressConnections.insert( AddressConnectionMapPair( clientAddress, pConnection ) );

								// Add the client to the user connection map
								m_UserConnections.insert( UserConnectionMapPair( userId, pConnection ) );

								// Unlock the connection mutex
								m_ConnectionMutex.Unlock( );
						
								// Start client thread.
								pConnection->StartThreads( this );

								// Answer the client with a SYNACK packet.
								buffer[ 0 ] = ePacketType::SynAck;
								m_Socket.Send( buffer, 1, address, port );

								// Run the on connection function
								OnConnection( userId );
							}
						}
				
					}
				}
			);

			// Start the cleanup thread.
			m_CleanupThread.Execute( [ this ] ( )
			{
				while( IsRunning( ) )
				{
					// Wait for semaphore to trigger
					m_CleanupSemaphore.Wait( );

					// Lock the cleanup connections mutex
					m_CleanupConnections.Mutex.Lock( );

					// Error check the queue
					if( m_CleanupConnections.Value.size( ) == 0 )
					{
						continue;
					}

					Connection * pConnection = m_CleanupConnections.Value.front( );
					m_CleanupConnections.Value.pop( );

					// Get the packet address and user id.
					const Uint64 packetAddress = pConnection->GetPackedAddress( );
					const Uint16 userId = pConnection->GetUserId( );

					// Unlock mutex
					m_CleanupConnections.Mutex.Unlock( );

					// Remove the connection and wait for the connection to thread to finish
					m_ConnectionMutex.Lock( );

					// Erase the connection from the address connections
					AddressConnectionMap::iterator it1 = m_AddressConnections.find( packetAddress );
					if( it1 != m_AddressConnections.end( ) )
					{
						m_AddressConnections.erase( it1 );
					}

					// Erase the connection from the user id connections
					UserConnectionMap::iterator it2 = m_UserConnections.find( userId );
					if( it2 != m_UserConnections.end( ) )
					{
						m_UserConnections.erase( it2 );
					}

					// Unlock the connection mutex
					m_ConnectionMutex.Unlock( );

					// Delete the connection
					delete pConnection;
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

				// Wait for the cleanup thread to finish
				m_CleanupSemaphore.Release( );
				m_CleanupThread.Finish( );

				// Clean up the cleanup connections
				m_CleanupConnections.Mutex.Lock( );
				while( m_CleanupConnections.Value.size( ) )
				{
					m_CleanupConnections.Value.pop( );
				}
				m_CleanupConnections.Mutex.Unlock( );

				// Wait for the main thread to finish
				m_MainThread.Finish( );

				// Disconnect all the connections
				m_ConnectionMutex.Lock( );
				AddressConnectionMap::iterator it = m_AddressConnections.begin( );
				while( it != m_AddressConnections.end( ) )
				{
					// Disconnect and delete the connection.
					Connection * pConnection = it->second;
					delete pConnection;

					// Erase the connection
					m_AddressConnections.erase( it );

					// Get the beginning again.
					it = m_AddressConnections.begin( );
				}

				// Remove all the user connections
				m_UserConnections.clear( );

				// Unlock the mutex
				m_ConnectionMutex.Unlock( );


				// Close the sockets
				m_Socket.Close( );

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