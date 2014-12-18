// ///////////////////////////////////////////////////////////////////////////
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

#include <Bit/Network/SocketSelector.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	SocketSelector::SocketSelector( )
	{
		FD_ZERO( &m_SocketSet );
	}

	SocketSelector::~SocketSelector( )
	{
	}

	Bool SocketSelector::Select( Socket & p_Socket, const Time & p_Timeout )
	{

		// Create timeout structure.
		/*struct timeval tv;
		tv.tv_sec	= static_cast<long>( p_Timeout.AsMicroseconds( ) / 1000000ULL );
		tv.tv_usec	= static_cast<long>( p_Timeout.AsMicroseconds( ) % 1000000ULL );
		

		// Select from the set
		if( select( static_cast<int>( SocketHandle ) + 1, &m_FDSet, NULL, NULL, &tv ) <= 0 )
		{
			return false;
		}


		// Set the handle for the TcpSocket.
		p_Socket.SetHandle( acceptSocket );

		*/
		return false;
	}

	void SocketSelector::AddSocket( Socket & p_Socket )
	{
		// Do not add the socket if it's already in the map
		SocketMap::iterator it = m_Sockets.find( p_Socket.GetHandle( ) );
		if( it == m_Sockets.end( ) )
		{
			return;
		}

		// Add the socket to the map
		m_Sockets[ p_Socket.GetHandle( ) ] = &p_Socket;

		// Add the socket to the set
		FD_SET( p_Socket.GetHandle( ), &m_SocketSet );
	}

}