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

#ifndef BIT_SYSTEM_VECTOR2_HPP
#define BIT_SYSTEM_VECTOR2_HPP

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
	/// \brief A 2D vector class.
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class Vector2
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Vector2( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_A x and y value.
		///
		////////////////////////////////////////////////////////////////
		Vector2( T p_A );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		///
		////////////////////////////////////////////////////////////////
		Vector2( T p_X, T p_Y );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		///
		////////////////////////////////////////////////////////////////
		template <typename U>
		Vector2( U p_X, U p_Y );

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
		explicit Vector2( const Vector2<U> & p_Vector );

        template <typename U>
		Vector2<T> & operator = ( const Vector2<U> & p_Vector );

		Bool operator == ( const Vector2<T> & p_Vector ) const;

		Bool operator != ( const Vector2<T> & p_Vector ) const;

		Vector2<T> & operator += ( const Vector2<T> & p_Vector );

		Vector2<T> & operator -= ( const Vector2<T> & p_Vector );

		Vector2<T> & operator *= ( const Vector2<T> & p_Vector );

		Vector2<T> & operator /= ( const Vector2<T> & p_Vector );

		Vector2<T> operator + ( const Vector2<T> & p_Vector ) const;

		Vector2<T> operator - ( ) const;

		Vector2 <T > operator - ( const Vector2<T> & p_Vector ) const;

		Vector2<T> operator * ( const Vector2<T> & p_Vector ) const;

		template <typename U>
		Vector2<T> operator * ( const U & p_Value ) const;

		Vector2<T> operator / ( const Vector2<T> & p_Vector ) const;

		template <typename U>
		Vector2<T> operator / ( const U & p_Value ) const;

		T operator [ ] ( const SizeType & p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \return The length of the vector.
		///
		////////////////////////////////////////////////////////////////
		Float64 Length( ) const;

		////////////////////////////////////////////////////////////////
		/// \return The normal of the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector2<T> Normal( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Normalizing the vector.
		///
		/// \return A reference to the vector.
		///
		////////////////////////////////////////////////////////////////
		const Vector2<T> & Normalize( );

		////////////////////////////////////////////////////////////////
		/// \return The absolute vector.
		///
		////////////////////////////////////////////////////////////////
		Vector2<T> Absolute( ) const;

		////////////////////////////////////////////////////////////////
		/// \return The dot value.
		///
		/// \param p_Vector The vector to calculate the dot product with.
		///
		////////////////////////////////////////////////////////////////
		Float64 Dot( const Vector2<T> & p_Vector ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Rotating the vector.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void Rotate( const Angle & p_Angle );

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the dot product between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		////////////////////////////////////////////////////////////////
		static Float64 Dot( const Vector2<T> & p_Vector1, const Vector2<T> & p_Vector2 );

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the angle between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		/// \return The angle in degrees between two vectors.
		///
		////////////////////////////////////////////////////////////////
		static Angle AngleBetweenVectors( const Vector2<T> & p_Vector1, const Vector2<T> & p_Vector2 );

		////////////////////////////////////////////////////////////////
		// Public variable members
		////////////////////////////////////////////////////////////////
		T x; ///< x-component of vector.
		T y; ///< y-component of vector.

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Vector2.inl>

	////////////////////////////////////////////////////////////////
	// Predefined vector types.
	////////////////////////////////////////////////////////////////
	typedef Vector2< Int8 >	Vector2i8;
	typedef Vector2< Uint8 >	Vector2u8;
	typedef Vector2< Int16 >	Vector2i16;
	typedef Vector2< Uint16 >	Vector2u16;
	typedef Vector2< Int32 >	Vector2i32;
	typedef Vector2< Uint32 >	Vector2u32;
	typedef Vector2< Int64 >	Vector2i64;
	typedef Vector2< Uint64 >	Vector2u64;
	typedef Vector2< Float32 >	Vector2f32;
	typedef Vector2< Float64 >	Vector2f64;

}

#endif
