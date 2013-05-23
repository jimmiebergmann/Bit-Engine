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

#include <Bit/System/Circle.hpp>
#include <Bit/System/Line2.hpp>
#include <Bit/System/Quad.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Circle::Circle( )
	{
	}

	Circle::Circle( const BIT_FLOAT32 p_Radius ) :
		Radius( p_Radius )
	{
	}

	Circle::Circle( const Vector2_f32 p_Position, const BIT_FLOAT32 p_Radius ) :
		Position( p_Position ),
		Radius( p_Radius )
	{
	}

	// Get functions functions
	BIT_FLOAT32 Circle::GetDiameter( ) const
	{
		return 2.0f * Radius;
	}

	BIT_FLOAT32 Circle::GetArea( ) const
	{
		return BIT_PI * Radius * Radius;
	}

	BIT_FLOAT32 Circle::GetCircumference( ) const
	{
		return 2.0f * BIT_PI * Radius;
	}


	// Intersection functions
	BIT_BOOL Circle::Intersection( Vector2_f32 p_Point )
	{
		return IntersectionPoint2Circle( p_Point, *this );
	}

	BIT_BOOL Circle::Intersection( Line2 p_Line )
	{
		return IntersectionLine2Circle( p_Line, *this );
	}

	BIT_UINT32 Circle::Intersection( Line2 p_Line, Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 )
	{
		return IntersectionLine2Circle( p_Line, *this, p_Point1, p_Point2 );
	}

	BIT_SINT32 Circle::Intersection( Circle p_Circle )
	{
		return IntersectionCircleCircle( *this, p_Circle );
	}

	BIT_SINT32 Circle::Intersection( Circle p_Circle, Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 )
	{
		return IntersectionCircleCircle( *this, p_Circle, p_Point1, p_Point2 );
	}

	BIT_BOOL Circle::Intersection( Quad p_Box )
	{
		return BIT_FALSE;
	}

}