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

#ifndef BIT_SYSTEM_THREAD_VALUE_HPP
#define BIT_SYSTEM_THREAD_VALUE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/SmartMutex.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Thread value struct
	///
	////////////////////////////////////////////////////////////////
	template<typename T>
	class BIT_API ThreadValue
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		ThreadValue( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		ThreadValue( T p_Value );

		////////////////////////////////////////////////////////////////
		/// \brief Get thread safe value
		///
		////////////////////////////////////////////////////////////////
		T Get( );

		////////////////////////////////////////////////////////////////
		/// \brief Set thread safe value
		///
		////////////////////////////////////////////////////////////////
		void Set( const T & p_Value );

		// Public variables
		T Value;
		Mutex Mutex;
	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/ThreadValue.inl>

}

#endif