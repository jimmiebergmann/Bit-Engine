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

#include <Bit/Network/TcpSocket.hpp>
#include <Bit/System/Sleep.hpp>
#include <iostream>

namespace Bit
{

	TcpSocket::TcpSocket( ) :
		Socket( )
	{
	}

	TcpSocket::~TcpSocket( )
	{
		Close( );
	}

	bool TcpSocket::Connect( const Address & p_Address, const Uint16 p_Port, const Uint32 p_Timeout )
	{
		// Create the socket
		if( ( m_Handle = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) <= 0 )
		{
			std::cout << "[TcpSocket::Connect] Can not create the socket. Error: " << GetLastError( ) << std::endl;
			return false;
		}

		// Create an object that's holding the host data
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl( static_cast<u_long>( p_Address.GetAddress( ) ) );
		service.sin_port = htons( static_cast<u_short>( p_Port ) );

		// We are not using any timeout at all.
		if( p_Timeout == 0 )
		{
			if( connect( m_Handle, ( const sockaddr * )&service, sizeof (sockaddr_in ) ) != SOCKET_ERROR )
			{
				return true;
			}
			else
			{
				std::cout << "[TcpSocket::Connect] Can not connect to the server. Error: " << GetLastError( ) << std::endl;
				Close( );
				return false;
			}
		}

		// We are using timeout
		// Get the blocking status.
		Bool blocking = GetBlocking( );

		// Disable socket blocking
		if( blocking )
		{
			SetBlocking( false );
		}

		// Create a FD_SET, and add the m_Handle to the set
		FD_SET fdset;
		FD_ZERO( &fdset );
		FD_SET( m_Handle, &fdset );

		// Connect
		if( connect( m_Handle, ( const sockaddr * )&service, sizeof (sockaddr_in ) ) != SOCKET_ERROR )
		{
			SetBlocking( blocking );
			return true;
		}
		else // The attempt to connect failed.
		{
			// Ignore the WSAEWOULDBLOCK error
			DWORD lastError = GetLastError( );
			if( lastError != WSAEWOULDBLOCK )
			{
				std::cout << "[TcpSocket::Connect] Can not connect to the server. Error: " << GetLastError( ) << std::endl;
				Close( );
				return false;
			}
		}
			
		// We failed to connect, but we are waiting for the connection to establish
		
		struct timeval tv;
		tv.tv_sec = static_cast<long>( p_Timeout ) / 1000;
		tv.tv_usec = 0;

		// Select from the set
		if( select( static_cast<int>( m_Handle ) + 1, NULL, &fdset, NULL, &tv ) > 0 )
		{
			// Check if the address is valid.
			Address address = GetPeerAddress( );
			if( address.GetAddress( ) != 0 )
			{
				SetBlocking( blocking );
				return true;
			}
		}
		else
		{
			std::cout << "[TcpSocket::Connect] Can not connect to the server. Error: " << GetLastError( ) << std::endl;
		}

		Close( );
		SetBlocking( blocking );
		return false;
	}

	void TcpSocket::Disconnect( )
	{
		// Close the socket.
		Close( );
	}

	Int32 TcpSocket::Receive( void * p_pData, const SizeType p_Size )
	{
		return recv( m_Handle, reinterpret_cast<char*>( p_pData ), static_cast<int>( p_Size ), 0 );
	}

	Int32 TcpSocket::Receive( void * p_pData, const SizeType p_Size, const Uint32 p_Timeout )
	{
		return recv( m_Handle, reinterpret_cast<char*>( p_pData ), static_cast<int>( p_Size ), 0 );
	}

	void TcpSocket::Receive( Packet & p_Packet )
	{
	}

	void TcpSocket::Receive( Packet & p_Packet, const Uint32 p_Timeout )
	{
	}

	Int32 TcpSocket::Send( const void * p_pData, const SizeType p_Size )
	{
		int size = send( m_Handle, reinterpret_cast<const char*>( p_pData ), static_cast<int>( p_Size ), 0 );
		return static_cast<Int32>( size );
	}

	void TcpSocket::Send( const Packet & p_Packet )
	{
		
	}

}