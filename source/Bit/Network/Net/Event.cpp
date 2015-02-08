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

#include <Bit/Network/Net/Event.hpp>
#include <Bit/Network/Net/Server.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Event::Event( const std::string & p_Name, Server * p_pServer ) :
			m_Name( p_Name ),
			m_pServer( p_pServer )
		{
		}

		Event::~Event( )
		{
			for( VariableMap::iterator it = m_Variables.begin( );
				 it != m_Variables.end( );
				 it++ )
			{
				delete [ ] it->second->second;

				if( it->second )
				{
					delete [ ] it->second;
				}
			}
		}

		Bool Event::FireEvent( )
		{
			// Create the event data message
			std::vector<Uint8> message;

			// Add the message type
			message.push_back( static_cast<Uint8>( eMessageType::EventMessageType ) );

			// Add the name to the event
			for( std::string::size_type i = 0; i < m_Name.size( ); i++ )
			{
				message.push_back( static_cast<Uint8>( m_Name[ i ] ) );
			}
			message.push_back( 0 );
			
			// Add the variables
			for( VariableMap::iterator it = m_Variables.begin( );
				 it != m_Variables.end( );
				 it++ )
			{
				// Add the name of the current variable
				for( std::string::size_type i = 0; i < it->first.size( ); i++ )
				{
					message.push_back( static_cast<Uint8>( it->first[ i ] ) );
				}
				message.push_back( 0 );

				// Add the legth of the variable data
				Uint16 networkLenth = Hton16( it->second->first );
				message.push_back( static_cast<Uint8>( networkLenth ) );
				message.push_back( static_cast<Uint8>( networkLenth >> 8 ) );

				// Add the data
				for( Uint16 i = 0; i < it->second->first; i++ )
				{
					message.push_back( it->second->second[ i ] );
				}
			}

			// Go throguh all the connections
			m_pServer->m_ConnectionMutex.Lock( );

			for( Server::UserConnectionMap::iterator it = m_pServer->m_UserConnections.begin( );
				 it != m_pServer->m_UserConnections.end( );
				 it++ )
			{
				// Get the connection
				Connection * pConnection = it->second;

				// Send the message
				pConnection->SendReliable( reinterpret_cast<void*>( message.data( ) ), message.size( ) );
			}

			m_pServer->m_ConnectionMutex.Unlock( );

			return true;
		}

		const std::string & Event::GetName( ) const
		{
			return m_Name;
		}

		void Event::SetByte( const std::string & p_VariableName, const Uint8 p_Byte )
		{
			// Get or create variable.
			Variable * pVariable = GetVariable( p_VariableName );

			// Error check the variable pointer
			if( pVariable == NULL )
			{
				return;
			}

			// Clear the old data if required.
			if( pVariable->first != sizeof( Uint8 ) )
			{
				pVariable->first = sizeof( Uint8 );
				delete [] pVariable->second;
				pVariable->second = new Uint8[ sizeof( Uint8 ) ];
			}

			// Set the data
			memcpy( pVariable->second, &p_Byte, sizeof( Uint8 ) );

		}

		void Event::SetInt( const std::string & p_VariableName, const Int32 p_Int )
		{
			// Get or create variable.
			Variable * pVariable = GetVariable( p_VariableName );

			// Error check the variable pointer
			if( pVariable == NULL )
			{
				return;
			}

			// Clear the old data if required.
			if( pVariable->first != sizeof( Int32 ) )
			{
				pVariable->first = sizeof( Int32 );
				delete [] pVariable->second;
				pVariable->second = new Uint8[ sizeof( Int32 ) ];
			}

			// Set the data
			Int32 networkInt = Hton32( p_Int );
			memcpy( pVariable->second, &networkInt, sizeof( Int32 ) );
		}

		void Event::SetFloat( const std::string & p_VariableName, const Float32 p_Float )
		{
			// Get or create variable.
			Variable * pVariable = GetVariable( p_VariableName );

			// Error check the variable pointer
			if( pVariable == NULL )
			{
				return;
			}

			// Clear the old data if required.
			if( pVariable->first != sizeof( Float32 ) )
			{
				pVariable->first = sizeof( Float32 );
				delete [] pVariable->second;
				pVariable->second = new Uint8[ sizeof( Float32 ) ];
			}

			// Set the data
			memcpy( pVariable->second, &p_Float, sizeof( Float32 ) );
		}

		void Event::SetString( const std::string & p_VariableName, const std::string & p_String )
		{
			// Get or create variable.
			Variable * pVariable = GetVariable( p_VariableName );

			// Error check the variable pointer
			if( pVariable == NULL )
			{
				return;
			}

			// Clear the old data if required.
			if( pVariable->first != p_String.size( ) )
			{
				pVariable->first = p_String.size( );
				delete [] pVariable->second;
				pVariable->second = new Uint8[ p_String.size( ) ];
			}

			// Set the data
			memcpy( pVariable->second, p_String.c_str( ), p_String.size( ) );
		}

		Event::Variable * Event::GetVariable( const std::string & p_VariableName )
		{
			// Find the variable
			VariableMap::iterator it = m_Variables.find( p_VariableName );
			Variable * pVariable = NULL;

			// Create the variable if needed
			if( it == m_Variables.end( ) )
			{
				pVariable = new Variable;
				pVariable->first = 0;
				pVariable->second = NULL;
				m_Variables[ p_VariableName ] = pVariable;
			}
			else
			{
				pVariable = it->second;
			}

			return pVariable;
		}

	}

}