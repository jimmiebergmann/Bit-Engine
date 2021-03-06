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
/*
#ifndef BIT_WINDOW_WIN32_HPP
#define BIT_WINDOW_WIN32_HPP

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_WIN32

#include <Bit/Window/Window.hpp>
#include <Bit/System/Win32/KeyboardWin32.hpp>
#include <Bit/System/Win32/MouseWin32.hpp>
#include <Bit/System/Vector2.hpp>
#include <windows.h>
#include <vector>

namespace Bit
{

	class BIT_API WindowWin32 : public Window
	{

	public:

		// Constructors/destructors
		WindowWin32( );
		virtual ~WindowWin32( );

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
		HDC GetDeviceContext( ) const;

	private:

		// Private functions
		static LRESULT WindowProcStatic( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
		LRESULT WindowProc( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
		static std::wstring StringToWideString( const std::string& p_String );

		// Private variables
		HDC		m_DeviceContext;
		HWND	m_Window;
		BIT_BOOL m_RegisteredClass;
		KeyboardWin32 * m_pKeyboard;
		MouseWin32 * m_pMouse;
		std::vector< Keyboard::eKey > m_PressedKeys;
		std::vector< Mouse::eButton > m_PressedButtons;

	};

}

#endif

#endif
*/