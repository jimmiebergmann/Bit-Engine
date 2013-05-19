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


#ifndef __BIT_SYSTEM_CIRCLE_HPP__
#define __BIT_SYSTEM_CIRCLE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Math.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	// Use this class for both circles and spheres (circle2/circle3)
	class Circle
	{

	public:

		// Construcotrs
		Circle( );
		Circle( const BIT_FLOAT32 p_Radius );
		Circle( const Vector2_f32 p_Position, const BIT_FLOAT32 p_Radius );
		Circle( const Vector3_f32 p_Position, const BIT_FLOAT32 p_Radius );

		// Get functions functions
		BIT_FLOAT32 GetRadius( ) const;
		BIT_FLOAT32 GetDiameter( ) const;
		BIT_FLOAT32 GetArea( ) const;
		BIT_FLOAT32 GetCircumference( ) const;

		// Intersection functions
		BIT_BOOL IntersectionPoint2( Vector2_f32 p_Point );
		BIT_BOOL IntersectionPoint3( Vector3_f32 p_Point );
		BIT_BOOL IntersectionLine2( Line p_Line );
		BIT_BOOL IntersectionLine3( Line p_Line );
		BIT_BOOL IntersectionCircle2( Circle p_Circle );
		BIT_BOOL IntersectionCircle3( Circle p_Circle );

		// Public variables
		Vector3_f32 Position;
		BIT_FLOAT32 Radius;

	};

}

#endif