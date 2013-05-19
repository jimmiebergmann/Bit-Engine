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


#ifndef __BIT_SYSTEM_LINE_HPP__
#define __BIT_SYSTEM_LINE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	class Line
	{

	public:

		// Construcotrs
		Line( );
		Line( const Line & p_Line );
		Line( const Vector3_f32 p_P1, const Vector3_f32 p_P2 );
		Line( const Vector2_f32 p_P1, const Vector2_f32 p_P2 );
		Line( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2,
			const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2 );
		Line( const BIT_FLOAT32 p_P1_1, const BIT_FLOAT32 p_P1_2, const BIT_FLOAT32 p_P1_3,
			const BIT_FLOAT32 p_P2_1, const BIT_FLOAT32 p_P2_2, const BIT_FLOAT32 p_P2_3 );

		// Public functions
		BIT_FLOAT32 Length( ) const;

		// Intersection functions
		BIT_BOOL IntersectionLine2( Line p_Line, Vector3_f32 & p_Intersection );

		// Public variables
		Vector3_f32 p[ 2 ];

	};

}

#endif