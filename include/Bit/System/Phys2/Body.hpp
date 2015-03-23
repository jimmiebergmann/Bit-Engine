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

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Body( Shape * p_pShape, const Float32 p_Density, const Vector2f32 & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Get position
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetPosition( ) const;

		private:

			// Private variables
			Scene *			m_pScene;
			Shape *			m_pShape;
			Vector2f32		m_Position;
			Vector2f32		m_Velocity;
			Float32			m_Density;
			Float32			m_Mass;
			Float32			m_MassInverse;

		};

	}

}

#endif