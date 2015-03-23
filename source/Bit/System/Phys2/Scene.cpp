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

		void Scene::Step( const Time & p_StepTime )
		{
		}

		Body * Scene::Add( Shape * p_pShape, const Vector2f32 & p_Position )
		{
			// Error check the shape pointer
			if( p_pShape == NULL )
			{
				return NULL;
			}

			// Create the body
			Body * pBody = new Body( p_pShape, 1.0f, p_Position );

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

	}

}