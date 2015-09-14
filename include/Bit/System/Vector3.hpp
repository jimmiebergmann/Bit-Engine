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

#ifndef BIT_SYSTEM_VECTOR3_HPP
#define BIT_SYSTEM_VECTOR3_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Math.hpp>
#include <cmath>

// Include isnan and make make it cross-platform.
#include <float.h>
#if defined( BIT_PLATFORM_WINDOWS )
	#define bitIsNan _isnan
#elif defined( BIT_PLATFORM_LINUX )
	#define bitIsNan isnan
#endif

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief A 3D vector class.
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class Vector3
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Vector3( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_A x, y and z value.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const T p_A );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		/// \param p_Z z value.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const T p_X, const T p_Y, const T p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		/// \param p_Z z value.
		///
		////////////////////////////////////////////////////////////////
		template <typename U>
		Vector3( const U p_X, const U p_Y, const U p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// This constructor makes it possible to cast between
		/// different vector types.
		///
		/// \param p_Vector vector to copy.
		///
		////////////////////////////////////////////////////////////////
		template <typename U>
		Vector3( const Vector3< U > & p_Vector );

        template <typename U>
		Vector3<T> & operator = ( const Vector3<U> & p_Vector );

		Bool operator == ( const Vector3<T> & p_Vector ) const;

		Bool operator != ( const Vector3<T> & p_Vector ) const;

		Vector3<T> & operator += ( const Vector3<T> & p_Vector );

		Vector3<T> & operator -= ( const Vector3<T> & p_Vector );

		Vector3<T> & operator *= ( const Vector3<T> & p_Vector );

		Vector3<T> & operator /= ( const Vector3<T> & p_Vector );

		Vector3<T> operator + ( const Vector3<T> & p_Vector ) const;

		Vector3<T> operator - ( ) const;

		Vector3<T> operator - ( const Vector3<T> & p_Vector ) const;

		Vector3<T> operator * ( const Vector3<T> & p_Vector ) const;

		Vector3<T> operator * ( const T & p_Value ) const;

		Vector3<T> operator / ( const Vector3<T> & p_Vector ) const;

		Vector3<T> operator / ( const T & p_Value ) const;

		T operator [ ] ( const SizeType & p_Index ) const;

		////////////////////////////////////////////////////////////////
		// Handy functions.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \return The length of the vector.
		///
		////////////////////////////////////////////////////////////////
		Float64 Length( ) const;

		////////////////////////////////////////////////////////////////
		/// \return The normal of the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3<T> Normal( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Normalizing the vector.
		///
		/// \return A reference to the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3<T> & Normalize( );

		////////////////////////////////////////////////////////////////
		/// \return The absolute vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3<T> Absolute( ) const;

		////////////////////////////////////////////////////////////////
		/// \return The dot value.
		///
		/// \param p_Vector The vector to calculate the dot product with.
		///
		////////////////////////////////////////////////////////////////
		Float64 Dot( const Vector3<T> & p_Vector ) const;

		////////////////////////////////////////////////////////////////
		/// \return The cross vector.
		///
		/// \param p_Vector The vector to calculate the cross product with.
		///
		////////////////////////////////////////////////////////////////
		Vector3<T> Cross(Vector3<T> p_Vector ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Do not use this yet.
		///
		/// \param p_Angle The angle in degrees to rotate.
		/// \param p_X The amount of x-component to rotate.
		/// \param p_Y The amount of y-component to rotate.
		/// \param p_Z The amount of z-component to rotate.
		///
		////////////////////////////////////////////////////////////////
		void Rotate( const Float64 p_Angle, const Float64 p_X, const Float64 p_Y, const Float64 p_Z );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the x-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateX( const Angle & p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the y-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateY( const Angle & p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the z-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateZ( const Angle & p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the dot product between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		////////////////////////////////////////////////////////////////
		static Float64 Dot( const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2 );

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the cross product between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		////////////////////////////////////////////////////////////////
		static Vector3<T> Cross( const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2);

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the angle between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		/// \return The angle in degrees between two vectors.
		///
		////////////////////////////////////////////////////////////////
		static Angle AngleBetweenVectors(const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2);

		////////////////////////////////////////////////////////////////
		// Public variable members
		////////////////////////////////////////////////////////////////
		T x; ///< x-component of vector.
		T y; ///< y-component of vector.
		T z; ///< z-component of vector.

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Vector3.inl>

	////////////////////////////////////////////////////////////////
	// Predefined vector types.
	////////////////////////////////////////////////////////////////
	typedef Vector3< Int8 >		Vector3i8;
	typedef Vector3< Uint8 >	Vector3u8;
	typedef Vector3< Int16 >	Vector3i16;
	typedef Vector3< Uint16 >	Vector3u16;
	typedef Vector3< Int32 >	Vector3i32;
	typedef Vector3< Uint32 >	Vector3u32;
	typedef Vector3< Int64 >	Vector3i64;
	typedef Vector3< Uint64 >	Vector3u64;
	typedef Vector3< Float32 >	Vector3f32;
	typedef Vector3< Float64 >	Vector3f64;

}

#endif
