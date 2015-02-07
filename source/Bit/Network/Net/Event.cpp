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
		}

		Bool Event::FireEvent( )
		{
			// Create the event data message
			std::vector<Uint8> message;

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
		}

		void Event::SetInt( const std::string & p_VariableName, const Int32 p_Int )
		{
		}

		void Event::SetFloat( const std::string & p_VariableName, const Float32 p_Float )
		{
		}

		void Event::SetString( const std::string & p_VariableName, const std::string & p_String )
		{
		}

	}

}