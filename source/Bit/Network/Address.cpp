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
#include <Bit/Network/Socket.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	const Address Address::NoAddress = Address( 0 );
	const Address Address::Localhost = Address( 2130706433 );

	Address::Address( ) :
        m_Address( 0 )
    {
	}

	Address::Address( const Uint8 p_A, const Uint8 p_B, const Uint8 p_C, const Uint8 p_D )
	{
		/// Bit swift the address
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
		// 127.0.0.1
		// 192.168.0.1
		// www.google.com
		// http://www.sunet.se
		// 0.europe.pool.ntp.org

		// Get the address
		Address address = Socket::GetHostByName( p_String );
		if( address == NoAddress )
		{
			return false;
		}

		// Set the internal address varaible.
		m_Address = address.GetAddress( );

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

	Bool Address::operator < ( const Address & p_Address ) const
	{
		return m_Address < p_Address.m_Address;
	}

	Bool Address::operator > ( const Address & p_Address ) const
	{
		return m_Address > p_Address.m_Address;
	}

}
