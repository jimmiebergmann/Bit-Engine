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

#include <Bit/Network/Linux/TcpListenerLinux.hpp>
#ifdef BIT_PLATFORM_LINUX
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	TcpListenerLinux::TcpListenerLinux( ) :
		m_Running( false ),
		m_Socket( )
	{
	}

	TcpListenerLinux::TcpListenerLinux( const Uint16 p_Port ) :
		m_Running( false ),
		m_Socket( )
	{
		// Start the listener
		Start( p_Port );
	}

	TcpListenerLinux::~TcpListenerLinux( )
	{
		Stop( );
	}

	Bool TcpListenerLinux::Start( const Uint16 p_Port )
	{
		// Create the socket
		SocketHandle socketHandle;
		if( ( socketHandle = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) <= 0 )
		{
			std::cout << "[TcpListenerLinux::Start] Can not create the socket. Error: " << errno << std::endl;
			return false;
		}
		m_Socket.SetHandle( socketHandle );

		// Create an object that's holding the host data
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl( INADDR_ANY );
		service.sin_port = htons( static_cast<u_short>( p_Port ) );

		// Bind
		if( bind( m_Socket.GetHandle( ), reinterpret_cast<const sockaddr *>( &service ), sizeof( service ) ) != 0 )
		{
			std::cout << "[TcpListenerLinux::Start] Can not bind the socket. Error: " << errno << std::endl;
			Stop( );
			return false;
		}

		// Succeeded
		m_Running = true;
		return true;
	}

	void TcpListenerLinux::Stop( )
	{
		// Close the socket handle
		m_Socket.CloseHandle( );

		m_Running = false;
	}

	Bool TcpListenerLinux::Listen( TcpSocket & p_TcpSocket )
	{
		// Listen for incomming clients
		if( listen( m_Socket.GetHandle( ), SOMAXCONN ) != 0 )
		{
			std::cout << "[TcpListenerLinux::Listen] Can not listen for clients. Error: " << errno << std::endl;
			return false;
		}

		// Accept the client
		SocketHandle acceptSocket = 0;
		if( ( acceptSocket = accept( m_Socket.GetHandle( ), NULL, NULL ) ) == -1 )
		{
			std::cout << "[TcpListenerLinux::Listen] Can not accept the client. Error: " << errno << std::endl;
			return false;
		}

		// Set the handle for the TcpSocket.
		p_TcpSocket.SetHandle( acceptSocket );

		// Succeeded
		return true;
	}

	Bool TcpListenerLinux::IsRunning( ) const
	{
		return m_Running;
	}

}

#endif
