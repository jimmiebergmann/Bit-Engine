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
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
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



		

	// Point 2
	BIT_API BIT_BOOL IntersectionPoint2Line2( Vector2_f32 p_Point, Line2 p_Line )
	{
		// Resources: http://answers.yahoo.com/question/index?qid=20100821030217AAISaE6

		// Calcuate the area from the 3 points
		Vector3_f32 A( p_Line.p[ 1 ].x - p_Point.x,			p_Line.p[ 1 ].y - p_Point.y,		0.0f );
		Vector3_f32 B( p_Line.p[ 1 ].x - p_Line.p[ 0 ].x,	p_Line.p[ 1 ].y - p_Line.p[ 0 ].y,	0.0f  );
		
		BIT_FLOAT32 Area = A.Cross( B ).Magnitude( ) * 0.5f;

		// Use the epsilon function to make sure that things wont get way too buggy
		if( !EqualEpsilon( Area, 0.0f ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint2Circle(	Vector2_f32 p_Point, Circle p_Circle )
	{
		if( Vector2_f32( p_Point - p_Circle.Position ).Magnitude( ) > p_Circle.Radius )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint2Quad( Vector2_f32 p_Point, Quad p_Quad )
	{
		Vector2_f32 Size = p_Quad.Size / 2.0f;

		if( p_Point.x > ( p_Quad.Positsion.x + Size.x ) ||
			p_Point.x < ( p_Quad.Positsion.x - Size.x ) ||
			p_Point.y > ( p_Quad.Positsion.y + Size.y ) ||
			p_Point.y < ( p_Quad.Positsion.y - Size.y ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	// Point 3
	BIT_API BIT_BOOL IntersectionPoint3Line3( Vector3_f32 p_Point, Line3 p_Line )
	{
		// Resources: http://answers.yahoo.com/question/index?qid=20100821030217AAISaE6

		// Calcuate the area from the 3 points
		BIT_FLOAT32 Area = 
			Vector3_f32( p_Line.p[ 1 ] - p_Point ).Cross(
			Vector3_f32( p_Line.p[ 1 ] - p_Line.p[ 0 ] ) ).Magnitude( ) * 0.5f;

		// Use the epsilon function to make sure that things wont get way too buggy
		if( !EqualEpsilon( Area, 0.0f ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint3Sphere( Vector3_f32 p_Point, Sphere p_Sphere )
	{
		if( Vector3_f32( p_Point - p_Sphere.Position ).Magnitude( ) > p_Sphere.Radius )
		{
			return BIT_FALSE;
		}
		
		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionPoint3Box( Vector3_f32 p_Point, Box p_Box )
	{
		Vector3_f32 Size = p_Box.Size / 2.0f;

		if( p_Point.x > ( p_Box.Positsion.x + Size.x ) ||
			p_Point.x < ( p_Box.Positsion.x - Size.x ) ||
			p_Point.y > ( p_Box.Positsion.y + Size.y ) ||
			p_Point.y < ( p_Box.Positsion.y - Size.y ) ||
			p_Point.z > ( p_Box.Positsion.z + Size.z ) ||
			p_Point.z < ( p_Box.Positsion.z - Size.z ) )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	// Line 2
	BIT_API BIT_BOOL IntersectionLine2Line2( Line2 p_Line1,	Line2 p_Line2, Vector2_f32 & p_Intersection )
	{
		// http://mathworld.wolfram.com/Line-LineIntersection.html
		// ( Same as in 3D )
		Vector3_f32 da( p_Line1.p[ 1 ].x - p_Line1.p[ 0 ].x, p_Line1.p[ 1 ].y - p_Line1.p[ 0 ].y, 0.0f );
		Vector3_f32 db( p_Line2.p[ 1 ].x - p_Line2.p[ 0 ].x, p_Line2.p[ 1 ].y - p_Line2.p[ 0 ].y, 0.0f );
		Vector3_f32 dc( p_Line2.p[ 0 ].x - p_Line1.p[ 0 ].x, p_Line2.p[ 0 ].y - p_Line1.p[ 0 ].y, 0.0f );

		// The lines are not coplanar
		if( dc.Dot( da.Cross( db ) ) != 0.0f )
		{
			return BIT_FALSE;
		}
	
		Vector3_f32 cs = da.Cross( db );
		BIT_FLOAT32 s = dc.Cross( db ).Dot( da.Cross( db ) ) /
			( ( cs.x * cs.x ) + ( cs.y * cs.y )+ ( cs.z * cs.z ) );

		if (s < 0.0f && s > 1.0f)
		{
			Vector3_f32 Intersection3D = Vector3_f32( p_Line1.p[ 0 ].x, p_Line1.p[ 0 ].y, 0.0f ) + ( da * Vector3_f32( s, s, s ) );
			p_Intersection.x = Intersection3D.x;
			p_Intersection.y = Intersection3D.y;
			
			return BIT_FALSE;
		}
	
		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionLine2Circle( Line2 p_Line, Circle p_Circle )
	{
		BIT_FLOAT32 k = ( p_Line.p[ 0 ].y - p_Line.p[ 1 ].y ) /
						( p_Line.p[ 0 ].x - p_Line.p[ 1 ].x );

		BIT_FLOAT32 m = ( p_Line.p[ 0 ].y - p_Circle.Position.y ) - ( k * p_Line.p[ 0 ].x - p_Circle.Position.x  ); 

		BIT_FLOAT32 a = ( k * k ) + 1;
		BIT_FLOAT32 b = 2.0f * k * m;
		BIT_FLOAT32 c = ( m * m ) - ( p_Circle.Radius * p_Circle.Radius );

		BIT_FLOAT32 z = ( b * b ) - ( 4.0f * a * c );

		// The line missed the circle
		if( z < 0.0f )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_UINT32 IntersectionLine2Circle(	Line2 p_Line, Circle p_Circle,
		Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 )
	{
		BIT_FLOAT32 k = ( p_Line.p[ 0 ].y - p_Line.p[ 1 ].y ) /
						( p_Line.p[ 0 ].x - p_Line.p[ 1 ].x );

		BIT_FLOAT32 m = ( p_Line.p[ 0 ].y - p_Circle.Position.y ) - ( k * p_Line.p[ 0 ].x - p_Circle.Position.x  ); 

		BIT_FLOAT32 a = ( k * k ) + 1;
		BIT_FLOAT32 b = 2.0f * k * m;
		BIT_FLOAT32 c = ( m * m ) - ( p_Circle.Radius * p_Circle.Radius );

		BIT_FLOAT32 z = ( b * b ) - ( 4.0f * a * c );

		// The line missed the circle
		if( z < 0.0f )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionLine2Quad( Line2 p_Line, Quad p_Quad )
	{
		// Low and high coordinates of the quad
		Vector2_f32 QuadLow = p_Quad.GetLowCoords( );
		Vector2_f32 QuadHigh = p_Quad.GetHighCoords( );
		Vector2_f32 Direction = p_Line.GetDirection( );

		// Check if the line is parallel with the quad.
		for( BIT_MEMSIZE i = 0; i < 2; i++ )
		{
			if( Direction[ i ] == 0.0f )
			{
				// Is the line intersecting with the quad.
				if( Direction[ i ] >= QuadLow[ i ] ||
					Direction[ i ] <= QuadHigh[ i ] )
				{
					return BIT_TRUE;
				}
				else
				{
					return BIT_FALSE;
				}
			}
		}

		// So the line isn't axis aligned...

		// Variables from the line's equation: y = kx + m
		BIT_FLOAT32 a = 0.0f;
		BIT_FLOAT32 k = ( p_Line.p[ 0 ].y - p_Line.p[ 1 ].y ) /
						( p_Line.p[ 0 ].x - p_Line.p[ 1 ].x );
		BIT_FLOAT32 m = ( p_Line.p[ 0 ].y ) - ( k * p_Line.p[ 0 ].x ); 
		

		// X low
		a = ( k * QuadLow.x ) + m;
		if( a >= QuadLow.y && a <= QuadHigh.y )
		{
			return BIT_TRUE;
		}

		// X high
		a = ( k * QuadHigh.x ) + m;
		if( a >= QuadLow.y && a <= QuadHigh.y )
		{
			return BIT_TRUE;
		}

		// Y low
		a = ( QuadLow.y - m ) / k;
		if( a >= QuadLow.x && a <= QuadHigh.x )
		{
			return BIT_TRUE;
		}

		// Y high
		a = ( QuadHigh.y - m ) / k;
		if( a >= QuadLow.x && a <= QuadHigh.x )
		{
			return BIT_TRUE;
		}

		return BIT_FALSE;
	}

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

		if (s < 0.0f && s > 1.0f)
		{
			p_Intersection = p_Line1.p[ 0 ] + ( da * Vector3_f32( s, s, s ) );
			return BIT_FALSE;
		}
	
		return BIT_TRUE;
	}

	BIT_API BIT_BOOL IntersectionLine3Sphere( Line3 p_Line, Sphere p_Sphere )
	{
				// Resources: http://stackoverflow.com/questions/6533856/ray-sphere-intersection

		const BIT_FLOAT32 xA = p_Line.p[ 0 ].x;
		const BIT_FLOAT32 yA = p_Line.p[ 0 ].y;
		const BIT_FLOAT32 zA = p_Line.p[ 0 ].z;
		const BIT_FLOAT32 xB = p_Line.p[ 1 ].x;
		const BIT_FLOAT32 yB = p_Line.p[ 1 ].y;
		const BIT_FLOAT32 zB = p_Line.p[ 1 ].z;
		const BIT_FLOAT32 xC = p_Sphere.Position.x;
		const BIT_FLOAT32 yC = p_Sphere.Position.y;
		const BIT_FLOAT32 zC = p_Sphere.Position.z;
		const BIT_FLOAT32 r	 = p_Sphere.Radius;

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

	BIT_API BIT_UINT32 IntersectionLine3Sphere(	Line3 p_Line, Sphere p_Sphere,
		Vector3_f32 & p_Point1, Vector3_f32 & p_Point2 )
	{
		// Resources: http://stackoverflow.com/questions/6533856/ray-sphere-intersection

		const BIT_FLOAT32 xA = p_Line.p[ 0 ].x;
		const BIT_FLOAT32 yA = p_Line.p[ 0 ].y;
		const BIT_FLOAT32 zA = p_Line.p[ 0 ].z;
		const BIT_FLOAT32 xB = p_Line.p[ 1 ].x;
		const BIT_FLOAT32 yB = p_Line.p[ 1 ].y;
		const BIT_FLOAT32 zB = p_Line.p[ 1 ].z;
		const BIT_FLOAT32 xC = p_Sphere.Position.x;
		const BIT_FLOAT32 yC = p_Sphere.Position.y;
		const BIT_FLOAT32 zC = p_Sphere.Position.z;
		const BIT_FLOAT32 r	 = p_Sphere.Radius;

		const BIT_FLOAT32 a = ( (xB-xA) * (xB-xA) ) + ( (yB-yA) * (yB-yA) ) + ( (zB-zA) * (zB-zA) );
		const BIT_FLOAT32 b = 2.0f *( (xB-xA) * (xA-xC) + (yB-yA) * (yA-yC) + (zB-zA) * (zA-zC) );
		const BIT_FLOAT32 c = ( (xA-xC) * (xA-xC) ) + ( (yA-yC) * (yA-yC) ) + ( (zA-zC) * (zA-zC) ) - ( r * r );

		// Get the delta( data under the root
		const BIT_FLOAT32 Delta = ( b * b ) - ( 4.0f * a * c );

		// No intersection at all
		if( Delta < 0.0f )
		{
			return 0;
		}

		// Find the intersection points
		BIT_FLOAT32 DeltaSqrt = sqrt( Delta );
	
		// Full equation 1
		BIT_FLOAT32 u1 = ( -b - DeltaSqrt ) / ( 2.0f * a );

		// Set the first interpolation point
		p_Point1.x = xA + ( u1 * ( xB - xA ) );
		p_Point1.y = yA + ( u1 * ( yB - yA ) );
		p_Point1.z = zA + ( u1 * ( zB - zA ) );

		// Are we just getting 1 intersection point?
		if( EqualEpsilon( Delta, 0.0f ) )
		{
			return 1;
		}

		// The line is going through 2 points
		// Full equation 2
		BIT_FLOAT32 u2 = ( -b + DeltaSqrt ) / ( 2.0f * a );

		// Set the first interpolation point
		p_Point2.x = xA + ( u2 * ( xB - xA ) );
		p_Point2.y = yA + ( u2 * ( yB - yA ) );
		p_Point2.z = zA + ( u2 * ( zB - zA ) );

		return 2;
	}
/*
	BIT_API BIT_BOOL IntersectionLine3Box( Line3 p_Line, Box p_Box )
	{
		return BIT_FALSE;
	}*/

	// Circle
	BIT_API BIT_SINT32 IntersectionCircleCircle( Circle p_Circle1, Circle p_Circle2 )
	{
		BIT_FLOAT32 Distance = Vector2_f32( p_Circle1.Position - p_Circle2.Position ).Magnitude( );

		// No solutions
		if( Distance > ( p_Circle1.Radius + p_Circle2.Radius ) )
		{
			return -1;
		}

		// Inside each other but no solutions
		if( Distance < abs( p_Circle1.Radius - p_Circle2.Radius ) )
		{
			return 0;
		}
	
		// Infinite numbers of solutions
		if( ( Distance == 0 ) && ( p_Circle1.Radius == p_Circle2.Radius ) )
		{
			return 0;
		};

		// One solution
		if( Distance == p_Circle1.Radius + p_Circle2.Radius )
		{
			return 1;
		}

		// Two solutions
		return 2;
	}

	BIT_API BIT_SINT32 IntersectionCircleCircle( Circle p_Circle1, Circle p_Circle2,
		Vector2_f32 & p_Point1, Vector2_f32 & p_Point2 )
	{
		BIT_FLOAT32 Distance = Vector2_f32( p_Circle1.Position - p_Circle2.Position ).Magnitude( );

		// No solutions
		if( Distance > ( p_Circle1.Radius + p_Circle2.Radius ) )
		{
			return -1;
		}

		// Inside each other but no solutions
		if( Distance < abs( p_Circle1.Radius - p_Circle2.Radius ) )
		{
			return 0;
		}
	
		// Infinite numbers of solutions
		if( ( Distance == 0 ) && ( p_Circle1.Radius == p_Circle2.Radius ) )
		{
			return 0;
		};

		// Tiangle width
		BIT_FLOAT32 A = (	( p_Circle1.Radius * p_Circle1.Radius ) -
							( p_Circle2.Radius * p_Circle2.Radius ) +
							( Distance * Distance ) ) /
							( 2.0f * Distance );
		
		// Triangle Height
		BIT_FLOAT32 H = sqrt( ( p_Circle1.Radius * p_Circle1.Radius ) - ( A * A ) );

		// Corner vector
		Vector2_f32 Corner = p_Circle1.Position +
			(( p_Circle2.Position - p_Circle1.Position ) * A ) / Distance;

		// One solution
		if( Distance == p_Circle1.Radius + p_Circle2.Radius )
		{
			p_Point1 = Corner;
			return 1;
		}

		// Two solutions
		p_Point1 = Vector2_f32( Corner.x + ( H * ( p_Circle2.Position.y - p_Circle1.Position.y ) / Distance ),
								Corner.y - ( H * ( p_Circle2.Position.x - p_Circle1.Position.x ) / Distance ) );

		p_Point2 = Vector2_f32( Corner.x - ( H * ( p_Circle2.Position.y - p_Circle1.Position.y ) / Distance ),
								Corner.y + ( H * ( p_Circle2.Position.x - p_Circle1.Position.x ) / Distance ) );

		return 2;
	}

	BIT_API BIT_BOOL IntersectionCircleQuad( Circle p_Circle, Quad p_Quad )
	{
		// Is the center of the circle inside the quad?
		if( IntersectionPoint2Quad( p_Circle.Position, p_Quad ) )
		{
			return BIT_TRUE;
		}

		Vector2_f32 QuadLow = p_Quad.GetLowCoords( );
		Vector2_f32 QuadHigh = p_Quad.GetHighCoords( );

		// Get the closest coordinate ( circle origin - quad boarder )
		Vector2_f32 Closest;
		Closest.x = Clamp< BIT_FLOAT32 >( p_Circle.Position.x, QuadLow.x, QuadHigh.x );
		Closest.y = Clamp< BIT_FLOAT32 >( p_Circle.Position.y, QuadLow.y, QuadHigh.y );

		// Calculate the distance
		BIT_FLOAT32 Distance = Vector2_f32( p_Circle.Position - Closest ).Magnitude( );

		if( Distance > p_Circle.Radius )
		{
			return BIT_FALSE;
		}

		return BIT_TRUE;
	}

	// Quad
	BIT_API BIT_BOOL IntersectionQuadQuad( Quad p_Quad1, Quad p_Quad2 )
	{
		// Pre-calculate all the points in the first quad
		Vector2_f32 QuadPoints[ 4 ] =
		{
				p_Quad1.Positsion - ( p_Quad1.Size / 2.0f ),
				Vector2_f32(	p_Quad1.Positsion.x + ( p_Quad1.Size.x / 2.0f ),
								p_Quad1.Positsion.y - ( p_Quad1.Size.y / 2.0f ) ),
				p_Quad1.Positsion + ( p_Quad1.Size / 2.0f ),
				Vector2_f32(	p_Quad1.Positsion.x - ( p_Quad1.Size.x / 2.0f ),
								p_Quad1.Positsion.y + ( p_Quad1.Size.y / 2.0f ) )
		};

		// Calculate the low and high values of the bound box we are testing all the points against.
		Vector2_f32 Low(	p_Quad2.Positsion - ( p_Quad2.Size / 2.0f )	);
		Vector2_f32 High(	p_Quad2.Positsion + ( p_Quad2.Size / 2.0f )	);

		// Go through all the points and check if any of them are inside the bounding box.
		for( BIT_MEMSIZE i = 0; i < 4; i++ )
		{
			if( QuadPoints[ i ].x <= High.x && QuadPoints[ i ].x >= Low.x &&
				QuadPoints[ i ].y <= High.y && QuadPoints[ i ].y >= Low.y )
			{
				return BIT_TRUE;
			}
		}

		return BIT_FALSE;
	}

	// Sphere
	BIT_API BIT_SINT32 IntersectionSphereSphere( Sphere p_Sphere1, Sphere p_Sphere2 )
	{
		BIT_FLOAT32 Distance = Vector3_f32( p_Sphere1.Position - p_Sphere2.Position ).Magnitude( );

		// No solutions
		if( Distance > ( p_Sphere1.Radius + p_Sphere2.Radius ) )
		{
			return -1;
		}

		// Inside each other but no solutions
		if( Distance < abs( p_Sphere1.Radius - p_Sphere2.Radius ) )
		{
			return 0;
		}
	
		// Infinite numbers of solutions
		if( ( Distance == 0 ) && ( p_Sphere1.Radius == p_Sphere2.Radius ) )
		{
			return 0;
		};

		// One solution
		if( Distance == p_Sphere1.Radius + p_Sphere2.Radius )
		{
			return 1;
		}

		// Two solutions
		return 2;
	}

	/*BIT_API BIT_BOOL IntersectionSphereBox(		Sphere p_Sphere,	Box p_Box );
*/
	// Box
	BIT_API BIT_BOOL IntersectionBoxBox( Box p_Box1, Box p_Box2 )
	{
		// Pre-calculate all the points in the first quad
		Vector3_f32 BoxPoints[ 8 ] =
		{
			p_Box1.Positsion - ( p_Box1.Size / 2.0f ),

			Vector3_f32(	p_Box1.Positsion.x + ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y - ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z - ( p_Box1.Size.z / 2.0f )	),

			Vector3_f32(	p_Box1.Positsion.x + ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y + ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z - ( p_Box1.Size.z / 2.0f )	),

			Vector3_f32(	p_Box1.Positsion.x - ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y + ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z - ( p_Box1.Size.z / 2.0f )	),



			Vector3_f32(	p_Box1.Positsion.x - ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y - ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z + ( p_Box1.Size.z / 2.0f )	),

			Vector3_f32(	p_Box1.Positsion.x + ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y - ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z + ( p_Box1.Size.z / 2.0f )	),

			p_Box1.Positsion + ( p_Box1.Size / 2.0f ),

			Vector3_f32(	p_Box1.Positsion.x - ( p_Box1.Size.x / 2.0f ),
							p_Box1.Positsion.y + ( p_Box1.Size.y / 2.0f ),
							p_Box1.Positsion.z + ( p_Box1.Size.z / 2.0f )	)

		};

		// Calculate the low and high values of the bound box we are testing all the points against.
		Vector3_f32 Low(	p_Box2.Positsion - ( p_Box2.Size / 2.0f )	);
		Vector3_f32 High(	p_Box2.Positsion + ( p_Box2.Size / 2.0f )	);

		// Go through all the points and check if any of them are inside the bounding box.
		for( BIT_MEMSIZE i = 0; i < 8; i++ )
		{
			if( BoxPoints[ i ].x <= High.x && BoxPoints[ i ].x >= Low.x &&
				BoxPoints[ i ].y <= High.y && BoxPoints[ i ].y >= Low.y &&
				BoxPoints[ i ].z <= High.z && BoxPoints[ i ].z >= Low.z )
			{
				return BIT_TRUE;
			}
		}

		return BIT_FALSE;
	}

	


	// Quadratic equation solver
	BIT_API BIT_UINT32 QuadraticEquation( const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B,
		const BIT_FLOAT32 p_C, BIT_FLOAT32 & p_X1, BIT_FLOAT32 & p_X2 )
	{
		// Returning the numbers of roots

		// Root data
		BIT_FLOAT32 z = ( p_B * p_B ) - ( 4.0f * p_A * p_C );

		// Roots were found
		if( z < 0.0f )
		{
			return 0;
		}
		else if( z == 0.0f )
		{
			p_X1 = -p_B / ( 2.0f * p_A );
			return 1;
		}
		
		// Else if we have 2 roots
		BIT_FLOAT32 Root = sqrt( z );
		p_X1 = (-p_B - Root) / ( 2.0f * p_A );
		p_X2 = (-p_B + Root) / ( 2.0f * p_A );

		return 2;
	}

}