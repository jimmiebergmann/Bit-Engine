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

	// Pre-defined classes.
	template < typename T >
	class Vector2;
	template < typename T >
	class Vector3;

	typedef Vector3< BIT_FLOAT32 > Vector3_f32;
	typedef Vector2< BIT_FLOAT32 > Vector2_f32;

	class Line;
	class Quad;
	class Box;
	class Sphere;


	// Inline functions
	BIT_INLINE BIT_FLOAT64 BIT_API DegreesToRadians( BIT_FLOAT64 p_Degrees)
	{
		return  p_Degrees * BIT_PI / 180.0f;
	}

	BIT_INLINE BIT_FLOAT64 BIT_API RadiansToDegrees( BIT_FLOAT64 p_Radians )
	{
		return p_Radians * 180.0f / BIT_PI;
	}

	BIT_INLINE BIT_FLOAT64 BIT_API SinDegrees( BIT_FLOAT64 p_angle )
	{
		return sin( DegreesToRadians( p_angle ) );
	}

	BIT_INLINE BIT_FLOAT64 BIT_API CosDegrees( BIT_FLOAT64 p_angle )
	{
		return cos( DegreesToRadians( p_angle ) );
	}

	BIT_INLINE BIT_FLOAT64 BIT_API TanDegrees( BIT_FLOAT64 p_angle )
	{
		return tan( DegreesToRadians( p_angle ) );
	}

	// Intersection tests
	BIT_API BIT_BOOL IntersectionLine2Line2( Line p_Line1, Line p_Line2,
		Vector3_f32 & p_Intersection );

	// Quadratic equation solver
	BIT_API BIT_UINT32 QuadraticEquation( const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B,
		const BIT_FLOAT32 p_C, BIT_FLOAT32 & p_X1, BIT_FLOAT32 & p_X2 );

}

#endif
