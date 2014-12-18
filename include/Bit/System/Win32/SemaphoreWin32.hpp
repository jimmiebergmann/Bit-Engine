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

#ifndef BIT_SYSTEM_SEMAPHORE_WIN32_HPP
#define BIT_SYSTEM_SEMAPHORE_WIN32_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Windows.h>
#undef SetPort

namespace Bit
{


	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Win32 sempahore.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API SemaphoreWin32
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		SemaphoreWin32( const Uint32 & p_InitialCount = 0, const Uint32 & p_MaximumCount = 10000 );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~SemaphoreWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Wait until the semaphore is larger than 0.
		///
		////////////////////////////////////////////////////////////////
		void Wait( );

		////////////////////////////////////////////////////////////////
		/// \brief Release the semaphore by the ammount.
		///
		////////////////////////////////////////////////////////////////
		void Release( const Uint32 & p_Ammount = 1 );

	private:

		HANDLE m_Semaphore;

	};

}

#endif

#endif