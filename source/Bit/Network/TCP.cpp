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


#include <Bit/Network/TCP.hpp>
#include <Bit/System.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	namespace Net
	{


		// Constructors/destructors
		TCP::TCP( ) :
			m_Type( Bit::Net::None ),
			m_Established( BIT_FALSE ),
			m_Socket( 0 )
		{
		}

		TCP::~TCP( )
		{
			Disconnect( );
		}
			
		// Public general functions
		BIT_UINT32 TCP::Connect( const Address & p_Address )
		{
			if( m_Established )
			{
				bitTrace( "[Bit::Net::TCP::Connect()] The connection is already established.\n" );
				return BIT_ERROR;
			}

			// Create the socket
			if( ( m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) <= 0 )
			{
				bitTrace( "[Bit::Net::TCP::Connect()] Can not create the socket.\n" );
				return BIT_ERROR;
			}

			// Create an object that's holding the host data
			sockaddr_in service;
			service.sin_family = AF_INET;
			service.sin_addr.s_addr = htonl( p_Address.GetAddress( ) );
			service.sin_port = htons( p_Address.GetPort( ) );

			// Connect
			if( connect( m_Socket, (const sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				bitTrace( "[Bit::Net::TCP::Connect()] "
					"Can not connect to the server: %i.\n", GetLastError() );

				CloseSocket( m_Socket );
				return BIT_ERROR;
			}

			m_Established = BIT_TRUE;
			m_Type = Bit::Net::Client;
			return BIT_OK;
		}

		BIT_UINT32 TCP::Disconnect( )
		{
			if( m_Established )
			{
				CloseSocket( m_Socket );
				m_Socket = 0;
				m_Established = BIT_FALSE;
				return BIT_OK;
			}


			return BIT_ERROR;
		}

		BIT_UINT32 TCP::Host( const BIT_UINT16 p_Port )
		{
			if( m_Established )
			{
				bitTrace( "[Bit::Net::TCP::Host()] Already established.\n" );
				return BIT_ERROR;
			}

			// Create the socket
			if( ( m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) <= 0 )
			{
				bitTrace( "[Bit::Net::TCP::Host()] Can not create the socket.\n" );
				return BIT_ERROR;
			}

			// create an object that's holding the host data
			sockaddr_in service;
			service.sin_family = AF_INET;
			service.sin_addr.s_addr = htonl(INADDR_ANY);
			service.sin_port = htons( p_Port);

			// Bind
			if( bind( m_Socket, (const sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR )
			{
				bitTrace( BIT_NULL, "[Bit::Net::TCP::Host()] Can not bind the socket.\n" );
				CloseSocket( m_Socket );
				return BIT_ERROR;
			}

			// Listen for incomming clients
			if( listen( m_Socket, SOMAXCONN ) == SOCKET_ERROR )
			{
				bitTrace( "[Bit::Net::TCP::Host()] Can not listen for clients.\n" );
				return BIT_ERROR;
			}

			// Accept the client
			Socket AcceptSocket = 0;
			if( ( AcceptSocket = accept( m_Socket, NULL, NULL ) ) == SOCKET_ERROR )
			{
				bitTrace( "[Bit::Net::TCP::Host()] Can not accept the client.\n" );
				return BIT_ERROR;
			}
			m_Socket = AcceptSocket;

			m_Established = BIT_TRUE;
			m_Type = Bit::Net::Server;
			return BIT_OK;
		}

		BIT_BOOL TCP::IsEstablished( ) const
		{
			return m_Established;
		}

		BIT_SINT32 TCP::Receive( BIT_BYTE * p_pBuffer, const BIT_UINT32 p_Size )
		{
			if( !m_Established )
			{
				return 0;
			}

			return recv( m_Socket, reinterpret_cast< char* >( p_pBuffer ), p_Size, 0 );
		}

		BIT_SINT32 TCP::Send( const BIT_BYTE * p_pBuffer, const BIT_UINT32 p_Size )
		{
			if( !m_Established )
			{
				return 0;
			}

			return send( m_Socket, reinterpret_cast< char* >( const_cast< BIT_BYTE* >( p_pBuffer ) ), p_Size, 0 );
		}

	}
}