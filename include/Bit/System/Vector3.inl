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
inline Vector3<T>::Vector3( )
{
}

template <typename T>
inline Vector3<T>::Vector3( T p_A ) :
	x( p_A ),
	y( p_A ),
	z( p_A )
{
}

template <typename T>
inline Vector3<T>::Vector3( T p_X, T p_Y, T p_Z ) :
	x( p_X ),
	y( p_Y ),
	z( p_Z )
{
}

template <typename T>
template <typename U>
inline Vector3<T>::Vector3( U p_X, U p_Y, U p_Z ) :
	x( static_cast<T>( p_X ) ),
	y( static_cast<T>( p_Y ) ),
	z(static_cast<T>(  p_Z ) )
{
}

template <typename T>
template <typename U>
inline Vector3<T>::Vector3( const Vector3< U > & p_Vector )  :
	x( static_cast< T >( p_Vector.x ) ),
	y( static_cast< T >( p_Vector.y ) ),
	z( static_cast< T >( p_Vector.z ) )
{
}

template <typename T>
inline Vector3<T> & Vector3<T>::operator = ( const Vector3<T> & p_Vector )
{
	x = p_Vector.x;
	y = p_Vector.y;
	z = p_Vector.z;
	return *this;
}

template <typename T>
inline Bool Vector3<T>::operator == ( const Vector3<T> & p_Vector ) const
{
	return ( x == p_Vector.x ) && ( y == p_Vector.y ) && ( z == p_Vector.z );
}

template <typename T>
inline Bool Vector3<T>::operator != ( const Vector3<T> & p_Vector ) const
{
	return ( x != p_Vector.x ) || ( y != p_Vector.y ) || ( z!= p_Vector.z );
}

template <typename T>
inline Vector3<T> & Vector3<T>::operator += ( const Vector3<T> & p_Vector )
{
	x += p_Vector.x;
	y += p_Vector.y;
	z += p_Vector.z;
	return *this;
}

template <typename T>
inline Vector3<T> & Vector3<T>::operator -= ( const Vector3<T> & p_Vector )
{
	x -= p_Vector.x;
	y -= p_Vector.y;
	z -= p_Vector.z;
	return *this;
}

template <typename T>
inline Vector3<T> & Vector3<T>::operator *= ( const Vector3<T> & p_Vector )
{
	x *= p_Vector.x;
	y *= p_Vector.y;
	z *= p_Vector.z;
	return *this;
}

template <typename T>
inline Vector3<T> & Vector3<T>::operator /= ( const Vector3<T> & p_Vector )
{
	x /= p_Vector.x;
	y /= p_Vector.y;
	z /= p_Vector.z;
	return *this;
}

