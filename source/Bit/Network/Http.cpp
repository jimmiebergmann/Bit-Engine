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
	static const SizeType g_ResponseBufferSize = 32768;
	static Uint8 g_ResponseBuffer[ g_ResponseBufferSize ];

	// Global functions
	static std::string GetWord( const Uint8 * p_pData, const SizeType p_DataSize )
	{
		std::string word;

		for( SizeType i = 0; i < p_DataSize; i++ )
		{
			if( p_pData[ i ] == ' ' || p_pData[ i ] == '\r' )
			{
				word.append( reinterpret_cast<const char *>( p_pData ), i );
				break;
			}
		}

		return word;
	}

	static std::string GetLine( const Uint8 * p_pData, const SizeType p_DataSize )
	{
		std::string line;

		for( SizeType i = 0; i < p_DataSize - 1; i++ )
		{
			if( p_pData[ i ] == '\r' && p_pData[ i + 1 ] == '\n' )
			{
				line.append( reinterpret_cast<const char *>( p_pData ), i );
				break;
			}
		}

		return line;
	}

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
	Http::Http( const Uint16 p_Port,
				const Time & p_Timeout ) :
		m_Port( p_Port ),
		m_Timeout( p_Timeout )
	{
	}

	void Http::SetPort( const Uint16 p_Port )
	{
		m_Port = p_Port;
	}

	void Http::SetTimeout( const Time & p_Timeout )
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

		// Create data buffers for the incoming data
		std::string headString;
		p_Response.m_Body.clear( );
		Int32 receiveSize = 0;
		SizeType bodyStartIndex = 0; // Index of where the body data starts.

		// Receive the header data( plus some body data sometimes perhaps).
		receiveSize = tcp.Receive( g_ResponseBuffer, g_ResponseBufferSize );
			
		// Parse the header fields,
		// returns the index of where the body data start.
		bodyStartIndex = ParseResponse( g_ResponseBuffer, receiveSize, p_Response );
	
		// Append the remaining data from the header buffer to the body string
		SizeType bodySize = receiveSize - bodyStartIndex;
		p_Response.m_Body.append( reinterpret_cast<char*>( g_ResponseBuffer ) + bodyStartIndex, bodySize );

		// The download is done in one single packet, clean up and return.
		if( receiveSize != g_ResponseBufferSize )
		{
			// Delete the buffer
			return true;
		}

		// Receive the body data.
		const std::string sizeString = p_Response.GetField( "Content-Length" );
		const SizeType contentSize = sizeString.size( ) ? atoi( sizeString.c_str( ) ) : 0;

		while( true )
		{
			// Finish the download if we've download the entire file.
			if( contentSize && p_Response.m_Body.size( ) == contentSize )
			{
				return true;
			}

			// Receive the packet.
			receiveSize = tcp.Receive( g_ResponseBuffer, g_ResponseBufferSize );
			
			// Break if the packet is invalid.( or lost connection )
			if( receiveSize <= 0 )
			{
				return false;
			}

			// Append the body data.
			p_Response.m_Body.append( reinterpret_cast<char*>( g_ResponseBuffer ), receiveSize );
		}

		// Succeeded
		return true;
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
	
	SizeType Http::ParseResponse( const Uint8 * p_pData, const SizeType p_DataSize, Response & p_Response )
	{
		// Error check the paramters
		if( p_pData == NULL || p_DataSize <= 0 )
		{
			return 0;
		}

		SizeType index = 0;

		// Parse the protocol
		std::string protocol = GetWord( p_pData, p_DataSize );

		// Error check the protocol
		if( protocol.size( ) != 8 || protocol[ 0 ] != 'H' || protocol[ 1 ] != 'T'
			|| protocol[ 2 ] != 'T' || protocol[ 3 ] != 'P' )
		{
			return 0;
		}

		// Get the status code
		index += protocol.size( ) + 1;
		std::string version = GetWord( p_pData + index, p_DataSize );

		// Error check the version
		if( version.size( ) != 3 )
		{
			return 0;
		}

		// Set the version
		p_Response.m_StatusCode = static_cast<eCode>( atoi( version.c_str( ) ) );

		// Get the rest of the line...
		index += GetLine( p_pData + index, p_DataSize ).size( ) + 2;

		// Let's parse all the field until we reach an empty word( empty line right before the body data)
		while( true )
		{
			// Get the key
			std::string key = GetWord( p_pData + index, p_DataSize );
			if( key.size( ) == 0 || key[ key.size( ) -1 ] != ':' )
			{
				index += 2;
				break;
			}

			// Get the feield data
			index += key.size( ) + 1;
			std::string content = GetLine( p_pData + index, p_DataSize );
			if( content.size( ) == 0 )
			{
				return 0;
			}

			// set the field
			key.resize( key.size( ) - 1 );
			p_Response.SetField( key, content );

			// Move to the next line
			index += content.size( ) + 2;
		}

		return index;
	}


}