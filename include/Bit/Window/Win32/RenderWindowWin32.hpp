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

#ifndef BIT_WINDOW_RENDER_WINDOW_WIN32_HPP
#define BIT_WINDOW_RENDER_WINDOW_WIN32_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/Window/Window.hpp>
#include <Windows.h>

namespace Bit
{

	class BIT_API RenderWindowWin32 : public Window
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		RenderWindowWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		RenderWindowWin32( const VideoMode & p_VideoMode, const std::string & p_Title = "", const Uint32 p_Style = Style::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~RenderWindowWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the window.
		///
		/// This function may be used instead of the custom constructor.
		///
		////////////////////////////////////////////////////////////////
		virtual bool Open( const VideoMode & p_VideoMode, const std::string & p_Title = "", const Uint32 p_Style = Style::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Close the window.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( );

		////////////////////////////////////////////////////////////////
		/// \brief Update the window events
		///
		/// This function must be called in your application's loop.
		///
		////////////////////////////////////////////////////////////////
		virtual void Update( );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is open( created ).
		///
		////////////////////////////////////////////////////////////////
		virtual bool IsOpen( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's current video mode.
		///
		////////////////////////////////////////////////////////////////
		virtual VideoMode GetVideoMode( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's caption(title)
		///
		////////////////////////////////////////////////////////////////
		virtual std::string GetTitle( ) const;

	private:

		// Private functions
		static LRESULT WindowProcStatic(	HWND p_HWND, UINT p_Message,
											WPARAM p_WParam, LPARAM p_LParam );

		LRESULT WindowProc( HWND p_HWND, UINT p_Message,
							WPARAM p_WParam, LPARAM p_LParam );

		static std::wstring StringToWideString( const std::string & p_String );

		// Private varaibles
		bool			m_Open;
		VideoMode		m_VideoMode;
		std::string		m_Title;
		Uint32			m_Style;
		HDC				m_DeviceContextHandle;
		HWND			m_WindowHandle;
		std::wstring	m_WidnowClassName;
		bool			m_RegisteredWindowClass;

	};

}

#endif

#endif