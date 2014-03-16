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

#ifndef BIT_WINDOW_HPP
#define BIT_WINDOW_HPP

#include <Bit/Build.hpp>
#include <Bit/Window/VideoMode.hpp>
#include <Bit/Window/Style.hpp>
#include <Bit/Window/Event.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Window base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Window
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~Window( );

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the window.
		///
		////////////////////////////////////////////////////////////////
		virtual bool Open( const VideoMode & p_VideoMode, const std::string & p_Title, const Uint32 p_Style ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Close the window.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Update the window events.
		///
		/// This function must be called in your application's loop.
		///
		////////////////////////////////////////////////////////////////
		virtual void Update( ) = 0;

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
		virtual bool PollEvent( Event & p_Event ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the window caption(title)
		///
		/// \param p_Title The new title of the window
		///
		////////////////////////////////////////////////////////////////
		virtual void SetTitle( const std::string & p_Title ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is open( created ).
		///
		////////////////////////////////////////////////////////////////
		virtual bool IsOpen( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is focused.
		///
		////////////////////////////////////////////////////////////////
		virtual bool IsFocused( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's current video mode.
		///
		////////////////////////////////////////////////////////////////
		virtual const VideoMode & GetVideoMode( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's caption(title)
		///
		////////////////////////////////////////////////////////////////
		virtual const std::string & GetTitle( ) const = 0;

		/*
		// Public virtual functions
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits ) = 0;
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title ) = 0;
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title,
                    const BIT_UINT32 p_Style ) = 0;
		virtual BIT_UINT32 Close( ) = 0;
		virtual BIT_UINT32 Update( ) = 0;
		virtual BIT_UINT32 Show( const BIT_BOOL p_State ) = 0;
		virtual BIT_UINT32 ShowCursor( const BIT_BOOL p_State ) = 0;
		virtual BIT_UINT32 SetCursorPosition( Vector2_si32 p_Position ) = 0;
		virtual Vector2_si32 GetCursorScreenPosition( ) = 0;

		// Public functions
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }
		BIT_INLINE BIT_BOOL IsFocused( ) const { return m_Focused; }
		BIT_BOOL PollEvent( Event & p_Event );

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title ) = 0;

		// Get functions
		BIT_INLINE BIT_UINT32 GetEventCount( ) const { return m_EventQueue.size( ); }
		BIT_INLINE Vector2_ui32 GetSize( ) const { return m_Size; }
		BIT_INLINE Vector2_si32 GetPosition( ) const { return m_Position; }
		BIT_INLINE BIT_UINT32 GetBits( ) const { return m_Bits; }
		BIT_INLINE std::string GetTitle( ) const { return m_Title; }
		BIT_INLINE BIT_UINT32 GetStyle( ) const { return m_Style; }
		*/
	protected:

		/*// Protected variables
		BIT_BOOL m_Open;
		BIT_BOOL m_Focused;
		Vector2_ui32 m_Size;
		Vector2_si32 m_Position;
		BIT_UINT32 m_Bits;
		std::string m_Title;
		BIT_UINT32 m_Style;
		std::list< Event > m_EventQueue;*/
	};

}

#endif










/*
#ifndef BIT_WINDOW_HPP
#define BIT_WINDOW_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/Window/Event.hpp>
#include <Bit/System/Vector2.hpp>
#include <string>
#include <list>

namespace Bit
{

	class BIT_API Window
	{

	public:

		// Public enums
		namespace Style
		{
			enum eStyle
			{
				Style_All = 15,
				Style_Close = 1,
				Style_Minimize = 2,
				Style_Resize = 4,
				Style_TitleBar = 8
			};
		}

		// Constructors/destructors
		virtual ~Window( ) {}

		// Public virtual functions
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits ) = 0;
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title ) = 0;
		virtual BIT_UINT32 Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title,
                    const BIT_UINT32 p_Style ) = 0;
		virtual BIT_UINT32 Close( ) = 0;
		virtual BIT_UINT32 Update( ) = 0;
		virtual BIT_UINT32 Show( const BIT_BOOL p_State ) = 0;
		virtual BIT_UINT32 ShowCursor( const BIT_BOOL p_State ) = 0;
		virtual BIT_UINT32 SetCursorPosition( Vector2_si32 p_Position ) = 0;
		virtual Vector2_si32 GetCursorScreenPosition( ) = 0;

		// Public functions
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }
		BIT_INLINE BIT_BOOL IsFocused( ) const { return m_Focused; }
		BIT_BOOL PollEvent( Event & p_Event );

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title ) = 0;

		// Get functions
		BIT_INLINE BIT_UINT32 GetEventCount( ) const { return m_EventQueue.size( ); }
		BIT_INLINE Vector2_ui32 GetSize( ) const { return m_Size; }
		BIT_INLINE Vector2_si32 GetPosition( ) const { return m_Position; }
		BIT_INLINE BIT_UINT32 GetBits( ) const { return m_Bits; }
		BIT_INLINE std::string GetTitle( ) const { return m_Title; }
		BIT_INLINE BIT_UINT32 GetStyle( ) const { return m_Style; }

	protected:

		// Protected variables
		BIT_BOOL m_Open;
		BIT_BOOL m_Focused;
		Vector2_ui32 m_Size;
		Vector2_si32 m_Position;
		BIT_UINT32 m_Bits;
		std::string m_Title;
		BIT_UINT32 m_Style;
		std::list< Event > m_EventQueue;
	};


	// Use this function for window creation!
	// Function for Window allocation
	BIT_API Window * CreateWindow( );




}

#endif
*/