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

#ifndef BIT_SYSTEM_MATH_HPP
#define BIT_SYSTEM_MATH_HPP

#include <Bit/Build.hpp>
#include <cmath>

namespace Bit
{
/*
	// Pre-defined classes.
	template < typename T >
	class Vector2;
	template < typename T >
	class Vector3;

	typedef Vector3< BIT_FLOAT32 > Vector3_f32;
	typedef Vector2< BIT_FLOAT32 > Vector2_f32;

	class Line2;
	class Line3;
	class Circle;
	class Quad;
	class Sphere;
	class Box;
	*/

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Math class
	/// 
	////////////////////////////////////////////////////////////////
	class Math
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Convert from degrees to radians
		/// 
		/// \see RadiansToDegrees
		////////////////////////////////////////////////////////////////
		inline static Float64 BIT_API DegreesToRadians( const Float64 p_Degrees)
		{
			return  p_Degrees * Pi / 180.0f;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Convert from radians to degrees
		/// 
		/// \see RadiansToDegrees
		////////////////////////////////////////////////////////////////
		inline static Float64 BIT_API RadiansToDegrees( const Float64 p_Radians )
		{
			return p_Radians * 180.0f / Pi;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the sin value in degrees
		/// 
		////////////////////////////////////////////////////////////////
		inline static Float64 BIT_API SinDegrees( const Float64 p_angle )
		{
			return sin( DegreesToRadians( p_angle ) );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the cos value in degrees
		/// 
		////////////////////////////////////////////////////////////////
		inline static Float64 BIT_API CosDegrees( const Float64 p_angle )
		{
			return cos( DegreesToRadians( p_angle ) );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the tan value in degrees
		/// 
		////////////////////////////////////////////////////////////////
		inline static Float64 BIT_API TanDegrees( const Float64 p_angle )
		{
			return tan( DegreesToRadians( p_angle ) );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Clamp the value between p_Min and p_Max
		///
		/// \param p_Value The value to clamp
		/// \param p_Min The minimum value
		/// \param p_Max The maximum value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		inline static T Clamp( const T & p_Value, const T & p_Min, const T & p_Max )
		{
			if( p_Value < p_Min )
			{
				return p_Min;
			}
			if( p_Value > p_Max )
			{
				return p_Max;
			}
			return p_Value;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Checking if two values are equal each other
		///
		/// Used for camparing two values with precision issues.
		///
		/// \param p_Value1 The first value
		/// \param p_Value2 The second value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		inline static Bool EqualEpsilon( const T & p_Value1, const T & p_Value2 )
		{
			if( static_cast<Float64>( p_Value1 ) > ( static_cast<Float64>( p_Value2 ) + Epsilon ) ||
				static_cast<Float64>( p_Value1 ) < ( static_cast<Float64>( p_Value2 ) - Epsilon ) )
			{
				return false;
			}

			return true;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Checking if two values are equal each other
		///
		/// Used for camparing two values with precision issues.
		///
		/// \param p_Value1 The first value
		/// \param p_Value2 The second value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		inline static Bool EqualEpsilonHalf( const T & p_Value1, const T & p_Value2 )
		{
			if( static_cast<Float64>( p_Value1 ) > ( static_cast<Float64>( p_Value2 ) + EpsilonHalf ) ||
				static_cast<Float64>( p_Value1 ) < ( static_cast<Float64>( p_Value2 ) - EpsilonHalf ) )
			{
				return false;
			}

			return true;
		}



		/*
		// Intersection tests

		// Point 2
		BIT_API BIT_BOOL IntersectionPoint2Line2(	Vector2_f32 p_Point, Line2 p_Line );
		BIT_API BIT_BOOL IntersectionPoint2Circle(	Vector2_f32 p_Point, Circle p_Circle );
		BIT_API BIT_BOOL IntersectionPoint2Quad(	Vector2_f32 p_Point, Quad p_Quad );

		// Point 3
		BIT_API BIT_BOOL IntersectionPoint3Line3(	Vector3_f32 p_Point, Line3 p_Line );
		BIT_API BIT_BOOL IntersectionPoint3Sphere(	Vector3_f32 p_Point, Sphere p_Sphere );
		BIT_API BIT_BOOL IntersectionPoint3Box(		Vector3_f32 p_Point, Box p_Box );

		// Line 2
		BIT_API BIT_BOOL IntersectionLine2Line2(	Line2 p_Line1,	Line2 p_Line2,		 Vector2_f32 & p_Intersection  );
		BIT_API BIT_BOOL IntersectionLine2Circle(	Line2 p_Line,	Circle p_Circle );
		BIT_API BIT_UINT32 IntersectionLine2Circle(	Line2 p_Line,	Circle p_Circle,
			Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 );
		BIT_API BIT_BOOL IntersectionLine2Quad(		Line2 p_Line,	Quad p_Quad );

		// Line 3
		BIT_API BIT_BOOL IntersectionLine3Line3(	Line3 p_Line1,	Line3 p_Line2,		 Vector3_f32 & p_Intersection );
		BIT_API BIT_BOOL IntersectionLine3Sphere(	Line3 p_Line,	Sphere p_Sphere );
		BIT_API BIT_UINT32 IntersectionLine3Sphere(	Line3 p_Line,	Sphere p_Sphere,
			Vector3_f32 & p_Point1, Vector3_f32 & p_Point2 );
		BIT_API BIT_BOOL IntersectionLine3Box(		Line3 p_Line,	Box p_Box );

		// Circle
		BIT_API BIT_SINT32 IntersectionCircleCircle(	Circle p_Circle1,	Circle p_Circle2 );
		BIT_API BIT_SINT32 IntersectionCircleCircle(	Circle p_Circle1,	Circle p_Circle2,
			Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 );
		BIT_API BIT_BOOL IntersectionCircleQuad(	Circle p_Circle,	Quad p_Quad );

		// Quad
		BIT_API BIT_BOOL IntersectionQuadQuad(		Quad p_Quad1,	Quad p_Quad2 );

		// Sphere
		BIT_API BIT_SINT32 IntersectionSphereSphere(	Sphere p_Sphere1,	Sphere p_Sphere2 );
		BIT_API BIT_BOOL IntersectionSphereBox(		Sphere p_Sphere,	Box p_Box );

		// Box
		BIT_API BIT_BOOL IntersectionBoxBox(		Box p_Box1,	Box p_Box2 );

	
		// Quadratic equation solver
		BIT_API BIT_UINT32 QuadraticEquation( const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B,
			const BIT_FLOAT32 p_C, BIT_FLOAT32 & p_X1, BIT_FLOAT32 & p_X2 );
		*/
	};
}

#endif
