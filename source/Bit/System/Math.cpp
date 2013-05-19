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

#include <Bit/System/Math.hpp>
#include <Bit/System/Line.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Intersection tests
	BIT_API BIT_BOOL IntersectionLine2Line2( Line p_Line1, Line p_Line2,
		Vector3_f32 & p_Intersection )
	{
		// calculate the "parallel constant"
		const BIT_FLOAT32 Parallel = ( ( p_Line1.p[ 0 ].x - p_Line1.p[ 1 ] .x ) * ( p_Line2.p[ 0 ].y - p_Line2.p[ 1 ].y ) ) -
			( ( p_Line2.p[ 0 ].x - p_Line2.p[ 1 ].x ) * ( p_Line1.p[ 0 ].y - p_Line1.p[ 1 ].y ) );

		// The line is parallel, what to do?
		if( Parallel == 0.0f )
		{
			return BIT_FALSE;
		}

		// Calculate the X coordinate
		BIT_FLOAT32 A = ( p_Line1.p[ 0 ].x * p_Line1.p[ 1 ].y - p_Line1.p[ 0 ].y * p_Line1.p[ 1 ].x  );
		BIT_FLOAT32 B = ( p_Line2.p[ 0 ].x * p_Line2.p[ 1 ].y - p_Line2.p[ 0 ].y * p_Line2.p[ 1 ].x  );

		BIT_FLOAT32 X = 
			( A * ( p_Line2.p[ 0 ].x  - p_Line2.p[ 1 ].x ) ) -
			( B * ( p_Line1.p[ 0 ].x  - p_Line1.p[ 1 ].x ) );
		X /= Parallel;

		BIT_FLOAT32 Y =
			( A * ( p_Line2.p[ 0 ].y  - p_Line2.p[ 1 ].y ) ) -
			( B * ( p_Line1.p[ 0 ].y  - p_Line1.p[ 1 ].y ) );
		Y /= Parallel;

		p_Intersection.x = X;
		p_Intersection.y = Y;
		p_Intersection.z = 0.0f;

		// Check if the lines actually are crossing each other.
		BIT_FLOAT32 LineMagnitudes[ 2 ] =
		{
			Vector3_f32( p_Line1.p[ 0 ] - p_Line1.p[ 1 ] ).Magnitude( ),
			Vector3_f32( p_Line1.p[ 0 ] - p_Line1.p[ 1 ] ).Magnitude( )
		};

		BIT_FLOAT32 PointMagnitudes[ 4 ] =
		{
			Vector3_f32( p_Intersection - p_Line1.p[ 0 ] ).Magnitude( ),
			Vector3_f32( p_Intersection - p_Line1.p[ 1 ] ).Magnitude( ),
			Vector3_f32( p_Intersection - p_Line2.p[ 1 ] ).Magnitude( ),
			Vector3_f32( p_Intersection - p_Line2.p[ 1 ] ).Magnitude( ),
		};

		if( ( PointMagnitudes[ 0 ] > LineMagnitudes[ 0 ] ) ||
			( PointMagnitudes[ 1 ] > LineMagnitudes[ 0 ] ) ||
			( PointMagnitudes[ 2 ] > LineMagnitudes[ 1 ] ) ||
			( PointMagnitudes[ 3 ] > LineMagnitudes[ 1 ] ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	// Quadratic equation solver
	BIT_API BIT_UINT32 QuadraticEquation( const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B,
		const BIT_FLOAT32 p_C, BIT_FLOAT32 & p_X1, BIT_FLOAT32 & p_X2 )
	{
		// Returning the numbers of roots

		// Root data
		BIT_FLOAT32 Z = ( p_B * p_B ) - (4 * p_A * p_C);

		// Roots were found
		if( Z < 0.0f )
		{
			return 0;
		}
		else if( Z == 0.0f )
		{
			p_X1 = -p_B / ( 2.0f * p_A );
			return 1;
		}
		
		// Else if we have 2 roots
		BIT_FLOAT32 Root = sqrt( Z );
		p_X1 = (-p_B - Root) / ( 2.0f * p_A );
		p_X2 = (-p_B + Root) / ( 2.0f * p_A );

		return 2;
	}

}