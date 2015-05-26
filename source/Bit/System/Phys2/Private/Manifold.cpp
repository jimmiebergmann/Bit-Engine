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

#include <Bit/System/Phys2/Private/Manifold.hpp>
#include <Bit/System/Phys2/Body.hpp>
#include <algorithm>
#include <limits>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

#define Proj( a, b ) a + b

namespace Bit
{

	namespace Phys2
	{

		namespace Private
		{

			static Vector2f32 Cross( const Float32 a, const Vector2f32 & v )
			{
			  return Vector2f32( -a * v.y, a * v.x );
			}

			static Float32 Cross( const Vector2f32 & a, const Vector2f32& b )
			{
				return a.x * b.y - a.y * b.x;
			}

			// Create a jump table for collison checks, based on the shape type.
			typedef void (Manifold::*CollisionFunctionPointer)( Body *, Body * );
			CollisionFunctionPointer g_CollisionJumpTable[ 2 ][ 2 ] =
			{
				{ &Manifold::CircleToCircle, &Manifold::CircleToRectangle },
				{ &Manifold::RectangleToCircle, &Manifold::RectangleToRectangle }
			};

		
			// Manifold class
			Manifold::Manifold( Body * p_pBodyA, Body * p_pBodyB ) :
				m_pBodyA( p_pBodyA ),
				m_pBodyB( p_pBodyB ),
				m_ContactCount( 0 ),
				m_Normal( 0.0f, 0.0f),
				m_Penetration( 0.0f )
			{
			}

			void Manifold::Solve( )
			{
				// Call the collision function from jumptable.
				(this->*g_CollisionJumpTable[ m_pBodyA->GetShape( ).GetType( ) ][ m_pBodyB->GetShape( ).GetType( ) ])( m_pBodyA, m_pBodyB );
			}

			void Manifold::ApplyImpulse( )
			{
				for( Uint32 i = 0; i < m_ContactCount; i++ )
				{

					// Compute velocity

					// Compute  radius from COM to contact point
					Vector2f32 rA = m_Contacts[ i ] - m_pBodyA->m_Position;
					Vector2f32 rB = m_Contacts[ i ] - m_pBodyB->m_Position;

					// Compute relative velocity
					Vector2f32 rv = m_pBodyB->m_Velocity + Cross( m_pBodyB->m_AngularVelocity, rB ) -
									m_pBodyA->m_Velocity - Cross( m_pBodyA->m_AngularVelocity, rA );

					// Calculate velocity along the normal
					Float32 velAlongNormal = static_cast<Float32>( Vector2f32::Dot( rv, m_Normal ) );

					// Do not apply impulse if the velocitites are separating
					if( velAlongNormal > 0.0f )
					{
						return;
					}

					// Calcualte the restitution
					Float32 e = std::min( m_pBodyA->m_Material.m_Restitution, m_pBodyB->m_Material.m_Restitution );


					// Calculate the inv mass sum
					Float32 raCrossN = Cross( rA, m_Normal );
					raCrossN *= raCrossN;
					Float32 rbCrossN = Cross( rB, m_Normal );
					rbCrossN *= rbCrossN;

					Float32 invMassSum =	m_pBodyA->m_MassInverse + m_pBodyB->m_MassInverse +
											( raCrossN * m_pBodyA->m_InertiaInverse )  +
											( rbCrossN * m_pBodyB->m_InertiaInverse );

					// Calculate the impulse scalar
					Float32 j = -( 1.0f + e ) * velAlongNormal;
					j /= invMassSum;
					j /= static_cast<Float32>( m_ContactCount );

					// Apply impulse to bodies
					Vector2f32 impulse = m_Normal * j;
					m_pBodyA->ApplyImpulse( -impulse, rA );
					m_pBodyB->ApplyImpulse( impulse, rB );


					// Compute friction
				
					// Compute relative velocity once again
					rv =	m_pBodyB->m_Velocity + Cross( m_pBodyB->m_AngularVelocity, rB ) -
							m_pBodyA->m_Velocity - Cross( m_pBodyA->m_AngularVelocity, rA );

					// Compute the tangent vector
					Vector2f32 tangent = rv - ( m_Normal * Vector2f32::Dot( rv, m_Normal ) );
					tangent.Normalize( );

					// Compute magnitude to apply along the friction vector
					Float32 jt = -static_cast<Float32>( Vector2f32::Dot( rv, tangent ) );
					jt /= invMassSum;

					// Make sure jt isn't invalid.
					if( Math::EqualEpsilon<Float32>( jt, 0.0f ) )
					{
						return;
					}

					// Compute static and dynamic friction
					Float32 sf = std::sqrt( m_pBodyA->m_Material.m_StaticFriction * m_pBodyB->m_Material.m_StaticFriction );
					Float32 df = std::sqrt( m_pBodyA->m_Material.m_DynamicFriction * m_pBodyB->m_Material.m_DynamicFriction );

					// Compute friction impulse
					Vector2f32 frictionImpulse;
					if( std::abs( jt ) < j * sf )
					{ 
						frictionImpulse = tangent * jt;
					}
					else
					{
						frictionImpulse = tangent * (-j) * df;
					}

					// Apply friction impulse to bodies
					m_pBodyA->ApplyImpulse( -frictionImpulse, rA );
					m_pBodyB->ApplyImpulse( frictionImpulse, rB );
					/*
					// Add fritction in the inverse direction of velocity
					m_pBodyB->m_Velocity += m_pBodyB->m_Velocity.Normal( ) * df;
					m_pBodyB->m_Velocity += m_pBodyB->m_Velocity.Normal( ) * df;
					*/
				}
			}

