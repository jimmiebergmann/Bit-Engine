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

#include <Bit/System/Phys2/Material.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Phys2
	{

		const Material Material::Default;

		Material::Material( const Float32 p_Density,		
							const Float32 p_Restitution,
							const Float32 p_StaticFriction,
							const Float32 p_DynamicFriction ) :
			m_Density( p_Density ),
			m_Restitution( p_Restitution ),
			m_StaticFriction( p_StaticFriction ),
			m_DynamicFriction( p_DynamicFriction )
		{
		}

		void Material::SetDensity( const Float32 p_Density )
		{
			m_Density = p_Density;
		}
			
		void Material::SetRestitution( const Float32 p_Restitution )
		{
			m_Restitution = p_Restitution;
		}

		void Material::SetStaticFriction( const Float32 p_StaticFriction )
		{
			m_StaticFriction = p_StaticFriction;
		}

		void Material::SetDynamicFriction( const Float32 p_DynamicFriction )
		{
			m_DynamicFriction = p_DynamicFriction;
		}

		Float32 Material::GetDensity( ) const
		{
			return m_Density;
		}
			
		Float32 Material::GetRestitution( ) const
		{
			return m_Restitution;
		}

		Float32 Material::GetStaticFriction( ) const
		{
			return m_StaticFriction;
		}

		Float32 Material::GetDynamicFriction( ) const
		{
			return m_DynamicFriction;
		}

	}

}