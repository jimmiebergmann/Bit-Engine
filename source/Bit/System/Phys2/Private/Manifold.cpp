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
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

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
			typedef void (Manifold::*CollisionFunctionPointer)( );
			CollisionFunctionPointer g_CollisionJumpTable[ 1 ][ 1 ] =
			{
				&Manifold::CircleToCircle
			};

		
			// Manifold class
			Manifold::Manifold( Body * p_pBodyA, Body * p_pBodyB ) :
				m_pBodyA( p_pBodyA ),
				m_pBodyB( p_pBodyB ),
				m_ContactCount( 0 ),
				m_Contact( 0.0f, 0.0f),	
				m_Normal( 0.0f, 0.0f),
				m_Penetration( 0.0f )
			{

			}

			void Manifold::Solve( )
			{
				// Call the collision function from jumptable.
				(this->*g_CollisionJumpTable[ m_pBodyA->GetShape( ).GetType( ) ][ m_pBodyB->GetShape( ).GetType( ) ])( );
			}

			void Manifold::ApplyImpulse( )
			{
				// Compute velocity

				// Compute  radius from COM to contact point
				Vector2f32 rA = m_Contact - m_pBodyA->m_Position;
				Vector2f32 rB = m_Contact - m_pBodyB->m_Position;

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

			void Manifold::CircleToCircle( )
			{
				// Get the shapes
				Circle * pA = reinterpret_cast<Circle *>( m_pBodyA->m_pShape );
				Circle * pB = reinterpret_cast<Circle *>( m_pBodyB->m_pShape );

				// Compute the normal between the circles
				Vector2f32 normal = m_pBodyB->m_Position - m_pBodyA->m_Position;

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
					m_Contact = m_pBodyA->m_Position;
				}
				else // The circles are not on the same positions
				{
					m_Penetration = radius - distance;
					m_Normal = normal / distance;
					m_Contact = ( m_Normal * pA->m_Radius ) + m_pBodyA->m_Position;
				}
			}

		}

	}

}