			void Manifold::PositionalCorrection( const Float32 p_InverseIterations )
			{
				const Float32 percent = 0.4f;	// Penetration percentage to corrent.
				const Float32 slop = 0.01f;		// Penetration allowance.
				Vector2f32 correction = m_Normal * ( std::max( m_Penetration - slop,  0.0f ) / ( m_pBodyA->m_MassInverse + m_pBodyB->m_MassInverse ) ) * percent * p_InverseIterations;

				// Correct the positions
				m_pBodyA->m_Position -= correction * m_pBodyA->m_MassInverse;
				m_pBodyB->m_Position += correction * m_pBodyB->m_MassInverse;
			}

			void Manifold::CircleToCircle( Body * p_pBodyA, Body * p_pBodyB )
			{
				// Get the shapes
				Circle * pA = reinterpret_cast<Circle *>( p_pBodyA->m_pShape );
				Circle * pB = reinterpret_cast<Circle *>( p_pBodyB->m_pShape );

				// Compute the normal between the circles
				Vector2f32 normal = p_pBodyB->m_Position - p_pBodyA->m_Position;

				// Compute the radiuses and distance
				Float32 radius = pA->m_Radius + pB->m_Radius;
				Float32 distance = static_cast<Float32>( normal.Length( ) );

				// No collision if the distance is larger than the radiuses
				if( distance > radius )
				{
					m_ContactCount = 0;
					return;
				}

				// The circles are intersecting, set contact count to 1
				m_ContactCount = 1;

				// The circles are on the same position
				if( distance == 0.0f )
				{
					m_Penetration = radius;
					m_Normal = Vector2f32( 1.0f, 0.0f );
					m_Contacts[ 0 ] = p_pBodyA->m_Position;
				}
				else // The circles are not on the same positions
				{
					m_Penetration = radius - distance;
					m_Normal = normal / distance;
					m_Contacts[ 0 ] = ( m_Normal * pA->m_Radius ) + p_pBodyA->m_Position;
				}
			}

			static Float32 Clip( Float32 p_Value, Float32 p_Lower, Float32 p_Upper)
			{
				return std::max( p_Lower, std::min( p_Value, p_Upper ) );
			}

			void Manifold::CircleToRectangle( Body * p_pCircle, Body * p_pRectangle )
			{
				// Get the shapes
				Circle * pCircleShape = reinterpret_cast<Circle *>( p_pCircle->m_pShape );
				Rectangle * pRectShape = reinterpret_cast<Rectangle *>( p_pRectangle->m_pShape );

				// Get min and max rectangle values.
				Vector2f32 min = Vector2f32( -pRectShape->m_Size.x / 2.0, -pRectShape->m_Size.y / 2.0 ) + p_pRectangle->m_Position; 
				Vector2f32 max = Vector2f32( pRectShape->m_Size.x / 2.0, pRectShape->m_Size.y / 2.0 ) + p_pRectangle->m_Position; 

				// Make the circle relative to the rectangle in AABB space.
				Vector2f32 circlePositionRot = p_pCircle->m_Position - p_pRectangle->m_Position;
				circlePositionRot.Rotate( Radians( -p_pRectangle->m_Orient ) );
				circlePositionRot += p_pRectangle->m_Position;

				// Get the clamped circle position
				Vector2f32 pointRot(	Clip( circlePositionRot.x, min.x, max.x ),
										Clip( circlePositionRot.y, min.y, max.y ) );

				// Get the circle to clamped circle position, and length.
				Vector2f32 circleToPointRot = pointRot - circlePositionRot;
				Float32 distance = static_cast<Float32>( circleToPointRot.Length( ) );

				// There's no collision
				if( distance > pCircleShape->m_Radius )
				{
					m_ContactCount = 0;
					return;
				}
				
				// Collided.
				m_ContactCount = 1;

				// Compute the real contact point
				Vector2f32 point = pointRot - p_pRectangle->m_Position;
				point.Rotate( Radians( p_pRectangle->m_Orient ) );
				point += p_pRectangle->m_Position; 

				// Get the circle to clamped circle position, and length.
				Vector2f32 circleToPoint = point - p_pCircle->m_Position;

				// The circles origin is inside the box.
				if( distance == 0.0f )
				{
					m_Penetration = pCircleShape->m_Radius;
					m_Normal = Vector2f32( 1.0f, 0.0f );
					m_Contacts[ 0 ] = p_pCircle->m_Position;
				}
				else
				{
					m_Penetration = pCircleShape->m_Radius - distance;
					m_Normal = circleToPoint.Normal( );
					m_Contacts[ 0 ] = point;
				}
				
			}

