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

#ifndef BIT_SYSTEM_PHYS2_BODY_HPP
#define BIT_SYSTEM_PHYS2_BODY_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Phys2/Material.hpp>
#include <Bit/System/Phys2/Shape.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	namespace Phys2
	{

		// Forward declarations
		class Scene;

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Body class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Body
		{

		public:

			// Friend classes
			friend class Scene;
			friend class Private::Manifold;
			
			////////////////////////////////////////////////////////////////
			/// \brief Apply force to the body
			/// 
			////////////////////////////////////////////////////////////////
			void ApplyForce( const Vector2f32 & p_Force );

			////////////////////////////////////////////////////////////////
			/// \brief Apply torque to body
			/// 
			////////////////////////////////////////////////////////////////
			void ApplyTorque( const Float32 p_Torque );

			////////////////////////////////////////////////////////////////
			/// \brief Apply impulse to body
			/// 
			////////////////////////////////////////////////////////////////
			void ApplyImpulse( const Vector2f32 & p_Impulse, const Vector2f32 & p_ContactPoint );

			////////////////////////////////////////////////////////////////
			/// \brief Set force
			/// 
			////////////////////////////////////////////////////////////////
			void SetForce( const Vector2f32 & p_Force );

			////////////////////////////////////////////////////////////////
			/// \brief Set position
			/// 
			////////////////////////////////////////////////////////////////
			void SetPosition( const Vector2f32 & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Set orientation
			/// 
			////////////////////////////////////////////////////////////////
			void SetOrientation( const Angle & p_Orientation );

			////////////////////////////////////////////////////////////////
			/// \brief Get position
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetPosition( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get force
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetForce( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get orientation
			/// 
			////////////////////////////////////////////////////////////////
			Angle GetOrientation( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get const reference to shape
			/// 
			////////////////////////////////////////////////////////////////
			const Shape & GetShape( ) const;

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Body(	Scene * p_pScene, Shape * p_pShape,
					const Vector2f32 & p_Position, const Material & p_Material );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			/// 
			////////////////////////////////////////////////////////////////
			~Body( );

			// Private variables
			Scene *			m_pScene;			///< Pointer to parent scene.
			Shape *			m_pShape;			///< Pointer to child shape.
			Material		m_Material;			///< Structure of material data.
			Vector2f32		m_Position;			///< The body's position.
			Vector2f32		m_Velocity;			///< The body's velocity.
			Float32			m_AngularVelocity;	///< The body's angular velocity.
			Float32			m_Torque;			///< Torque.
			Float32			m_Orient;			///< In radians.
			Vector2f32		m_Force;			///< Force acting on body.
			Float32			m_Mass;				///< Body's mass.
			Float32			m_MassInverse;		///< Inverse of mass, for faster computing.
			Float32			m_Inertia;			///< Body's moment of intertia.
			Float32			m_InertiaInverse;	///< Inverse of intertia, for faster computing.

		};

	}

}

#endif