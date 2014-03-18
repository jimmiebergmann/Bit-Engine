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
		m_Handle( 0 )
	{
	}

	void SocketWin32::SetBlocking( bool p_Blocking )
	{
		if( m_Handle )
		{
			// If blocking = 0, blocking is enabled.
			// If blocking != 0, non-blocking mode is enabled.
			u_long blocking = p_Blocking ? 0 : 1;

			int result = ioctlsocket( m_Handle, FIONBIO, &blocking);
			if ( result != NO_ERROR )
			{
				std::cout << "[SocketWin32::SetBlocking] Failed to set blocking. Error: " << result << std::endl;
			}
		}
	}

	void SocketWin32::Close( )
	{
		if( m_Handle )
		{
			closesocket( m_Handle );
			m_Handle = 0;
		}
	}

	SocketHandle SocketWin32::GetHandle( ) const
	{
		return m_Handle;
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