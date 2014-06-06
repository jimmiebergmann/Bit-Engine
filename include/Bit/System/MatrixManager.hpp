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

// TODO: Make sure to multiply a new matrix with the old one when you are transforming.
// Right now, scaling wont work if you're rotating the object.


#ifndef BIT_SYSTEM_MATRIX_MANAGER_HPP
#define BIT_SYSTEM_MATRIX_MANAGER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Matrix4x4.hpp>
#include <vector>
#include <stack>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Matrix manager class.
	///
	/// The matrix manager contains 2 stacks by default,
	/// but it's possible to add new stacks with the AddStack function.
	/// You can change the current matrix stack by using the already existing
	/// enumeration called eMatrixStack as the index.
	/// The first manually added stack has the index 2,
	///	the next one has the index 3, and so on...
	/// The maximum ammount of stacks is 16.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API MatrixManager
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Matrix stack enumerator.
		///
		////////////////////////////////////////////////////////////////
		enum eMatrixStack
		{
			ModelView	= 0,	///< Model view matrix stack.
			Projection	= 1		///< Projection matrix stack.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Add another stack.
		///
		/// \return The index of the stack.
		///
		////////////////////////////////////////////////////////////////
		static SizeType AddStack( );

		////////////////////////////////////////////////////////////////
		/// \brief Remove stack from matrix manager.
		///
		/// \param p_Index Index 2 to 15 of any manually added stack.
		///
		/// \return True if successfully removed stack, else false.
		///
		////////////////////////////////////////////////////////////////
		static Bool RemoveStack( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Push the current matrix to the stack.
		///
		////////////////////////////////////////////////////////////////
		static void Push( );
		
		////////////////////////////////////////////////////////////////
		/// \brief Pop the last pushed matrix from the stack.
		///
		////////////////////////////////////////////////////////////////
		static void Pop( );

		////////////////////////////////////////////////////////////////
		/// \brief Set the current matrix stack(only default stacks)
		///
		////////////////////////////////////////////////////////////////
		static void SetCurrentStack( const eMatrixStack p_Stack );

		////////////////////////////////////////////////////////////////
		/// \brief Set the current matrix stack(any stack, even manually added).
		///
		////////////////////////////////////////////////////////////////
		static void SetCurrentStack( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Set the current matrix.
		///
		/// This function will override any changes to the current matrix.
		///
		////////////////////////////////////////////////////////////////
		static void SetMatrix( const Matrix4x4f32 & p_Matrix );
		
		////////////////////////////////////////////////////////////////
		/// \brief Turn the current matrix into an identity matrix.
		///
		////////////////////////////////////////////////////////////////
		static void LoadIdentity( );

		////////////////////////////////////////////////////////////////
		/// \brief Turn the current matrix into a "look at" matrix.
		///
		/// This kind of matrix works well for any kind of camera.
		///
		////////////////////////////////////////////////////////////////
		static void LoadLookAt( const Vector3f32 p_Eye, const Vector3f32 p_Center, const Vector3f32 p_Up );

		////////////////////////////////////////////////////////////////
		/// \brief Turn the current matrix into an orthograpic projection matrix.
		///
		////////////////////////////////////////////////////////////////
		static void LoadOrthographic(	const Float32 p_Left, const Float32 p_Right, const Float32 p_Bottom,
										const Float32 p_Top, const Float32 p_ZNear, const Float32 p_ZFar );

		////////////////////////////////////////////////////////////////
		/// \brief Turn the current matrix into a perspective projection matrix.
		///
		////////////////////////////////////////////////////////////////
		static void LoadPerspective(	const Float32 p_Fov, const Float32 p_Aspect,
										const Float32 p_ZNear, const Float32 p_ZFar );
		
		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix by the X axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		static void RotateX( const Float32 p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix by the Y axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		static void RotateY( const Float32 p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix by the Z axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		static void RotateZ( const Float32 p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Scale the current matrix.
		///
		/// \param p_X Amount of scale by the x-axis.
		/// \param p_Y Amount of scale by the y-axis.
		/// \param p_Z Amount of scale by the z-axis.
		///
		////////////////////////////////////////////////////////////////
		static void Scale( const Float32 p_X, const Float32 p_Y, const Float32 p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Scale the current matrix.
		///
		/// \param p_Scale Vector representing the amount of scale by the xyz-axis.
		///
		////////////////////////////////////////////////////////////////
		static void Scale( const Vector3f32 p_Scale );

		////////////////////////////////////////////////////////////////
		/// \brief Translate the current matrix.
		///
		/// \param p_X Amount of translation by the x-axis.
		/// \param p_Y Amount of translation by the y-axis.
		/// \param p_Z Amount of translation by the z-axis.
		///
		////////////////////////////////////////////////////////////////
		static void Translate( const Float32 p_X, const Float32 p_Y, const Float32 p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Translate the current matrix.
		///
		/// \param p_Scale Vector representing the amount of translation by the xyz-axis.
		///
		////////////////////////////////////////////////////////////////
		static void Translate( const Vector3f32 p_Translation );
		
		////////////////////////////////////////////////////////////////
		/// \brief Get the index of the current stack.
		///
		////////////////////////////////////////////////////////////////
		static SizeType GetCurrentStack( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the current matrix.
		///
		////////////////////////////////////////////////////////////////
		static const Matrix4x4f32 & GetMatrix( );

		////////////////////////////////////////////////////////////////
		/// \brief Get current state of any default matrix.
		///
		////////////////////////////////////////////////////////////////
		static const Matrix4x4f32 & GetMatrix( const eMatrixStack p_Stack );

		////////////////////////////////////////////////////////////////
		/// \brief Get current state of any matrix.
		///
		////////////////////////////////////////////////////////////////
		static const Matrix4x4f32 & GetMatrix( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get model view matrix.
		///
		////////////////////////////////////////////////////////////////
		static const Matrix4x4f32 & GetModelViewMatrix( );

		////////////////////////////////////////////////////////////////
		/// \brief Get projection matrix.
		///
		////////////////////////////////////////////////////////////////
		static const Matrix4x4f32 & GetProjectionMatrix( );

	private:

		// Private typedefs
		
		typedef std::stack<Matrix4x4f32> MatrixStack;
		typedef std::vector<MatrixStack> MatrixStackVector;

		// Private variables.
		static SizeType s_CurrentStackIndex;		///< Current matrix mode.
		static MatrixStackVector s_MatrixStacks;	///< Vector of matrix stacks.
		static MatrixStack & s_CurrentStack;		///< Reference to the current stack.

	};

}

#endif
