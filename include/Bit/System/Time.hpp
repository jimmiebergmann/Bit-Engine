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

#ifndef BIT_SYSTEM_TIME_HPP
#define BIT_SYSTEM_TIME_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Time class
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Time
	{

	public:

		// Friend functions
		friend Time Seconds( const Float64 & );
		friend Time Milliseconds( const Uint64 & );
		friend Time Microseconds( const Uint64 & );

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Time( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the time in seconds.
		///
		/// \return Time in seconds as a 64 bit floating point value.
		///
		////////////////////////////////////////////////////////////////
		Float64 AsSeconds( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the time in seconds.
		///
		/// \return Time in milliseconds as a 64 bit floating point value.
		///
		////////////////////////////////////////////////////////////////
		Uint64 AsMilliseconds( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the time in seconds.
		///
		/// \return Time in microseconds as a 64 bit floating point value.
		///
		////////////////////////////////////////////////////////////////
		Uint64 AsMicroseconds( ) const;

	private:

		// Private functions
		Time( const Uint64 & p_Microseconds );

		// Private variables
		Uint64 m_Microseconds;	///< Time in nanoseconds.

	};

	// Functions for initializing time classes
	BIT_API Time Seconds( const Float64 & p_Seconds );

	BIT_API Time Milliseconds( const Uint64 & p_Milliseconds );

	BIT_API Time Microseconds( const Uint64 & p_Microseconds );

}

#endif