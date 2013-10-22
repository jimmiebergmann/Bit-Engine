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

#ifndef __BIT_SYSTEM_MATRXI4X4_HPP__
#define __BIT_SYSTEM_MATRXI4X4_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/System/Math.hpp>

namespace Bit
{

	class BIT_API Matrix4x4
	{

	public:

		// Constructors
		Matrix4x4( );
		Matrix4x4(	const BIT_FLOAT32 X_x, const BIT_FLOAT32 Y_x, const BIT_FLOAT32 Z_x, const BIT_FLOAT32 P_x,
					const BIT_FLOAT32 X_y, const BIT_FLOAT32 Y_y, const BIT_FLOAT32 Z_y, const  BIT_FLOAT32 P_y,
					const BIT_FLOAT32 X_z, const BIT_FLOAT32 Y_z, const BIT_FLOAT32 Z_z, const BIT_FLOAT32 P_z,
					const BIT_FLOAT32 X_a, const BIT_FLOAT32 Y_a, const BIT_FLOAT32 Z_a, const BIT_FLOAT32 P_a );
		
		// Public functions
		void Identity( );
		void LookAt( Vector3_f32 p_Eye, Vector3_f32 p_Center, Vector3_f32 p_Up );
		void Orthographic( const BIT_FLOAT32 p_Left, const BIT_FLOAT32 p_Right, const BIT_FLOAT32 p_Bottom,
			const BIT_FLOAT32 p_Top, const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar );
		void Perspective( const BIT_FLOAT32 p_Fov, const BIT_FLOAT32 p_Aspect,
			const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar );
		void Position( Vector3_f32 p_Postion );
		void RotateX( const BIT_FLOAT32 p_Angle );
		void RotateY( const BIT_FLOAT32 p_Angle );
		void RotateZ( const BIT_FLOAT32 p_Angle );
		void Scale( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z );
		void Translate( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z );

		// Operators
		Matrix4x4 operator * ( Matrix4x4 p_Mat );

		// Public variables
		BIT_FLOAT32 m[ BIT_MATRIX4X4_SIZE ];

	};

}

#endif