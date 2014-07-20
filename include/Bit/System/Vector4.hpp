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

#ifndef BIT_SYSTEM_VECTOR4_HPP
#define BIT_SYSTEM_VECTOR4_HPP

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
	/// \brief A 4D vector class.
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class Vector4
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Vector4( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_A x, y, z and w value.
		///
		////////////////////////////////////////////////////////////////
		Vector4( const T p_A );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		/// \param p_Z z value.
		/// \param p_W w value.
		///
		////////////////////////////////////////////////////////////////
		Vector4( const T p_X, const T p_Y, const T p_Z, const T p_W );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		/// \param p_Z z value.
		/// \param p_W w value.
		///
		////////////////////////////////////////////////////////////////
		template <typename U>
		Vector4( const U p_X, const U p_Y, const U p_Z, const T p_W );

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
		Vector4( const Vector4< U > & p_Vector );

        template <typename U>
		Vector4<T> & operator = ( const Vector4<U> & p_Vector );

		Bool operator == ( const Vector4<T> & p_Vector ) const;

		Bool operator != ( const Vector4<T> & p_Vector ) const;

		Vector4<T> & operator += ( const Vector4<T> & p_Vector );

		Vector4<T> & operator -= ( const Vector4<T> & p_Vector );

		Vector4<T> & operator *= ( const Vector4<T> & p_Vector );

		Vector4<T> & operator /= ( const Vector4<T> & p_Vector );

		Vector4<T> operator + ( const Vector4<T> & p_Vector ) const;

		Vector4<T> operator - ( ) const;

		Vector4<T> operator - ( const Vector4<T> & p_Vector ) const;

		Vector4<T> operator * ( const Vector4<T> & p_Vector ) const;

		Vector4<T> operator * ( const T & p_Value ) const;

		Vector4<T> operator / ( const Vector4<T> & p_Vector ) const;

		Vector4<T> operator / ( const T & p_Value ) const;

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
		Vector4<T> Normal( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Normalizing the vector.
		///
		/// \return A reference to the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector4<T> & Normalize( );

		////////////////////////////////////////////////////////////////
		/// \return The absolute vector.
		///
		////////////////////////////////////////////////////////////////
		Vector4<T> Absolute( ) const;

		////////////////////////////////////////////////////////////////
		// Public variable members
		////////////////////////////////////////////////////////////////
		T x; ///< x-component of vector.
		T y; ///< y-component of vector.
		T z; ///< z-component of vector.
		T w; ///< w-component of vector.

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Vector4.inl>

	////////////////////////////////////////////////////////////////
	// Predefined vector types.
	////////////////////////////////////////////////////////////////
	typedef Vector4< Int8 >		Vector4i8;
	typedef Vector4< Uint8 >	Vector4u8;
	typedef Vector4< Int16 >	Vector4i16;
	typedef Vector4< Uint16 >	Vector4u16;
	typedef Vector4< Int32 >	Vector4i32;
	typedef Vector4< Uint32 >	Vector4u32;
	typedef Vector4< Int64 >	Vector4i64;
	typedef Vector4< Uint64 >	Vector4u64;
	typedef Vector4< Float32 >	Vector4f32;
	typedef Vector4< Float64 >	Vector4f64;

}

#endif
