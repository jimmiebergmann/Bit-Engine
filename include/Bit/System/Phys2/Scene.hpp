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

#ifndef BIT_SYSTEM_PHYS2_SCENE_HPP
#define BIT_SYSTEM_PHYS2_SCENE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Phys2/Body.hpp>
#include <Bit/System/Phys2/Private/Manifold.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Time.hpp>
#include <list>
#include <vector>

namespace Bit
{

	namespace Phys2
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Scene class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Scene
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Scene( const Vector2f32 & p_Gravity = Vector2f32( 0.0f, 0.0f ) );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			/// 
			////////////////////////////////////////////////////////////////
			~Scene( );

			////////////////////////////////////////////////////////////////
			/// \brief Step the physics in the scene.
			/// 
			////////////////////////////////////////////////////////////////
			void Step(	const Time & p_StepTime,
						const Uint32 p_VelocityIterations = 3,
						const Uint32 p_PositionIterations = 2 );

			////////////////////////////////////////////////////////////////
			/// \brief Add body to scene.
			///
			/// \return Pointer to created body.
			/// 
			////////////////////////////////////////////////////////////////
			Body * Add( Shape * p_pShape, const Vector2f32 & p_Position, const Material & p_Material = Material::Default );

			////////////////////////////////////////////////////////////////
			/// \brief Remove body from the scene.
			/// 
			////////////////////////////////////////////////////////////////
			void Remove( Body * p_pBody );

			////////////////////////////////////////////////////////////////
			/// \brief Clear the scenes from all bodies
			/// 
			////////////////////////////////////////////////////////////////
			void Clear( );

			////////////////////////////////////////////////////////////////
			/// \brief Set gravtivt
			/// 
			////////////////////////////////////////////////////////////////
			void SetGravity( const Vector2f32 & p_Gravity );

			////////////////////////////////////////////////////////////////
			/// \brief Get gravtivt
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetGravity( ) const;

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Compute position for body
			/// 
			////////////////////////////////////////////////////////////////
			void ComputePosition( Body * p_pBody, const Time & p_StepTime, const Float32 p_InverseIterations );

			////////////////////////////////////////////////////////////////
			/// \brief Apply forces to body
			/// 
			////////////////////////////////////////////////////////////////
			void ApplyForces( Body * p_pBody, const Time & p_StepTime );

			// Private typedefs
			typedef std::list<Body *>					BodyList;
			typedef std::vector<Body *>					BodyVector;
			typedef std::vector<Private::Manifold *>	ManifoldVector;

			// Private variables
			BodyVector		m_Bodies;
			Vector2f32		m_Gravity;

		};

	}

}

#endif