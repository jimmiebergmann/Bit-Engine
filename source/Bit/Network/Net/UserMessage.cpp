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

#include <Bit/Network/Net/UserMessage.hpp>
#include <Bit/Network/Net/Server.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		UserMessage::UserMessage( const std::string & p_Name, Server * p_pServer, Int32 p_MessageSize ) :
			m_Name( p_Name ),
			m_pServer( p_pServer )
		{
			if( p_MessageSize > 0 )
			{
				m_Message.reserve( static_cast<MessageVector::size_type>( p_MessageSize ) +
								   static_cast<MessageVector::size_type>( p_Name.size( ) ) + 2 );
			}
			else
			{
				m_Message.reserve( static_cast<MessageVector::size_type>( p_Name.size( ) ) + 2  );
			}

			// Add the message type and message name
			m_Message.push_back( static_cast<Uint8>( eMessageType::UserMessageType ) );
			for( std::string::size_type i = 0; i < p_Name.size( ); i++ )
			{
				m_Message.push_back( static_cast<Uint8>( p_Name[ i ] ) );
			}
			m_Message.push_back( 0 );
		}

		void UserMessage::WriteByte( const Uint8 p_Byte )
		{
			m_Message.push_back( p_Byte );
		}

		void UserMessage::WriteInt( const Int32 p_Int )
		{
			Uint32 integer = Hton32( static_cast<Uint32>( p_Int ) );

			m_Message.push_back( static_cast<Uint8>( integer ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 8 ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 16 ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 24 ) );
		}

		void UserMessage::WriteFloat( const Float32 p_Float )
		{
			// Get void pointer to float
			const Uint8 * pPointer = reinterpret_cast<const Uint8 *>( &p_Float );

			m_Message.push_back( pPointer[ 0 ] );
			m_Message.push_back( pPointer[ 1 ] );
			m_Message.push_back( pPointer[ 2 ] );
			m_Message.push_back( pPointer[ 3 ] );
		}

		void UserMessage::WriteString( const std::string & p_String )
		{
			for( std::string::size_type i = 0; i < p_String.size( ); i++ )
			{
				m_Message.push_back( static_cast<Uint8>( p_String[ i ] ) );
			}
			m_Message.push_back( 0 );
		}

		Bool UserMessage::Send( UserRecipientFilter * p_pFilter )
		{
			// Error check the filter pointer.
			if( p_pFilter == NULL )
			{
				return false;
			}

			// Go throguh the connections from the server and send the data
			m_pServer->m_ConnectionMutex.Lock( );

			UserRecipientFilter::UserSet::const_iterator it1 = p_pFilter->m_Users.begin( );
			while(it1 != p_pFilter->m_Users.end( ) )
			{
				// Find the connection
				Server::UserConnectionMap::iterator it2 = m_pServer->m_UserConnections.find( *it1 );
				if( it2 == m_pServer->m_UserConnections.end( ) )
				{
					// Remove the user from the set if it doesn't exist.
					p_pFilter->m_Users.erase( it1 );

					// Continue to the next user
					continue;
				}
				else
				{
					// Increase the iterator for users.
					it1++;
				}

				// Get the connection
				Connection * pConnection = it2->second;

				// Send the message
				if( p_pFilter->IsReliable( ) )
				{
					pConnection->SendReliable( reinterpret_cast<void*>( m_Message.data( ) ), m_Message.size( ) );
				}
				else
				{
					pConnection->SendUnreliable( reinterpret_cast<void*>( m_Message.data( ) ), m_Message.size( ) );
				}
			}

			m_pServer->m_ConnectionMutex.Unlock( );

			return true;
		}

		const std::string & UserMessage::GetName( ) const
		{
			return m_Name;
		}

	}

}