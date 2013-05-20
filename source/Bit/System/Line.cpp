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

#include <Bit/System/Line.hpp>
#include <Bit/System/Circle.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Line::Line( )
	{
	}

	Line::Line( const Line & p_Line )
	{
		p[ 0 ] = p_Line.p[ 0 ];
		p[ 1 ] = p_Line.p[ 1 ];
	}

	Line::Line( const Vector3_f32 p_P1, const Vector3_f32 p_P2 )
	{
		p[ 0 ] = p_P1;
		p[ 1 ] = p_P2;
	}

	Line::Line( const Vector2_f32 p_P1, const Vector2_f32 p_P2 )
	{
		p[ 0 ].x = p_P1.x;
		p[ 0 ].y = p_P1.y;
		p[ 0 ].z = 0.0f;
		p[ 1 ].x = p_P2.x;
		p[ 1 ].y = p_P2.y;
		p[ 1 ].z = 0.0f;
	}

	Line::Line( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2,
		const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2 )
	{
		p[ 0 ] = Vector3_f32( p_P1_1, p_P1_2, 0.0f );
		p[ 1 ] = Vector3_f32( p_P2_1, p_P2_2, 0.0f );
	}

	Line::Line( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2, const BIT_FLOAT32 p_P1_3,
		const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2, const BIT_FLOAT32 p_P2_3 )
	{
		p[ 0 ] = Vector3_f32( p_P1_1, p_P1_2, p_P1_3 );
		p[ 1 ] = Vector3_f32( p_P2_1, p_P2_2, p_P2_3 );
	}

	// Public functions
	BIT_FLOAT32 Line::GetLength( ) const
	{
		return Vector3_f32( p[ 0 ] - p[ 1 ] ).Magnitude( );
	}

	// Intersection functions
	BIT_BOOL Line::IntersectionPoint2( Vector2_f32 p_Point )
	{
		return IntersectionPoint2Line2( p_Point, *this );
	}

	BIT_BOOL Line::IntersectionPoint3( Vector3_f32 p_Point )
	{
		return IntersectionPoint3Line3( p_Point, *this );
	}

	BIT_BOOL Line::IntersectionLine2( Line p_Line, Vector3_f32 & p_Intersection )
	{
		return IntersectionLine2Line2( *this, p_Line, p_Intersection ) ;
	}

	BIT_BOOL Line::IntersectionCircle2( Circle p_Circle )
	{
		return IntersectionLine2Circle2( *this, p_Circle);
	}

	BIT_BOOL Line::IntersectionCircle3( Circle p_Circle )
	{
		/*BIT_UINT32 Count;
		Vector3_f32 A, B;*/
		return IntersectionLine3Circle3( *this, p_Circle/*, Count, A, B*/ );
	}


}