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

#include <Bit/System/MatrixManager.hpp>
#include <deque>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// Dummy matrix variable used for the stack initialization
	Matrix4x4f32 DummyMatrix;
	std::deque< Matrix4x4f32 > DummyDeque( 1, DummyMatrix );

	// Set the private static variables
	MatrixManager::eMatrixMode MatrixManager::s_Mode = MatrixManager::ModelView;
	// Intialize the stacks
	MatrixManager::MatrixStack MatrixManager::s_MatrixStacks[ 2 ] =
	{
		std::stack< Matrix4x4f32 >( DummyDeque ),
		std::stack< Matrix4x4f32 >( DummyDeque )
	};
	// Intiialize the reference to the current stack.
	MatrixManager::MatrixStack & MatrixManager::s_CurrentStack = MatrixManager::s_MatrixStacks[ 0 ];


	void MatrixManager::Push( )
	{
		// Get the current matrix.
		const Matrix4x4f32 & matrix = s_CurrentStack.top( );

		// Push the current matrix.
		s_CurrentStack.push( matrix );
	}

	void MatrixManager::Pop( )
	{
		// Pop the stack if possible.
		if( s_CurrentStack.size( ) > 1 )
		{
			s_CurrentStack.pop( );
		}
	}

	void MatrixManager::SetMode( const eMatrixMode p_Mode )
	{
		s_Mode = p_Mode;
		s_CurrentStack = s_MatrixStacks[ static_cast<SizeType>( s_Mode ) ];
	}

	void MatrixManager::SetMatrix( const Matrix4x4f32 & p_Matrix )
	{
		// Set the current matrix.
		s_CurrentStack.top( ) = p_Matrix;
	}

	void MatrixManager::LoadIdentity( )
	{
		s_CurrentStack.top( ).Identity( );
	}

	void MatrixManager::LoadLookAt( const Vector3f32 p_Eye, const Vector3f32 p_Center, const Vector3f32 p_Up )
	{
		s_CurrentStack.top( ).LookAt( p_Eye, p_Center, p_Up );
	}

	void MatrixManager::LoadOrthographic(	const Float32 p_Left, const Float32 p_Right, const Float32 p_Bottom,
											const Float32 p_Top, const Float32 p_ZNear, const Float32 p_ZFar )
	{
		s_CurrentStack.top( ).Orthographic( p_Left, p_Right, p_Bottom, p_Top, p_ZNear, p_ZFar );
	}

	void MatrixManager::LoadPerspective(	const Float32 p_Fov, const Float32 p_Aspect,
											const Float32 p_ZNear, const Float32 p_ZFar )
	{
		s_CurrentStack.top( ).Perspective( p_Fov, p_Aspect, p_ZNear, p_ZFar );
	}

	void MatrixManager::RotateX( const Float32 p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::SinDegrees<Float32>( p_Angle );
		const Float32 AngleCos = Math::CosDegrees<Float32>( p_Angle );

		Matrix4x4f32 rotation (	1.0f, 0.0,			0.0,		0.0f,
								0.0f, AngleCos,		-AngleSin,	0.0f,
								0.0f, AngleSin,		AngleCos,	0.0f,
								0.0f, 0.0,			0.0,		1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * rotation;
	}

	void MatrixManager::RotateY( const Float32 p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::SinDegrees<Float32>( p_Angle );
		const Float32 AngleCos = Math::CosDegrees<Float32>( p_Angle );

		Matrix4x4f32 rotation (	AngleCos,	0.0, AngleSin,	0.0,
								0.0f,		1.0, 0.0,		0.0,
								-AngleSin,	0.0, AngleCos,	0.0,
								0.0f,		0.0, 0.0,		1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * rotation;
	}

	void MatrixManager::RotateZ( const Float32 p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::SinDegrees<Float32>( p_Angle );
		const Float32 AngleCos = Math::CosDegrees<Float32>( p_Angle );

		Matrix4x4f32 rotation (	AngleCos,	-AngleSin,	0.0, 0.0f,
								AngleSin,	AngleCos,	0.0, 0.0f,
								0.0f,		0.0,		1.0, 0.0f,
								0.0f,		0.0,		0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * rotation;
	}

	void MatrixManager::Scale( const Float32 p_X, const Float32 p_Y, const Float32 p_Z )
	{
		Matrix4x4f32 scale (	p_X, 0.0, 0.0, 0.0f,
								0.0f, p_Y, 0.0, 0.0f,
								0.0f, 0.0, p_Z, 0.0f,
								0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * scale;
	}

	void MatrixManager::Scale( const Vector3f32 p_Scale )
	{
		Matrix4x4f32 scale (	p_Scale.x, 0.0, 0.0, 0.0f,
								0.0f, p_Scale.y, 0.0, 0.0f,
								0.0f, 0.0, p_Scale.z, 0.0f,
								0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * scale;
	}

	void MatrixManager::Translate( const Float32 p_X, const Float32 p_Y, const Float32 p_Z )
	{
		Matrix4x4f32 translation (	1.0f, 0.0, 0.0, p_X,
									0.0f, 1.0, 0.0, p_Y,
									0.0f, 0.0, 1.0, p_Z,
									0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * translation;
	}

	void MatrixManager::Translate( const Vector3f32 p_Translation )
	{
		Matrix4x4f32 translation (	1.0f, 0.0, 0.0, p_Translation.x,
									0.0f, 1.0, 0.0, p_Translation.y,
									0.0f, 0.0, 1.0, p_Translation.z,
									0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		s_CurrentStack.top( ) = s_CurrentStack.top( ) * translation;
	}

	MatrixManager::eMatrixMode MatrixManager::GetMatrixMode( )
	{
		return s_Mode;
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( )
	{
		return s_CurrentStack.top( );
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( const eMatrixMode p_Mode )
	{
		return s_MatrixStacks[ static_cast<SizeType>( p_Mode ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetModelViewMatrix( )
	{
		return s_MatrixStacks[ static_cast<SizeType>( ModelView ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetProjectionMatrix( )
	{
		return s_MatrixStacks[ static_cast<SizeType>( Projection ) ].top( );
	}

}
