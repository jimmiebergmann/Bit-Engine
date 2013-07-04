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


#ifndef __BIT_SYSTEM_VECTOR2_HPP__
#define __BIT_SYSTEM_VECTOR2_HPP__

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
	class Vector2
	{

	public:

		Vector2( )
		{
		}

		Vector2( T a )
		{
			this->x = a;
			this->y = a;
		}

		Vector2( T x, T y )
		{
			this->x = x;
			this->y = y;
		}

		// Cast between vectors
		Vector2( const Vector2< BIT_SINT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
		}
		Vector2( const Vector2< BIT_UINT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
		}
		Vector2( const Vector2< BIT_FLOAT32 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
		}
		Vector2( const Vector2< BIT_FLOAT64 > & p_Vector )
		{
			this->x = static_cast< T >( p_Vector.x );
			this->y = static_cast< T >( p_Vector.y );
		}

		// Operators
		const Vector2< T > & operator = ( const Vector2< T > p_Vector )
		{
			this->x = p_Vector.x;
			this->y = p_Vector.y;
			return *this;
		}

		BIT_BOOL operator == ( const Vector2< T > p_Vector )
		{
			return ( this->x == p_Vector.x ) && ( this->y == p_Vector.y );
		}

		BIT_BOOL operator != ( const Vector2< T > p_Vector )
		{
			return ( this->x != p_Vector.x ) || ( this->y != p_Vector.y );
		}

		const Vector2< T > & operator += ( const Vector2< T > p_Vector )
		{
			this->x += p_Vector.x;
			this->y += p_Vector.y;
			return *this;
		}

		const Vector2< T > & operator -= ( const Vector2< T > p_Vector )
		{
			this->x -= p_Vector.x;
			this->y -= p_Vector.y;
			return *this;
		}

		const Vector2< T > & operator *= ( const Vector2< T > p_Vector )
		{
			this->x *= p_Vector.x;
			this->y *= p_Vector.y;
			return *this;
		}

		const Vector2< T > & operator /= ( const Vector2< T > p_Vector )
		{
			this->x /= p_Vector.x;
			this->y /= p_Vector.y;
			return *this;
		}

		const Vector2< T > operator + ( const Vector2< T > p_Vector ) const
		{
			return Vector2< T >( this->x + p_Vector.x, this->y + p_Vector.y );
		}

		const Vector2 <T > operator - ( const Vector2< T > p_Vector ) const
		{
			return Vector2< T >( this->x - p_Vector.x, this->y - p_Vector.y );
		}

		const Vector2< T > operator * ( const Vector2< T > p_Vector ) const
		{
			return Vector2< T >( this->x * p_Vector.x, this->y * p_Vector.y );
		}

		const Vector2< T > operator * ( const T p_Value ) const
		{
			return Vector2< T >( this->x * p_Value, this->y * p_Value );
		}

		const Vector2< T > operator / ( const Vector2< T > p_Vector ) const
		{
			return Vector2< T >( this->x / p_Vector.x, this->y / p_Vector.y );
		}

		const Vector2< T > operator / ( const T p_Value ) const
		{
			return Vector2< T >( this->x / p_Value, this->y / p_Value );
		}

		T operator [ ] ( const BIT_UINT32 & p_Index )
		{
			return *( &x + p_Index );
		}

		// Useful functions
		const BIT_FLOAT32 Magnitude( ) const
		{
			return sqrt( static_cast< BIT_FLOAT32 >( ( x*x ) + ( y*y ) ) );
		};

		const Vector2< T > Normal( ) const
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector2< T >( static_cast< T >( 0 ),
					static_cast< T >( 0 ) );
			}

			return Vector2< T >( ( x / Length ),( y / Length ) );
		}

		const Vector2< T > & Normalize( )
		{
			float Length = Magnitude( );

			if( isnan( Length ) )
			{
				return Vector2< T >( static_cast< T >( 0 ),
					static_cast< T >( 0 ) );
			}

			this->x /= Length;
			this->y /= Length;
			return *this;
		}

		const Vector2< T > Absolute( ) const
		{
			return Vector2< T >((T)abs((T)x), (T)abs((T)y) );
		}

		BIT_FLOAT32 Dot( const Vector2< T > & v ) const
		{
			BIT_FLOAT32 Dot = ( x * v.x ) + ( y * v.y );
			if( isnan( Dot ) )
			{
				return 0.0f;
			}

			return Dot;
		}

		void Rotate( BIT_FLOAT32 p_Angle )
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
		static BIT_FLOAT64 AngleBetweenVectors( Vector2< T > p_Vec1, Vector2< T > p_Vec2 )
		{
			return Bit::RadiansToDegrees( acos( p_Vec1.Dot( p_Vec2 ) ) );
		}


		// Public variable members
		T x;
		T y;

	};

	// Predefined vector types
	typedef Vector2< BIT_BYTE >		Vector2_byte;
	typedef Vector2< BIT_SCHAR8 >	Vector2_sc8;
	typedef Vector2< BIT_UCHAR8 >	Vector2_uc8;
	typedef Vector2< BIT_SINT32 >	Vector2_si32;
	typedef Vector2< BIT_UINT32 >	Vector2_ui32;
	typedef Vector2< BIT_FLOAT32 >	Vector2_f32;
	typedef Vector2< BIT_FLOAT64 >	Vector2_f64;

}

#endif
