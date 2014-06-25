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

#include <Bit/System/Hash.hpp>
#include <Bit/Network/Url.hpp>
#include <stdexcept>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Global functions
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

	static Bool GetValueFromHex( const Uint8 p_A, const Uint8 p_B, Uint8 & p_Output )
	{
		const Uint8 a = tolower( static_cast<int>( p_A ) );
		const Uint8 b = tolower( static_cast<int>( p_B ) );

		if( a < '0' || a > '9' &&
			a < 'a' || a > 'f' )
		{
			return false;
		}

		if( b < '0' || b > '9' &&
			b < 'a' || b > 'f' )
		{
			return false;
		}

		std::string string;
		string.push_back( a );
		string.push_back( b );
		try
		{
			p_Output = static_cast<Uint8>( std::stoul( string, nullptr, 16 ) );
		}
		catch( const std::invalid_argument e )
		{
			return false;
		}

		return true;
	}

	// Hash class
	Hash::Hash( )
	{
	}

	Hash::Hash( const std::string & p_String )
	{
		Set( p_String );
	}

	Hash::Hash( const Uint8 * p_pBytes, const SizeType p_Count )
	{
		Set( p_pBytes, p_Count );
	}

	void Hash::Set( const std::string & p_String )
	{
		m_Bytes.clear( );

		// Push back all the characters in the string.
		for( SizeType i = 0; i < p_String.size( ); i++ )
		{
			m_Bytes.push_back( p_String[ i ] );
		}
	}

	void Hash::Set( const Uint8 * p_pBytes, const SizeType p_Count )
	{
		m_Bytes.clear( );

		// Push back all the bytes.
		for( SizeType i = 0; i < p_Count; i++ )
		{
			m_Bytes.push_back( p_pBytes[ i ] );
		}
	}

	Bool Hash::SetHexString( const std::string & p_HexString )
	{
		m_Bytes.clear( );

		// Go throguh all characters except the last one if the
		// hexstring size is uneven.
		SizeType mod = static_cast<SizeType>( p_HexString.size( ) ) % 2;

		for( SizeType i = 0; i < p_HexString.size( ) - mod; i+= 2 )
		{
			// Get the value from the current hex value.
			Uint8 value = 0;
			if( GetValueFromHex( p_HexString[ i ], p_HexString[ i + 1 ], value ) == false )
			{
				Clear( );
				return false;
			}

			// Push back the value
			m_Bytes.push_back( value );
		}

		return true;
	}

	void Hash::AddByte( const Uint8 p_Byte )
	{
		m_Bytes.push_back( p_Byte );
	}

	void Hash::Clear( )
	{
		m_Bytes.clear( );
	}

	Uint8 Hash::GetByte( const SizeType p_Index ) const
	{
		if( p_Index >= m_Bytes.size( ) )
		{
			return 0;
		}

		return m_Bytes[ p_Index ];
	}

	std::string Hash::AsString( ) const
	{
		// Create an output string
		std::string output;
		output.reserve( m_Bytes.size( ) );

		// Fill the output string
		for( SizeType i = 0; i < m_Bytes.size( ); i++ )
		{
			output.push_back( m_Bytes[ i ] );
		}

		// Return the output string
		return output;
	}

	std::string Hash::AsHexString( const bool p_Uppercase ) const
	{
		// Create an output string
		std::string output;
		output.reserve( m_Bytes.size( ) );

		// Fill the output string
		for( SizeType i = 0; i < m_Bytes.size( ); i++ )
		{
			output.append( GetHexValue( m_Bytes[ i ], p_Uppercase ) );
		}

		// Return the output string
		return output;
	}

	std::string Hash::AsUrlEncodedString( ) const
	{
		return Url::GetBinaryEncodedString( AsString( ) );
	}

	const Hash::ByteVector & Hash::AsRaw( ) const
	{
		return m_Bytes;
	}

}
