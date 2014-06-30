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

#ifndef BIT_WINDOW_RENDER_WINDOW_LINUX_HPP
#define BIT_WINDOW_RENDER_WINDOW_LINUX_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_LINUX
#include <Bit/Window/Window.hpp>
#include <Bit/NonCopyable.hpp>
#define Bool int
#include <X11/Xlib.h>
// We need to undef Bool for linux build( In x11 lib)
#undef Bool
#undef None

#include <queue>


namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Render window for Linux.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API RenderWindowLinux : public Window, NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		RenderWindowLinux( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		RenderWindowLinux(	const VideoMode & p_VideoMode,
							const std::string & p_Title = "",
							const Uint32 p_Style = Style::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~RenderWindowLinux( );

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
		/// \brief Get the window's display device.
		///
		/// This function is for linux only.
		///
		////////////////////////////////////////////////////////////////
		::Display * GetDisplayDevice( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's display device.
		///
		/// This function is for linux only.
		///
		////////////////////////////////////////////////////////////////
		::Window GetWindowDevice( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's screen device.
		///
		/// This function is for linux only.
		///
		////////////////////////////////////////////////////////////////
		int GetScreenDevice( ) const;

    private:

		// Private typedefs
		typedef std::queue< Event > EventQueue;

		// Private variables
		Bool m_Open;
		Bool m_Focused;
		VideoMode m_VideoMode;
		std::string m_Title;
		Uint32 m_Style;
		Vector2u32 m_Size;
        Vector2i32 m_Position;
		::Display * m_pDisplay;
		int m_Screen;
		::Window m_Window;
		EventQueue m_Events;
		Mouse m_Mouse;
		Keyboard m_Keyboard;
		std::vector< Keyboard::eKey > m_PressedKeys;
		std::vector< Mouse::eButton > m_PressedButtons;

	};

}


#endif

#endif
