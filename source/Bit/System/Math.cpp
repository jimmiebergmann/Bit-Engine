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
#include <Bit/System/Line2.hpp>
#include <Bit/System/Line3.hpp>
#include <Bit/System/Circle.hpp>
#include <Bit/System/Quad.hpp>
#include <Bit/System/Sphere.hpp>
#include <Bit/System/Box.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Intersection tests



		/*

	// Point 2
	BIT_API BIT_BOOL IntersectionPoint2Line2(	Vector2_f32 p_Point, Line2 p_Line );
	BIT_API BIT_BOOL IntersectionPoint2Circle(	Vector2_f32 p_Point, Cicle p_Circle );
	BIT_API BIT_BOOL IntersectionPoint2Quad(	Vector2_f32 p_Point, Quad p_Quad );
*/
	// Point 3
	BIT_API BIT_BOOL IntersectionPoint3Line3( Vector3_f32 p_Point, Line3 p_Line )
	{
		// Resources: http://answers.yahoo.com/question/index?qid=20100821030217AAISaE6

		// Calcuate the area from the 3 points
		BIT_FLOAT32 Area = 
			Vector3_f32( p_Line.p[ 1 ] - p_Point ).Cross(
			Vector3_f32( p_Line.p[ 1 ] - p_Line.p[ 0 ] ) ).Magnitude( ) * 0.5f;

		// Use the epsilon function to make sure that things wont get way too buggy
		if( EqualEpsilon( Area, 0.0f ) )
		{
			return BIT_TRUE;
		}

		return BIT_FALSE;
	}

	/*BIT_API BIT_BOOL IntersectionPoint3Sphere(	Vector3_f32 p_Point, Sphere p_Sphere );
	BIT_API BIT_BOOL IntersectionPoint3Box(		Vector3_f32 p_Point, Box p_Box );

	// Line 2
	BIT_API BIT_BOOL IntersectionLine2Line2(	Line2 p_Line1,	Line2 p_Line2 );
	BIT_API BIT_BOOL IntersectionLine2Circle(	Line2 p_Line,	Circle p_Circle );
	BIT_API BIT_BOOL IntersectionLine2Quad(		Line2 p_Line,	Quad p_Quad );
*/
	// Line 3
	BIT_API BIT_BOOL IntersectionLine3Line3( Line3 p_Line1,	Line3 p_Line2, Vector3_f32 & p_Intersection )
	{
		// http://mathworld.wolfram.com/Line-LineIntersection.html
		// in 3d; will also work in 2d if z components are 0
	
		Vector3_f32 da = p_Line1.p[ 1 ] - p_Line1.p[ 0 ]; 
		Vector3_f32 db = p_Line2.p[ 1 ] - p_Line2.p[ 0 ];
		Vector3_f32 dc = p_Line2.p[ 0 ] - p_Line1.p[ 0 ];

		// The lines are not coplanar
		if( dc.Dot( da.Cross( db ) ) != 0.0f )
		{
			return BIT_FALSE;
		}
	
		Vector3_f32 cs = da.Cross( db );
		BIT_FLOAT32 s = dc.Cross( db ).Dot( da.Cross( db ) ) /
			( ( cs.x * cs.x ) + ( cs.y * cs.y )+ ( cs.z * cs.z ) );


		//Point s = dot(cross(dc,db),cross(da,db)) / norm2(cross(da,db));
		if (s >= 0.0f && s <= 1.0f)
		{
			p_Intersection = p_Line1.p[ 0 ] + ( da * Vector3_f32( s, s, s ) );
			//ip = a.first + da * Coord(s,s,s);
			return BIT_TRUE;
		}
	
		return BIT_FALSE;
	}

	BIT_API BIT_BOOL IntersectionLine3Sphere(	Line3 p_Line,	Sphere p_Sphere )
	{
		return BIT_FALSE;
	}

	BIT_API BIT_BOOL IntersectionLine3Box(		Line3 p_Line,	Box p_Box )
	{
		return BIT_FALSE;
	}
