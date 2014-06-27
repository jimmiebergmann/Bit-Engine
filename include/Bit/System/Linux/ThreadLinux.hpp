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

#ifndef BIT_SYSTEM_THREAD_LINUX_HPP
#define BIT_SYSTEM_THREAD_LINUX_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_LINUX
#include <Bit/System/SmartMutex.hpp>
#include <functional>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Linux thread.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ThreadLinux
	{

	public:

		// Public typedefs
		typedef std::function< void( ) > Function;

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		ThreadLinux( );

		////////////////////////////////////////////////////////////////
		/// \brief Creates and executes the thread.
		///
		/// \param p_Function The function to execute.
		///
		////////////////////////////////////////////////////////////////
		ThreadLinux( Function p_Function );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~ThreadLinux( );

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
		static void * StaticThreadFunction( void * p_pParam );

		// Private variables
		Bool m_Running;			///< Thread status.
		pthread_t m_Thread;		///< POSIX thread.
		Function m_Function;	///< The thread function.
		Mutex m_Mutex;			///< Mutex for m_Running variable,


	};

}

#endif

#endif
