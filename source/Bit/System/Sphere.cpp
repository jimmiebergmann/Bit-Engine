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

#include <Bit/System/Sphere.hpp>
#include <Bit/System/Line3.hpp>
#include <Bit/System/Box.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Sphere::Sphere( )
	{
	}

	Sphere::Sphere( const BIT_FLOAT32 p_Radius ) :
		Radius( p_Radius ),
		Position( 0.0f, 0.0f, 0.0f )
	{
	}

	Sphere::Sphere(  const BIT_FLOAT32 p_Radius, const Vector3_f32 p_Position) :
		Radius( p_Radius ),
		Position( p_Position )
	{

	}


	// Get functions functions
	BIT_FLOAT32 Sphere::GetArea( ) const
	{
		return 4.0f * Radius * Radius * static_cast<BIT_FLOAT32>( BIT_PI );
	}

	BIT_FLOAT32 Sphere::GetCircumference( ) const
	{
		return Radius * 2 * static_cast<BIT_FLOAT32>( BIT_PI );
	}

	BIT_FLOAT32 Sphere::GetDiameter( ) const
	{
		return Radius * 2.0f;
	}

	BIT_FLOAT32 Sphere::GetVolume( ) const
	{
		return ( 4.0f * Radius * Radius * Radius * static_cast<BIT_FLOAT32>( BIT_PI ) ) / 3.0f;
	}


	// Intersection functions
	BIT_BOOL Sphere::Intersection( Vector3_f32 p_Point )
	{
		return IntersectionPoint3Sphere( p_Point, *this );
	}

	BIT_BOOL Sphere::Intersection( Line3 p_Line )
	{
		return IntersectionLine3Sphere( p_Line, *this );
	}

	BIT_UINT32 Sphere::Intersection( Line3 p_Line, Vector3_f32 & p_Point1, Vector3_f32 & p_Point2 )
	{
		return IntersectionLine3Sphere( p_Line, *this, p_Point1, p_Point2 );
	}

	BIT_SINT32 Sphere::Intersection( Sphere p_Sphere )
	{
		return IntersectionSphereSphere( *this, p_Sphere );
	}

	BIT_BOOL Sphere::Intersection( Box p_Box )
	{
		return IntersectionSphereBox( *this, p_Box );
	}

}