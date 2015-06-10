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

#include <Bit/Network/Win32/TcpSocketWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/System/Sleep.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static const Uint64 g_MaxTimeout = 2147483647;

	TcpSocketWin32::TcpSocketWin32( ) :
		TcpSocketBase( )
	{
	}

	TcpSocketWin32::~TcpSocketWin32( )
	{
		Disconnect( );
	}

	Bool TcpSocketWin32::Connect(const Address & p_Address, const Uint16 p_Port, const Time & p_Timeout, const Uint16 p_EndpointPort)
	{
		// Create the socket
		if ((m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			std::cout << "[TcpSocketLinux::Connect] Can not create the socket. Error: " << GetLastError( ) << std::endl;
			return false;
		}

		// Bind the socket to a port
		sockaddr_in service;

		if (p_EndpointPort != 0)
		{
			service.sin_family = AF_INET;
			service.sin_addr.s_addr = htonl(INADDR_ANY);
			service.sin_port = htons(static_cast<u_short>(p_EndpointPort));

			/*int reusable = 1;
			if (setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&reusable), sizeof(int)) != 0)
			{
				std::cout << "[TcpSocketWin32::Connect] Can not set reusable socket. Error: " << GetLastError() << std::endl;
			}*/


			const int optVal = 1;
			const int optLen = sizeof(optVal);
			int rtn = setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, (const char*)&optVal, optLen);
			if( rtn != 0 )
			{
				std::cout << "[TcpSocketWin32::Connect] Can not set reusable socket. Error: " << GetLastError() << std::endl;
				return false;
			}

			// Bind
			if (bind(m_Handle, reinterpret_cast<const sockaddr *>(&service), sizeof(service)) == SOCKET_ERROR)
			{
				std::cout << "[TcpSocketWin32::Connect] Can not bind the socket. Error: " << GetLastError() << std::endl;
				return false;
			}
		}
		
		// Create an object that's holding the host data
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl( static_cast<u_long>( p_Address.GetAddress( ) ) );
		service.sin_port = htons( static_cast<u_short>( p_Port ) );

		// We are using timeout
		// Get the blocking status and disable it.
		Bool blocking = GetBlocking( );
		SetBlocking( false );

		// Connect
		if( connect( m_Handle, ( const sockaddr * )&service, sizeof (sockaddr_in ) ) != 0 )
		{
		   // Ignore the WSAEWOULDBLOCK error
			DWORD lastError = GetLastError( );
			if( lastError != WSAEWOULDBLOCK )
			{
				Disconnect( );
				return false;
			}
		}

		// We failed to connect, but we are waiting for the connection to establish
		struct timeval tv;
		if( p_Timeout.AsMicroseconds( ) / 1000000ULL > g_MaxTimeout )
		{
			tv.tv_sec	= static_cast<long>( g_MaxTimeout );
			tv.tv_usec	= static_cast<long>( 0 );
		}
		else
		{
			tv.tv_sec	= static_cast<long>( p_Timeout.AsMicroseconds( ) / 1000000ULL );
			tv.tv_usec
			= static_cast<long>( p_Timeout.AsMicroseconds( ) % 1000000ULL );
		}

		// Create a FD_SET, and add the m_Handle to the set
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(m_Handle, &fdset);

		// Select from the set
		if (select(static_cast<int>(m_Handle)+1, NULL, &fdset, NULL, &tv) > 0)
		{
			// Check if the address is valid.
			Address address = GetPeerAddress( );
			if( address == p_Address )
			{
				// The address is not 0, we successfully connected.
				SetBlocking( blocking );
				return true;
			}
		}

		DWORD lastError = GetLastError();

		// Failed to connect. Close the socket.
		Disconnect( );

		// Failed.
		return false;
	}

	void TcpSocketWin32::Disconnect( )
	{
		//shutdown(m_Handle, 2);

		// Close the socket handle.
		CloseHandle( );
	}

	Int32 TcpSocketWin32::Receive( void * p_pData, const SizeType p_Size )
	{
		return recv( m_Handle, reinterpret_cast<char*>( p_pData ), static_cast<int>( p_Size ), 0 );
	}

	Int32 TcpSocketWin32::Receive( void * p_pData, const SizeType p_Size, const Time & p_Timeout )
	{
		// Create a socket address storage
		sockaddr_in address;
		int addressSize = sizeof( address );

		// Set blocking status
		Bool blocking = GetBlocking( );
		if( blocking )
		{
			SetBlocking( false );
		}

		// Put the socket handle in a fdset
		fd_set fdset;
		FD_ZERO( &fdset );
		FD_SET( m_Handle, &fdset );

		// Set the time
		struct timeval tv;
		if( p_Timeout.AsMicroseconds( ) / 1000000ULL > g_MaxTimeout )
		{
			tv.tv_sec	= static_cast<long>( g_MaxTimeout );
			tv.tv_usec	= static_cast<long>( 0 );
		}
		else
		{
			tv.tv_sec	= static_cast<long>( p_Timeout.AsMicroseconds( ) / 1000000ULL );
			tv.tv_usec	= static_cast<long>( p_Timeout.AsMicroseconds( ) % 1000000ULL );
		}

		// Select from the fdset
		int status = 0;
		if( ( status = select( static_cast<int>( m_Handle ) + 1, &fdset, NULL, NULL, &tv ) ) > 0 )
		{
			// Receive the message
			int size = recv( m_Handle, reinterpret_cast<char*>( p_pData ), static_cast<int>( p_Size ), 0 );

			// Restore the block status
			SetBlocking( blocking );

			// return the received message's size
			return size;
		}

		// Reset the blocking status and return false
		SetBlocking( blocking );
		return status;
	}

	void TcpSocketWin32::Receive( Packet & p_Packet )
	{
	}

	void TcpSocketWin32::Receive( Packet & p_Packet, const Time & p_Timeout )
	{
	}

	Int32 TcpSocketWin32::Send( const void * p_pData, const SizeType p_Size )
	{
		int size = send( m_Handle, reinterpret_cast<const char*>( p_pData ), static_cast<int>( p_Size ), 0 );
		return static_cast<Int32>( size );
	}

	void TcpSocketWin32::Send( const Packet & p_Packet )
	{

	}

}

#endif
