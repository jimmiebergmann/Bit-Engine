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

#ifndef __BIT_SYSTEM_VECTOR3_HPP__
#define __BIT_SYSTEM_VECTOR3_HPP__

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

		Vector3( T x, T y, T z )
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		// Cast between vectors
		Vector3( const Vector3< BIT_SINT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
			this->z = static_cast< T >( p_Vector.z );
		}
		Vector3( const Vector3< BIT_UINT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
			this->z = static_cast< T >( p_Vector.z );
		}
		Vector3( const Vector3< BIT_FLOAT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
			this->z = static_cast< T >( p_Vector.z );
		}
		Vector3( const Vector3< BIT_FLOAT64 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
			this->z = static_cast< T >( p_Vector.z );
		}

		// Operators
		const Vector3< T > & operator = ( const Vector3< T > p_Vector )
		{
			this->x = p_Vector.x;
			this->y = p_Vector.y;
			this->z = p_Vector.z;
			return *this;
		}

		const Vector3< T > & operator += ( const Vector3< T > p_Vector )
		{
			this->x += p_Vector.x;
			this->y += p_Vector.y;
			this->z += p_Vector.z;
			return *this;
		}

		const Vector3< T > & operator -= ( const Vector3< T > p_Vector )
		{
			this->x -= p_Vector.x;
			this->y -= p_Vector.y;
			this->z -= p_Vector.z;
			return *this;
		}

		const Vector3< T > & operator *= ( const Vector3< T > p_Vector )
		{
			this->x *= p_Vector.x;
			this->y *= p_Vector.y;
			this->z *= p_Vector.z;
			return *this;
		}

		const Vector3< T > & operator /= ( const Vector3< T > p_Vector )
		{
			this->x /= p_Vector.x;
			this->y /= p_Vector.y;
			this->z /= p_Vector.z;
			return *this;
		}

		const Vector3< T > operator + ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( this->x + p_Vector.x, this->y + p_Vector.y, this->z + p_Vector.z );
		}

		const Vector3< T > operator - ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( this->x - p_Vector.x, this->y - p_Vector.y, this->z - p_Vector.z );
		}

		const Vector3< T > operator * ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( this->x * p_Vector.x, this->y * p_Vector.y, this->z * p_Vector.z );
		}

		const Vector3< T > operator * ( const T p_Value ) const
		{
			return Vector3< T >( this->x * p_Value, this->y * p_Value, this->z * p_Value );
		}

		const Vector3< T > operator / ( const Vector3< T > p_Vector ) const
		{
			return Vector3< T >( this->x / p_Vector.x, this->y / p_Vector.y, this->z / p_Vector.z );
		}

		const Vector3< T > operator / ( const T p_Value ) const
		{
			return Vector3< T >( this->x / p_Value, this->y / p_Value, this->z / p_Value );
		}

		T operator [ ] ( const BIT_UINT32 & p_Index )
		{
			return *( &x + p_Index );
		}

		// Useful functions
		const BIT_FLOAT32 Magnitude( ) const
		{
			return sqrt( ( x*x ) + ( y*y ) + ( z*z ) );
		};
		const Vector3< T > Normal( ) const
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector3< T >( static_cast< T >( 0 ),
					static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			return Vector3< T >( ( x / Length ),( y / Length ), ( z / Length ) );
		}

		const Vector3< T > & Normalize( )
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector3< T >( static_cast< T >( 0 ),
					static_cast< T >( 0 ), static_cast< T >( 0 ) );
			}

			this->x /= Length;
			this->y /= Length;
			this->z /= Length;
			return *this;
		}

		const Vector3< T > Absolute( ) const
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

		Vector3< T > Cross(Vector3< T > p_Vector )
		{
			Vector3< T > Dest;
			Dest.x = ( ( y * p_Vector.z ) - ( z * p_Vector.y ) );
			Dest.y = ( ( z * p_Vector.x ) - ( x * p_Vector.z ) );
			Dest.z = ( ( x * p_Vector.y ) - ( y * p_Vector.x ) );
			return Dest;
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
		static Vector3< T > Cross(Vector3< T > p_Vector1, Vector3< T > p_Vector2)
		{
			Vector3< T > Dest;
			Dest.x = ( ( p_Vector1.y * p_Vector2.z ) - ( p_Vector1.z * p_Vector2.y ) );
			Dest.y = ( ( p_Vector1.z * p_Vector2.x ) - ( p_Vector1.x * p_Vector2.z ) );
			Dest.z = ( ( p_Vector1.x * p_Vector2.y ) - ( p_Vector1.y * p_Vector2.x ) );
			return Dest;
		}

		static BIT_FLOAT64 AngleBetweenVectors( Vector3< T > p_Vec1, Vector3< T > p_Vec2 )
		{
			return Bit::RadiansToDegrees( acos( p_Vec1.Dot( p_Vec2 ) ) );
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
