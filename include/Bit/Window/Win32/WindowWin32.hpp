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

#ifndef __BIT_WINDOW_WIN32_HPP__
#define __BIT_WINDOW_WIN32_HPP__

#ifdef PLATFORM_WINDOWS

#include <Bit/DataTypes.hpp>
#include <Bit/Window/WindowBase.hpp>

namespace Bit
{

	class WindowWin32 : public WindowBase
	{

	public:


		/*// Constructors/destructors
		WindowWin32( );
		WindowWin32( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height,
			const std::string p_Title, const BIT_BOOL p_Fullscreen );
		~WindowWin32( );
*/
		// Public functions
		//virtual BIT_BOOL PollEvent( Event & p_Event ) = 0;
		/*
		virtual BIT_UINT32 Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height,
			const std::string p_Title, const BIT_BOOL p_Fullscreen );
		virtual BIT_UINT32 Destroy( );
		virtual BIT_UINT32 Update( );

		// Get functions
		virtual WindowData GetData( ) const;

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title );
*/
	private:

		// Private functions
		/*static LRESULT WindowProcStatic( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
		LRESULT WindowProc( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );
*/
		// Private variables
	/*	HDC		m_DeviceContext;
		HWND	m_Window;
		VML_BOOL m_RegisteredClass;*/

	};

}

#endif

#endif