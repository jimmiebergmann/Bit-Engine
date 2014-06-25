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

#ifndef BIT_SYSTEM_MATRXI4X4_HPP
#define BIT_SYSTEM_MATRXI4X4_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Math.hpp>
#include <Bit/System/Vector3.hpp>

#include <float.h>
#if defined( BIT_PLATFORM_WIN32 )
	#define bitIsNan _isnan
#elif defined( BIT_PLATFORM_LINUX )
	#define bitIsNan isnan
#endif

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Matrix class
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class Matrix4x4
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		///	Fill all the matrix's components with one single value.
		///
		/// \param p_Val Input value
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4( const T p_Val );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4(	const T X_x, const T Y_x, const T Z_x, const T P_x,
					const T X_y, const T Y_y, const T Z_y, const T P_y,
					const T X_z, const T Y_z, const T Z_z, const T P_z,
					const T X_a, const T Y_a, const T Z_a, const T P_a );

		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into an identity matrix.
		///
		////////////////////////////////////////////////////////////////
		void Identity( );

		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into a fixed camera matrix.
		///
		/// \param p_Eye The position of the camera(The eye).
		/// \param p_Center The center position of where the camera is pointing.
		/// \param p_Up The up vector of the camera(Used for orientation).
		///
		////////////////////////////////////////////////////////////////
		void LookAt( Vector3<T> p_Eye, Vector3<T> p_Center, Vector3<T> p_Up );

		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into an orthographic matrix.
		///
		/// \param p_Left The left value.
		/// \param p_Right The right value.
		/// \param p_Bottom The bottom value.
		/// \param p_Top The top value.
		/// \param p_ZNear The nearest depth value.
		/// \param p_ZFar The farthest depth value.
		///
		////////////////////////////////////////////////////////////////
		void Orthographic(	const T p_Left, const T p_Right, const T p_Bottom,
							const T p_Top, const T p_ZNear, const T p_ZFar );


		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into a perspective matrix.
		///
		/// \param p_Fov Field of view value.
		/// \param p_Aspect Aspect ratio value.
		/// \param p_ZNear The nearest depth value.
		/// \param p_ZFar The farthest depth value.
		///
		////////////////////////////////////////////////////////////////
		void Perspective(	const T p_Fov, const T p_Aspect,
							const T p_ZNear, const T p_ZFar );

		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into a position matrix.
		///
		////////////////////////////////////////////////////////////////
		void Position( const Vector3<T> & p_Postion );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix about the x-axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		void RotateX( const T p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix about the y-axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		void RotateY( const T p_Angle );
		////////////////////////////////////////////////////////////////
		/// \brief Rotate the matrix about the z-axis.
		///
		/// \param p_Angle The angle in degrees to rotate the matrix.
		///
		////////////////////////////////////////////////////////////////
		void RotateZ( const T p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Scale the matrix.
		///
		/// \param p_X Amount of scale by the x-axis.
		/// \param p_Y Amount of scale by the y-axis.
		/// \param p_Z Amount of scale by the z-axis.
		///
		////////////////////////////////////////////////////////////////
		void Scale( const T p_X, const T p_Y, const T p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Translate the matrix.
		///
		/// \param p_X Amount of translation by the x-axis.
		/// \param p_Y Amount of translation by the y-axis.
		/// \param p_Z Amount of translation by the z-axis.
		///
		////////////////////////////////////////////////////////////////
		void Translate( const T p_X, const T p_Y, const T p_Z );


		////////////////////////////////////////////////////////////////
		// Operators.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Copy operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4<T> & operator = ( const Matrix4x4<T> & p_Mat );

		////////////////////////////////////////////////////////////////
		/// \brief Addition operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4<T> operator + ( const Matrix4x4<T> & p_Mat ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Subtraction operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4<T> operator - ( const Matrix4x4<T> & p_Mat ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Multiplication operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4<T> operator * ( const Matrix4x4<T> & p_Mat ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Equal to operator.
		///
		////////////////////////////////////////////////////////////////
		Bool operator == ( const Matrix4x4<T> & p_Mat ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Not equal to operator.
		///
		////////////////////////////////////////////////////////////////
		Bool operator != ( const Matrix4x4<T> & p_Mat ) const;

		////////////////////////////////////////////////////////////////
		// Public variables.
		////////////////////////////////////////////////////////////////
		T m[ 16 ]; ///< The matrix components

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Matrix4x4.inl>

	////////////////////////////////////////////////////////////////
	// Predefined matrix4x4 types.
	////////////////////////////////////////////////////////////////
	typedef Matrix4x4< Float32 >	Matrix4x4f32;
	typedef Matrix4x4< Float64 >	Matrix4x4f64;

}

#endif
