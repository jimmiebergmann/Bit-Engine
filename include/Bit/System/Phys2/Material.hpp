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

#ifndef BIT_SYSTEM_PHYS2_MATERIAL_HPP
#define BIT_SYSTEM_PHYS2_MATERIAL_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	namespace Phys2
	{

		// Forward declarations
		namespace Private
		{
			class Manifold;
		}

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Material base class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Material
		{

		public:

			// Friend classes
			friend class Body;
			friend class Private::Manifold;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Material(	const Float32 p_Density = 1.0f,		
						const Float32 p_Restitution = 0.2,
						const Float32 p_StaticFriction = 0.3f,
						const Float32 p_DynamicFriction = 0.1f );

			////////////////////////////////////////////////////////////////
			/// \brief Set density
			/// 
			////////////////////////////////////////////////////////////////
			void SetDensity( const Float32 p_Density );

			////////////////////////////////////////////////////////////////
			/// \brief Set restitution
			/// 
			////////////////////////////////////////////////////////////////
			void SetRestitution( const Float32 p_Restitution );

			////////////////////////////////////////////////////////////////
			/// \brief Set static friction
			/// 
			////////////////////////////////////////////////////////////////
			void SetStaticFriction( const Float32 p_StaticFriction );

			////////////////////////////////////////////////////////////////
			/// \brief Set dynamic friction
			/// 
			////////////////////////////////////////////////////////////////
			void SetDynamicFriction( const Float32 p_DynamicFriction );

			////////////////////////////////////////////////////////////////
			/// \brief Get density
			/// 
			////////////////////////////////////////////////////////////////
			Float32 GetDensity( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get restitution
			/// 
			////////////////////////////////////////////////////////////////
			Float32 GetRestitution( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get static friction
			/// 
			////////////////////////////////////////////////////////////////
			Float32 GetStaticFriction( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get dynamic friction
			/// 
			////////////////////////////////////////////////////////////////
			Float32 GetDynamicFriction( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Static default material
			/// 
			////////////////////////////////////////////////////////////////
			static const Material Default;

		private:

			Float32	m_Density;
			Float32	m_Restitution;
			Float32 m_StaticFriction;
			Float32 m_DynamicFriction;

		};

	}

}

#endif