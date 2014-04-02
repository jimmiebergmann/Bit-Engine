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

#include <Bit/Network/Url.hpp>
#include <algorithm>
#include <iterator>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// Global functions
	static Bool IsValidUrlEncodingByte( const Uint8 p_Value )
	{
		if( p_Value >= '0' && p_Value <= '9' ||
			p_Value >= 'a' && p_Value <= 'z' ||
			p_Value >= 'A' && p_Value <= 'Z' ||
			p_Value == '.' || p_Value == '-' ||
			p_Value == '_' || p_Value == '~' )
		{
			return true;
		}

		return false;
	}

	static std::string GetHexValue( const Uint8 p_Value, const bool p_Uppercase )
	{
		// Array of the 16 base values in upper and lowercase.
		static Uint8 hexLow[ 16 ] = 
		{
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f'
		};

		static Uint8 hexHigh[ 16 ] = 
		{
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F'
		};

		// Create an output string
		std::string output;
		if( p_Uppercase )
		{
			output += hexHigh[ p_Value / 16 ];
			output += hexHigh[ p_Value % 16 ];
		}
		else
		{
			output += hexLow[ p_Value / 16 ];
			output += hexLow[ p_Value % 16 ];
		}

		// Return the output string
		return output;
	}

	// Static members
	std::string Url::GetBinaryEncodedString( const std::string & p_String )
	{
		// Create an output string
		std::string output;
		output.reserve( p_String.size( ) );

		// Fill the output string
		for( SizeType i = 0; i < p_String.size( ); i++ )
		{
			// Add the byte if it's a valid Url encoding char
			if( IsValidUrlEncodingByte( p_String[ i ] ) )
			{
				output.push_back( p_String[ i ] );
			}
			else
			{
				output.push_back( '%' );
				output.append( GetHexValue( p_String[ i ], false ) );
			}
		}

		// Return the output string
		return output;
	}

	Url::Url( )
	{
	}

	Url::Url( const std::string & p_UrlString )
	{
		Parse( p_UrlString );
	}

	void Url::Clear( )
	{
		m_Protocol.clear( );
		m_Username.clear( );
		m_Password.clear( );
		m_Domain.clear( );
		m_Port.clear( );
		m_Path.clear( );
		m_Query.clear( );
		m_Fragment.clear( );
	}

	Bool Url::Set( const std::string & p_UrlString )
	{
		//  Return the validation status
		return Parse( p_UrlString );
	}

	std::string Url::Get( ) const
	{
		std::string output = "";

		// Add the protocol portion
		if( m_Protocol.size( ) )
		{
			output += m_Protocol + "://";
		}

		// Add the username portion
		if( m_Username.size( ) )
		{
			output += m_Username;

			if( m_Password.size( ) )
			{
				output += ":";
			}
		}

		// Add the password portion
		if( m_Password.size( ) )
		{
			output += m_Password;
			if( m_Domain.size( ) )
			{
				output += "@";
			}
		}

		// Add the domain portion
		output += m_Domain;

		// Add the port portion
		if( m_Port.size( ) )
		{
			output += ":" + m_Port;
		}

		// Add the path portion
		if( m_Path.size( ) )
		{
			output += "/" + m_Path;
		}

		// Add the query portion
		if( m_Query.size( ) )
		{
			output += "?" + m_Query;
		}

		// Add the fragment portion
		if( m_Fragment.size( ) )
		{
			output += "#" + m_Fragment;
		}

		return output;
	}

	std::string Url::GetProtocol( ) const
	{
		return m_Protocol;
	}

	std::string Url::GetUsername( ) const
	{
		return m_Username;
	}

	std::string Url::GetPassword( ) const
	{
		return m_Password;
	}

	std::string Url::GetDomain( ) const
	{
		return m_Domain;
	}

	Uint16 Url::GetPort( ) const
	{
		if( m_Port.size( ) == 0 )
		{
			return 0;
		}

		return static_cast<Uint16>( atoi( m_Port.c_str( ) ) );
	}

	std::string Url::GetPath( ) const
	{
		return m_Path;
	}

	std::string Url::GetQuery( ) const
	{
		return m_Query;
	}

	std::string Url::GetFragment( ) const
	{
		return m_Fragment;
	}

	Bool Url::Parse( const std::string & p_UrlString )
	{
		// Clear the Url
		Clear( );

		// Find the protocol
		const std::string prot_signs = "://";
		std::string::const_iterator prot_it = std::search(	p_UrlString.begin( ), p_UrlString.end( ),
															prot_signs.begin( ), prot_signs.end( ) );

		if( prot_it == p_UrlString.end( ) )
		{
			prot_it = p_UrlString.begin( );
		}
		else // we found the protocol
		{
			std::copy( p_UrlString.begin( ), prot_it, std::back_inserter( m_Protocol ) );
			std::advance( prot_it, 3 );
		}

		// Find the end of the domain
		const std::string pqf = "/?#";
		std::string::const_iterator end_domain_it = std::find_first_of( prot_it, p_UrlString.end( ),
																		pqf.begin( ), pqf.end( ) );
		
		// We could not find the end of the domain( of yeah we did, the last sign )
		if( end_domain_it == p_UrlString.end( ) )
		{
			std::copy( prot_it, p_UrlString.end( ), std::back_inserter( m_Domain ) );
		}
		else
		{
			// Set the domain
			std::copy( prot_it, end_domain_it, std::back_inserter( m_Domain ) );

			// set the path
			std::copy( ++end_domain_it, p_UrlString.end( ), std::back_inserter( m_Path ) );

			// Parse the domain and path
			ParseDomain( );
			ParsePath( );
		}

		return true;
	}

	void Url::ParseDomain( )
	{
		// Find the @ sign in order to determine if there's any username/password
		std::string::iterator up_it = std::find( m_Domain.begin( ), m_Domain.end( ), '@' );
		
		// We found a username/password
		if( up_it != m_Domain.end( ) )
		{
			std::copy( m_Domain.begin( ), up_it, std::back_inserter( m_Username ) );
			m_Domain.erase( m_Domain.begin( ), up_it + 1 );

			// Parse the username
			ParseUsername( );
		}

		// Find the port
		std::string::iterator port_it = std::find( m_Domain.begin( ), m_Domain.end( ), ':' );
		if( port_it != m_Domain.end( ) )
		{
			std::copy( port_it + 1, m_Domain.end( ), std::back_inserter( m_Port ) );
			m_Domain.erase( port_it, m_Domain.end( ) );
		}
	}

	void Url::ParseUsername( )
	{
		std::string::iterator p_it = std::find( m_Username.begin( ), m_Username.end( ), ':' );
		if( p_it != m_Username.end( ) )
		{
			std::copy( p_it + 1, m_Username.end( ), std::back_inserter( m_Password ) );
			m_Username.erase( p_it, m_Username.end( ) );
		}
	}

	void Url::ParsePath( )
	{
		// Find the fragment
		std::string::iterator frag_it = std::find( m_Path.begin( ), m_Path.end( ), '#' );

		// Could we find the fragment id?
		if( frag_it != m_Path.end( ) )
		{
			std::copy( frag_it + 1, m_Path.end( ), std::back_inserter( m_Fragment ) );
			m_Path.erase( frag_it, m_Path.end( ) );
		}

		// Find the query
		std::string::iterator query_it = std::find( m_Path.begin( ), m_Path.end( ), '?' );
		if( query_it != m_Path.end( ) )
		{
			std::copy( query_it + 1, m_Path.end( ), std::back_inserter( m_Query ) );
			m_Path.erase( query_it, m_Path.end( ) );
		}
	}

}