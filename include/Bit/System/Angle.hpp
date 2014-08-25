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
	class BIT_API Angle
	{

	public:

		// Friend functions
		friend BIT_API Angle Degrees( const Float64 & );
		friend BIT_API Angle Radians( const Float64 & );

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////////
		Angle( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in degrees.
		/// 
		////////////////////////////////////////////////////////////////
		Float64 AsDegrees( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in radians.
		/// 
		////////////////////////////////////////////////////////////////
		Float64 AsRadians( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the angle in radians.
		/// 
		////////////////////////////////////////////////////////////////
		Float64 operator( ) ( ) const;

	private:

		// Private functions
		Angle( const Float64 & p_Radians );

		// Private variables
		Float64 m_Radians;

	};

	////////////////////////////////////////////////////////////////
	/// \brief Function for initializing a angle class
	///
	/// \param p_Degrees Angle in degrees.
	///
	////////////////////////////////////////////////////////////////
	BIT_API Angle Degrees( const Float64 & p_Degrees );

	////////////////////////////////////////////////////////////////
	/// \brief Function for initializing a angle class
	///
	/// \param p_Radians Angle in radians.
	///
	////////////////////////////////////////////////////////////////
	BIT_API Angle Radians( const Float64 & p_Radians );

}

#endif