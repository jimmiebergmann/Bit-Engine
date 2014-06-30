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

#include <Bit/Window/Linux/RenderWindowLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

// X11 hack, we need to do this because of conflicts with the Bit engine data types.
#define Bool int
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
// We need to undef Bool for linux build( In x11 lib)
#undef Bool
#undef None

#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

// Motif window hints
#define MWM_HINTS_FUNCTIONS (1L << 0)
#define MWM_HINTS_DECORATIONS (1L << 1)
#define MWM_HINTS_INPUT_MODE (1L << 2)
#define MWM_HINTS_STATUS (1L << 3)

// bit definitions for MwmHints.functions
#define MWM_FUNC_ALL (1L << 0)
#define MWM_FUNC_RESIZE (1L << 1)
#define MWM_FUNC_MOVE (1L << 2)
#define MWM_FUNC_MINIMIZE (1L << 3)
#define MWM_FUNC_MAXIMIZE (1L << 4)
#define MWM_FUNC_CLOSE (1L << 5)

// bit definitions for MwmHints.decorations
#define MWM_DECOR_ALL (1L << 0)
#define MWM_DECOR_BORDER (1L << 1)
#define MWM_DECOR_RESIZEH (1L << 2)
#define MWM_DECOR_TITLE (1L << 3)
#define MWM_DECOR_MENU (1L << 4)
#define MWM_DECOR_MINIMIZE (1L << 5)
#define MWM_DECOR_MAXIMIZE (1L << 6)

namespace Bit
{


	// Constructors/destructors
	RenderWindowLinux::RenderWindowLinux( ) :
        m_Open( false ),
		m_VideoMode( ),
		m_Title( "" ),
		m_Style( static_cast<Uint32>( Style::None ) ),
        m_pDisplay( NULL ),
        m_Screen( 0 ),
        m_Window( 0 ),
		m_Events( ),
		m_Focused( false )
	{
		m_PressedButtons.reserve( Mouse::ButtonCount );
		m_PressedKeys.reserve( 8 );
	}

	RenderWindowLinux::RenderWindowLinux( const VideoMode & p_VideoMode, const std::string & p_Title, const Uint32 p_Style ) :
		m_Open( false ),
		m_VideoMode( ),
		m_Title( p_Title ),
		m_Style( static_cast<Uint32>( Style::None ) ),
		m_pDisplay( NULL ),
        m_Screen( 0 ),
        m_Window( 0 ),
		m_Events( ),
		m_Focused( false )
	{
		m_PressedButtons.reserve( Mouse::ButtonCount );
		m_PressedKeys.reserve( 8 );
		Open( p_VideoMode, p_Title, p_Style );
	}

	RenderWindowLinux::~RenderWindowLinux( )
	{
		Close( );
	}

