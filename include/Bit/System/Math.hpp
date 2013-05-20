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
	class Circle;
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

	BIT_INLINE BIT_BOOL EqualEpsilon( BIT_FLOAT32 p_N1, BIT_FLOAT32 p_N2 )
	{
		if( p_N1 > ( p_N2 + BIT_EPSILON ) || p_N1 < ( p_N2 - BIT_EPSILON ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	// Intersection tests
	BIT_API BIT_BOOL IntersectionPoint2Circle2( Vector2_f32 p_Point, Circle p_Circle );
	BIT_API BIT_BOOL IntersectionPoint3Circle3( Vector3_f32 p_Point, Circle p_Circle );
	BIT_API BIT_BOOL IntersectionPoint2Line2( Vector2_f32 p_Point, Line p_Line );
	BIT_API BIT_BOOL IntersectionPoint3Line3( Vector3_f32 p_Point, Line p_Line );
	BIT_API BIT_BOOL IntersectionLine2Line2( Line p_Line1, Line p_Line2,
		Vector3_f32 & p_Intersection );
	BIT_API BIT_BOOL IntersectionLine2Circle2( Line p_Line, Circle p_Circle );
	BIT_API BIT_BOOL IntersectionLine3Circle3( Line p_Line, Circle p_Circle );
	BIT_API BIT_BOOL IntersectionLine3Circle3( Line p_Line, Circle p_Circle,
		BIT_UINT32 & p_IntersectionCount, Vector3_f32 & m_IP1, Vector3_f32 & m_IP2 );
	BIT_API BIT_BOOL IntersectionCircle2Circle2( Circle p_Circle1, Circle p_Circle2 );
	BIT_API BIT_BOOL IntersectionCircle3Circle3( Circle p_Circle1, Circle p_Circle2 );

	// Quadratic equation solver
	BIT_API BIT_UINT32 QuadraticEquation( const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B,
		const BIT_FLOAT32 p_C, BIT_FLOAT32 & p_X1, BIT_FLOAT32 & p_X2 );

}

#endif
