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

#include <Bit/Network/Http.hpp>
#include <Bit/Network/TcpSocket.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

// Undefine the SetPort definition for win32 platform
#ifdef BIT_PLATFORM_WINDOWS
#undef SetPort
#endif

namespace Bit
{

	
	// Global variables
	static const std::string g_EmptyString = "";
	static const std::string g_MethodStrings[ Http::Connect + 1 ] =
	{
		"", "OPTIONS", "GET", "POST", "PUT", "DELETE", "TRACE", "CONNECT"
	};

	// Http packet base class
	void Http::HttpPacket::SetField( const std::string & p_Key, const std::string & p_Content )
	{
		// Check if the field already exists
		StringMap::iterator it = m_Fields.find( p_Key );

		// Add the field if it wasn't found
		if( it == m_Fields.end( ) )
		{
			m_Fields[ p_Key ] = p_Content;

			/// Terminate the function, we are done here.
			return;
		}

		// Set the already existing field
		it->second = p_Content;
	}

	const std::string & Http::HttpPacket::GetField( const std::string & p_Key ) const
	{
		// Find the field in the hash map.
		StringMap::const_iterator it = m_Fields.find( p_Key );

		// Return the field if it was found.
		if( it != m_Fields.end( ) )
		{
			return it->second;
		}

		// Return nothing if it wasn't found.
		return g_EmptyString;
	}

	// Request class
	Http::Request::Request(	const eMethod p_Method,
							const std::string & p_Path,
							const std::string & p_Protocol ) :
		m_Method( p_Method ),
		m_Path( p_Path ),
		m_Protocol( p_Protocol )
	{
	}

	void Http::Request::SetMethod( const eMethod p_Method )
	{
		m_Method = p_Method;
	}

	void Http::Request::SetPath( const std::string & p_Path )
	{
		m_Path = p_Path;
	}

	void Http::Request::SetProtocol( const std::string & p_Protocol )
	{
		m_Protocol = p_Protocol;
	}

	Http::eMethod Http::Request::GetMethod( ) const
	{
		return m_Method;
	}

	const std::string & Http::Request::GetPath( ) const
	{
		return m_Path;
	}

	const std::string & Http::Request::GetProtocol( ) const
	{
		return m_Protocol;
	}

	// Response class
	Http::Response::Response(	const std::string & p_Protocol,
								const eCode p_StatusCode,
								const std::string & p_Body ) :
		m_Protocol( p_Protocol ),
		m_StatusCode( p_StatusCode ),
		m_Body( p_Body )
	{
	}

	void Http::Response::SetProtocol( const std::string & p_Protocol )
	{
		m_Protocol = p_Protocol;
	}

	void Http::Response::SetStatusCode( const eCode p_StatusCode )
	{
		m_StatusCode = p_StatusCode;
	}

	void Http::Response::SetBody( const std::string & p_Body )
	{
		m_Body = p_Body;
	}

	const std::string & Http::Response::GetProtocol( ) const
	{
		return m_Protocol;
	}

	Http::eCode Http::Response::GetStatusCode( ) const
	{
		return m_StatusCode;
	}

	const std::string & Http::Response::GetBody( ) const
	{
		return m_Body;
	}

	// Http class
	Http::Http( ) :
		m_Port( 80 ),
		m_Timeout( 0 )
	{
	}

	void Http::SetPort( const Uint16 p_Port )
	{
		m_Port = p_Port;
	}

	void Http::SetTimeout( const Uint32 p_Timeout )
	{
		m_Timeout = p_Timeout;
	}

	Bool Http::SendRequest( const Request & p_Request, Response & p_Response, const Address & p_Address )
	{
		// Get the request string
		std::stringstream requestSs;
		CreateRequestString( p_Request, requestSs );
		const Uint32 requestSize = requestSs.str( ).size( );

		// Connect to the server
		Bit::TcpSocket tcp;
		tcp.SetBlocking( false );
		if( tcp.Connect( p_Address, m_Port, m_Timeout ) == false )
		{
			std::cout << "[Http::SendRequest] Could not connect to the server." << std::endl;
			return false;
		}

		// Send the request
		if( tcp.Send( reinterpret_cast<const void *>( requestSs.str( ).c_str( ) ), requestSize ) != requestSize )
		{
			std::cout << "[Http::SendRequest] Could not send the request." << std::endl;
			return false;
		}

		// Receive the response
		const SizeType bufferSize = 512;
		Uint8 pBuffer[ bufferSize ];

		if( m_Timeout > 0 )
		{
			if( tcp.Receive( pBuffer, bufferSize, m_Timeout ) != bufferSize )
			{
				std::cout << "[Http::SendRequest(Timeout enabled)] Could not receive the response." << std::endl;
				return false;
			}
		}
		else
		{
			if( tcp.Receive( pBuffer, bufferSize ) != bufferSize )
			{
				std::cout << "[Http::SendRequest(Timeout disabled)] Could not receive the response." << std::endl;
				return false;
			}
		}

		// Succeeded
		return true;
	}

	Bool Http::ParseResponsePacket( const std::string & p_Data, HttpPacket & p_Packet )
	{

		return false;
	}

	void Http::CreateRequestString( const Http::Request & p_Request, std::stringstream & p_StringStream )
	{
		// Clear the string stream
		if( p_StringStream.str( ).size( ) )
		{
			p_StringStream.clear( );
		}

		// Add the request line
		p_StringStream << g_MethodStrings[ static_cast<SizeType>( p_Request.GetMethod( ) ) ];
		p_StringStream << " ";
		p_StringStream << p_Request.GetPath( );
		p_StringStream << " ";
		p_StringStream << p_Request.GetProtocol( );
		p_StringStream << "\r\n";

		// Add the fields
		for(	HttpPacket::StringMap::const_iterator it = p_Request.m_Fields.begin( );
				it != p_Request.m_Fields.end( );
				it++ )
		{
			p_StringStream << it->first;
			p_StringStream << ": ";
			p_StringStream << it->second;
			p_StringStream << "\r\n";
		}

		// Add another newline
		p_StringStream << "\r\n";
	}

}