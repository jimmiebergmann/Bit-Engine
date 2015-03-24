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

		void Scene::Step( const Time & p_StepTime, const Uint32 p_Iterations )
		{
			// Store intersecting bodies
			ManifoldVector contacts;

			// Go through all the bodies
			//for( SizeType i = 0; i < m_Bodies.size( ); i++ )
			for( BodyList::iterator it1 = m_Bodies.begin( ); it1 != m_Bodies.end( ); it1++ )
			{
				Body * pA = *it1;

				// Go through all the bodies once again
				for( BodyList::iterator it2 = m_Bodies.begin( ); it2 != m_Bodies.end( ); it2++ )
				{
					Body * pB = *it2;

					// Ignore checks against the same body
					if( pA == pB )
					{
						continue;
					}

					// Ignore bodies with infinity mass
					if( pA->m_MassInverse == 0.0f || pB->m_MassInverse == 0.0f )
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
			for( BodyList::iterator it = m_Bodies.begin( ); it != m_Bodies.end( ); it++ )
			{
				ApplyForces( *it, p_StepTime );
				//Body * pA = *it1;
			}

			// Apply impulse to contacting bodies(solve collisions)
			//for( SizeType i = 0; i < p_Iterations; i++ )
			//{
			for( SizeType j = 0; j < contacts.size( ); j++ )
			{
				contacts[ j ]->ApplyImpulse( );
			}
			//}

			// Compute positions for all bodies
			for( BodyList::iterator it = m_Bodies.begin( ); it != m_Bodies.end( ); it++ )
			{
				//Body * pA = *it1;
				ComputePosition( *it, p_StepTime );
			}
			
			// Corrent positions
			// ...

			// Clear all forces on the bodies
			for( BodyList::iterator it = m_Bodies.begin( ); it != m_Bodies.end( ); it++ )
			{
				(*it)->m_Force = Vector2f32( 0.0f, 0.0f );
			}

			// Delete all the contacts
			for( SizeType i = 0; i < contacts.size( ); i++ )
			{
				delete contacts[ i ];
			}
			contacts.clear( );

		}

		Body * Scene::Add( Shape * p_pShape, const Vector2f32 & p_Position, const Float32 p_Restitution )
		{
			// Error check the shape pointer
			if( p_pShape == NULL )
			{
				return NULL;
			}

			// Create the body
			Body * pBody = new Body( this, p_pShape, p_Position, 1.0f, p_Restitution );

			// Add the body to the body list
			m_Bodies.push_back( pBody );

			// Return the body
			return pBody;
		}
			
		void Scene::Remove( Body * p_pBody )
		{
			m_Bodies.remove( p_pBody );
		}

		void Scene::Clear( )
		{
			// Delete all the bodies.
			for( BodyList::iterator it = m_Bodies.begin( ); it != m_Bodies.end( ); it++ )
			{
				delete *it;
				
			}

			// Clear the body list.
			m_Bodies.clear( );
		}

		void Scene::ComputePosition( Body * p_pBody, const Time & p_StepTime )
		{
			p_pBody->m_Position += p_pBody->m_Velocity * static_cast<Float32>( p_StepTime.AsSeconds( ) );
		}

		void Scene::ApplyForces( Body * p_pBody, const Time & p_StepTime )
		{
			p_pBody->m_Velocity += p_pBody->m_Force * p_pBody->m_MassInverse;
		}

	}

}