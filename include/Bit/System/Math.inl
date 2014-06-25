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
inline T Math::DegreesToRadians( const T p_Degrees)
{
	return p_Degrees * static_cast<T>( Pi / 180.0f );
}

template <typename T>
inline T Math::RadiansToDegrees( const T p_Radians )
{
	return p_Radians * static_cast<T>( 180.0f / Pi );
}

template <typename T>
inline T Math::Sin( const T p_Radians )
{
	return static_cast<T>( sin( static_cast<double>(p_Radians ) ) );
}

template <typename T>
inline T Math::Cos( const T p_Radians )
{
	return static_cast<T>( cos( static_cast<double>( p_Radians ) ) );
}

template <typename T>
inline T Math::Tan( const T p_Radians )
{
	return static_cast<T>( tan( static_cast<double>( p_Radians ) ) );
}

template <typename T>
inline T Math::SinDegrees( const T p_Degrees )
{
	return static_cast<T>( sin( static_cast<double>( DegreesToRadians<T>( p_Degrees ) ) ) );
}

template <typename T>
inline T Math::CosDegrees( const T p_Degrees )
{
	return static_cast<T>( cos( static_cast<double>( DegreesToRadians<T>( p_Degrees ) ) ) );
}

template <typename T>
inline T Math::TanDegrees( const T p_Degrees )
{
	return static_cast<T>( tan( static_cast<double>( DegreesToRadians<T>( p_Degrees ) ) ) );
}

template <typename T>
inline T Math::RowToColumnIndex( const T p_Index, const T p_RowSize , const T p_ColumnSize )
{
	int a = p_Index / p_RowSize;
	int b = ( p_Index % p_RowSize ) * p_ColumnSize;
	return a + b;
}

template < typename T >
inline T Math::Clamp( const T & p_Value, const T & p_Min, const T & p_Max )
{
	if( p_Value < p_Min )
	{
		return p_Min;
	}
	if( p_Value > p_Max )
	{
		return p_Max;
	}
	return p_Value;
}

template < typename T >
inline Bool Math::EqualEpsilon( const T & p_Value1, const T & p_Value2 )
{
	if( static_cast<Float64>( p_Value1 ) > ( static_cast<Float64>( p_Value2 ) + Epsilon ) ||
		static_cast<Float64>( p_Value1 ) < ( static_cast<Float64>( p_Value2 ) - Epsilon ) )
	{
		return false;
	}

	return true;
}

template < typename T >
inline Bool Math::EqualEpsilonHalf( const T & p_Value1, const T & p_Value2 )
{
	if( static_cast<Float64>( p_Value1 ) > ( static_cast<Float64>( p_Value2 ) + EpsilonHalf ) ||
		static_cast<Float64>( p_Value1 ) < ( static_cast<Float64>( p_Value2 ) - EpsilonHalf ) )
	{
		return false;
	}

	return true;
}
