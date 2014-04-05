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

#include <Bit/System/Time.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	const Time Time::Infinite = Microseconds( 0xFFFFFFFFFFFFFFFFULL );
	const Time Time::Zero = Microseconds( 0 );


	Time::Time( ) :
		m_Microseconds( 0 )
	{
	}

	Float64 Time::AsSeconds( ) const
	{
		return static_cast<Float64>( m_Microseconds ) / 1000000.0f;
	}

	Uint64 Time::AsMilliseconds( ) const
	{
		return m_Microseconds / 1000ULL;
	}

	Uint64 Time::AsMicroseconds( ) const
	{
		return m_Microseconds;
	}

	// Private functions
	Time::Time( const Uint64 & p_Microseconds ) :
		m_Microseconds( p_Microseconds )
	{
	}

	// Functions for initializing time classes
	BIT_API Time Seconds( const Float64 & p_Seconds )
	{
		return Time( static_cast<Uint64>( p_Seconds * 1000000.0f ) );
	}

	BIT_API Time Milliseconds( const Uint64 & p_Milliseconds )
	{
		return Time( p_Milliseconds * 1000ULL );
	}

	BIT_API Time Microseconds( const Uint64 & p_Microseconds )
	{
		return Time( p_Microseconds );
	}

}