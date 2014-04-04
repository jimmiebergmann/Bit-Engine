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

	Time::Time( ) :
		m_Nanoseconds( 0 )
	{
	}

	Time::Time( const Uint64 & p_Nanoseconds ) :
		m_Nanoseconds( p_Nanoseconds )
	{
	}

	void Time::Set( const Uint64 & p_Nanoseconds )
	{
		m_Nanoseconds = p_Nanoseconds;
	}

	Float64 Time::AsSeconds( ) const
	{
		return static_cast<Float64>( m_Nanoseconds ) / 1000000.0f;
	}

	Uint64 Time::AsMilliseconds( ) const
	{
		return m_Nanoseconds / 1000ULL;
	}

	Uint64 Time::AsNanoseconds( ) const
	{
		return m_Nanoseconds;
	}

}