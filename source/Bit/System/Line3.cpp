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

#include <Bit/System/Line3.hpp>
#include <Bit/System/Sphere.hpp>
#include <Bit/System/Box.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Line3::Line3( )
	{
	}

	Line3::Line3( const Line3 & p_Line )
	{
		p[ 0 ] = p_Line.p[ 0 ];
		p[ 1 ] = p_Line.p[ 1 ];
	}

	Line3::Line3( const Vector3_f32 p_P1, const Vector3_f32 p_P2 )
	{
		p[ 0 ] = p_P1;
		p[ 1 ] = p_P2;
	}

	Line3::Line3( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2, const BIT_FLOAT32 p_P1_3,
		const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2, const BIT_FLOAT32 p_P2_3 )
	{
		p[ 0 ] = Vector3_f32( p_P1_1, p_P1_2, p_P1_3 );
		p[ 1 ] = Vector3_f32( p_P2_1, p_P2_2, p_P2_3 );
	}


	// Public functions
	BIT_FLOAT32 Line3::GetLength( ) const
	{
		return Vector3_f32( p[ 0 ] - p[ 1 ] ).Magnitude( );
	}


	// Intersection functions
	BIT_BOOL Line3::Intersection( Vector3_f32 p_Point )
	{
		return IntersectionPoint3Line3( p_Point, *this );
	}

	BIT_BOOL Line3::Intersection( Line3 p_Line )
	{
		Vector3_f32 Dummy;
		return IntersectionLine3Line3( *this, p_Line, Dummy );
	}

	BIT_BOOL Line3::Intersection( Line3 p_Line, Vector3_f32 & p_Intersection )
	{
		return IntersectionLine3Line3( *this, p_Line, p_Intersection );
	}

	BIT_BOOL Line3::Intersection( Sphere p_Sphere )
	{
		return IntersectionLine3Sphere( *this, p_Sphere );
	}

	BIT_UINT32 Line3::Intersection( Sphere p_Sphere, Vector3_f32 & p_Point1, Vector3_f32 & p_Point2 )
	{
		return IntersectionLine3Sphere( *this, p_Sphere, p_Point1, p_Point2 );
	}

	BIT_BOOL Line3::Intersection( Box p_Box )
	{
		return IntersectionLine3Box( *this, p_Box );
	}

}