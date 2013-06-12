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


#ifndef __BIT_SYSTEM_MATRIX_MANAGER_HPP__
#define __BIT_SYSTEM_MATRIX_MANAGER_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Matrix4x4.hpp>
#include <stack>

namespace Bit
{

	class MatrixManager
	{

	public:

		// Public enums
		enum eMode
		{
			Mode_Projection = 0,
			Mode_ModelView = 1
		};

		// Public functions
		static void Push( );
		static void Pop( );
		static void LoadIdentity( );
		static void LoadPerspective( const BIT_FLOAT32 p_Fov, const BIT_FLOAT32 p_Aspect,
			const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar );
		static void LoadOrthographic( const BIT_FLOAT32 p_Left, const BIT_FLOAT32 p_Right, const BIT_FLOAT32 p_Bottom,
			const BIT_FLOAT32 p_Top, const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar );
		static void Translate( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z );
		static void RotateX( const BIT_FLOAT32 p_Angle );
		static void RotateY( const BIT_FLOAT32 p_Angle );
		static void RotateZ( const BIT_FLOAT32 p_Angle );

		// Set functions
		static void SetMode( const eMode p_Mode );
		static void SetMatrix( const Matrix4x4 & p_Matrix );

		// Get functions
		static eMode GetMode( );
		static Matrix4x4 GetMatrix( );
		static Matrix4x4 GetMatrix( const eMode p_Mode );

	private:

		static eMode m_Mode;
		static std::stack< Matrix4x4 > m_MatrixStacks[ 2 ];
		//static Matrix4x4 m_Matrices[ 2 ];

	};

}

#endif