template <typename T>
inline Vector3<T> Vector3<T>::operator + ( const Vector3<T> & p_Vector ) const
{
	return Vector3<T>( x + p_Vector.x, y + p_Vector.y, z + p_Vector.z );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator - ( ) const
{
	return Vector3<T>( -x, -y, -z );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator - ( const Vector3<T> & p_Vector ) const
{
	return Vector3<T>( x - p_Vector.x, y - p_Vector.y, z - p_Vector.z );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator * ( const Vector3<T> & p_Vector ) const
{
	return Vector3<T>( x * p_Vector.x, y * p_Vector.y, z * p_Vector.z );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator * ( const T & p_Value ) const
{
	return Vector3<T>( x * p_Value, y * p_Value, z * p_Value );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator / ( const Vector3<T> & p_Vector ) const
{
	return Vector3<T>( x / p_Vector.x, y / p_Vector.y, z / p_Vector.z );
}

template <typename T>
inline Vector3<T> Vector3<T>::operator / ( const T & p_Value ) const
{
	return Vector3<T>( x / p_Value, y / p_Value, z / p_Value );
}

template <typename T>
inline T Vector3<T>::operator [ ] ( const SizeType & p_Index ) const
{
	return *( &x + p_Index );
}

template <typename T>
inline Float64 Vector3<T>::Length( ) const
{
	return sqrt( ( x*x ) + ( y*y ) + ( z*z ) );
}

template <typename T>
inline Vector3<T> Vector3<T>::Normal( ) const
{
	Float64 length = Length( );

	if( bitIsNan( length ) )
	{
		return Vector3<T>(	static_cast< T >( 0 ),
								static_cast< T >( 0 ), static_cast< T >( 0 ) );
	}

	return Vector3<T>(	static_cast<Float64>( x ) / length,
						static_cast<Float64>( y ) / length,
						static_cast<Float64>( z ) / length );
}

template <typename T>
inline Vector3<T> & Vector3<T>::Normalize( )
{
	T length = static_cast<T>( Length( ) );

	if( bitIsNan( length ) )
	{
		x = y = z = static_cast< T >( 0 ); 
		return *this;
	}

	x /= length;
	y /= length;
	z /= length;
	return *this;
}

template <typename T>
inline Vector3<T> Vector3<T>::Absolute( ) const
{
	return Vector3<T>((T)abs((T)x), (T)abs((T)y), (T)abs((T)z));
}

template <typename T>
inline Float64 Vector3<T>::Dot( const Vector3<T> & p_Vector ) const
{
	Float64 dot =	( x * p_Vector.x ) +
					( y * p_Vector.y ) +
					( z * p_Vector.z );

	if( bitIsNan( dot ) )
	{
		return 0.0f;
	}

	return dot;
}

template <typename T>
inline Vector3<T> Vector3<T>::Cross(Vector3<T> p_Vector ) const
{
	Vector3<T> crossVector;
	crossVector.x = ( ( y * p_Vector.z ) - ( z * p_Vector.y ) );
	crossVector.y = ( ( z * p_Vector.x ) - ( x * p_Vector.z ) );
	crossVector.z = ( ( x * p_Vector.y ) - ( y * p_Vector.x ) );
	return crossVector;
}

template <typename T>
inline void Vector3<T>::Rotate( const Float64 p_Angle, const Float64 p_X, const Float64 p_Y, const Float64 p_Z )
{
	// Create a rotation quaternion
	Float64 qx = p_X * Math::SinDegrees( p_Angle / 2.0f );
	Float64 qy = p_Y * Math::SinDegrees( p_Angle / 2.0f );
	Float64 qz = p_Z * Math::SinDegrees(  p_Angle / 2.0f );
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

template <typename T>
inline void Vector3<T>::RotateX( const Float64 p_Angle )
{
	const Float64 sin = Math::Sin<Float64>( Math::DegreesToRadians<Float64>( p_Angle ) );
	const Float64 cos = Math::Cos<Float64>( Math::DegreesToRadians<Float64>( p_Angle ) );

	// Store the new y and z in tempory variables
	const Float64 ty = ( static_cast<Float64>( y ) * cos ) - ( static_cast<Float64>( z ) * sin );
	const Float64 tz = ( static_cast<Float64>( y ) * sin ) + ( static_cast<Float64>( z ) * cos );

	y = static_cast<T>( ty );
	z = static_cast<T>( tz );
}

template <typename T>
inline void Vector3<T>::RotateY( const Float64 p_Angle )
{
	const Float64 sin = Math::Sin<Float64>( Math::DegreesToRadians<Float64>(p_Angle ) );
	const Float64 cos = Math::Cos<Float64>( Math::DegreesToRadians<Float64>(p_Angle ) );
	// Store the new x and z in tempory variables
	const Float64 tx = ( static_cast<Float64>( x ) * cos ) - ( static_cast<Float64>( z ) * sin );
	const Float64 tz = ( static_cast<Float64>( x ) * sin ) + ( static_cast<Float64>( z ) * cos );

	x = static_cast<T>( tx );
	z = static_cast<T>( tz );
}

template <typename T>
inline void Vector3<T>::RotateZ( const Float64 p_Angle )
{
	const Float64 sin = Math::Sin<Float64>( Math::DegreesToRadians<Float64>(p_Angle ) );
	const Float64 cos = Math::Cos<Float64>( Math::DegreesToRadians<Float64>(p_Angle ) );

	// Store the new x and y in tempory variables
	const Float64 tx = ( static_cast<Float64>( x ) * cos ) - ( static_cast<Float64>( y ) * sin );
	const Float64 ty = ( static_cast<Float64>( x ) * sin ) + ( static_cast<Float64>( y ) * cos );

	x = static_cast<T>( tx );
	y = static_cast<T>( ty );
}

template <typename T>
inline Float64 Vector3<T>::Dot( const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2 )
{
	Float64 dot =	( p_Vector1.x * p_Vector2.x ) +
					( p_Vector1.y * p_Vector2.y ) +
					( p_Vector1.z * p_Vector2.z );
	if( bitIsNan( dot ) )
	{
		return 0.0f;
	}

	return dot;
}

template <typename T>
inline Vector3<T> Vector3<T>::Cross( const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2)
{
	Vector3<T> dest;
	dest.x = ( ( p_Vector1.y * p_Vector2.z ) - ( p_Vector1.z * p_Vector2.y ) );
	dest.y = ( ( p_Vector1.z * p_Vector2.x ) - ( p_Vector1.x * p_Vector2.z ) );
	dest.z = ( ( p_Vector1.x * p_Vector2.y ) - ( p_Vector1.y * p_Vector2.x ) );
	return dest;
}

template <typename T>
inline Angle<Float64> Vector3<T>::AngleBetweenVectors( const Vector3<T> & p_Vector1, const Vector3<T> & p_Vector2 )
{
	Float64 angleRad = acos( p_Vector1.Normal( ).Dot( p_Vector2 ) );

	if( bitIsNan( angleRad ) )
	{
		return 0.0f;
	}

	return Angle<Float64>( angleRad );
}