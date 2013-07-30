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

#ifndef __BIT_WINDOW_LINUX_HPP__
#define __BIT_WINDOW_LINUX_HPP__

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/Window/Window.hpp>
#include <Bit/System/Linux/KeyboardLinux.hpp>
#include <Bit/System/Linux/MouseLinux.hpp>
#include <Bit/System/Vector2.hpp>
#include <X11/Xlib.h>

namespace Bit
{

	class WindowLinux : public Window
	{

	public:

		// Constructors/destructors
		WindowLinux( );
		virtual ~WindowLinux( );

		// Public general functions
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits );
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title );
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title,
                    const BIT_UINT32 p_Style );
		virtual BIT_UINT32 Close( );
		virtual BIT_UINT32 Update( );
		virtual BIT_UINT32 Show( const BIT_BOOL p_State );
		virtual BIT_UINT32 ShowCursor( const BIT_BOOL p_State );
		virtual BIT_UINT32 SetCursorPosition( Vector2_si32 p_Position );
		virtual Vector2_si32 GetCursorScreenPosition( );

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title );

        // Get functions
		::Window GetWindowDevice( ) const;
		::Display * GetDisplayDevice( ) const;
		int GetScreenDevice( ) const;

	private:

		// Private variables
		::Display * m_pDisplay;
		int m_Screen;
		::Window m_Window;
		KeyboardLinux * m_pKeyboard;
		MouseLinux * m_pMouse;

	};

}

#endif

#endif
