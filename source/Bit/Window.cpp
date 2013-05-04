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

#include <Bit/System/MemoryLeak.hpp>

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
	BIT_UINT32 Window::Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits )
	{
		// Make sure the window isn't already created
		if( IsCreated( ) )
		{
			return BIT_ERROR;
		}

		// Allocate the window platoform base class.
		m_pWindowBase = new WindowPlatformType( );

		// Let's create the window
		BIT_UINT32 Error;
		if( ( Error = m_pWindowBase->Create( p_Size, p_Bits, "Bit Engine" ) ) != BIT_OK )
		{
			// Delete the allocated base platfor class
			delete m_pWindowBase;
			m_pWindowBase = BIT_NULL;

			// Return the error
			return Error;
		}

		// Everything went ok. The window is created.
		return BIT_OK;
	}

	BIT_UINT32 Window::Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title )
	{
		// Make sure the window isn't already created
		if( IsCreated( ) )
		{
			return BIT_ERROR;
		}

		// Allocate the window platoform base class.
		m_pWindowBase = new WindowPlatformType( );

		// Let's create the window
		BIT_UINT32 Error;
		if( ( Error = m_pWindowBase->Create( p_Size, p_Bits, p_Title ) ) != BIT_OK )
		{
			// Delete the allocated base platfor class
			delete m_pWindowBase;
			m_pWindowBase = BIT_NULL;

			// Return the error
			return Error;
		}

		// Everything went ok. The window is created.
		return BIT_OK;
	}

	BIT_UINT32 Window::Destroy( )
	{
		// Delete the window platform base class if it's allocated.
		if( m_pWindowBase )
		{
			m_pWindowBase->Destroy( );
			delete m_pWindowBase;
			m_pWindowBase = BIT_NULL;
		}

		return BIT_OK;
	}

	BIT_UINT32 Window::DoEvents( )
	{
		// Make sure the window is created
		if( !IsCreated( ) )
		{
			return BIT_ERROR;
		}

		return m_pWindowBase->DoEvents( );
	}

	BIT_BOOL Window::IsCreated( ) const
	{
		// The window is allocated if the base class pointer isn't NULL(it's allocated)
		return m_pWindowBase != BIT_NULL;
	}

	BIT_BOOL Window::PollEvent( Event & p_Event )
	{
		// Make sure the window is created
		if( !IsCreated( ) )
		{
			return BIT_FALSE;
		}

		// Poll the next event in the queue
		if( m_pWindowBase->m_EventQueue.size( ) )
		{
			p_Event = m_pWindowBase->m_EventQueue.front( );
			m_pWindowBase->m_EventQueue.pop_front( );
			return BIT_TRUE;
		}

		return BIT_FALSE;
	}

	// Set functions
	BIT_UINT32 Window::SetTitle( std::string p_Title )
	{
		// Make sure the window is created
		if( !IsCreated( ) )
		{
			return BIT_ERROR;
		}

		return m_pWindowBase->SetTitle( p_Title );
	}

};