			void Manifold::RectangleToCircle( Body * p_pBodyA, Body * p_pBodyB )
			{
				CircleToRectangle( p_pBodyB, p_pBodyA );
				m_Normal = -m_Normal;
			}


			static const Vector2f32 rectCorners[ 4 ] =
			{
				Vector2f32(-0.5f, -0.5f), Vector2f32(0.5f, -0.5f),
				Vector2f32(0.5f, 0.5f), Vector2f32(-0.5f, 0.5f)
			};

			static const Vector2f32 rectNormals[ 4 ] =
			{
				Vector2f32(  0.0f, -1.0f ),
				Vector2f32(  1.0f,  0.0f ),
				Vector2f32(  0.0f,  1.0f ),
				Vector2f32( -1.0f,  0.0f )
			};

			// Return the distance
			Float32 Manifold::FindFaceLeastPenetration(Uint32 * p_pFaceIndex, Body * p_pBodyA, Body * p_pBodyB)
			{
				// Get the shapes, [ shape ]
				const Rectangle * pShapes[2] =
				{
					reinterpret_cast<Rectangle *>(p_pBodyA->m_pShape),
					reinterpret_cast<Rectangle *>(p_pBodyB->m_pShape)
				};

				Float32 bestDistance = -std::numeric_limits<Float32>::max( );

				// Go through Body A's corners
				for ( Uint32 i = 0; i < 4; i++ )
				{
					// Get face normal of A
					Vector2f32 n = rectNormals[ i ];
					n.Rotate( Radians( p_pBodyA->m_Orient ) );

					// Turn face normal of A into B's model space.
					n.Rotate(Radians(-p_pBodyB->m_Orient));

					// Get extreme point
					const Vector2f32 s = pShapes[ 1 ]->GetExtremePoint( -n );


					Vector2f32 v = rectCorners[ i ] * pShapes[ 0 ]->m_Size;
					v.Rotate( Radians( p_pBodyA->m_Orient ) );
					v += p_pBodyA->m_Position;
					v -= p_pBodyB->m_Position;
					v.Rotate(Radians(-p_pBodyB->m_Orient));

					// Compute the penetration distance
					const Float32 d = static_cast<Float32>( Vector2f32::Dot(n, s - v) );

					if (d > bestDistance)
					{
						bestDistance = d;
						*p_pFaceIndex = i;
					}

				}

				return bestDistance;
			}

