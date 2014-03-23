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

#include <Bit/Network/Win32/SocketWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <iostream>

namespace Bit
{

	SocketWin32::SocketWin32( ) :
		m_Handle( 0 ),
		m_Blocking( true )
	{
	}

	SocketWin32::SocketWin32( const SocketHandle & m_SocketHandle ) :
		m_Blocking( true )
	{
		m_Handle = m_SocketHandle;
	}

	SocketWin32::~SocketWin32( )
	{
		if( m_Handle )
		{
			closesocket( m_Handle );
		}
	}

	void SocketWin32::SetBlocking( Bool p_Blocking )
	{
		// The blocking status is already set.
		if( m_Blocking == p_Blocking )
		{
			return;
		}

		if( m_Handle )
		{
			// If blocking = 0, blocking is enabled.
			// If blocking != 0, non-blocking mode is enabled.
			u_long blocking = p_Blocking ? 0 : 1;

			int result = ioctlsocket( m_Handle, FIONBIO, &blocking);
			if ( result != NO_ERROR )
			{
				std::cout << "[SocketWin32::SetBlocking] Failed to set blocking. Error: " << result << std::endl;
				return;
			}

			m_Blocking = !static_cast<Bool>( blocking );
		}
	}

	Bool SocketWin32::GetBlocking( ) const
	{
		return m_Blocking;
	}

	Address SocketWin32::GetHostByName( const std::string & p_Hostname )
	{
		struct hostent *he;
		struct in_addr **addr_list;
		if( ( he = gethostbyname( p_Hostname.c_str() ) ) == NULL )
		{
			return Address::NoAddress;
		}

		// Get the hostname in string form
		char * hostname =  inet_ntoa (*( (struct in_addr *) he->h_addr_list[0] ) );

		// Convert it to a string.
		Uint32 address = inet_addr( hostname );
		if( address == INADDR_NONE )
		{
			return Address::NoAddress;
		}

		// Convert the address to a network side number
		return Address( htonl( address ) );
	}

	void SocketWin32::Close( )
	{
		if( m_Handle )
		{
			closesocket( m_Handle );
			m_Handle = 0;
		}
	}

	void SocketWin32::SetHandle( const SocketHandle & m_SocketHandle )
	{
		m_Handle = m_SocketHandle;
	}

	SocketHandle SocketWin32::GetHandle( ) const
	{
		return m_Handle;
	}

	Address SocketWin32::GetPeerAddress( ) const
	{
		// Get the address
		sockaddr_in address;
		int size = sizeof( address );
		getpeername( m_Handle, reinterpret_cast<sockaddr *>( &address ), &size );

		// Return the address
		return Address( static_cast<Uint32>( ntohl( (u_long)address.sin_addr.S_un.S_addr ) ) );
	}

	// Intiialize winsock( A little hack from sfml )
	struct WinsockInitializer
	{
		WinsockInitializer( )
		{
			WSADATA wsaData;
			if( WSAStartup( MAKEWORD(2,2), &wsaData ) )
			{
				std::cout << "[Bit::WinsockInitializer] Failed to initialize winsock." << std::endl;
			}
		}
	};

	// Create an instance of the intiialize
	static WinsockInitializer g_WinsockInitializer; 

}

#endif