/*
	// Circle
	BIT_API BIT_BOOL IntersectionCircleCircle(	Circle p_Circle1,	Circle p_Circle2 );
	BIT_API BIT_BOOL IntersectionCircleQuad(	Circle p_Circle,	Quad p_Quad );

	// Quad
	BIT_API BIT_BOOL IntersectionQuadQuad(		Quad p_Quad1,	Quad p_Quad2 );

	// Sphere
	BIT_API BIT_BOOL IntersectionSphereSphere(	Sphere p_Sphere1,	Sphere p_Sphere2 );
	BIT_API BIT_BOOL IntersectionSphereBox(		Sphere p_Sphere,	Box p_Box );

	// Box
	BIT_API BIT_BOOL IntersectionBoxBox(		Box p_Box1,	Box p_Box2 );

	*/






	
	/*
	BIT_API BIT_BOOL IntersectionPoint2Circle2( Vector2_f32 p_Point, Circle p_Circle )
	{
		BIT_FLOAT32 Distance = Vector2_f32( p_Point - Vector2_f32( p_Circle.Position.x, p_Circle.Position.y ) ).Magnitude( );

		if( Distance > ( p_Circle.Radius ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint3Circle3( Vector3_f32 p_Point, Circle p_Circle )
	{
		BIT_FLOAT32 Distance = Vector3_f32( p_Point - p_Circle.Position ).Magnitude( );

		if( Distance > ( p_Circle.Radius ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint2Line2( Vector2_f32 p_Point, Line p_Line )
	{
		// Resouces: http://www.mathopenref.com/coordtrianglearea.html

		// Calcuate the area from the 3 points
		BIT_FLOAT32 Area = 
			(	( p_Point.x * ( p_Line.p[ 0 ].y - p_Line.p[ 1 ].y ) ) +
				( p_Line.p[ 0 ].x * ( p_Line.p[ 1 ].y - p_Point.y ) ) +
				( p_Line.p[ 1 ].x * ( p_Point.y - p_Line.p[ 0 ].y ) ) ) / 2.0f;

		// Use the epsilon function to make sure that things wont get way too buggy
		if( EqualEpsilon( Area, 0.0f ) )
		{
			return BIT_TRUE;
		}

		return BIT_FALSE;
	}

	BIT_API BIT_BOOL IntersectionPoint3Line3( Vector3_f32 p_Point, Line p_Line )
	{
		// Resources: http://answers.yahoo.com/question/index?qid=20100821030217AAISaE6

		// Calcuate the area from the 3 points
		BIT_FLOAT32 Area = 
			Vector3_f32( p_Line.p[ 1 ] - p_Point ).Cross(
			Vector3_f32( p_Line.p[ 1 ] - p_Line.p[ 0 ] ) ).Magnitude( ) * 0.5f;

		// Use the epsilon function to make sure that things wont get way too buggy
		if( EqualEpsilon( Area, 0.0f ) )
		{
			return BIT_TRUE;
		}

		return BIT_FALSE;
	}


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

	BIT_API BIT_BOOL IntersectionLine2Circle2( Line p_Line, Circle p_Circle )
	{
		// Resources: http://stackoverflow.com/questions/6533856/ray-sphere-intersection

		const BIT_FLOAT32 xA = p_Line.p[ 0 ].x;
		const BIT_FLOAT32 yA = p_Line.p[ 0 ].y;
		const BIT_FLOAT32 xB = p_Line.p[ 1 ].x;
		const BIT_FLOAT32 yB = p_Line.p[ 1 ].y;
		const BIT_FLOAT32 xC = p_Circle.Position.x;
		const BIT_FLOAT32 yC = p_Circle.Position.y;
		const BIT_FLOAT32 r = p_Circle.Radius;

		const BIT_FLOAT32 a = ( (xB-xA) * (xB-xA) ) + ( (yB-yA) * (yB-yA) );
		const BIT_FLOAT32 b = 2.0f *( (xB-xA) * (xA-xC) + (yB-yA) * (yA-yC) );
		const BIT_FLOAT32 c = ( (xA-xC) * (xA-xC) ) + ( (yA-yC) * (yA-yC) )  - ( r * r );

		// Get the delta( data under the root
		const BIT_FLOAT32 Delta = ( b * b ) - ( 4.0f * a * c );

		// No intersection at all
		if( Delta < 0.0f )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
*/
		/*// Resources: 
		Vector3_f32 A;
		A.x = p_Line.p[ 1 ].x - p_Line.p[ 0 ].x;
		A.y = p_Line.p[ 1 ].y - p_Line.p[ 0 ].y;
		A.z = sqrt( ( A.x * A.x ) + ( A.y * A.y ) );

		// Calculate the determine
		BIT_FLOAT32 D = ( p_Line.p[ 0 ].x - p_Line.p[ 1 ].y ) -
						( p_Line.p[ 1 ].x - p_Line.p[ 0 ].y );

		BIT_FLOAT32 Delta = ( ( p_Circle.Radius * p_Circle.Radius ) * ( A.z * A.z ) ) - ( D * D );

		return BIT_FALSE;*/
	/*}

	BIT_API BIT_BOOL IntersectionLine3Circle3( Line p_Line, Circle p_Circle )
	{
		// Resources: http://stackoverflow.com/questions/6533856/ray-sphere-intersection

		const BIT_FLOAT32 xA = p_Line.p[ 0 ].x;
		const BIT_FLOAT32 yA = p_Line.p[ 0 ].y;
		const BIT_FLOAT32 zA = p_Line.p[ 0 ].z;
		const BIT_FLOAT32 xB = p_Line.p[ 1 ].x;
		const BIT_FLOAT32 yB = p_Line.p[ 1 ].y;
		const BIT_FLOAT32 zB = p_Line.p[ 1 ].z;
		const BIT_FLOAT32 xC = p_Circle.Position.x;
		const BIT_FLOAT32 yC = p_Circle.Position.y;
		const BIT_FLOAT32 zC = p_Circle.Position.z;
		const BIT_FLOAT32 r = p_Circle.Radius;

		const BIT_FLOAT32 a = ( (xB-xA) * (xB-xA) ) + ( (yB-yA) * (yB-yA) ) + ( (zB-zA) * (zB-zA) );
		const BIT_FLOAT32 b = 2.0f *( (xB-xA) * (xA-xC) + (yB-yA) * (yA-yC) + (zB-zA) * (zA-zC) );
		const BIT_FLOAT32 c = ( (xA-xC) * (xA-xC) ) + ( (yA-yC) * (yA-yC) ) + ( (zA-zC) * (zA-zC) ) - ( r * r );

		// Get the delta( data under the root
		const BIT_FLOAT32 Delta = ( b * b ) - ( 4.0f * a * c );

		// No intersection at all
		if( Delta < 0.0f )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionLine3Circle3( Line p_Line, Circle p_Circle,
		BIT_UINT32 & p_IntersectionCount, Vector3_f32 & m_IP1, Vector3_f32 & m_IP2 )
	{

		// Resources: http://stackoverflow.com/questions/6533856/ray-sphere-intersection

		const BIT_FLOAT32 xA = p_Line.p[ 0 ].x;
		const BIT_FLOAT32 yA = p_Line.p[ 0 ].y;
		const BIT_FLOAT32 zA = p_Line.p[ 0 ].z;
		const BIT_FLOAT32 xB = p_Line.p[ 1 ].x;
		const BIT_FLOAT32 yB = p_Line.p[ 1 ].y;
		const BIT_FLOAT32 zB = p_Line.p[ 1 ].z;
		const BIT_FLOAT32 xC = p_Circle.Position.x;
		const BIT_FLOAT32 yC = p_Circle.Position.y;
		const BIT_FLOAT32 zC = p_Circle.Position.z;
		const BIT_FLOAT32 r = p_Circle.Radius;

		const BIT_FLOAT32 a = ( (xB-xA) * (xB-xA) ) + ( (yB-yA) * (yB-yA) ) + ( (zB-zA) * (zB-zA) );
		const BIT_FLOAT32 b = 2.0f *( (xB-xA) * (xA-xC) + (yB-yA) * (yA-yC) + (zB-zA) * (zA-zC) );
		const BIT_FLOAT32 c = ( (xA-xC) * (xA-xC) ) + ( (yA-yC) * (yA-yC) ) + ( (zA-zC) * (zA-zC) ) - ( r * r );

		// Get the delta( data under the root
		const BIT_FLOAT32 Delta = ( b * b ) - ( 4.0f * a * c );

		// No intersection at all
		if( Delta < 0.0f )
		{
			return BIT_FALSE;
		}

		// Find the intersection points
		BIT_FLOAT32 DeltaSqrt = sqrt( Delta );
	
		// Full equation 1
		BIT_FLOAT32 u1 = ( -b - DeltaSqrt ) / ( 2.0f * a );

		// Set the first interpolation point
		m_IP1.x = xA + ( u1 * ( xB - xA ) );
		m_IP1.y = yA + ( u1 * ( yB - yA ) );
		m_IP1.z = zA + ( u1 * ( zB - zA ) );

		// Are we just getting 1 intersection point?
		if( EqualEpsilon( Delta, 0.0f ) )
		{
			return BIT_TRUE;
		}

		// The line is going through 2 points
		// Full equation 2
		BIT_FLOAT32 u2 = ( -b + DeltaSqrt ) / ( 2.0f * a );

		// Set the first interpolation point
		m_IP2.x = xA + ( u2 * ( xB - xA ) );
		m_IP2.y = yA + ( u2 * ( yB - yA ) );
		m_IP2.z = zA + ( u2 * ( zB - zA ) );

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionCircle2Circle2( Circle p_Circle1, Circle p_Circle2 )
	{
		BIT_FLOAT32 Distance = Vector2_f32(
			Vector2_f32( p_Circle1.Position.x, p_Circle1.Position.y ) -
			Vector2_f32( p_Circle2.Position.x, p_Circle2.Position.y  ) ).Magnitude( );

		if( Distance > ( p_Circle1.Radius + p_Circle2.Radius ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionCircle3Circle3( Circle p_Circle1, Circle p_Circle2 )
	{
		BIT_FLOAT32 Distance = Vector3_f32( p_Circle1.Position - p_Circle2.Position ).Magnitude( );

		if( Distance > ( p_Circle1.Radius + p_Circle2.Radius ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}
*/
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