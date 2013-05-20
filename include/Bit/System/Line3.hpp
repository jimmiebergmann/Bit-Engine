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


#ifndef __BIT_SYSTEM_LINE3_HPP__
#define __BIT_SYSTEM_LINE3_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Math.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	// Use this class for both 2D and 3D lines (line2/line3)
	class Line3
	{

	public:

		// Construcotrs
		Line3( );
		Line3( const Line3 & p_Line );
		Line3( const Vector3_f32 p_P1, const Vector3_f32 p_P2 );
		Line3( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2, const BIT_FLOAT32 p_P1_3,
			const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2, const BIT_FLOAT32 p_P2_3 );

		// Public functions
		BIT_FLOAT32 GetLength( ) const;

		// Intersection functions
		BIT_BOOL Intersection( Vector3_f32 p_Point );
		BIT_BOOL Intersection( Line3 p_Line );
		BIT_BOOL Intersection( Line3 p_Line, Vector3_f32 & p_Intersection );
		BIT_BOOL Intersection( Sphere p_Sphere );
		BIT_UINT32 Intersection( Sphere p_Sphere, Vector3_f32 & p_Point1, Vector3_f32 & p_Point2 );
		BIT_BOOL Intersection( Box p_Box );

		// Public variables
		Vector3_f32 p[ 2 ];

	};

}

#endif