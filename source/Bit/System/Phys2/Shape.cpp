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

#include <Bit/System/Phys2/Shape.hpp>
#include <Bit/System/Math.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Phys2
	{
		
		// Shape class
		Shape::eType Shape::GetType( ) const
		{
			return m_Type;
		}

		Float32 Shape::GetRadius( ) const
		{
			return 0.0f;
		}

		Shape::Shape( const eType p_Type ) :
			m_Type( p_Type )
		{
		}


		// Circle class
		Circle::Circle( const Float32 p_Radius ) :
			Shape( CircleType ),
			m_Radius( p_Radius )
		{
		}

		Float32 Circle::GetRadius( ) const
		{
			return m_Radius;
		}

		Shape * Circle::Clone( ) const
		{
			return new Circle( m_Radius );
		}

		Float32 Circle::ComputeMass( const Float32 p_Density )
		{
			return PiFloat32 * m_Radius * m_Radius * p_Density;
		}

	}

}