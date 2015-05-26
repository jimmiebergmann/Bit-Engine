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
#include <limits>
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

		void Circle::SetRadius( const Float32 p_Radius )
		{
			m_Radius = p_Radius;
		}

		Shape * Circle::Clone( ) const
		{
			return new Circle( m_Radius );
		}

		Float32 Circle::ComputeMass( const Float32 p_Density )
		{
			return PiFloat32 * m_Radius * m_Radius * p_Density;
		}

		Float32 Circle::ComputeInertia( const Float32 p_Mass )
		{
			return p_Mass * m_Radius * m_Radius / 2.0f;
		}


		// Static function for rectangle corners
		static const Vector2f32 rectCorners[ 4 ] =
		{
			Vector2f32( -0.5f, -0.5f ),	Vector2f32( 0.5f, -0.5f ),
			Vector2f32( 0.5f,	0.5f ),	Vector2f32( -0.5f, 0.5f)
		};

		// Rectangle class
		Rectangle::Rectangle( const Vector2f32 & p_Size ) :
			Shape( RectangleType ),
			m_Size( p_Size )
		{
		}

		Vector2f32 Rectangle::GetSize( ) const
		{
			return m_Size;
		}

		void Rectangle::SetSize( const Vector2f32 & p_Size )
		{
			m_Size = p_Size;
		}

		Vector2f32 Rectangle::GetExtremePoint(const Vector2f32 & p_Direction) const
		{
			// Compute the best point and projection
			Vector2f32 bestPoint;
			Float64 bestProjection = static_cast<Float32>(-std::numeric_limits<Float32>::max());

			// Go through the corners
			for (SizeType i = 0; i < 4; i++)
			{
				const Vector2f32 vertex = rectCorners[ i ] * m_Size;

				// Calculate the projection
				const Float64 projection = Vector2f32::Dot( vertex, p_Direction);

				if (projection > bestProjection)
				{
					bestPoint = vertex;
					bestProjection = projection;
				}
			}

			// Return the best point.
			return bestPoint;
		}

		Shape * Rectangle::Clone( ) const
		{
			return new Rectangle( m_Size );
		}

		Float32 Rectangle::ComputeMass( const Float32 p_Density )
		{
			return m_Size.x * m_Size.y * p_Density;
		}

		Float32 Rectangle::ComputeInertia( const Float32 p_Mass )
		{
			return ( p_Mass * ( m_Size.x * m_Size.x + m_Size.y * m_Size.y ) ) / 12.0f;
		}

	}

}