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

#include <Bit/System/Line2.hpp>
#include <Bit/System/Circle.hpp>
#include <Bit/System/Quad.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Line2::Line2( )
	{
	}

	Line2::Line2( const Line2 & p_Line )
	{
		p[ 0 ] = p_Line.p[ 0 ];
		p[ 1 ] = p_Line.p[ 1 ];
	}

	Line2::Line2( const Vector2_f32 p_P1, const Vector2_f32 p_P2 )
	{
		p[ 0 ] = p_P1;
		p[ 1 ] = p_P2;
	}

	Line2::Line2( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2,
		const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2 )
	{
		p[ 0 ] = Vector2_f32( p_P1_1, p_P1_2 );
		p[ 1 ] = Vector2_f32( p_P2_1, p_P2_2 );
	}


	// Public functions
	BIT_FLOAT32 Line2::GetLength( ) const
	{
		return Vector2_f32( p[ 0 ] - p[ 1 ] ).Magnitude( );
	}


	// Intersection functions
	BIT_BOOL Line2::Intersection( Vector2_f32 p_Point )
	{
		return IntersectionPoint2Line2( p_Point, *this );
	}

	BIT_BOOL Line2::Intersection( Line2 p_Line )
	{
		Vector2_f32 Dummy;
		return IntersectionLine2Line2( *this, p_Line, Dummy );
	}

	BIT_BOOL Line2::Intersection( Line2 p_Line, Vector2_f32 & p_Intersection )
	{
		return IntersectionLine2Line2( *this, p_Line, p_Intersection );
	}

	BIT_BOOL Line2::Intersection( Circle p_Circle )
	{
		return IntersectionLine2Circle( *this, p_Circle );
	}

	BIT_UINT32 Line2::Intersection( Circle p_Circle, Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 )
	{
		return IntersectionLine2Circle( *this, p_Circle, p_Point1, p_Point2 );
	}

	/*BIT_BOOL Line2::Intersection( Quad p_Quad )
	{
		return IntersectionLine2Quad( *this, p_Quad );
	}
	*/
}