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

#include <Bit/System/Win32/SemaphoreWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	SemaphoreWin32::SemaphoreWin32( const Uint32 & p_InitialCount, const Uint32 & p_MaximumCount )
	{
		m_Semaphore = CreateSemaphore(	NULL, static_cast<LONG>( p_InitialCount ),
										static_cast<LONG>( p_MaximumCount ), NULL );
	}
		
	SemaphoreWin32::~SemaphoreWin32( )
	{
		CloseHandle( m_Semaphore );
	}
		
	void SemaphoreWin32::Wait( )
	{
		WaitForSingleObject(  m_Semaphore, INFINITE );
	}

	void SemaphoreWin32::Release( const Uint32 & p_Ammount )
	{
		ReleaseSemaphore( m_Semaphore, static_cast<LONG>( p_Ammount ), NULL );
	}

}

#endif