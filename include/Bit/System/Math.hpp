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

#ifndef BIT_SYSTEM_MATH_HPP
#define BIT_SYSTEM_MATH_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Angle.hpp>
#include <cmath>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Math class
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API Math
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Convert from degrees to radians
		///
		/// \param p_Radians Input value in degrees.
		/// 
		/// \see RadiansToDegrees
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T DegreesToRadians( const T p_Degrees);

		////////////////////////////////////////////////////////////////
		/// \brief Convert from radians to degrees.
		///
		/// \param p_Radians Input value in radians.
		/// 
		/// \see RadiansToDegrees
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T RadiansToDegrees( const T p_Radians );

		////////////////////////////////////////////////////////////////
		/// \brief Sine function.
		///
		/// \param p_Radians Input value in radians.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T Sin( const T p_Radians );

		////////////////////////////////////////////////////////////////
		/// \brief Cosine function.
		///
		/// \param p_Radians Input value in radians.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T Cos( const T p_Radians );

		////////////////////////////////////////////////////////////////
		/// \brief Tangent function
		///
		/// \param p_Radians Input value in radians.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T Tan( const T p_Radians );

		////////////////////////////////////////////////////////////////
		/// \brief Sine function.
		///
		/// \param p_Radians Input value in degrees.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T SinDegrees( const T p_Degrees );

		////////////////////////////////////////////////////////////////
		/// \brief Cosine function.
		///
		/// \param p_Radians Input value in degrees.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T CosDegrees( const T p_Degrees );

		////////////////////////////////////////////////////////////////
		/// \brief Tangent function
		///
		/// \param p_Radians Input value in degrees.
		/// 
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T TanDegrees( const T p_Degrees );

		////////////////////////////////////////////////////////////////
		/// \brief Convert between row and column index
		///
		/// 0	1	2	3
		/// 4	5	6	7
		/// 8	9	10	11
		/// 12	13	14	15
		/// 
		/// turns into
		///
		/// 0	4	8	12
		/// 1	5	9	13
		/// 2	6	10	14
		/// 3	7	11	15
		///
		/// \param p_Index The row index
		/// \param p_RowSize Size of the row
		/// \param p_ColumnSize Size of the column
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		static T RowToColumnIndex( const T p_Index, const T p_RowSize, const T p_ColumnSize );
		
		////////////////////////////////////////////////////////////////
		/// \brief Clamp the value between p_Min and p_Max
		///
		/// \param p_Value The value to clamp
		/// \param p_Min The minimum value
		/// \param p_Max The maximum value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		static T Clamp( const T & p_Value, const T & p_Min, const T & p_Max );

		////////////////////////////////////////////////////////////////
		/// \brief Checking if two values are equal each other
		///
		/// Used for camparing two values with precision issues.
		///
		/// \param p_Value1 The first value
		/// \param p_Value2 The second value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		static Bool EqualEpsilon( const T & p_Value1, const T & p_Value2 );

		////////////////////////////////////////////////////////////////
		/// \brief Checking if two values are equal each other
		///
		/// Used for camparing two values with precision issues.
		///
		/// \param p_Value1 The first value
		/// \param p_Value2 The second value
		///
		////////////////////////////////////////////////////////////////
		template < typename T >
		static Bool EqualEpsilonHalf( const T & p_Value1, const T & p_Value2 );

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Math.inl>
}

#endif
