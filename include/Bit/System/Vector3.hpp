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

#include <Bit/DataTypes.hpp>
#include <Bit/System/Math.hpp>
#include <cmath>
#include <float.h>

#ifdef BIT_PLATFORM_WIN32
	#define isnan _isnan
#endif

namespace Bit
{

	template <typename T>
	class Vector3
	{

	public:

		Vector3( )
		{
		}

		Vector3( T p_A )
		{
			x = p_A;
			y = p_A;
			z = p_A;
		}

		Vector3( T p_X, T p_Y, T p_Z )
		{
			x = p_X;
			y = p_Y;
			z = p_Z;
		}

		// Cast between vectors
		Vector3( const Vector3< BIT_SCHAR8 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		Vector3( const Vector3< BIT_UCHAR8 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		Vector3( const Vector3< BIT_SINT32 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		Vector3( const Vector3< BIT_UINT32 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		Vector3( const Vector3< BIT_FLOAT32 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		Vector3( const Vector3< BIT_FLOAT64 > & p_Vector )
		{
			x = static_cast< T >( p_Vector.x );
			y = static_cast< T >( p_Vector.y );
			z = static_cast< T >( p_Vector.z );
		}

		// Operators
		Vector3< T > & operator = ( const Vector3< T > p_Vector )
		{
			x = p_Vector.x;
			y = p_Vector.y;
			z = p_Vector.z;
			return *this;
		}

		BIT_BOOL operator == ( const Vector2< T > p_Vector ) const
		{
			return ( x == p_Vector.x ) && ( y == p_Vector.y ) && ( z == p_Vector.z );
		}

		BIT_BOOL operator != ( const Vector2< T > p_Vector ) const
		{
			return ( x != p_Vector.x ) || ( y != p_Vector.y ) || ( z!= p_Vector.z );
		}

		Vector3< T > & operator += ( const Vector3< T > p_Vector )
		{
			x += p_Vector.x;
			y += p_Vector.y;
			z += p_Vector.z;
			return *this;
		}

		Vector3< T > & operator -= ( const Vector3< T > p_Vector )
		{
			x -= p_Vector.x;
			y -= p_Vector.y;
			z -= p_Vector.z;
			return *this;
		}

		Vector3< T > & operator *= ( const Vector3< T > p_Vector )
		{
			x *= p_Vector.x;
			y *= p_Vector.y;
			z *= p_Vector.z;
			return *this;
		}

		Vector3< T > & operator /= ( const Vector3< T > p_Vector )
		{
			x /= p_Vector.x;
			y /= p_Vector.y;
			z /= p_Vector.z;
			return *this;
		}

		Vector3< T > operator + ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( x + p_Vector.x, y + p_Vector.y, z + p_Vector.z );
		}

		Vector3< T > operator - ( ) const
		{
			return Vector3< T >( -x, -y, -z );
		}

		Vector3< T > operator - ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( x - p_Vector.x, y - p_Vector.y, z - p_Vector.z );
		}

		Vector3< T > operator * ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( x * p_Vector.x, y * p_Vector.y, z * p_Vector.z );
		}

		Vector3< T > operator * ( const T p_Value ) const
		{
			return Vector3< T >( x * p_Value, y * p_Value, z * p_Value );
		}

		Vector3< T > operator / ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( x / p_Vector.x, y / p_Vector.y, z / p_Vector.z );
		}

		const Vector3< T > operator / ( const T p_Value ) const
		{
			return Vector3< T >( x / p_Value, y / p_Value, z / p_Value );
		}

		T operator [ ] ( const BIT_UINT32 & p_Index ) const
		{
			return *( &x + p_Index );
		}

		// Useful functions
		BIT_FLOAT32 Magnitude( ) const
		{
			return sqrt( ( x*x ) + ( y*y ) + ( z*z ) );
		}

		Vector3< T > Normal( ) const
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector3< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			return Vector3< T >( ( x / Length ),( y / Length ), ( z / Length ) );
		}

		Vector3< T > & Normalize( )
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector3< T >(	static_cast< T >( 0 ),
										static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			x /= Length;
			y /= Length;
			z /= Length;
			return *this;
		}

		Vector3< T > Absolute( ) const
		{
			return Vector3< T >((T)abs((T)x), (T)abs((T)y), (T)abs((T)z));
		}

