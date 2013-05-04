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

#ifdef PLATFORM_LINUX

#include <Bit/DataTypes.hpp>
#include <Bit/Window/WindowBase.hpp>
#include <X11/Xlib.h>

namespace Bit
{

	class WindowLinux : public WindowBase
	{

	public:

		// Constructors/destructors
		WindowLinux( );

		// Public functions
		virtual BIT_UINT32 Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
			const std::string p_Title, const BIT_UINT32 p_Style );
		virtual BIT_UINT32 Destroy( );
		virtual BIT_UINT32 DoEvents( );

		// Get functions
		// ...

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title );

	private:

		// Private functions
		/*static LRESULT WindowProcStatic( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
		LRESULT WindowProc( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
		static std::wstring StringToWideString( const std::string& p_String );
*/

		// Private variables
		::Display * m_pDisplay;
		int m_Screen;
		::Window m_Window;
		:: Colormap m_ColorMap;
		/*HDC		m_DeviceContext;
		HWND	m_Window;
		BIT_BOOL m_RegisteredClass;

		*/

	};

}

#endif

#endif
