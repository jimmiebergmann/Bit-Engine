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
#include <queue>

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
		/// \brief Poll a window event.
		///
		/// \param p_Event The current event.
		///
		/// \return false if event stack is empty, else true.
		///
		////////////////////////////////////////////////////////////////
		virtual bool PollEvent( Event & p_Event );

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

		////////////////////////////////////////////////////////////////
		/// \brief Clearing all the events in the queue
		///
		////////////////////////////////////////////////////////////////
		void ClearEvents( );

		////////////////////////////////////////////////////////////////
		/// \brief Static win32 event function,
		/// required for win32 c++ wrapper.
		///
		////////////////////////////////////////////////////////////////
		static LRESULT WindowProcStatic(	HWND p_HWND, UINT p_Message,
											WPARAM p_WParam, LPARAM p_LParam );

		////////////////////////////////////////////////////////////////
		/// \brief Win32 event function, required for win32 c++ wrapper.
		///
		////////////////////////////////////////////////////////////////
		LRESULT WindowProc( HWND p_HWND, UINT p_Message,
							WPARAM p_WParam, LPARAM p_LParam );

		// Private typedefs
		typedef std::queue< Event > EventQueue;

		// Private varaibles
		bool			m_Open;
		VideoMode		m_VideoMode;
		std::string		m_Title;
		Uint32			m_Style;
		HDC				m_DeviceContextHandle;
		HWND			m_WindowHandle;
		std::wstring	m_WidnowClassName;
		bool			m_RegisteredWindowClass;
		EventQueue		m_Events;
		bool			m_Resizing;
		bool			m_Moving;
		Vector2i32		m_LastResize;

	};

}

#endif

#endif