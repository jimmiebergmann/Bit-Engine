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


#ifndef __BIT_SYSTEM_MATH_HPP__
#define __BIT_SYSTEM_MATH_HPP__

#include <Bit/DataTypes.hpp>
#include <cmath>

namespace Bit
{

	BIT_FLOAT64 DegreesToRadians( BIT_FLOAT64 p_Degrees)
	{
		return  p_Degrees * BIT_PI / 180.0f;
	}
	BIT_FLOAT64 RadiansToDegrees( BIT_FLOAT64 p_Radians )
	{
		return p_Radians * 180.0f / BIT_PI;
	}

	BIT_FLOAT64 SinDegrees( BIT_FLOAT64 p_angle )
	{
		return sin( DegreesToRadians( p_angle ) );
	}

	BIT_FLOAT64 CosDegrees( BIT_FLOAT64 p_angle )
	{
		return cos( DegreesToRadians( p_angle ) );
	}
	BIT_FLOAT64 TanDegrees( BIT_FLOAT64 p_angle )
	{
		return tan( DegreesToRadians( p_angle ) );
	}

}

#endif