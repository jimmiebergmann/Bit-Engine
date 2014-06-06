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

	// Global varaibles
	static SizeType g_StackCountLimit = 16;

	// Dummy matrix variable used for the stack initialization
	Matrix4x4f32 dummyMatrix;
	std::deque< Matrix4x4f32 > dummyDeque( 1, dummyMatrix );

	// Set the private static variables
	SizeType MatrixManager::s_CurrentStackIndex = 0;
	// Intialize the stacks
	MatrixManager::MatrixStackVector MatrixManager::s_MatrixStacks( 2, std::stack< Matrix4x4f32 >( dummyDeque ) );
	// Intiialize the reference to the current stack.
	MatrixManager::MatrixStack & MatrixManager::s_CurrentStack = MatrixManager::s_MatrixStacks[ 0 ];

	////////////////////////////////////////////////////////////////
	/// \brief Add another stack.
	///
	/// \return The index of the stack.
	///
	////////////////////////////////////////////////////////////////
	SizeType MatrixManager::AddStack( )
	{
		// Set a stack limit.
		if( s_MatrixStacks.size( ) >= g_StackCountLimit )
		{
			return 0;
		}

		// Push a new stack
		MatrixStack stack;
		s_MatrixStacks.push_back( stack );

		// Return the index of the stack
		return s_MatrixStacks.size( ) - 1;
	}

	Bool MatrixManager::RemoveStack( const SizeType p_Index )
	{
		// Make sure that the index is valid.
		if( p_Index < 2 || p_Index > g_StackCountLimit - 1 || p_Index >= s_MatrixStacks.size( ) )
		{
			return false;
		}

		// Erase the stack
		s_MatrixStacks.erase( s_MatrixStacks.begin( ) + p_Index );

		// Succeeded.
		return true;
	}

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

	void MatrixManager::SetCurrentStack( const eMatrixStack p_Stack )
	{
		s_CurrentStackIndex = static_cast<SizeType>( p_Stack ); 
		s_CurrentStack = s_MatrixStacks[ s_CurrentStackIndex ];
	}

	void MatrixManager::SetCurrentStack( const SizeType p_Index )
	{
		if( p_Index >= s_MatrixStacks.size( ) )
		{
			return;
		}

		s_CurrentStackIndex = static_cast<SizeType>( p_Index ); 
		s_CurrentStack = s_MatrixStacks[ s_CurrentStackIndex ];
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

	SizeType MatrixManager::GetCurrentStack( )
	{
		return s_CurrentStackIndex;
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( )
	{
		return s_CurrentStack.top( );
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( const eMatrixStack p_Stack )
	{
		return s_MatrixStacks[ static_cast<SizeType>( p_Stack ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( const SizeType p_Index )
	{
		if( p_Index >= s_MatrixStacks.size( ) )
		{
			return dummyMatrix;
		}

		return s_MatrixStacks[ static_cast<SizeType>( p_Index ) ].top( );
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
