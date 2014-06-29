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

#include <Bit/Network/Linux/SocketLinux.hpp>
#ifdef BIT_PLATFORM_LINUX
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	SocketLinux::SocketLinux( ) :
		SocketBase( )
	{
	}

	SocketLinux::SocketLinux( const SocketHandle & p_SocketHandle ) :
		SocketBase( p_SocketHandle )
	{
	}

	SocketLinux::~SocketLinux( )
	{
		CloseHandle( );
	}

	void SocketLinux::SetBlocking( const Bool p_Blocking )
	{
		// The blocking status is already set.
		if( m_Blocking == p_Blocking )
		{
			return;
		}

		if( m_Handle )
		{
			// Get the current  file descriptor.
			int opts = fcntl( m_Handle, F_GETFL );
            if (opts < 0)
            {
                std::cout << "[SocketLinux::SetBlocking] Failed to get file descriptor. Error: " << opts << std::endl;
                return;
            }

            // Toggle the non blocking flag.
            opts ^= O_NONBLOCK;

            // Try to set the new file descriptor.
            int result = 0;
            if( ( result = fcntl( m_Handle, F_SETFL, opts ) ) < 0 )
            {
                std::cout << "[SocketLinux::SetBlocking] Failed to set blocking. Error: " << result << std::endl;
                return;
            }

            // Set the current blocking.
			m_Blocking = p_Blocking;
		}
	}

	Bool SocketLinux::GetBlocking( ) const
	{
		return m_Blocking;
	}

	void SocketLinux::CloseHandle( )
	{
        if( m_Handle )
		{
			close( m_Handle );
		}
	}

	void SocketLinux::SetHandle( const SocketHandle & m_SocketHandle )
	{
		m_Handle = m_SocketHandle;
	}

	const SocketHandle SocketLinux::GetHandle( ) const
	{
		return m_Handle;
	}

	Address SocketLinux::GetPeerAddress( ) const
	{
		// Get the address
		sockaddr_in address;
		socklen_t size = sizeof( address );
		getpeername( m_Handle, reinterpret_cast<sockaddr *>( &address ), &size );

		// Return the address
		return Address( static_cast<Uint32>( ntohl( (u_long)address.sin_addr.s_addr ) ) );
	}

}

#endif
