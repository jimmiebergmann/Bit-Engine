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

#ifndef BIT_SYSTEM_THREAD_WIN32_HPP
#define BIT_SYSTEM_THREAD_WIN32_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/System/SmartMutex.hpp>
#include <Windows.h>
#include <functional>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Win32 thread.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ThreadWin32
	{

	public:

		// Public typedefs
		typedef std::function< void( ) > Function;

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		ThreadWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Creates and executes the thread.
		///
		/// \param p_Function The function to execute.
		///
		////////////////////////////////////////////////////////////////
		ThreadWin32( Function p_Function );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~ThreadWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Executes the thread.
		///
		/// \param p_Function The function to execute.
		///
		////////////////////////////////////////////////////////////////
		void Execute( Function p_Function );

		////////////////////////////////////////////////////////////////
		/// \brief Wait for the thread to finish
		///
		/// This function is a modal function.
		/// It's going to wait for the thread to exit.
		///
		////////////////////////////////////////////////////////////////
		void Finish( );

		////////////////////////////////////////////////////////////////
		/// \brief Force the thread to exit.
		///
		////////////////////////////////////////////////////////////////
		void Terminate( );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if thread is running.
		///
		/// \return True if running, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool IsRunning( );

	private:

		// Private functions
		static DWORD WINAPI StaticThreadFunction( LPVOID p_pParam );

		// Private variables
		Bool m_Running;			///< Thread status.
		HANDLE m_Handle;		///< Win32 thread handle.
		Function m_Function;	///< The thread function.
		Mutex m_Mutex;			///< Mutex for m_Running variable,


	};

}

#endif

#endif