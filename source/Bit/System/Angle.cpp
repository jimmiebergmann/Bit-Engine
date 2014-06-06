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

#include <Bit/System/Angle.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Angle::Angle( ) :
		m_Radians( 0 )
	{
	}

	Angle::Angle( const Float64 & p_Radians ) :
		m_Radians( p_Radians )
	{
	}

	Float64 Angle::AsDegrees( ) const
	{
		return m_Radians * 180.0f / Pi ;
	}

	Float64 Angle::AsRadians( ) const
	{
		return m_Radians;
	}

	Float64 Angle::operator( ) ( ) const
	{
		return m_Radians;
	}

	BIT_API Angle Degrees( const Float64 & p_Degrees )
	{
		return Angle( p_Degrees * Pi / 180.0f );
	}

	BIT_API Angle Radians( const Float64 & p_Radians )
	{
		return Angle( p_Radians );
	}

}