    Bool RenderWindowLinux::Open( const VideoMode & p_VideoMode, const std::string & p_Title, const Uint32 p_Style )
	{
	    // open a connection with X server
	    if( ( m_pDisplay = XOpenDisplay( NULL ) ) == NULL )
        {
            std::cout << "[RenderWindowLinux::Create] Can not connect to X server." << std::endl;
            return false;
        }

        // Initialize the X thread
        // Should we?!?!
        XInitThreads( );

	    // Get the screen
	    m_Screen = DefaultScreen( m_pDisplay );

        // Creat the window attributes
        XSetWindowAttributes WindowAttributes;
        WindowAttributes.colormap = DefaultColormap( m_pDisplay, m_Screen );
        WindowAttributes.event_mask =   KeyPressMask | KeyReleaseMask |
                                        ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask |
                                        EnterWindowMask | LeaveWindowMask | VisibilityChangeMask |
                                        FocusChangeMask | ExposureMask | StructureNotifyMask;

        // Create the window
        m_Window = XCreateWindow( m_pDisplay,
                                 DefaultRootWindow( m_pDisplay ),
                                 0, 0, p_VideoMode.GetSize( ).x, p_VideoMode.GetSize( ).y,
                                 0,
                                 DefaultDepth( m_pDisplay, m_Screen ),
                                 InputOutput,
                                 DefaultVisual( m_pDisplay, m_Screen ),
                                 CWBorderPixel | CWEventMask | CWColormap,
                                 &WindowAttributes );



        // It's very important to set the delete message. Else we wont be able to close the window.
        ::Atom wmDeleteMessage = XInternAtom( m_pDisplay, "WM_DELETE_WINDOW", false );
        XSetWMProtocols( m_pDisplay, m_Window, &wmDeleteMessage, 1 );

        // Set the window title
        SetTitle( p_Title.c_str( ) );


        // Let's set up the window decoration and the functionality
        ::Atom PropertyAtom = XInternAtom( m_pDisplay, "_MOTIF_WM_HINTS", false );
        if( PropertyAtom )
        {
            struct HintsStruct
            {
                Uint32 Flags;
                Uint32 Functions;
                Uint32 Decorations;
                Int32 InputMode;
                Uint32 State;
            };

            HintsStruct Hints;
            Hints.Flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_FUNCTIONS;
            Hints.Functions = 0;
            Hints.Decorations = 0;


            // Go through all the styles we want to apply to the window
            if( p_Style == Bit::Style::Default )
            {
                Hints.Functions |= MWM_FUNC_ALL;
                Hints.Decorations |= MWM_DECOR_ALL;
            }
            else
            {
                // Always set the resize and maximize functions and decorations.
                // Some window managers seems to require this.
                // Resizing can be disabled.
                Hints.Functions |= MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE;
                Hints.Decorations |= MWM_DECOR_RESIZEH | MWM_DECOR_MAXIMIZE;


                if( p_Style & Bit::Style::Close )
                {
                     Hints.Functions |= MWM_FUNC_CLOSE;
                }

                if( p_Style & Bit::Style::Minimize )
                {
                     Hints.Functions |= MWM_FUNC_MINIMIZE;
                     Hints.Decorations |= MWM_DECOR_MINIMIZE;
                }

                if( p_Style & Bit::Style::TitleBar )
                {
                     Hints.Functions |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
                     Hints.Decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MENU | MWM_DECOR_MINIMIZE;
                }
            }

            // Apply the changes
            XChangeProperty( m_pDisplay, m_Window, PropertyAtom, PropertyAtom, 32, PropModeReplace, (unsigned char *) &Hints, 5 );

            // Force x server to disable window resizing
            if (!( p_Style & Bit::Style::Resize ) )
            {
                XSizeHints * SizeHints = XAllocSizeHints( );
                SizeHints->flags = PMinSize | PMaxSize;
                SizeHints->min_width = p_VideoMode.GetSize( ).x;
                SizeHints->min_height = p_VideoMode.GetSize( ).y;
                SizeHints->max_width = p_VideoMode.GetSize( ).x;
                SizeHints->max_height = p_VideoMode.GetSize( ).y;

                // Set the hints
                XSetWMNormalHints( m_pDisplay, m_Window, SizeHints);

                // Free the size hints
                XFree(SizeHints);
            }
        }
        else
        {
            std::cout << "[RenderWindowLinux::Open] Can not get the property atom \"_MOTIF_WM_HINTS\"." << std::endl;
        }

        // Display the window.
        XMapWindow( m_pDisplay, m_Window );
        XFlush( m_pDisplay );


		// Set the rest of the member variables
		m_VideoMode = p_VideoMode;
		m_Title = p_Title;
		m_Style = p_Style;
		m_Size = p_VideoMode.GetSize( );
		m_Open = true;
		m_Focused = true;
		return true;
	}

    void RenderWindowLinux::Close( )
	{
        if( m_pDisplay )
	    {
	        // Destroy the display
            XDestroyWindow( m_pDisplay, m_Window );

            // Clost the display
	        XCloseDisplay( m_pDisplay );
            m_pDisplay = NULL;
            m_Screen = 0;
            m_Window = 0;
	    }

        m_Open = false;
	}


