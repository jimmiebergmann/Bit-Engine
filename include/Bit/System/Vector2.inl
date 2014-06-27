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

template <typename T>
inline Vector2<T>::Vector2( )
{
}

template <typename T>
inline Vector2<T>::Vector2( T p_A ) :
	x( p_A ),
	y( p_A )
{
}

template <typename T>
inline Vector2<T>::Vector2( T p_X, T p_Y ) :
	x( p_X ),
	y( p_Y )
{
}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2( U p_X, U p_Y ) :
	x( static_cast<T>( p_X ) ),
	y( static_cast<T>( p_Y ) )
{
}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2( const Vector2<U> & p_Vector )  :
	x( static_cast< T >( p_Vector.x ) ),
	y( static_cast< T >( p_Vector.y ) )
{
}

template <typename T>
inline Vector2<T> & Vector2<T>::operator = ( const Vector2<T> & p_Vector )
{
	x = p_Vector.x;
	y = p_Vector.y;
	return *this;
}

template <typename T>
inline Bool Vector2<T>::operator == ( const Vector2<T> & p_Vector ) const
{
	return ( x == p_Vector.x ) && ( y == p_Vector.y );
}

template <typename T>
inline Bool Vector2<T>::operator != ( const Vector2<T> & p_Vector ) const
{
	return ( x != p_Vector.x ) || ( y != p_Vector.y );
}

template <typename T>
inline Vector2<T> & Vector2<T>::operator += ( const Vector2<T> & p_Vector )
{
	x += p_Vector.x;
	y += p_Vector.y;
	return *this;
}

template <typename T>
inline Vector2<T> & Vector2<T>::operator -= ( const Vector2<T> & p_Vector )
{
	x -= p_Vector.x;
	y -= p_Vector.y;
	return *this;
}

template <typename T>
inline Vector2<T> & Vector2<T>::operator *= ( const Vector2<T> & p_Vector )
{
	x *= p_Vector.x;
	y *= p_Vector.y;
	return *this;
}

template <typename T>
inline Vector2<T> & Vector2<T>::operator /= ( const Vector2<T> & p_Vector )
{
	x /= p_Vector.x;
	y /= p_Vector.y;
	return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::operator + ( const Vector2<T> & p_Vector ) const
{
	return Vector2<T>( x + p_Vector.x, y + p_Vector.y );
}

template <typename T>
inline Vector2<T> Vector2<T>::operator - ( ) const
{
	return Vector2<T>( -x, -y );
}

template <typename T>
inline Vector2 <T > Vector2<T>::operator - ( const Vector2<T> & p_Vector ) const
{
	return Vector2<T>( x - p_Vector.x, y - p_Vector.y );
}

template <typename T>
inline Vector2<T> Vector2<T>::operator * ( const Vector2<T> & p_Vector ) const
{
	return Vector2<T>( x * p_Vector.x, y * p_Vector.y );
}

template <typename T>
template <typename U>
inline Vector2<T> Vector2<T>::operator * ( const U & p_Value ) const
{
	return Vector2<T>(	x * static_cast<T>( p_Value ),
						y * static_cast<T>( p_Value ) );
}

template <typename T>
inline Vector2<T> Vector2<T>::operator / ( const Vector2<T> & p_Vector ) const
{
	return Vector2<T>( x / p_Vector.x, y / p_Vector.y );
}

template <typename T>
template <typename U>
inline Vector2< T > Vector2< T >::operator / ( const U & p_Value ) const
{
	return Vector2<T>(	x / static_cast<T>( p_Value),
						y / static_cast<T>( p_Value ) );
}

template <typename T>
inline T Vector2<T>::operator [ ] ( const SizeType & p_Index ) const
{
	return *( &x + p_Index );
}

template <typename T>
inline Float64 Vector2<T>::Length( ) const
{
	return sqrt( static_cast< Float64 >( ( x*x ) + ( y*y ) ) );
}

template <typename T>
inline Vector2<T> Vector2<T>::Normal( ) const
{
	Float64 length = Length( );

	if( bitIsNan( length ) )
	{
		return Vector2<T>(	static_cast< T >( 0 ),
								static_cast< T >( 0 ) );
	}

	return Vector2<T>( ( x / length ),( y / length ) );
}

template <typename T>
inline const Vector2<T> & Vector2<T>::Normalize( )
{
	T length = static_cast<T>( Length( ) );

	if( bitIsNan( length ) )
	{
		x = y = static_cast< T >( 0 );
		return *this;
	}

	x /= length;
	y /= length;
	return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::Absolute( ) const
{
	return Vector2<T>(	(T)std::abs( (T)x ),
						(T)std::abs( (T)y ) );
}

template <typename T>
inline Float64 Vector2<T>::Dot( const Vector2<T> & p_Vector ) const
{
	Float64 Dot =	( x * p_Vector.x ) +
					( y * p_Vector.y );

	if( bitIsNan( Dot ) )
	{
		return 0.0f;
	}

	return Dot;
}

template <typename T>
inline void Vector2<T>::Rotate( const Angle & p_Angle )
{
	const Float64 sin = Math::Sin<Float64>( p_Angle.AsRadians( ) );
	const Float64 cos = Math::Cos<Float64>( p_Angle.AsRadians( ) );

	// Store the new x and y in tempory variables
	const Float64 tx = ( static_cast<Float64>( x ) * cos ) - ( static_cast<Float64>( y ) * sin );
	const Float64 ty = ( static_cast<Float64>( x ) * sin ) + ( static_cast<Float64>( y ) * cos );

	x = static_cast<T>( tx );
	y = static_cast<T>( ty );
}

template <typename T>
inline Float64 Vector2<T>::Dot( const Vector2<T> & p_Vector1, const Vector2<T> & p_Vector2 )
{
	Float64 dot =	( p_Vector1.x * p_Vector2.x ) +
					( p_Vector1.y * p_Vector2.y );

	if( bitIsNan( dot ) )
	{
		return 0.0f;
	}

	return dot;
}

template <typename T>
inline Angle Vector2<T>::AngleBetweenVectors( const Vector2<T> & p_Vector1, const Vector2<T> & p_Vector2 )
{
	Float64 angleRad = acos( p_Vector1.Normal( ).Dot( p_Vector2 ) );

	if( bitIsNan( angleRad ) )
	{
		return Radians( 0.0f );
	}

	return Radians( angleRad );
}
