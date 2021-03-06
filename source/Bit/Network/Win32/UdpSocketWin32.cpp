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

#include <Bit/Network/Win32/UdpSocketWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static const Uint64 g_MaxTimeout = 2147483647;

	UdpSocketWin32::UdpSocketWin32( ) :
		UdpSocketBase( )
	{
	}

	UdpSocketWin32::UdpSocketWin32( const Uint16 p_Port ) :
		UdpSocketBase( )
	{
		Open( p_Port );
	}

	UdpSocketWin32::~UdpSocketWin32( )
	{
	}

	Bool UdpSocketWin32::Open( const Uint16 p_Port )
	{
		// Create the socket
		if( ( m_Handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) <= 0 )
		{
			bitLogNetErr( "Can not create the socket. Error: " << static_cast<Int32>(GetLastError()));
			return false;
		}

		// Bind the socket
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl( INADDR_ANY );
		service.sin_port = htons( static_cast<u_short>( p_Port ) );

		if( bind( m_Handle, reinterpret_cast<const sockaddr *>( &service ), sizeof( service ) ) != 0 )
		{
			bitLogNetErr(  "Can not bind the socket. Error: " << static_cast<Int32>(GetLastError( )) );
			Close( );
			return false;
		}

		return true;
	}

	void UdpSocketWin32::Close( )
	{
		// Close the socket handle
		CloseHandle( );
	}

	Int32 UdpSocketWin32::Send( const void * p_pData, const SizeType p_Size, const Address & p_Address, const Uint16 p_Port )
	{
		// Destination socket address struct
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl( static_cast<u_long>( p_Address.GetAddress( ) ) );
		address.sin_port = htons( static_cast<u_short>( p_Port ) );

		// Send the message
		int size = sendto(	m_Handle,
							reinterpret_cast<const char *>( p_pData ),
							static_cast<int>( p_Size ),
							0,
							reinterpret_cast<const sockaddr *>( &address ),
							sizeof( address ) );

		// Return the meessage size
		return static_cast<Int32>( size );
	}

	Int32 UdpSocketWin32::Receive( void * p_pData, const SizeType p_Size, Address & p_Address, Uint16 & p_Port )
	{
		// Note from the interwebz:
		/*
			You're supposed to use a struct sockaddr_in for an IPv4 address or
			a struct sockaddr_in6 for an IPv6 address, or better yet,
			a struct sockaddr_storage to cope with either.
		*/

		// Create a socket address storage
		sockaddr_in address;
		int addressSize = sizeof( address );

		// Receive the message
		int size = recvfrom( m_Handle,
							reinterpret_cast<char *>( p_pData ),
							static_cast<size_t>( p_Size ),
							0,
							reinterpret_cast< sockaddr * >( &address ),
							&addressSize );

		// Set the address and port
		p_Address = Address( static_cast<Uint32>( ntohl( (u_long)address.sin_addr.s_addr ) ) );
		p_Port = static_cast<Uint16>( ntohs( address.sin_port ) );

		// Return the meessage size
		return size;
	}

	Int32 UdpSocketWin32::Receive( void * p_pData, const SizeType p_Size, Address & p_Address, Uint16 & p_Port, const Time & p_Timeout )
	{
		// Create a socket address storage
		sockaddr_in address;
		int addressSize = sizeof( address );

		// Set blocking status
		Bool blocking = GetBlocking( );
		SetBlocking( false );


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
			int size = recvfrom(	m_Handle,
									reinterpret_cast<char *>( p_pData ),
									static_cast<int>( p_Size ),
									0,
									reinterpret_cast< sockaddr * >( &address ),
									&addressSize );

			// Reset the block status
			SetBlocking( blocking );

			// Set the address and port
			p_Address = Address( static_cast<Uint32>( ntohl( (u_long)address.sin_addr.s_addr ) ) );
			p_Port = static_cast<Uint16>( ntohs( address.sin_port ) );
			return size;
		}

		// Reset the blocking status and return false
		SetBlocking( blocking );
		return -1;
	}

}

#endif