	void RenderWindowLinux::Update( )
	{
	  // Make sure that the window is open
		if( m_Open == false )
		{
		    return;
		}

        // Update the keyboard and mouse events
      // m_pKeyboard->Update( );
        //m_pMouse->Update( );


        // Declare an x server event.
		XEvent E;

		// Declare our event
		Bit::Event Event;

		// Loop through all the events
		while( XPending( m_pDisplay ) > 0 & m_Open )
		{
		    // Get the next event
		    XNextEvent( m_pDisplay, &E );

		    switch( E.type )
		    {
                case ClientMessage:
		        {
                    // This is ahacky way of checking if we closed the window
		            if( *XGetAtomName( m_pDisplay, E.xclient.message_type ) == *"WM_PROTOCOLS" )
		            {
		                Event.Type = Bit::Event::Closed;
		                m_Events.push( Event );

                        return;
		            }
		        }
		        break;
                case ConfigureNotify: // Resized and moved event
		        {

		            // Resizing
		            if( E.xconfigure.width != m_Size.x || E.xconfigure.height != m_Size.y )
		            {
		                m_Size = Bit::Vector2u32( E.xconfigure.width, E.xconfigure.height );
                        m_Position = Bit::Vector2i32( E.xconfigure.x, E.xconfigure.y );

                        Event.Type = Bit::Event::Resized;
                        Event.Size = m_Size;
                        m_Events.push( Event );
		            }

		            // Moving
		            if( ( E.xconfigure.x != 0 || E.xconfigure.y != 0 ) &&
                        ( E.xconfigure.x != m_Position.x || E.xconfigure.x != m_Position.y ) )
		            {
		                m_Position = Bit::Vector2i32( E.xconfigure.x, E.xconfigure.y );

                        Event.Type = Bit::Event::Moved;
                        Event.Position = m_Position;
                        m_Events.push( Event );
		            }

		        }
		        break;
		        case FocusIn:
		        {
                    Event.Type = Bit::Event::GainedFocus;
                    m_Events.push( Event );
		        }
		        break;
		        case FocusOut:
		        {
                    Event.Type = Bit::Event::LostFocus;
                    m_Events.push( Event );
		        }
		        break;

		        // The key and button events are a little special for x server.
		        // Let's just grab the just[pressed][released] events and elulate the
		        // KeyIsDown / ButtonIsDown events. Else, we'll get key register gaps that are
		        // depending on the keyboard speed. If that makes sense...
		        case KeyPress:
		        {
                    // Get the right key index
                    KeySym Keysym = XLookupKeysym( &E.xkey, 0 );

                    // not supporting more than 2^16 keys
                    if( Keysym < 65536 )
                    {
                        Keyboard::eKey Key = m_Keyboard.TranslateKeyToBitKey( static_cast< Uint16 >( Keysym ) );

                        if( Key != Keyboard::Unknown )
                        {
                            // Was the key just pressed? (KeyJustPressed event)
                            if( !m_Keyboard.GetPreviousKeyState( Key ) )
                            {
                                Event.Type = Bit::Event::KeyJustPressed;
                                Event.Key = Key;
                                m_Events.push( Event );

                                // Add the event to the input vector if it's not already in the list
                                Bool NotInList = true;
                                for( SizeType i = 0; i < m_PressedKeys.size( ); i++ )
                                {
                                    if( Key == m_PressedKeys[ i ] )
                                    {
                                        NotInList = false;
                                        break;
                                    }
                                }
                                if( NotInList )
                                {
                                    m_PressedKeys.push_back( Key );
                                }

                            }

                            // Set the keyboard state
                            m_Keyboard.SetCurrentKeyState( Key, true );
                        }
                    }
                }
		        break;
                case KeyRelease:
		        {
		            // Get the right key index
                    KeySym Keysym = XLookupKeysym( &E.xkey, 0 );

                    // not supporting more than 2^16 keys
                    if( Keysym < 65536 )
                    {
                        Keyboard::eKey Key = m_Keyboard.TranslateKeyToBitKey( static_cast< Uint16 >( Keysym ) );

                        if( Key != Keyboard::Unknown )
                        {
                            // Is the key really "just" released?
                            Bool JustReleased = true;

                            // Check if the key just was pressed(check for KeyPress event)
                            if( XEventsQueued( m_pDisplay, QueuedAfterReading ) )
                            {
                                XEvent NewEvent;
                                XPeekEvent( m_pDisplay, &NewEvent );

                                // Set the flag fales if the time for the two events are the same
                                if( NewEvent.type == KeyPress && NewEvent.xkey.time == E.xkey.time  &&
                                   NewEvent.xkey.keycode == E.xkey.keycode )
                                {
                                    JustReleased = false;
                                }
                            }

                            // Yes, the key is just released.
                            if( JustReleased )
                            {
                                Event.Type = Bit::Event::KeyJustReleased;
                                Event.Key = Key;
                                m_Events.push( Event );

                                // Set the keyboard state
                                m_Keyboard.SetCurrentKeyState( Key, false );

                                 // Remove the key from the input vector
                                for( SizeType i = 0; i < m_PressedKeys.size( ); i++ )
                                {
                                    if( Key == m_PressedKeys[ i ] )
                                    {
                                        m_PressedKeys.erase( m_PressedKeys.begin( ) + i );
                                        break;
                                    }
                                }
                            }
                        }
                    }
		        }
		        break;
                case MotionNotify:
		        {
                    Event.Type = Bit::Event::MouseMoved;
                    Event.MousePosition = Bit::Vector2i32( E.xmotion.x, E.xmotion.y );
                    m_Events.push( Event );
		        }
		        break;
		        case ButtonPress:
		        {
                    // not supporting more than 2^16 keys
                    if( E.xbutton.button <= Mouse::ButtonCount )
                    {
                        Mouse::eButton Button = m_Mouse.TranslateButtonToBitButton( static_cast< Uint16 >( E.xbutton.button ) );

                        if( Button != Mouse::Unknown )
                        {
                            // Was the button just pressed? (ButtonJustPressed event)
                            if( !m_Mouse.GetPreviousButtonState( Button ) )
                            {
                                Event.Type = Bit::Event::MouseJustPressed;
                                Event.Button = Button;
                                Event.MousePosition = Bit::Vector2i32( E.xbutton.x, E.xbutton.y );
                                m_Events.push( Event );

                                // Add the event to the input vector if it's not already in the list
                                Bool NotInList = true;
                                for( SizeType i = 0; i < m_PressedButtons.size( ); i++ )
                                {
                                    if( Button == m_PressedButtons[ i ] )
                                    {
                                        NotInList = false;
                                        break;
                                    }
                                }
                                if( NotInList )
                                {
                                    m_PressedButtons.push_back( Button );
                                }
                            }

                            // Set the mouse state
                            m_Mouse.SetCurrentButtonState( Button, true );
                        }
                    }
		        }
		        break;
                case ButtonRelease:
		        {
		            // not supporting more than 2^16 keys
                    if( E.xbutton.button <= Mouse::ButtonCount )
                    {
                        Mouse::eButton Button = m_Mouse.TranslateButtonToBitButton( static_cast< Uint16 >( E.xbutton.button ) );

                        if( Button != Mouse::Unknown )
                        {
                            Event.Type = Bit::Event::MouseJustReleased;
                            Event.Button = Button;
                            Event.MousePosition = Bit::Vector2i32( E.xbutton.x, E.xbutton.y );
                            m_Events.push( Event );

                            // Set the mouse state
                            m_Mouse.SetCurrentButtonState( Button, false );

                             // Remove the key from the input vector
                            for( SizeType i = 0; i < m_PressedButtons.size( ); i++ )
                            {
                                if( Button == m_PressedButtons[ i ] )
                                {
                                    m_PressedButtons.erase( m_PressedButtons.begin( ) + i );
                                    break;
                                }
                            }
                        }
                    }
		        }
		        break;
		        default:
		        {
                    // ...
		        }
		        break;
		    }
		}

		// Add additional keyboard and mouse events.
		// We need to fill up some event gaps.
		// Add the event to the input vector if it's not already in the list
        for( SizeType i = 0; i < m_PressedKeys.size( ); i++ )
        {
            Event.Type = Bit::Event::KeyPressed;
            Event.Key = m_PressedKeys[ i ];
            m_Events.push( Event );
        }
        for(  SizeType i = 0; i < m_PressedButtons.size( ); i++ )
        {
            Event.Type = Bit::Event::MousePressed;
            Event.Button = m_PressedButtons[ i ];
            m_Events.push( Event );
        }
	}

