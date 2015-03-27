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

#ifndef BIT_SYSTEM_PHYS2_PRIVATE_MANIFOLD_HPP
#define BIT_SYSTEM_PHYS2_PRIVATE_MANIFOLD_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	namespace Phys2
	{

		// Forward declarations
		class Body;

		namespace Private
		{

			////////////////////////////////////////////////////////////////
			/// \ingroup System
			/// \brief Private Manifold class for 2D physics.
			/// 
			////////////////////////////////////////////////////////////////
			class BIT_API Manifold
			{

			public:

				friend class Scene;

				////////////////////////////////////////////////////////////////
				/// \brief Constructor
				/// 
				////////////////////////////////////////////////////////////////
				Manifold( Body * p_pBodyA, Body * p_pBodyB );

				////////////////////////////////////////////////////////////////
				/// \brief Check for intersections
				/// 
				////////////////////////////////////////////////////////////////
				void Solve( );

				////////////////////////////////////////////////////////////////
				/// \brief Apply implulse to bodies
				/// 
				////////////////////////////////////////////////////////////////
				void ApplyImpulse( );

				////////////////////////////////////////////////////////////////
				/// \brief Correcting the positions of the bodies.
				/// 
				////////////////////////////////////////////////////////////////
				void PositionalCorrection( const Float32 p_InverseIterations );

				////////////////////////////////////////////////////////////////
				/// \brief Solve circle to circle intersections
				/// 
				////////////////////////////////////////////////////////////////
				void CircleToCircle( Body * p_pBodyA, Body * p_pBodyB );

				////////////////////////////////////////////////////////////////
				/// \brief Solve circle to rectangle intersections
				/// 
				////////////////////////////////////////////////////////////////
				void CircleToRectangle( Body * p_pBodyA, Body * p_pBodyB );

				////////////////////////////////////////////////////////////////
				/// \brief Solve rectangle to circle intersections
				/// 
				////////////////////////////////////////////////////////////////
				void RectangleToCircle( Body * p_pBodyA, Body * p_pBodyB );

				////////////////////////////////////////////////////////////////
				/// \brief Solve rectangle to rectangle intersections
				/// 
				////////////////////////////////////////////////////////////////
				void RectangleToRectangle( Body * p_pBodyA, Body * p_pBodyB );

			private:

				// Private varaibles
				Body *			m_pBodyA;		///< Pointer to the first body.
				Body *			m_pBodyB;		///< Pointer to the second body.
				Uint32			m_ContactCount;	///< Number of contacts.
				Vector2f32		m_Contact;		///< Point of contact during collision.
				Vector2f32		m_Normal;		///< Normal from body A to B.
				Float32			m_Penetration;	///< depth of penetration from collision.

			};

		}

	}

}

#endif