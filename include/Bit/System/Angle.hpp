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

#ifndef BIT_SYSTEM_ANGLE_HPP
#define BIT_SYSTEM_ANGLE_HPP

#include <Bit/Build.hpp>
#include <cmath>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Angle class
	/// 
	////////////////////////////////////////////////////////////////
	template <typename T>
	class Angle
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////////
		Angle( ) :
			m_Radians( static_cast<T>( 0 ) )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// \param p_Angle The angle in radians.
		/// 
		////////////////////////////////////////////////////////////////
		Angle( const T p_Angle ) :
			m_Radians( p_Angle )
		{
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in degrees.
		/// 
		////////////////////////////////////////////////////////////////
		T AsDegrees( ) const
		{
			return m_Radians * static_cast<T>( 180.0f / Pi );
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in radians.
		/// 
		////////////////////////////////////////////////////////////////
		T AsRadians( ) const
		{
			return m_Radians;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in radians.
		/// 
		////////////////////////////////////////////////////////////////
		T operator( ) ( ) const
		{
			return m_Radians;
		}

	private:

		T m_Radians;

	};

}

#endif