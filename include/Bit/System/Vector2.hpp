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
#include <cmath>

// Include isnan and make make it cross-platform.
#include <float.h>
#ifdef BIT_PLATFORM_WIN32
	#define bitIsNan _isnan
#elif BIT_PLATFORM_LINUX
	define bitIsNan isnan
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
		// Constructors.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Vector2( ) :
			x( static_cast< T >( 0 ),
			y( static_cast< T >( 0 )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_A x and y value.
		///
		////////////////////////////////////////////////////////////////
		Vector2( T p_A ) :
			x( p_A ),
			y( p_A )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		///
		////////////////////////////////////////////////////////////////
		Vector2( T p_X, T p_Y ) :
			x( p_A ),
			y( p_A )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 8 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Int8 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 8 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< UInt8 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 16 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Int16 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 16 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< UInt16 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Int32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 bit unsgined integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< UInt32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Int64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 bit unsgined integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< UInt64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 bit floating point vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Float32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 bit floating point vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector2( const Vector2< Float64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) )
		{
		}

		////////////////////////////////////////////////////////////////
		// Operators.
		////////////////////////////////////////////////////////////////
		Vector2< T > & operator = ( const Vector2< T > & p_Vector )
		{
			x = p_Vector.x;
			y = p_Vector.y;
			return *this;
		}

		BIT_BOOL operator == ( const Vector2< T > & p_Vector ) const
		{
			return ( x == p_Vector.x ) && ( y == p_Vector.y );
		}

		BIT_BOOL operator != ( const Vector2< T > & p_Vector ) const
		{
			return ( x != p_Vector.x ) || ( y != p_Vector.y );
		}

		Vector2< T > & operator += ( const Vector2< T > & p_Vector )
		{
			x += p_Vector.x;
			y += p_Vector.y;
			return *this;
		}

		Vector2< T > & operator -= ( const Vector2< T > & p_Vector )
		{
			x -= p_Vector.x;
			y -= p_Vector.y;
			return *this;
		}

		Vector2< T > & operator *= ( const Vector2< T > & p_Vector )
		{
			x *= p_Vector.x;
			y *= p_Vector.y;
			return *this;
		}

		Vector2< T > & operator /= ( const Vector2< T > & p_Vector )
		{
			x /= p_Vector.x;
			y /= p_Vector.y;
			return *this;
		}

		Vector2< T > operator + ( const Vector2< T > & p_Vector ) const
		{
			return Vector2< T >( x + p_Vector.x, y + p_Vector.y );
		}

		Vector2< T > operator - ( ) const
		{
			return Vector2< T >( -x, -y );
		}

		Vector2 <T > operator - ( const Vector2< T > & p_Vector ) const
		{
			return Vector2< T >( x - p_Vector.x, y - p_Vector.y );
		}

		Vector2< T > operator * ( const Vector2< T > & p_Vector ) const
		{
			return Vector2< T >( x * p_Vector.x, y * p_Vector.y );
		}

		Vector2< T > operator * ( const T & p_Value ) const
		{
			return Vector2< T >( x * p_Value, y * p_Value );
		}

		Vector2< T > operator / ( const Vector2< T > & p_Vector ) const
		{
			return Vector2< T >( x / p_Vector.x, y / p_Vector.y );
		}

		Vector2< T > operator / ( const T & p_Value ) const
		{
			return Vector2< T >( x / p_Value, y / p_Value );
		}

		T operator [ ] ( const SizeType & p_Index ) const
		{
			return *( &x + p_Index );
		}

		////////////////////////////////////////////////////////////////
		// Handy functions.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \return The length of the vector.
		///
		////////////////////////////////////////////////////////////////
		Float64 Magnitude( ) const
		{
			return sqrt( static_cast< Float64 >( ( x*x ) + ( y*y ) ) );
		}

		////////////////////////////////////////////////////////////////
		/// \return The normal of the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector2< T > Normal( ) const
		{
			Float64 Length = Magnitude( );

			if( bitIsNan( Length ) )
			{
				return Vector2< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ) );
			}

			return Vector2< T >( ( x / Length ),( y / Length ) );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Normalizing the vector.
		///
		/// \return A reference to the vector.
		///
		////////////////////////////////////////////////////////////////
		const Vector2< T > & Normalize( )
		{
			Float64 Length = Magnitude( );

			if( bitIsNan( Length ) )
			{
				return Vector2< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ) );
			}

			x /= Length;
			y /= Length;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \return The absolute vector.
		///
		////////////////////////////////////////////////////////////////
		Vector2< T > Absolute( ) const
		{
			return Vector2< T >(	(T)abs( (T)x ),
									(T)abs( (T)y ) );
		}

		////////////////////////////////////////////////////////////////
		/// \return The dot value.
		///
		/// \param p_Vector The vector to calculate the dot product with.
		///
		////////////////////////////////////////////////////////////////
		Float64 Dot( const Vector2< T > & p_Vector ) const
		{
			Float64 Dot =	( x * p_Vector.x ) +
							( y * p_Vector.y );

			if( bitIsNan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Rotating the vector.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void Rotate( Float64 p_Angle )
		{
			const Float64 AngleSin = SinDegrees( p_Angle );
			const Float64 AngleCos = CosDegrees( p_Angle );

			// Store the new x and y in tempory variables
			T tx = ( x * AngleCos ) - ( y * AngleSin );
			T ty = ( x * AngleSin ) + ( y * AngleCos );

			x = tx;
			y = ty;
		}

		////////////////////////////////////////////////////////////////
		// Static functions.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the dot product between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		////////////////////////////////////////////////////////////////
		static Float64 Dot( const Vector2< T > & p_Vector1, const Vector2< T > & p_Vector2 )
		{
			Float64 Dot =	( p_Vector1.x * p_Vector2.x ) +
							( p_Vector1.y * p_Vector2.y );

			if( bitIsNan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}


		////////////////////////////////////////////////////////////////
		/// \brief Calculate the angle between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		/// \return The angle in degrees between two vectors.
		///
		////////////////////////////////////////////////////////////////
		static Float64 AngleBetweenVectors( const Vector2< T > & p_Vector1, const Vector2< T > & p_Vector2 )
		{
			Float64 Angle = acos( p_Vector1.Normal( ).Dot( p_Vector2 ) );

			if( bitIsNan( Angle ) )
			{
				return 0.0f;
			}

			return Bit::RadiansToDegrees( Angle );
		}

		////////////////////////////////////////////////////////////////
		// Public variable members
		////////////////////////////////////////////////////////////////
		T x; ///< x-component of vector.
		T y; ///< y-component of vector.

	};

	////////////////////////////////////////////////////////////////
	// Predefined vector types.
	////////////////////////////////////////////////////////////////
	typedef Vector2< Bit::Int8 >	Vector2i8;
	typedef Vector2< Bit::UInt8 >	Vector2u8;
	typedef Vector2< Bit::Int16 >	Vector2i16;
	typedef Vector2< Bit::UInt16 >	Vector2u16;
	typedef Vector2< Bit::Int32 >	Vector2i32;
	typedef Vector2< Bit::UInt32 >	Vector2u32;
	typedef Vector2< Bit::Int64 >	Vector2i64;
	typedef Vector2< Bit::UInt64 >	Vector2u64;
	typedef Vector2< Bit::Float32 >	Vector2f32;
	typedef Vector2< Bit::Float64 >	Vector2f64;

}

#endif
