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

#include <Bit/System/Phys2/Scene.hpp>
#include <Bit/System/Phys2/Body.hpp>
#include <Bit/System/Phys2/Shape.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Phys2
	{
		Scene::Scene( const Vector2f32 & p_Gravity ) :
			m_Gravity( p_Gravity )
		{
		}

		Scene::~Scene( )
		{
			Clear( );
		}

		void Scene::Step(	const Time & p_StepTime,
							const Uint32 p_VelocityIterations,
							const Uint32 p_PositionIterations )
		{
			// Store intersecting bodies
			ManifoldVector contacts;

			// Go through all the bodies
			for( SizeType i = 0; i < m_Bodies.size( ); i++ )
			{
				Body * pA = m_Bodies[ i ];
				
				// Go through all the bodies once again
				for( SizeType j = i + 1; j < m_Bodies.size( ); j++ )
				{
					Body * pB = m_Bodies[ j ];

					// Ignore bodies with infinity mass
					if( pA->m_MassInverse == 0.0f && pB->m_MassInverse == 0.0f )
					{
						continue;
					}

					// Create a manifold
					Private::Manifold * pManifold = new Private::Manifold( pA, pB );

					// Check if the bodies intersects with each other
					pManifold->Solve( );

					// Check if the bodies intersects with each other
					if( pManifold->m_ContactCount )
					{
						// Store the manifold if the bodies are intersecting
						contacts.push_back( pManifold );
					}
					else
					{
						// Delete the manifold if the intersection tests failed
						delete pManifold;
					}
				}
			}

			// Apply forces on all bodies
			for( SizeType i = 0; i < m_Bodies.size( ); i++ )
			{
				ApplyForces( m_Bodies[ i ], p_StepTime );
			}

			// Apply impulse to contacting bodies(solve collisions)
			
			for( Uint32 i = 0; i < p_VelocityIterations; i++ )
			{
				for( SizeType j = 0; j < contacts.size( ); j++ )
				{
					contacts[ j ]->ApplyImpulse( );
				}
			}

			// Compute positions for all bodies
			Float32 inversePositionIterations = 1.0f / static_cast<Float32>( p_PositionIterations );
			for( Uint32 i = 0; i < p_PositionIterations; i++ )
			{
				for( SizeType i = 0; i < m_Bodies.size( ); i++ )
				{
					ComputePosition( m_Bodies[ i ], p_StepTime, inversePositionIterations );
				}
			
				// Corrent positions
				for( SizeType j = 0; j < contacts.size( ); j++ )
				{
					contacts[ j ]->PositionalCorrection( inversePositionIterations );
				}
			}

			// Clear all forces on the bodies
			for( SizeType i = 0; i < m_Bodies.size( ); i++ )
			{
				m_Bodies[ i ]->m_Force = Vector2f32( 0.0f, 0.0f );
				m_Bodies[ i ]->m_Torque = 0.0f;
			}

			// Delete all the contacts
			for( SizeType i = 0; i < contacts.size( ); i++ )
			{
				delete contacts[ i ];
			}
			contacts.clear( );

		}

		Body * Scene::Add( Shape * p_pShape, const Vector2f32 & p_Position, const Material & p_Material )
		{
			// Error check the shape pointer
			if( p_pShape == NULL )
			{
				return NULL;
			}

			// Create the body
			Body * pBody = new Body( this, p_pShape, p_Position, p_Material );

			// Add the body to the body list
			m_Bodies.push_back( pBody );

			// Return the body
			return pBody;
		}
			
		void Scene::Remove( Body * p_pBody )
		{
			for( BodyVector::iterator it = m_Bodies.begin( ); it != m_Bodies.end( ); it++ )
			{
				if( p_pBody == *it )
				{
					m_Bodies.erase( it );
					break;
				}
			}
			
		}

		void Scene::Clear( )
		{
			// Delete all the bodies.
			for( SizeType i = 0; i < m_Bodies.size( ); i++ )
			{
				delete  m_Bodies[ i ];
			}

			// Clear the body list.
			m_Bodies.clear( );
		}

		void Scene::SetGravity( const Vector2f32 & p_Gravity )
		{
			m_Gravity = p_Gravity;
		}
			
		Vector2f32 Scene::GetGravity( ) const
		{
			return m_Gravity;
		}

		void Scene::ComputePosition( Body * p_pBody, const Time & p_StepTime, const Float32 p_InverseIterations )
		{
			if( p_pBody->m_MassInverse == 0.0f )
			{
				return;
			}

			// Compute position
			p_pBody->m_Position += p_pBody->m_Velocity * static_cast<Float32>( p_StepTime.AsSeconds( ) ) * p_InverseIterations;

			// Compute orientation
			p_pBody->m_Orient += p_pBody->m_AngularVelocity * static_cast<Float32>( p_StepTime.AsSeconds( ) ) * p_InverseIterations; 
			/*if( p_pBody->m_Orient >= Pi2Float32 )
			{
				p_pBody->m_Orient -= Pi2Float32;
			}
			else if( p_pBody->m_Orient < 0.0f )
			{
				p_pBody->m_Orient += Pi2Float32;
			}*/

		}

		void Scene::ApplyForces( Body * p_pBody, const Time & p_StepTime )
		{
			if( p_pBody->m_MassInverse == 0.0f )
			{
				return;
			}

			p_pBody->m_Velocity += p_pBody->m_Force * p_pBody->m_MassInverse;
			p_pBody->m_Velocity += m_Gravity * static_cast<Float32>( p_StepTime.AsSeconds( ) );
			p_pBody->m_AngularVelocity += p_pBody->m_Torque * p_pBody->m_InertiaInverse;
		}

	}

}