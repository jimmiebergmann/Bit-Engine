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
inline Vector4<T>::Vector4( )
{
}

template <typename T>
inline Vector4<T>::Vector4( const T p_A ) :
	x( p_A ),
	y( p_A ),
	z( p_A ),
	w( p_A )
{
}

template <typename T>
inline Vector4<T>::Vector4( const T p_X, const T p_Y, const T p_Z, const T p_W ) :
	x( p_X ),
	y( p_Y ),
	z( p_Z ),
	w( p_W )
{
}

template <typename T>
template <typename U>
inline Vector4<T>::Vector4( const U p_X, const U p_Y, const U p_Z, const U p_W ) :
	x( static_cast<T>( p_X ) ),
	y( static_cast<T>( p_Y ) ),
	z(static_cast<T>( p_Z ) ),
	w(static_cast<T>( p_W ) )
{
}

template <typename T>
template <typename U>
inline Vector4<T>::Vector4( const Vector3< U > & p_Vector, const U p_W )  :
	x( static_cast< T >( p_Vector.x ) ),
	y( static_cast< T >( p_Vector.y ) ),
	z( static_cast< T >( p_Vector.z ) ),
	w( static_cast< T >( p_W ) )
{
}

template <typename T>
template <typename U>
inline Vector4<T>::Vector4( const Vector4< U > & p_Vector )  :
	x( static_cast< T >( p_Vector.x ) ),
	y( static_cast< T >( p_Vector.y ) ),
	z( static_cast< T >( p_Vector.z ) ),
	w( static_cast< T >( p_Vector.w ) )
{
}

template <typename T>
template <typename U>
inline Vector4<T> & Vector4<T>::operator = ( const Vector4<U> & p_Vector )
{
	x = static_cast< T >( p_Vector.x );
	y = static_cast< T >( p_Vector.y );
	z = static_cast< T >( p_Vector.z );
	w = static_cast< T >( p_Vector.w );
	return *this;
}

template <typename T>
inline Bool Vector4<T>::operator == ( const Vector4<T> & p_Vector ) const
{
	return ( x == p_Vector.x ) && ( y == p_Vector.y ) && ( z == p_Vector.z ) && ( w == p_Vector.w );
}

template <typename T>
inline Bool Vector4<T>::operator != ( const Vector4<T> & p_Vector ) const
{
	return ( x != p_Vector.x ) || ( y != p_Vector.y ) || ( z != p_Vector.z ) || ( w != p_Vector.w );
}

template <typename T>
inline Vector4<T> & Vector4<T>::operator += ( const Vector4<T> & p_Vector )
{
	x += p_Vector.x;
	y += p_Vector.y;
	z += p_Vector.z;
	w += p_Vector.w;
	return *this;
}

template <typename T>
inline Vector4<T> & Vector4<T>::operator -= ( const Vector4<T> & p_Vector )
{
	x -= p_Vector.x;
	y -= p_Vector.y;
	z -= p_Vector.z;
	w -= p_Vector.w;
	return *this;
}

template <typename T>
inline Vector4<T> & Vector4<T>::operator *= ( const Vector4<T> & p_Vector )
{
	x *= p_Vector.x;
	y *= p_Vector.y;
	z *= p_Vector.z;
	w *= p_Vector.w;
	return *this;
}

template <typename T>
inline Vector4<T> & Vector4<T>::operator /= ( const Vector4<T> & p_Vector )
{
	x /= p_Vector.x;
	y /= p_Vector.y;
	z /= p_Vector.z;
	w /= p_Vector.w;
	return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::operator + ( const Vector4<T> & p_Vector ) const
{
	return Vector4<T>( x + p_Vector.x, y + p_Vector.y, z + p_Vector.z, w + p_Vector.w );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator - ( ) const
{
	return Vector4<T>( -x, -y, -z, -w );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator - ( const Vector4<T> & p_Vector ) const
{
	return Vector4<T>( x - p_Vector.x, y - p_Vector.y, z - p_Vector.z, w - p_Vector.w );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator * ( const Vector4<T> & p_Vector ) const
{
	return Vector4<T>( x * p_Vector.x, y * p_Vector.y, z * p_Vector.z, w * p_Vector.w );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator * ( const T & p_Value ) const
{
	return Vector4<T>( x * p_Value, y * p_Value, z * p_Value, w * p_Value );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator / ( const Vector4<T> & p_Vector ) const
{
	return Vector4<T>( x / p_Vector.x, y / p_Vector.y, z / p_Vector.z, w / p_Vector.w );
}

template <typename T>
inline Vector4<T> Vector4<T>::operator / ( const T & p_Value ) const
{
	return Vector4<T>( x / p_Value, y / p_Value, z / p_Value, w / p_Value );
}

template <typename T>
inline T Vector4<T>::operator [ ] ( const SizeType & p_Index ) const
{
	return *( &x + p_Index );
}

template <typename T>
inline Float64 Vector4<T>::Length( ) const
{
	return sqrt( ( x*x ) + ( y*y ) + ( z*z ) + ( w*w ) );
}

template <typename T>
inline Vector4<T> Vector4<T>::Normal( ) const
{
	Float64 length = Length( );

	if( bitIsNan( length ) )
	{
		return Vector4<T>(	static_cast< T >( 0 ),
							static_cast< T >( 0 ),
							static_cast< T >( 0 ),
							static_cast< T >( 0 ) );
	}

	return Vector4<T>(	static_cast<T>(	static_cast<Float64>( x ) / length ),
						static_cast<T>( static_cast<Float64>( y ) / length ),
						static_cast<T>( static_cast<Float64>( z ) / length ),
						static_cast<T>( static_cast<Float64>( w ) / length ) );
}

template <typename T>
inline Vector4<T> & Vector4<T>::Normalize( )
{
	T length = static_cast<T>( Length( ) );

	if( bitIsNan( length ) )
	{
		x = y = z = w = static_cast< T >( 0 );
		return *this;
	}

	x /= length;
	y /= length;
	z /= length;
	w /= length;
	return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::Absolute( ) const
{
	return Vector4<T>(	(T)std::abs( (T)x ),
                        (T)std::abs( (T)y ),
                        (T)std::abs( (T)z ),
                        (T)std::abs( (T)w ));
}
