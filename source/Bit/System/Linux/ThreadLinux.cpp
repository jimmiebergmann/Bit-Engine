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

#include <Bit/System/Linux/ThreadLinux.hpp>
#ifdef BIT_PLATFORM_LINUX
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	ThreadLinux::ThreadLinux( ) :
		m_Running( false )
	{
	}

	ThreadLinux::ThreadLinux( Function p_Function ) :
		m_Running( false )
	{
		Execute( p_Function );
	}

	ThreadLinux::~ThreadLinux( )
	{
	}

	void ThreadLinux::Execute( Function p_Function )
	{
		if( m_Running == false )
		{
			m_Function = p_Function;

			pthread_create( &m_Thread, NULL, StaticThreadFunction, reinterpret_cast<void *>( this ) );
		}
	}

	void ThreadLinux::Finish( )
	{
	    // Wait for the function to finish
        pthread_join( m_Thread, NULL );
    }

	void ThreadLinux::Terminate( )
	{
		// Terminate the thread.
		pthread_cancel( m_Thread );
	}

	Bool ThreadLinux::IsRunning( )
	{
		Bit::SmartMutex smartMutex( m_Mutex );
		smartMutex.Lock( );
		return m_Running;
	}


    void * ThreadLinux::StaticThreadFunction( void * p_pParam )
    {
        // Make the thread cancelable.
        pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL );

        // Get the thread from the param
		ThreadLinux * pThread = reinterpret_cast< ThreadLinux * >( p_pParam );

        // Mark the thread as running
		pThread->m_Mutex.Lock( );
		pThread->m_Running = true;
		pThread->m_Mutex.Unlock( );

		// Run the thread function and get the return value.
		pThread->m_Function( );

		// Mark the thread as not
		pThread->m_Mutex.Lock( );
		pThread->m_Running = false;
		pThread->m_Mutex.Unlock( );

        return NULL;
    }

}

#endif
