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

#include <Bit/Network/Net/HostRecipientFilter.hpp>
#include <Bit/Network/Net/Server.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		HostRecipientFilter::HostRecipientFilter( Server * p_pServer, const Bool p_Reliable ) :
			m_pServer( p_pServer ),
			m_Reliable( p_Reliable )
		{
		}

		Bool HostRecipientFilter::AddUser( const Uint16 p_UserId )
		{
			m_Users.insert( p_UserId );
			return true;
		}

		Bool HostRecipientFilter::AddAllUsers( )
		{
			// Go through the connections and add the connections
			m_pServer->m_ConnectionMutex.Lock( );

			for( Server::UserConnectionMap::iterator it = m_pServer->m_UserConnections.begin( );
				 it != m_pServer->m_UserConnections.end( );
				 it++ )
			{
				m_Users.insert( it->first );
			}

			m_pServer->m_ConnectionMutex.Unlock( );

			return true;
		}

		Bool HostRecipientFilter::AddGroup( const std::string & p_GroupName )
		{
			return false;
		}

		void HostRecipientFilter::MakeReliable( )
		{
			m_Reliable = true;
		}

		void HostRecipientFilter::MakeUnreliable( )
		{
			m_Reliable = false;
		}

		Bool HostRecipientFilter::IsReliable( ) const
		{
			return m_Reliable;
		}

	}

}