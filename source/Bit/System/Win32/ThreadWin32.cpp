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

#include <Bit/System/Win32/ThreadWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS

namespace Bit
{

	ThreadWin32::ThreadWin32( ) :
		m_Running( false ),
		m_Handle( NULL )
	{
	}

	ThreadWin32::ThreadWin32( Function p_Function ) :
		m_Running( false ),
		m_Handle( NULL )
	{
		Execute( p_Function );
	}

	ThreadWin32::~ThreadWin32( )
	{
		if( m_Handle )
		{
			CloseHandle( m_Handle );
			m_Handle = NULL;
		}
	}

	void ThreadWin32::Execute( Function p_Function )
	{
		if( m_Running == false )
		{
			m_Function = p_Function;
			m_Handle = CreateThread( NULL, 0, StaticThreadFunction, reinterpret_cast<LPVOID>( this ), 0, NULL );
		}
	}

	void ThreadWin32::Finish( )
	{
		// Wait for the function to finish
		WaitForSingleObject( m_Handle, INFINITE );

		// Close the handle
		if( m_Handle )
		{
			CloseHandle( m_Handle );
			m_Handle = NULL;
		}

	}

	void ThreadWin32::Terminate( )
	{
		// Terminate the thread.
		TerminateThread( m_Handle, 0 );

		// Close the handle
		if( m_Handle )
		{
			CloseHandle( m_Handle );
			m_Handle = NULL;
		}
	}

	Bool ThreadWin32::IsRunning( )
	{
		Bit::SmartMutex smartMutex( m_Mutex );
		smartMutex.Lock( );
		return m_Running;
	}

	DWORD WINAPI ThreadWin32::StaticThreadFunction( LPVOID p_pParam )
	{
		// Get the thread from the param
		ThreadWin32 * pThread = reinterpret_cast< ThreadWin32 * >( p_pParam );

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

		// Succeeded
		return 0;
	}

}

#endif