	Bool RenderWindowLinux::PollEvent( Event & p_Event )
	{
		if( m_Events.size( ) )
		{
			// Get the top event
			p_Event = m_Events.front( );
			m_Events.pop( );
			return true;
		}

		return false;
	}

	void RenderWindowLinux::SetTitle( const std::string & p_Title )
	{
	    // Set the name of the X11 window.
	    XStoreName( m_pDisplay, m_Window, p_Title.c_str( ) );

        // Store the title
        m_Title = p_Title;
	}

	Bool RenderWindowLinux::IsOpen( ) const
	{
		return m_Open;
	}

	Bool RenderWindowLinux::IsFocused( ) const
	{
		return m_Focused;
	}

	const VideoMode & RenderWindowLinux::GetVideoMode( ) const
	{
		return m_VideoMode;
	}

	const std::string & RenderWindowLinux::GetTitle( ) const
	{
		return m_Title;
	}

	::Display * RenderWindowLinux::GetDisplayDevice( ) const
	{
        return m_pDisplay;
	}

    ::Window RenderWindowLinux::GetWindowDevice( ) const
    {
        return m_Window;
    }

    int RenderWindowLinux::GetScreenDevice( ) const
    {
        return m_Screen;
    }



/*
	BIT_UINT32 WindowLinux::Show( const BIT_BOOL p_State )
	{
		// No code here yet.

		return BIT_ERROR;
	}

	BIT_UINT32 WindowLinux::ShowCursor( const BIT_BOOL p_State )
	{
		//::ShowCursor( p_State );
		return BIT_OK;
	}

	BIT_UINT32 WindowLinux::SetCursorPosition( Vector2_si32 p_Position )
	{
		//SetCursorPos( p_Position.x, p_Position.y );
		return BIT_OK;
	}

	Vector2_si32 WindowLinux::GetCursorScreenPosition( )
	{
		//POINT Point;
		//GetCursorPos( &Point );
		//return Vector2_si32( Point.x, Point.y );
		return Vector2_si32( 0, 0 );
	}

	// Set functions
	BIT_UINT32 WindowLinux::SetTitle( std::string p_Title )
	{
        // Set the window title
        XStoreName( m_pDisplay, m_Window, p_Title.c_str( ) );
        return BIT_OK;
	}

	// Get functions
    ::Window WindowLinux::GetWindowDevice( ) const
    {
        return m_Window;
    }

    ::Display * WindowLinux::GetDisplayDevice( ) const
    {
        return m_pDisplay;
    }

    int WindowLinux::GetScreenDevice( ) const
    {
        return m_Screen;
    }

*/
}

#endif