		BIT_FLOAT32 Dot( const Vector3< T > & v ) const
		{
			BIT_FLOAT32 Dot = ( x * v.x ) + ( y * v.y ) + ( z * v.z );
			if( isnan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		Vector3< T > Cross(Vector3< T > p_Vector ) const
		{
			Vector3< T > CrossVector;
			CrossVector.x = ( ( y * p_Vector.z ) - ( z * p_Vector.y ) );
			CrossVector.y = ( ( z * p_Vector.x ) - ( x * p_Vector.z ) );
			CrossVector.z = ( ( x * p_Vector.y ) - ( y * p_Vector.x ) );
			return CrossVector;
		}

		// Do not use, not tested yet.
		void Rotate( const BIT_FLOAT32 p_Angle, const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z )
		{
			// Create a rotation quaternion
			BIT_FLOAT32 qx = p_X * Bit::SinDegrees( p_Angle / 2.0f );
			BIT_FLOAT32 qy = p_Y * Bit::SinDegrees( p_Angle / 2.0f );
			BIT_FLOAT32 qz = p_Z * Bit::SinDegrees(  p_Angle / 2.0f );
			BIT_FLOAT32 qw = Bit::CosDegrees( p_Angle / 2.0f );

			// Convert our vector into a quaternion
			BIT_FLOAT32 vx = x;
			BIT_FLOAT32 vy = y;
			BIT_FLOAT32 vz = z;
			BIT_FLOAT32 vw = 0.0f;

			// Quaternion multiplication(ignore the w component)
			x = vy * qz - vz * qy + vw * qx + vx * qw;
			y = vz * qx - vx * qz + vw * qy + vy * qw;
			z = vx * qy - vy * qx + vw * qz + vz * qw;
		}

		void RotateX( BIT_FLOAT64 p_Angle )
		{
			const BIT_FLOAT32 AngleSin = SinDegrees( p_Angle );
			const BIT_FLOAT32 AngleCos = CosDegrees( p_Angle );

			// Store the new y and z in tempory variables
			T ty = ( y * AngleCos ) - ( z * AngleSin );
			T tz = ( y * AngleSin ) + ( z * AngleCos );

			y = ty;
			z = tz;
		}

		void RotateY( BIT_FLOAT64 p_Angle )
		{
			const BIT_FLOAT32 AngleSin = SinDegrees( p_Angle );
			const BIT_FLOAT32 AngleCos = CosDegrees( p_Angle );

			// Store the new x and z in tempory variables
			T tx = ( x * AngleCos ) - ( z * AngleSin );
			T tz = ( x * AngleSin ) + ( z * AngleCos );

			x = tx;
			z = tz;
		}

		void RotateZ( BIT_FLOAT64 p_Angle )
		{
			const BIT_FLOAT32 AngleSin = SinDegrees( p_Angle );
			const BIT_FLOAT32 AngleCos = CosDegrees( p_Angle );

			// Store the new x and y in tempory variables
			T tx = ( x * AngleCos ) - ( y * AngleSin );
			T ty = ( x * AngleSin ) + ( y * AngleCos );

			x = tx;
			y = ty;
		}


		// Static functions
		static BIT_FLOAT32 Dot( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2 )
		{
			BIT_FLOAT32 Dot =	( p_Vector1.x * p_Vector2.x ) +
								( p_Vector1.y * p_Vector2.y ) +
								( p_Vector1.z * p_Vector2.z );
			if( isnan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		static Vector3< T > Cross( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2)
		{
			Vector3< T > Dest;
			Dest.x = ( ( p_Vector1.y * p_Vector2.z ) - ( p_Vector1.z * p_Vector2.y ) );
			Dest.y = ( ( p_Vector1.z * p_Vector2.x ) - ( p_Vector1.x * p_Vector2.z ) );
			Dest.z = ( ( p_Vector1.x * p_Vector2.y ) - ( p_Vector1.y * p_Vector2.x ) );
			return Dest;
		}

		static BIT_FLOAT64 AngleBetweenVectors( const Vector3< T > & p_Vector1, const Vector3< T > & p_Vector2 )
		{
			BIT_FLOAT64 Angle = acos( p_Vector1.Normal( ).Dot( p_Vector2 ) );

			if( isnan( Angle ) )
			{
				return 0.0f;
			}

			return Bit::RadiansToDegrees( Angle );
		}


		// Public variable members
		T x;
		T y;
		T z;

	};

	// Predefined vector types
	typedef Vector3< BIT_BYTE >		Vector3_byte;
	typedef Vector3< BIT_SCHAR8 >	Vector3_sc8;
	typedef Vector3< BIT_UCHAR8 >	Vector3_uc8;
	typedef Vector3< BIT_SINT32 >	Vector3_si32;
	typedef Vector3< BIT_UINT32 >	Vector3_ui32;
	typedef Vector3< BIT_FLOAT32 >	Vector3_f32;
	typedef Vector3< BIT_FLOAT64 >	Vector3_f64;

}

#endif