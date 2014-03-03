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
		// Constructors.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Vector3( ) :
			x( static_cast< T >( 0 ) ),
			y( static_cast< T >( 0 ) ),
			z( static_cast< T >( 0 ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_A x, y and z value.
		///
		////////////////////////////////////////////////////////////////
		Vector3( T p_A ) :
			x( p_A ),
			y( p_A ),
			z( p_A )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_X x value.
		/// \param p_Y y value.
		/// \param p_Z z value.
		///
		////////////////////////////////////////////////////////////////
		Vector3( T p_X, T p_Y, T p_Z ) :
			x( p_X ),
			y( p_Y ),
			z( p_Z )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 8 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Int8 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 8 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< UInt8 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 16 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Int16 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 16 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< UInt16 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Int32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< UInt32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 bit signed integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Int64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 bit unsigned integer vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< UInt64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 32 floating point vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Float32 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Copy constructor.
		///
		/// \param p_Vector 64 floating point vector to copy.
		///
		////////////////////////////////////////////////////////////////
		Vector3( const Vector3< Float64 > & p_Vector ) :
			x( static_cast< T >( p_Vector.x ) ),
			y( static_cast< T >( p_Vector.y ) ),
			z( static_cast< T >( p_Vector.z ) )
		{
		}

		////////////////////////////////////////////////////////////////
		// Operators.
		////////////////////////////////////////////////////////////////
		Vector3< T > & operator = ( const Vector3< T > & p_Vector )
		{
			x = p_Vector.x;
			y = p_Vector.y;
			z = p_Vector.z;
			return *this;
		}

		Bool operator == ( const Vector3< T > & p_Vector ) const
		{
			return ( x == p_Vector.x ) && ( y == p_Vector.y ) && ( z == p_Vector.z );
		}

		Bool operator != ( const Vector3< T > & p_Vector ) const
		{
			return ( x != p_Vector.x ) || ( y != p_Vector.y ) || ( z!= p_Vector.z );
		}

		Vector3< T > & operator += ( const Vector3< T > & p_Vector )
		{
			x += p_Vector.x;
			y += p_Vector.y;
			z += p_Vector.z;
			return *this;
		}

		Vector3< T > & operator -= ( const Vector3< T > & p_Vector )
		{
			x -= p_Vector.x;
			y -= p_Vector.y;
			z -= p_Vector.z;
			return *this;
		}

		Vector3< T > & operator *= ( const Vector3< T > & p_Vector )
		{
			x *= p_Vector.x;
			y *= p_Vector.y;
			z *= p_Vector.z;
			return *this;
		}

		Vector3< T > & operator /= ( const Vector3< T > & p_Vector )
		{
			x /= p_Vector.x;
			y /= p_Vector.y;
			z /= p_Vector.z;
			return *this;
		}

		Vector3< T > operator + ( const Vector3< T > & p_Vector ) const
		{
			return Vector3< T >( x + p_Vector.x, y + p_Vector.y, z + p_Vector.z );
		}

		Vector3< T > operator - ( ) const
		{
			return Vector3< T >( -x, -y, -z );
		}

		Vector3< T > operator - ( const Vector3< T > & p_Vector ) const
		{
			return Vector3< T >( x - p_Vector.x, y - p_Vector.y, z - p_Vector.z );
		}

		Vector3< T > operator * ( const Vector3< T > & p_Vector ) const
		{
			return Vector3< T >( x * p_Vector.x, y * p_Vector.y, z * p_Vector.z );
		}

		Vector3< T > operator * ( const T & p_Value ) const
		{
			return Vector3< T >( x * p_Value, y * p_Value, z * p_Value );
		}

		Vector3< T > operator / ( const Vector3< T > & p_Vector ) const
		{
			return Vector3< T >( x / p_Vector.x, y / p_Vector.y, z / p_Vector.z );
		}

		const Vector3< T > operator / ( const T & p_Value ) const
		{
			return Vector3< T >( x / p_Value, y / p_Value, z / p_Value );
		}

		T operator [ ] ( const UInt32 & p_Index ) const
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
			return sqrt( ( x*x ) + ( y*y ) + ( z*z ) );
		}

		////////////////////////////////////////////////////////////////
		/// \return The normal of the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3< T > Normal( ) const
		{
			Float64 Length = Magnitude( );

			if( bitIsNan( Length ) )
			{
				return Vector3< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			return Vector3< T >( ( x / Length ),( y / Length ), ( z / Length ) );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Normalizing the vector.
		///
		/// \return A reference to the vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3< T > & Normalize( )
		{
			Float64 Length = Magnitude( );

			if( bitIsNan( Length ) )
			{
				return Vector3< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			x /= Length;
			y /= Length;
			z /= Length;
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \return The absolute vector.
		///
		////////////////////////////////////////////////////////////////
		Vector3< T > Absolute( ) const
		{
			return Vector3< T >((T)abs((T)x), (T)abs((T)y), (T)abs((T)z));
		}

		////////////////////////////////////////////////////////////////
		/// \return The dot value.
		///
		/// \param p_Vector The vector to calculate the dot product with.
		///
		////////////////////////////////////////////////////////////////
		Float64 Dot( const Vector3< T > & p_Vector ) const
		{
			Float64 Dot =	( x * p_Vector.x ) +
							( y * p_Vector.y ) +
							( z * p_Vector.z );

			if( bitIsNan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		////////////////////////////////////////////////////////////////
		/// \return The cross vector.
		///
		/// \param p_Vector The vector to calculate the cross product with.
		///
		////////////////////////////////////////////////////////////////
		Vector3< T > Cross(Vector3< T > p_Vector ) const
		{
			Vector3< T > CrossVector;
			CrossVector.x = ( ( y * p_Vector.z ) - ( z * p_Vector.y ) );
			CrossVector.y = ( ( z * p_Vector.x ) - ( x * p_Vector.z ) );
			CrossVector.z = ( ( x * p_Vector.y ) - ( y * p_Vector.x ) );
			return CrossVector;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Do not use this yet.
		///
		/// \param p_Angle The angle in degrees to rotate.
		/// \param p_X The amount of x-component to rotate.
		/// \param p_Y The amount of y-component to rotate.
		/// \param p_Z The amount of z-component to rotate.
		///
		////////////////////////////////////////////////////////////////
		void Rotate( const Float64 p_Angle, const Float64 p_X, const Float64 p_Y, const Float64 p_Z )
		{
			// Create a rotation quaternion
			Float64 qx = p_X * Math::SinDegrees( p_Angle / 2.0f );
			Float64 qy = p_Y * SinDegrees( p_Angle / 2.0f );
			Float64 qz = p_Z * SinDegrees(  p_Angle / 2.0f );
			Float64 qw = CosDegrees( p_Angle / 2.0f );

			// Convert our vector into a quaternion
			Float64 vx = x;
			Float64 vy = y;
			Float64 vz = z;
			Float64 vw = 0.0f;

			// Quaternion multiplication(ignore the w component)
			x = vy * qz - vz * qy + vw * qx + vx * qw;
			y = vz * qx - vx * qz + vw * qy + vy * qw;
			z = vx * qy - vy * qx + vw * qz + vz * qw;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the x-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateX( const Float64 p_Angle )
		{
			const Float64 AngleSin = SinDegrees( p_Angle );
			const Float64 AngleCos = CosDegrees( p_Angle );

			// Store the new y and z in tempory variables
			T ty = ( y * AngleCos ) - ( z * AngleSin );
			T tz = ( y * AngleSin ) + ( z * AngleCos );

			y = ty;
			z = tz;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the y-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateY( const Float64 p_Angle )
		{
			const Float64 AngleSin = SinDegrees( p_Angle );
			const Float64 AngleCos = CosDegrees( p_Angle );

			// Store the new x and z in tempory variables
			T tx = ( x * AngleCos ) - ( z * AngleSin );
			T tz = ( x * AngleSin ) + ( z * AngleCos );

			x = tx;
			z = tz;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Rotate the vector about the z-axis.
		///
		/// \param p_Angle The angle in degrees to rotate.
		///
		////////////////////////////////////////////////////////////////
		void RotateZ( const Float64 p_Angle )
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
		static Float64 Dot( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2 )
		{
			Float64 Dot =	( p_Vector1.x * p_Vector2.x ) +
							( p_Vector1.y * p_Vector2.y ) +
							( p_Vector1.z * p_Vector2.z );
			if( bitIsNan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		////////////////////////////////////////////////////////////////
		// Static functions.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Calculate the cross product between two vectors.
		///
		/// \param p_Vector1 The first vector.
		/// \param p_Vector2 The second vector.
		///
		////////////////////////////////////////////////////////////////
		static Vector3< T > Cross( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2)
		{
			Vector3< T > Dest;
			Dest.x = ( ( p_Vector1.y * p_Vector2.z ) - ( p_Vector1.z * p_Vector2.y ) );
			Dest.y = ( ( p_Vector1.z * p_Vector2.x ) - ( p_Vector1.x * p_Vector2.z ) );
			Dest.z = ( ( p_Vector1.x * p_Vector2.y ) - ( p_Vector1.y * p_Vector2.x ) );
			return Dest;
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
		static Float64 AngleBetweenVectors( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2 )
		{
			Float64 Angle = acos( p_Vector1.Normal( ).Dot( p_Vector2 ) );

			if( bitIsNan( Angle ) )
			{
				return 0.0f;
			}

			return RadiansToDegrees( Angle );
		}

		////////////////////////////////////////////////////////////////
		// Public variable members
		////////////////////////////////////////////////////////////////
		T x; ///< x-component of vector.
		T y; ///< y-component of vector.
		T z; ///< z-component of vector.

	};

	////////////////////////////////////////////////////////////////
	// Predefined vector types.
	////////////////////////////////////////////////////////////////
	typedef Vector3< Int8 >		Vector3i8;
	typedef Vector3< UInt8 >	Vector3u8;
	typedef Vector3< Int16 >	Vector3i16;
	typedef Vector3< UInt16 >	Vector3u16;
	typedef Vector3< Int32 >	Vector3i32;
	typedef Vector3< UInt32 >	Vector3u32;
	typedef Vector3< Int64 >	Vector3i64;
	typedef Vector3< UInt64 >	Vector3u64;
	typedef Vector3< Float32 >	Vector3f32;
	typedef Vector3< Float64 >	Vector3f64;

}

#endif