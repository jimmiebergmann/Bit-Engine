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

#include <Bit/System/Phys2/Body.hpp>
#include <Bit/System/Phys2/Shape.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Phys2
	{


		static Float32 Cross( const Vector2f32 & a, const Vector2f32& b )
		{
			return a.x * b.y - a.y * b.x;
		}


		// Default shape
		static const Circle g_DefaultShape( 1.0f );

		// Body implementations
		void Body::ApplyForce( const Vector2f32 & p_Force )
		{
			m_Force += p_Force;
		}

		void Body::ApplyTorque( const Float32 p_Torque )
		{
			m_Torque += p_Torque;
		}

		void Body::ApplyImpulse( const Vector2f32 & p_Impulse, const Vector2f32 & p_ContactPoint )
		{
			m_Velocity += p_Impulse * m_MassInverse;
			m_AngularVelocity += Cross( p_ContactPoint, p_Impulse ) * m_InertiaInverse;
		}

		void Body::SetForce( const Vector2f32 & p_Force )
		{
			m_Force = p_Force;
		}

		void Body::SetPosition( const Vector2f32 & p_Position )
		{
			m_Position = p_Position;
		}

		Vector2f32 Body::GetPosition( ) const
		{
			return m_Position;
		}

		Vector2f32 Body::GetForce( ) const
		{
			return m_Force;
		}

		Angle Body::GetOrientation( ) const
		{
			return Radians( m_Orient );
		}

		const Shape & Body::GetShape( ) const
		{
			if( m_pShape )
			{
				return *m_pShape;
			}

			// Return the default shape if the shape isn't set yet.
			return g_DefaultShape;
		}

		Body::Body( Scene * p_pScene, Shape * p_pShape,
					const Vector2f32 & p_Position, const Material & p_Material ) :
			m_pScene( p_pScene),
			m_pShape( p_pShape->Clone( ) ),
			m_Material( p_Material ),
			m_Position( p_Position ),
			m_Velocity( 0.0f, 0.0f ),
			m_AngularVelocity( 0.0f ),
			m_Torque( 0.0f ),
			m_Orient( 0.0f ),
			m_Force( 0.0f, 0.0f )
		{
			// Compute the mass
			m_Mass = m_pShape->ComputeMass( m_Material.m_Density );
			m_MassInverse = m_Mass ? 1.0f / m_Mass : 0.0f;

			m_Mass = m_pShape->ComputeMass( m_Material.m_Density );
			m_MassInverse = m_Mass ? 1.0f / m_Mass : 0.0f;
			m_Inertia = m_pShape->ComputeInertia( m_Mass );
			m_InertiaInverse = m_Inertia ? 1.0f / m_Inertia : 0.0f;
		}

		Body::~Body( )
		{
			if( m_pShape )
			{
				delete m_pShape;
			}
		}

	}

}