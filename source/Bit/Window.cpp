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

#include <Bit/Window.hpp>

// Platform independent 
#ifdef PLATFORM_WINDOWS
	#include <Bit/Window/Win32/WindowWin32.hpp>
	typedef Bit::WindowWin32 WindowPlatformType;
#elif PLATFORM_LINUX
	#include <Bit/Window/Linux/WindowLinux.hpp>
	typedef Bit::WindowLinux WindowPlatformType;
#endif

namespace Bit
{

	// Constructors/destructors
	Window::Window( ) :
		m_pWindowBase( BIT_NULL )
	{
		
	}

	Window::~Window( )
	{
		Destroy( );
	}

	// Public general functions
	BIT_UINT32 Window::Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height, BIT_UINT32 p_Bits )
	{
		m_pWindowBase = new WindowPlatformType( );

		return BIT_OK;
	}

	BIT_UINT32 Window::Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height, BIT_UINT32 p_Bits,
		const std::string p_Title )
	{
		m_pWindowBase = new WindowPlatformType( );

		return BIT_OK;
	}

	BIT_UINT32 Window::Destroy( )
	{
		if( m_pWindowBase )
		{
			delete m_pWindowBase;
			m_pWindowBase = BIT_NULL;
		}

		return BIT_OK;
	}

	BIT_BOOL Window::IsCreated( ) const
	{
		return m_pWindowBase != BIT_NULL;
	}

	BIT_BOOL Window::PollEvent( Event & p_Event )
	{
		if( !IsCreated( ) )
		{
			return BIT_FALSE;
		}

		// Poll the next event in the queue
		if( m_pWindowBase->m_EventQueue.size( ) )
		{
			p_Event = m_pWindowBase->m_EventQueue.front( );
			m_pWindowBase->m_EventQueue.pop( );
			return BIT_TRUE;
		}
	}

};