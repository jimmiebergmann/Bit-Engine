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

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Time( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// Initializing the time class.
		///
		/// \param p_NanoSeconds Time in nanoseconds.
		///
		////////////////////////////////////////////////////////////////
		Time( const Uint64 & p_Nanoseconds );

		////////////////////////////////////////////////////////////////
		/// \brief Sets the time.
		///
		/// \param p_NanoSeconds Time in nanoseconds.
		///
		////////////////////////////////////////////////////////////////
		void Set( const Uint64 & p_Nanoseconds );

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
		/// \return Time in seconds as a 64 bit floating point value.
		///
		////////////////////////////////////////////////////////////////
		Uint64 AsMilliseconds( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the time in seconds.
		///
		/// \return Time in seconds as a 64 bit floating point value.
		///
		////////////////////////////////////////////////////////////////
		Uint64 AsNanoseconds( ) const;

	private:

		// Private variables
		Uint64 m_Nanoseconds;	///< Time in nanoseconds.

	};

}

#endif