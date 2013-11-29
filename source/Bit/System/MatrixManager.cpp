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
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// Dummy matrix variable used for the stack initialization
	Matrix4x4 DummyMatrix;
	std::deque< Matrix4x4 > DummyDeque( 1, DummyMatrix );

	// Set the private static variables
	MatrixManager::eMode MatrixManager::m_Mode = MatrixManager::Mode_ModelView;
	std::stack< Matrix4x4 > MatrixManager::m_MatrixStacks[ 2 ] =
	{
		std::stack< Matrix4x4 >( DummyDeque ),
		std::stack< Matrix4x4 >( DummyDeque )
	};

	// Public functions
	void MatrixManager::LoadIdentity( )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).Identity( );
	}

	void MatrixManager::LoadLookAt( const Vector3_f32 p_Eye, const Vector3_f32 p_Center, const Vector3_f32 p_Up )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).LookAt( p_Eye, p_Center, p_Up );
	}

	void MatrixManager::LoadOrthographic( const BIT_FLOAT32 p_Left, const BIT_FLOAT32 p_Right, const BIT_FLOAT32 p_Bottom,
			const BIT_FLOAT32 p_Top, const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).Orthographic( p_Left, p_Right, p_Bottom, p_Top, p_ZNear, p_ZFar );
	}

	void MatrixManager::LoadPerspective( const BIT_FLOAT32 p_Fov, const BIT_FLOAT32 p_Aspect,
			const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).Perspective( p_Fov, p_Aspect, p_ZNear, p_ZFar );
	}

	void MatrixManager::Push( )
	{
		// Push the top
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].push( m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ) );
	}

	void MatrixManager::Pop( )
	{
		// Pop the top
		if( m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].size( ) > 1 )
		{
			m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].pop( );
		}
	}

	void MatrixManager::RotateX( const BIT_FLOAT32 p_Angle )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).RotateX( p_Angle );
	}

	void MatrixManager::RotateY( const BIT_FLOAT32 p_Angle )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).RotateY( p_Angle );
	}

	void MatrixManager::RotateZ( const BIT_FLOAT32 p_Angle )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).RotateZ( p_Angle );
	}

	void MatrixManager::Scale( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z )
	{
		Matrix4x4 ScaleMat;
		ScaleMat.Identity( );
		ScaleMat.Scale( p_X, p_Y, p_Z );
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ) = m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ) * ScaleMat;
	}

	void MatrixManager::Translate( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ).Translate( p_X, p_Y, p_Z );
	}

	// Set functions
	void MatrixManager::SetMode( const eMode p_Mode )
	{
		m_Mode = p_Mode;
	}

	void MatrixManager::SetMatrix( const Matrix4x4 & p_Matrix )
	{
		m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( ) = p_Matrix;
	}

	// Get functions
	Matrix4x4 & MatrixManager::GetMatrix( )
	{
		return m_MatrixStacks[ (BIT_MEMSIZE)m_Mode ].top( );
	}

	Matrix4x4 & MatrixManager::GetMatrix( const eMode p_Mode )
	{
		return m_MatrixStacks[ (BIT_MEMSIZE)p_Mode ].top( );
	}

	MatrixManager::eMode MatrixManager::GetMode( )
	{
		return m_Mode;
	}

	Matrix4x4 & MatrixManager::GetModelViewMatrix( )
	{
		return m_MatrixStacks[ (BIT_MEMSIZE)Mode_ModelView ].top( );
	}

	Matrix4x4 & MatrixManager::GetProjectionMatrix( )
	{
		return m_MatrixStacks[ (BIT_MEMSIZE)Mode_Projection ].top( );
	}

}
