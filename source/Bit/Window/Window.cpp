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

#include <Bit/Window/Window.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// Constructors/destructors
	Window::~Window( )
	{
	}

}

/*
// Platform independent
#ifdef BIT_PLATFORM_WIN32
	#include <Bit/Window/Win32/WindowWin32.hpp>
	typedef Bit::WindowWin32 WindowPlatformType;
	#undef CreateWindow
#elif defined( BIT_PLATFORM_LINUX )
	#include <Bit/Window/Linux/WindowLinux.hpp>
	typedef Bit::WindowLinux WindowPlatformType;
#endif

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	BIT_BOOL Window::PollEvent( Event & p_Event )
	{
		// Poll the next event in the queue
		if( m_EventQueue.size( ) )
		{
			p_Event = m_EventQueue.front( );
			m_EventQueue.pop_front( );

			return BIT_TRUE;
		}

		return BIT_FALSE;
	}

	// Use this function for window creation!
	// Function for Window allocation
	BIT_API Window * CreateWindow( )
	{
        return new WindowPlatformType( );
	}

}
*/