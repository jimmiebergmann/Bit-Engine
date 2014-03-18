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

#include <Bit/Network/Address.hpp>

namespace Bit
{

	Address::Address( ) :
        m_Address( 0 )
    {
	}

	Address::Address( const Uint8 p_A, const Uint8 p_B, const Uint8 p_C, const Uint8 p_D )
	{
		/// Bit swift the adress
		m_Address = (p_A << 24) | (p_B << 16) | (p_C << 8) | p_D;
	}

	Address::Address( const Uint32 p_Address ) :
		m_Address( p_Address )
	{
	}

	Address::Address( const std::string & p_String )
	{
		if( SetAddressFromString( p_String ) == false )
		{
			m_Address = 0;
		}
	}

	Bool Address::SetAddressFromString( const std::string & p_String )
	{
		// Example input:
		// 1.1.1.1
		// 127.0.0.1
		// 192.168.0.1
		// 123.456.789.123

		// Make sure the size of the string is larger than the minimum limit
		if( p_String.size( ) < 7 )
		{
			return false;
		}

		// Find the positions of the periods
		SizeType periodPos[ 4 ]; // Store another period for later use.
		SizeType currentPeriod = 0;
		for( SizeType i = 0; i < p_String.size( ); i++ )
		{
			if( p_String[ i ] == '.' )
			{
				// Make sure that we don't have more than 3 periods
				if( currentPeriod > 2 )
				{
					break;
				}

				// Set the index of the current period
				periodPos[ currentPeriod ] = i;
				currentPeriod++;
			}
		}

		// Make sure that we found all the periods
		if( currentPeriod != 3 )
		{
			return false;
		}

		// Store the address bytes
		Uint8 adressBytes[ 4 ];

		// Set the last period pos to the size of the string
		periodPos[ 3 ] = p_String.size( );

		// Let's find the address bytes from the string
		SizeType start = 0;
		SizeType end = periodPos[ 0 ];
		for( SizeType i = 0; i < 4; i++ )
		{
			// Cut out the number string
			std::string newString = p_String.substr( start, end );

			// Get the string as a number
			Int32 number = atoi( newString.c_str( ) );
				
			// Error check the size
			if( number < 0 || number > 255 )
			{
				return false;
			}

			// Set the current address byte
			adressBytes[ i ] = static_cast<Uint32>( number );

			// Set the new start and end
			start = periodPos[ i ] + 1;
			end = periodPos[ i + 1 ] - periodPos[ i ] - 1;
		}

		// Set the new address
		m_Address = ( adressBytes[ 0 ] << 24 )	|
					( adressBytes[ 1 ] << 16 )	|
					( adressBytes[ 2 ] << 8 )	|
						adressBytes[ 3 ];

		// Succeeded
		return true;
	}

	unsigned int Address::GetAddress( ) const
	{
		return m_Address;
	}

	Uint8 Address::GetA( ) const
	{
		return static_cast<Uint8>( m_Address >> 24 );
	}

	Uint8 Address::GetB( ) const
	{
		return static_cast<Uint8>( m_Address >> 16 );
	}

	Uint8 Address::GetC( ) const
	{
		return static_cast<Uint8>( m_Address >> 8 );
	}

	Uint8 Address::GetD( ) const
	{
		return static_cast<Uint8>( m_Address );
	}


	void Address::SetA( const Uint8 p_A )
	{
		m_Address &= ~( 0xFF000000 );
		m_Address |= p_A << 24;
	}
	
	void Address::SetB( const Uint8 p_B )
	{
		m_Address &= ~( 0xFF0000 );
		m_Address |= p_B << 16;
	}

	void Address::SetC( const Uint8 p_C )
	{
		m_Address &= ~( 0xFF00 );
		m_Address |= p_C << 8;
	}

	void Address::SetD( const Uint8 p_D )
	{
		m_Address &= ~( 0xFF );
		m_Address |= p_D;
	}

	Bool Address::operator == ( const Address & p_Address ) const
	{
		return m_Address == p_Address.GetAddress( );
	}

	Bool Address::operator != ( const Address & p_Address ) const
	{
		return m_Address != p_Address.GetAddress( );
	}

}
