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
#include <Bit/System/Timestep.hpp>
#include <Bit/System/SmartMutex.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Server::Server( ) :
			m_EntityManager( new ServerEntityChanger( &m_EntityManager ) ),
			m_MaxConnections( 0 ),
			m_EntityUpdatesPerSecond( 0 )
		{
	
		}

		Server::~Server( )
		{
			Stop( );
			UnhookUserMessages( );
		}

		void Server::OnConnection( const Uint16 p_UserId )
		{
		}
			
		void Server::OnDisconnection( const Uint16 p_UserId )
		{
		}

		HostRecipientFilter * Server::CreateRecipientFilter( const Bool p_Reliable )
		{
			return new HostRecipientFilter( this, p_Reliable );
		}

		HostMessage * Server::CreateHostMessage( const std::string & p_Name, const Int32 p_MessageSize )
		{
			return new HostMessage( p_Name, this, p_MessageSize );
		}
			
		Bool Server::DisconnectUser( const Uint16 p_UserId )
		{
			// Find the connection via user id.
			m_ConnectionMutex.Lock( );
			UserConnectionMap::iterator it = m_UserConnections.find( p_UserId );
			if( it == m_UserConnections.end( ) )
			{
				// Unlock the connection mutex.
				m_ConnectionMutex.Unlock( );
				return false;
			}

			// Get the connection
			Connection * pConnection = it->second;

			// Unlock the connection mutex.
			m_ConnectionMutex.Unlock( );

			// Add the connection for cleanup
			AddConnectionForCleanup( pConnection );

			return true;
		}

		Bool Server::BanUser( const Uint16 p_UserId, const Time p_Time )
		{
			// Find the connection via user id.
			m_ConnectionMutex.Lock( );
			UserConnectionMap::iterator it = m_UserConnections.find( p_UserId );
			if( it == m_UserConnections.end( ) )
			{
				// Unlock the connection mutex.
				m_ConnectionMutex.Unlock( );
				return false;
			}

			// Get the connection
			Connection * pConnection = it->second;

			// Unlock the connection mutex.
			m_ConnectionMutex.Unlock( );

			// Add addresss to ban set
			m_BanSet.Mutex.Lock( );
			m_BanSet.Value.insert( pConnection->GetAddress( ) );
			m_BanSet.Mutex.Unlock( );

			Uint8 reason = DisconnectType::Banned;
			pConnection->SendUnreliable(PacketType::Disconnect, &reason, sizeof(reason), false, false);

			// Add the connection for cleanup
			AddConnectionForCleanup( pConnection );

			return true;
		}

		Bool Server::RemoveBan( const Address & p_Address )
		{
			// Check if the address is banned
			m_BanSet.Mutex.Lock( );
			AddressSet::iterator it = m_BanSet.Value.find( p_Address );
			if( it == m_BanSet.Value.end( ) )
			{
				// Unlock the ban set mutex.
				m_BanSet.Mutex.Unlock( );
				return false;
			}

			// Remove the address from the set.
			m_BanSet.Value.erase( it );

			// Unlock the ban set mutex.
			m_BanSet.Mutex.Unlock( );

			// Succeeded.
			return true;
		}

		void Server::BanIp( const Address & p_Address )
		{
			// Add addresss to ban set
			m_BanSet.Mutex.Lock( );
			m_BanSet.Value.insert( p_Address );
			m_BanSet.Mutex.Unlock( );
		}

		Bool Server::Start( const Uint16 p_Port,
							const Uint8 p_MaxConnections,
							const Uint8 p_EntityUpdatesPerSecond,
							const std::string & p_Identifier)
		{
			// Open the udp socket.
			if( m_Socket.Open( p_Port ) == false )
			{
				return false;
			}
			m_Socket.SetBlocking( true );

			// Set max connections.
			m_MaxConnections = p_MaxConnections;

			// Set entity updates per second
			m_EntityUpdatesPerSecond = p_EntityUpdatesPerSecond;

			// Add the free user IDs to the queue
			for( Uint16 i = 0; i < static_cast<Uint16>(m_MaxConnections); i++ )
			{
				m_FreeUserIds.push( i );
			}

			// Set the identifier
			m_Identifier = p_Identifier;

			// Start the server thread.
			m_MainThread.Execute([this]()
			{
				Uint8 buffer[BufferSize];
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
					recvSize = m_Socket.Receive(buffer, BufferSize, address, port, Milliseconds(5));

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
						if( buffer[ 0 ] == PacketType::Connect )
						{
							// Make sure that the identifier is right.
							if (recvSize != m_Identifier.size() + ConnectPacketSize)
							{
								continue;
							}
							if (memcmp(buffer + ConnectPacketSize, m_Identifier.data(), m_Identifier.size()) != 0)
							{
								continue;
							}

							// Check if the address is banned
							m_BanSet.Mutex.Lock( );
							if( m_BanSet.Value.find( address.GetAddress( ) ) != m_BanSet.Value.end( ) )
							{
								// Send ban packet
								buffer[ 0 ] = PacketType::Reject;
								buffer[ 1 ] = RejectType::Banned;
								m_Socket.Send(buffer, RejectPacketSize, address, port);

								// Unlock the ban set mutex.
								m_BanSet.Mutex.Unlock( );

								// Unlock the connection mutex
								m_ConnectionMutex.Unlock( );
								continue;
							}
							m_BanSet.Mutex.Unlock( );

							// Send Deny packet if the server is full.
							if( m_AddressConnections.size( ) == m_MaxConnections || m_FreeUserIds.size( ) == 0 )
							{
								buffer[0] = PacketType::Reject;
								buffer[1] = RejectType::Full;
								m_Socket.Send(buffer, RejectPacketSize, address, port);
									
								// Unlock the connection mutex
								m_ConnectionMutex.Unlock( );
								continue;
							}

							// Answer the client with a SYNACK packet.
							buffer[ 0 ] = PacketType::Accept;
							m_Socket.Send(buffer, AcceptPacketSize, address, port);

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


							// Run the on connection function
							OnConnection( userId );
						}
						else
						{
							// Unlock the connection mutex
							m_ConnectionMutex.Unlock( );
						}
					}
				
				}
			}
			);

			// Start the entity thread
			m_EntityThread.Execute( [ this ] ( )
			{
				// Create an instance of a timestep
				Timestep timestep;

				// Run the 
				while( IsRunning( ) )
				{
					if( m_EntityUpdatesPerSecond == 0 )
					{
						Sleep( Microseconds( 1000 ) );
						continue;
					}

					// Calculate the timestep time
					Time time = Seconds( 1.0f / static_cast<Float32>( m_EntityUpdatesPerSecond ) );

					// Execute the timestep
					timestep.Execute( time, [ this ] ( )
					{
						// Create the entity message
						std::vector<Uint8> message;
						if( m_EntityManager.CreateEntityMessage( message, false ) == false )
						{
							return;
						}

						// Error check the message
						if( message.size( ) == 0 )
						{
							return;
						}

						// Send the message to all the connections
						m_ConnectionMutex.Lock( );

						for(	UserConnectionMap::iterator it = m_UserConnections.begin( );
								it != m_UserConnections.end( );
								it++ )
						{
							it->second->SendUnreliable( PacketType::EntityUpdate, reinterpret_cast<Uint8 * >( message.data( ) ), message.size( ), true, true );
						}

						m_ConnectionMutex.Unlock( );
					}
					);
					
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
						m_CleanupConnections.Mutex.Unlock( );
						continue;
					}

					// Get the connection
					Connection * pConnection = m_CleanupConnections.Value.front( );
					
					// Error check the connection pointer
					if( pConnection == NULL )
					{
						m_CleanupConnections.Mutex.Unlock( );
						continue;
					}

					// Get the packet address and user id.
					const Uint64 packetAddress = pConnection->GetPackedAddress( );
					const Uint16 userId = pConnection->GetUserId( );

					// Delete the connection
					delete pConnection;

					// Pop the connection from the list
					m_CleanupConnections.Value.pop_front( );					

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

					// Restore the user id to the free user id queue
					m_FreeUserIds.push( userId );

					// Unlock the connection mutex
					m_ConnectionMutex.Unlock( );
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

				// Wait for the entity thread to finish
				m_EntityThread.Finish( );

				// Wait for the cleanup thread to finish
				m_CleanupSemaphore.Release( );
				m_CleanupThread.Finish( );

				// Clean up the cleanup connections
				m_CleanupConnections.Mutex.Lock( );
				m_CleanupConnections.Value.clear( );
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

		Bool Server::HookUserMessage( UserMessageListener * p_pListener, const std::string & m_MessageName )
		{
			// Error check the parameters
			if( p_pListener == NULL || m_MessageName.size( ) == 0 )
			{
				return false;
			}

			m_UserMessageListeners.Mutex.Lock( );

			// Find the user mesage set pointer.
			UserMessageListenerSet * pMessageSet = NULL;

			// Find the message
			UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.find( m_MessageName );
			if( it == m_UserMessageListeners.Value.end( ) )
			{
				// Create a new message
				pMessageSet = new UserMessageListenerSet;
				m_UserMessageListeners.Value.insert( UserMessageListenerPair( m_MessageName, pMessageSet ) );
			}
			else
			{
				pMessageSet = it->second;
			}

			// Add the listener to the message set
			pMessageSet->insert( p_pListener );

			m_UserMessageListeners.Mutex.Unlock( );

			return true;
		}

		void Server::UnhookUserMessages( )
		{
			m_UserMessageListeners.Mutex.Lock( );

			for(	UserMessageListenerMap::iterator it = m_UserMessageListeners.Value.begin( );
					it != m_UserMessageListeners.Value.end( );
					it++ )
			{
				delete it->second;
			}
			m_UserMessageListeners.Value.clear( );


			m_UserMessageListeners.Mutex.Unlock( );
		}

		void Server::AddConnectionForCleanup( Connection * p_pConnection )
		{
			// Add the connection to the cleanup thread.
			m_CleanupConnections.Mutex.Lock( );

			// Go throguh the cleanup connection and make sure that the connection is unique
			for( ConnectionList::iterator it = m_CleanupConnections.Value.begin( );
				 it != m_CleanupConnections.Value.end( );
				 it++ )
			{
				if( p_pConnection == (*it) )
				{
					return;
				}
			}

			m_CleanupConnections.Value.push_back( p_pConnection );
			m_CleanupConnections.Mutex.Unlock( );

			// Increase the semaphore for cleanups
			m_CleanupSemaphore.Release( );
		}

	}

}