			void Manifold::RectangleToRectangle( Body * p_pBodyA, Body * p_pBodyB )
			{
				// Store bodies in array, [ body ]
				const Body * pBodies[ 2 ] = { p_pBodyA, p_pBodyB };

				// Get the shapes, [ shape ]
				const Rectangle * pShapes[ 2 ] = 
				{ 
					reinterpret_cast<Rectangle *>( p_pBodyA->m_pShape ),
					reinterpret_cast<Rectangle *>( p_pBodyB->m_pShape )
				};

				// Set contact count to 0.
				m_ContactCount = 0;

				// Get the first penetration
				Uint32 faceA = 0;
				Float32 penetrationA = FindFaceLeastPenetration(&faceA, p_pBodyA, p_pBodyB);
				if (penetrationA >= 0.0f)
				{
					return;
				}

				// Get the seconds penetration
				Uint32 faceB = 0;
				Float32 penetrationB = FindFaceLeastPenetration(&faceB, p_pBodyB, p_pBodyA);
				if (penetrationB >= 0.0f)
				{
					return;
				}

				// Find collision face
				Uint32 refIndex;
				Bool flip;
				Body * pRefBody;
				Body * pCollisionBody;

				if (penetrationA > penetrationB)
				{
					refIndex = faceA;
					flip = false;
					pRefBody = p_pBodyA;
					pCollisionBody = p_pBodyB;
				}
				else
				{
					refIndex = faceB;
					flip = true;
					pRefBody = p_pBodyB;
					pCollisionBody = p_pBodyA;
				}


/*
				// Get the rectangle sizes, [ body ]
				const Vector2f32 sizes[ 2 ] = { pShapes[ 0 ]->m_Size, pShapes[ 1 ]->m_Size };

				// Get the shape corners in model space, [ shape ][ corner ]
				Vector2f32 corners[ 2 ][ 4 ] =
				{
					{	Vector2f32( -sizes[ 0 ].x / 2.0f, -sizes[ 0 ].y / 2.0f ), Vector2f32( sizes[ 0 ].x / 2.0f, -sizes[ 0 ].y / 2.0f ),
						Vector2f32( sizes[ 0 ].x / 2.0f, sizes[ 0 ].y / 2.0f ), Vector2f32( -sizes[ 0 ].x / 2.0f, sizes[ 0 ].y / 2.0f ) 
					},
					{	Vector2f32( -sizes[ 1 ].x / 2.0f, -sizes[ 1 ].y / 2.0f ), Vector2f32( sizes[ 1 ].x / 2.0f, -sizes[ 1 ].y / 2.0f ),
						Vector2f32( sizes[ 1 ].x / 2.0f, sizes[ 1 ].y / 2.0f ), Vector2f32( -sizes[ 1 ].x / 2.0f, sizes[ 1 ].y / 2.0f ) 
					}
				};

				// Rotate the corners and turn the corners in world space.
				for( SizeType b = 0; b < 2; b++ )
				{
					for( SizeType c = 0; c < 4; c++ )
					{
						corners[ b ][ c ].Rotate( Radians( pBodies[ b ]->m_Orient ) );
					}
				}

				// Make the position of the corners relative to each other
				Vector2f32 rPos = p_pBodyB->m_Position - p_pBodyA->m_Position;
				
				for( SizeType c = 0; c < 4; c++ )
				{
					corners[ 1 ][ c ] += rPos;
				}

				// Get the rectangle normals, [ shape ][ normal ]
				Vector2f32 normals[ 2 ][ 2 ] =
				{
					{ Vector2f32( 0.0f, 1.0f ), Vector2f32( 1.0f, 0.0f ) },
					{ Vector2f32( 0.0f, 1.0f ), Vector2f32( 1.0f, 0.0f ) }
				};

				// Rotate the normals
				for( SizeType b = 0; b < 2; b++ )
				{
					normals[ b ][ 0 ].Rotate( Radians( pBodies[ b ]->m_Orient ) );
					normals[ b ][ 1 ].Rotate( Radians( pBodies[ b ]->m_Orient ) );
				}

				// Create normals corner indices, [ normal ][ corners(just 2 needed ]
				const SizeType cornerIndices[ 2 ][ 2 ] =
				{
					{ 1, 2 },
					{ 0, 1 }
				};

				// Go through the bidies normals and check if the corners overlap each other.
				for( SizeType b = 0; b < 2; b++ )
				{
					// Go through the current bodys normals
					for( SizeType n = 0; n < 2; n++ )
					{
						// Store min and max value of the scalar, [ body ]
						Float32 min[ 2 ] = { std::numeric_limits<Float32>::max( ), std::numeric_limits<Float32>::max( ) };
						Float32 max[ 2 ] = { -std::numeric_limits<Float32>::max( ), -std::numeric_limits<Float32>::max( ) };

						// Project the corners( just 2, use the corner indices ) on the current normal
						const Vector2f32 currentNormal = normals[ b ][ n ];

						// Go through the corners of both bodies and all the corners
						for( SizeType bb = 0; bb < 2; bb++ )
						{
							for( SizeType c = 0; c < 4; c++ )
							{
								const Vector2f32 curCorner = corners[ bb ][ c ];
								const Float32 scalar = static_cast<Float32>( curCorner.Dot( currentNormal ) / currentNormal.Length( ) );

								// Store the scalar in the min/max array.
								if( scalar < min[ bb ] )
								{
									min[ bb ] = scalar;
								}
								if( scalar > max[ bb ] )
								{
									max[ bb ] = scalar;
								}
							}
						}

						// Check if there's not any collision
						if( max[ 0 ] < min[ 1 ] || min[ 0 ] > max[ 1 ] )
						{
							// Set the contact points to 0
							m_ContactCount = 0;
							return;
						}
					}
				}

				// Set the contact points to 1
				m_ContactCount = 1;
				*/

				// Compute contact points, penetration and normal.

				
			}

		}

	}

}