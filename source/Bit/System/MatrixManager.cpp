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
	Matrix4x4f32 dummyMatrix;
	std::deque< Matrix4x4f32 > dummyDeque(1, dummyMatrix);

	// Global typedefs
	typedef std::stack<Matrix4x4f32> MatrixStack;
	typedef std::vector<MatrixStack> MatrixStackVector;

	// Global varaibles
	static SizeType g_StackCountLimit = 16;
	static SizeType g_CurrentStackIndex = 0;		///< Current matrix mode.
	static MatrixStackVector g_MatrixStacks(2, std::stack< Matrix4x4f32 >(dummyDeque));;	///< Vector of matrix stacks.
	static MatrixStack * g_pCurrentStack = &g_MatrixStacks[0];								///< Reference to the current stack.

	////////////////////////////////////////////////////////////////
	/// \brief Add another stack.
	///
	/// \return The index of the stack.
	///
	////////////////////////////////////////////////////////////////
	SizeType MatrixManager::AddStack()
	{
		// Set a stack limit.
		if (g_MatrixStacks.size() >= g_StackCountLimit)
		{
			return 0;
		}

		// Push a new stack
		MatrixStack stack;
		g_MatrixStacks.push_back(stack);

		// Return the index of the stack
		return g_MatrixStacks.size() - 1;
	}

	Bool MatrixManager::RemoveStack(const SizeType p_Index)
	{
		// Make sure that the index is valid.
		if (p_Index < 2 || p_Index > g_StackCountLimit - 1 || p_Index >= g_MatrixStacks.size())
		{
			return false;
		}

		// Erase the stack
		g_MatrixStacks.erase(g_MatrixStacks.begin() + p_Index);

		// Succeeded.
		return true;
	}

	void MatrixManager::Push()
	{
		// Get the current matrix.
		const Matrix4x4f32 & matrix = g_pCurrentStack->top();

		// Push the current matrix.
		g_pCurrentStack->push(matrix);
	}

	void MatrixManager::Pop()
	{
		// Pop the stack if possible.
		if (g_pCurrentStack->size() > 1)
		{
			g_pCurrentStack->pop();
		}
	}

	void MatrixManager::SetCurrentStack(const eMatrixStack p_Stack)
	{
		g_CurrentStackIndex = static_cast<SizeType>(p_Stack);
		g_pCurrentStack = &(g_MatrixStacks[g_CurrentStackIndex]);
	}

	void MatrixManager::SetCurrentStack(const SizeType p_Index)
	{
		if (p_Index >= g_MatrixStacks.size())
		{
			return;
		}

		g_CurrentStackIndex = static_cast<SizeType>(p_Index);
		g_pCurrentStack = &(g_MatrixStacks[g_CurrentStackIndex]);
	}

	void MatrixManager::SetMatrix(const Matrix4x4f32 & p_Matrix)
	{
		// Set the current matrix.
		g_pCurrentStack->top() = p_Matrix;
	}

	void MatrixManager::LoadIdentity()
	{
		g_pCurrentStack->top().Identity();
	}

	void MatrixManager::LoadLookAt(const Vector3f32 p_Eye, const Vector3f32 p_Center, const Vector3f32 p_Up)
	{
		g_pCurrentStack->top().LookAt(p_Eye, p_Center, p_Up);
	}

	void MatrixManager::LoadOrthographic(const Float32 p_Left, const Float32 p_Right, const Float32 p_Bottom,
		const Float32 p_Top, const Float32 p_ZNear, const Float32 p_ZFar)
	{
		g_pCurrentStack->top().Orthographic(p_Left, p_Right, p_Bottom, p_Top, p_ZNear, p_ZFar);
	}

	void MatrixManager::LoadPerspective(const Float32 p_Fov, const Float32 p_Aspect,
		const Float32 p_ZNear, const Float32 p_ZFar)
	{
		g_pCurrentStack->top().Perspective(p_Fov, p_Aspect, p_ZNear, p_ZFar);
	}

	void MatrixManager::Multiply(const Matrix4x4f32 & p_Matrix)
	{
		g_pCurrentStack->top() = g_pCurrentStack->top() * p_Matrix;
	}

	void MatrixManager::RotateEuler(const Vector3<Angle> & p_Angles)
	{
		Matrix4x4f32 rotation;
		rotation.Identity();
		rotation.RotateEuler(p_Angles);

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top() = g_pCurrentStack->top() * rotation;
	}

	void MatrixManager::RotateQuaternion(const Vector4f32 & p_Quaterinion)
	{
		Matrix4x4f32 rotation;
		rotation.Identity();
		rotation.RotateQuaternion(p_Quaterinion);

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top() = g_pCurrentStack->top() * rotation;
	}

	void MatrixManager::RotateX( const Angle p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::Sin<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );
		const Float32 AngleCos = Math::Cos<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );

		Matrix4x4f32 rotation (	1.0f, 0.0,			0.0,		0.0f,
								0.0f, AngleCos,		-AngleSin,	0.0f,
								0.0f, AngleSin,		AngleCos,	0.0f,
								0.0f, 0.0,			0.0,		1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * rotation;
	}

	void MatrixManager::RotateY( const Angle p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::Sin<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );
		const Float32 AngleCos = Math::Cos<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );

		Matrix4x4f32 rotation (	AngleCos,	0.0, AngleSin,	0.0,
								0.0f,		1.0, 0.0,		0.0,
								-AngleSin,	0.0, AngleCos,	0.0,
								0.0f,		0.0, 0.0,		1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * rotation;
	}

	void MatrixManager::RotateZ( const Angle p_Angle )
	{
		// Calculate the rotation matrix
		const Float32 AngleSin = Math::Sin<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );
		const Float32 AngleCos = Math::Cos<Float32>( static_cast<Float32>( p_Angle.AsRadians( ) ) );

		Matrix4x4f32 rotation (	AngleCos,	-AngleSin,	0.0, 0.0f,
								AngleSin,	AngleCos,	0.0, 0.0f,
								0.0f,		0.0,		1.0, 0.0f,
								0.0f,		0.0,		0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * rotation;
	}

	void MatrixManager::Scale( const Float32 p_X, const Float32 p_Y, const Float32 p_Z )
	{
		Matrix4x4f32 scale (	p_X, 0.0, 0.0, 0.0f,
								0.0f, p_Y, 0.0, 0.0f,
								0.0f, 0.0, p_Z, 0.0f,
								0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * scale;
	}

	void MatrixManager::Scale( const Vector3f32 p_Scale )
	{
		Matrix4x4f32 scale (	p_Scale.x, 0.0, 0.0, 0.0f,
								0.0f, p_Scale.y, 0.0, 0.0f,
								0.0f, 0.0, p_Scale.z, 0.0f,
								0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * scale;
	}

	void MatrixManager::Translate( const Float32 p_X, const Float32 p_Y, const Float32 p_Z )
	{
		Matrix4x4f32 translation (	1.0f, 0.0, 0.0, p_X,
									0.0f, 1.0, 0.0, p_Y,
									0.0f, 0.0, 1.0, p_Z,
									0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * translation;
	}

	void MatrixManager::Translate( const Vector3f32 p_Translation )
	{
		Matrix4x4f32 translation (	1.0f, 0.0, 0.0, p_Translation.x,
									0.0f, 1.0, 0.0, p_Translation.y,
									0.0f, 0.0, 1.0, p_Translation.z,
									0.0f, 0.0, 0.0, 1.0f );

		// Multiply the rotation matrix by the current matrix.
		g_pCurrentStack->top( ) = g_pCurrentStack->top( ) * translation;
	}

	SizeType MatrixManager::GetStackCount( )
	{
		return g_MatrixStacks.size( );
	}

	SizeType MatrixManager::GetCurrentStack( )
	{
		return g_CurrentStackIndex;
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( )
	{
		return g_pCurrentStack->top( );
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( const eMatrixStack p_Stack )
	{
		return g_MatrixStacks[ static_cast<SizeType>( p_Stack ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetMatrix( const SizeType p_Index )
	{
		if( p_Index >= g_MatrixStacks.size( ) )
		{
			return dummyMatrix;
		}

		return g_MatrixStacks[ static_cast<SizeType>( p_Index ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetModelViewMatrix( )
	{
		return g_MatrixStacks[ static_cast<SizeType>( ModelView ) ].top( );
	}

	const Matrix4x4f32 & MatrixManager::GetProjectionMatrix( )
	{
		return g_MatrixStacks[ static_cast<SizeType>( Projection ) ].top( );
	}

}
