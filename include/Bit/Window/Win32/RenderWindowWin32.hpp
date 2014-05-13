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
#include <Bit/NonCopyable.hpp>
#include <Windows.h>
#include <queue>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Render window for win32
	///
	////////////////////////////////////////////////////////////////
	class BIT_API RenderWindowWin32 : public Window, NonCopyable
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
		RenderWindowWin32(	const VideoMode & p_VideoMode,
							const std::string & p_Title = "",
							const Uint32 p_Style = Style::Default );

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
		virtual Bool Open(	const VideoMode & p_VideoMode,
							const std::string & p_Title = "",
							const Uint32 p_Style = Style::Default );

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
		/// This function can be used in a while loop
		/// in order to make sure you poll all the events
		/// in the event queue.
		///
		/// Example:
		/// 
		/// Event e;
		/// while( window.PollEvent( e ) )
		/// {
		///		// Handle the event
		///		// ...
		///	}
		///
		/// \param p_Event The current event.
		///
		/// \return true if function succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool PollEvent( Event & p_Event );

		////////////////////////////////////////////////////////////////
		/// \brief Set the window caption(title)
		///
		/// \param p_Title The new title of the window
		///
		////////////////////////////////////////////////////////////////
		virtual void SetTitle( const std::string & p_Title );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is open( created ).
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is focused.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsFocused( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's current video mode.
		///
		////////////////////////////////////////////////////////////////
		virtual const VideoMode & GetVideoMode( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's caption(title)
		///
		////////////////////////////////////////////////////////////////
		virtual const std::string & GetTitle( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's device context handle.
		///
		/// This function is for win32 only.
		///
		////////////////////////////////////////////////////////////////
		HDC GetDeviceContextHandle( ) const;

	private:

		////////////////////////////////////////////////////////////////
		/// \brief Clearing all the events in the queue
		///
		////////////////////////////////////////////////////////////////
		void ClearEvents( );

		////////////////////////////////////////////////////////////////
		/// \brief Update the pre events.
		///
		////////////////////////////////////////////////////////////////
		void PreUpdateEvents( );

		void AddMousePressEvent( const Mouse::eButton p_Button);

		void RemoveMousePressEvent( const Mouse::eButton p_Button );

		void AddKeyPressEvent( const Keyboard::eKey p_Key );

		void RemoveKeyPressEvent( const Keyboard::eKey p_Key );

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
		Bool m_Open;
		Bool m_Focused;
		VideoMode m_VideoMode;
		std::string m_Title;
		Uint32 m_Style;
		HDC m_DeviceContextHandle;
		HWND m_WindowHandle;
		std::wstring m_WindowClassName;
		Bool m_RegisteredWindowClass;
		EventQueue m_Events;
		Mouse m_Mouse;
		Keyboard					m_Keyboard;
		std::vector<Mouse::eButton>	m_PressedButtons;
		std::vector<Keyboard::eKey>	m_PressedKeys;
		Bool m_Resizing;
		Bool m_Moving;

	};

}

#endif

#endif