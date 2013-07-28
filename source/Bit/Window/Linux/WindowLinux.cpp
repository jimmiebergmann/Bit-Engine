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

#include <Bit/Window/Linux/WindowLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Debugger.hpp>
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
	WindowLinux::WindowLinux( ) :
         m_pDisplay( BIT_NULL ),
         m_Screen( 0 ),
         m_pKeyboard( BIT_NULL )
	{
        m_Open = BIT_FALSE;

        // Create the keyboard
        m_pKeyboard = CreateKeyboard();
	}

	WindowLinux::~WindowLinux( )
	{
	    // Close the window
        Close( );
        	    // Delete the keyboard
	    if( m_pKeyboard )
	    {
	        delete m_pKeyboard;
	        m_pKeyboard = BIT_NULL;
	    }
	}

	// Public functions
	BIT_UINT32 WindowLinux::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits )
	{
	    return Open( p_Size, p_Bits, "Bit Engine Application", Bit::Window::Style_All );
	}

    BIT_UINT32 WindowLinux::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title)
	{
	    return Open( p_Size, p_Bits, p_Title, Bit::Window::Style_All );
	}

	BIT_UINT32 WindowLinux::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title, const BIT_UINT32 p_Style )
	{
	    // Make sure that the keyboard is ok
	    if( m_pKeyboard == BIT_NULL )
	    {
	        bitTrace( "[WindowLinux::Create] NULL keyboard." );
            return BIT_ERROR;
	    }

        // open a connection with X server
	    if( ( m_pDisplay = XOpenDisplay( BIT_NULL ) ) == BIT_NULL )
        {
            bitTrace( "[WindowLinux::Create] Can not connect to X server." );
            return BIT_ERROR;
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
                                 0, 0, p_Size.x, p_Size.y,
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
                BIT_UINT32 Flags;
                BIT_UINT32 Functions;
                BIT_UINT32 Decorations;
                BIT_SINT32 InputMode;
                BIT_UINT32 State;
            };

            HintsStruct Hints;
            Hints.Flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_FUNCTIONS;
            Hints.Functions = 0;
            Hints.Decorations = 0;

            // Go through all the styles we want to apply to the window
            if( p_Style == Bit::Window::Style_All )
            {
                Hints.Functions |= MWM_FUNC_ALL;
                Hints.Decorations |= MWM_DECOR_ALL;
            }
            else
            {
                if( p_Style & Bit::Window::Style_Close )
                {
                     Hints.Functions |= MWM_FUNC_CLOSE;
                }

                if( p_Style & Bit::Window::Style_Minimize )
                {
                     Hints.Functions |= MWM_FUNC_MINIMIZE;
                     Hints.Decorations |= MWM_DECOR_MINIMIZE;
                }

                if( p_Style & Bit::Window::Style_Resize )
                {
                     Hints.Functions |= MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE;
                     Hints.Decorations |= MWM_DECOR_RESIZEH | MWM_DECOR_MAXIMIZE;
                }

                if( p_Style & Bit::Window::Style_TitleBar )
                {
                     Hints.Functions |= MWM_FUNC_MOVE;
                     Hints.Decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MENU;
                }
            }

            // Apply the changes
            XChangeProperty( m_pDisplay, m_Window, PropertyAtom, PropertyAtom, 32, PropModeReplace, (unsigned char *) &Hints, 5 );

        }
        else
        {
            bitTrace( "[WindowLinux::Create] Can not get the property atom \"_MOTIF_WM_HINTS\"." );
        }

        // Display the window.
        XMapWindow( m_pDisplay, m_Window );
        XFlush( m_pDisplay );

        // Finally set the base class attributes
		m_Open = BIT_TRUE;
		m_Size = p_Size;
		m_Bits = p_Bits;
		m_Title = p_Title;
		m_Style = p_Style;

		return BIT_OK;
	}

	BIT_UINT32 WindowLinux::Close( )
	{
	    if( m_pDisplay )
	    {
	        // Destroy the display
            XDestroyWindow( m_pDisplay, m_Window );

            // Clost the display
	        XCloseDisplay( m_pDisplay );
            m_pDisplay = BIT_NULL;
	    }

        m_Open = BIT_FALSE;
		return BIT_ERROR;
	}

	BIT_UINT32 WindowLinux::Update( )
	{
		if( m_pDisplay == BIT_NULL )
		{
		    return BIT_ERROR;
		}

        // Declare an x server event.
		XEvent E;

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
		                Bit::Event Event;
		                Event.Type = Bit::Event::Closed;
		                m_EventQueue.push_back( Event );

		                // Remember to actually close the window
		                //Close( );
                        return BIT_OK;
		            }
		        }
		        break;
                case ConfigureNotify: // Resized and moved event
		        {

		            // Resizing
		            if( E.xconfigure.width != m_Size.x || E.xconfigure.height != m_Size.y )
		            {
		                m_Size = Bit::Vector2_ui32( E.xconfigure.width, E.xconfigure.height );
		                // Make sure to calculate the position as well size it may change when you resize the window
                        m_Position = Bit::Vector2_si32( E.xconfigure.x, E.xconfigure.y );

                        Bit::Event Event;
                        Event.Type = Bit::Event::Resized;
                        Event.Size = m_Size;
                        m_EventQueue.push_back( Event );
		            }

		            // Moving
		            if( ( E.xconfigure.x != 0 || E.xconfigure.y != 0 ) &&
                        ( E.xconfigure.x != m_Position.x || E.xconfigure.x != m_Position.y ) )
		            {
		                m_Position = Bit::Vector2_si32( E.xconfigure.x, E.xconfigure.y );

                        Bit::Event Event;
                        Event.Type = Bit::Event::Moved;
                        Event.Position = m_Position;
                        m_EventQueue.push_back( Event );
		            }

		        }
		        break;
		        case FocusIn:
		        {
		            Bit::Event Event;
                    Event.Type = Bit::Event::GainedFocus;
                    m_EventQueue.push_back( Event );
		        }
		        break;
		        case FocusOut:
		        {
		            Bit::Event Event;
                    Event.Type = Bit::Event::LostFocus;
                    m_EventQueue.push_back( Event );
		        }
		        break;
		        // TEMPORARY DISABLED
		        case KeyPress:
		        {
                    // Get the right key index
                    KeySym Keysym = XLookupKeysym( &E.xkey, 0 );

                    // not supporting more than 2^16 keys
                    if( Keysym < 65536 )
                    {
                        Keyboard::eKey Key = m_pKeyboard->TranslateKeyToBitKey( static_cast< BIT_UINT16 >( Keysym ) );

                        if( Key != Keyboard::Key_None )
                        {
                            Bit::Event Event;
                            Event.Type = Bit::Event::KeyPressed;
                            Event.Key = Key;
                            m_EventQueue.push_back( Event );
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
                        Keyboard::eKey Key = m_pKeyboard->TranslateKeyToBitKey( static_cast< BIT_UINT16 >( Keysym ) );

                        if( Key != Keyboard::Key_None )
                        {
                            Bit::Event Event;
                            Event.Type = Bit::Event::KeyPressed;
                            Event.Key = Key;
                            m_EventQueue.push_back( Event );
                        }
                    }
		        }
		        break;
                case MotionNotify:
		        {
		            Bit::Event Event;
                    Event.Type = Bit::Event::MouseMoved;
                    Event.MousePosition = Bit::Vector2_si32( E.xmotion.x, E.xmotion.y );
                    m_EventQueue.push_back( Event );
		        }
		        break;
                // TEMPORARY DISABLED
		      /*  case ButtonPress:
		        {
		            Bit::Event Event;
                    Event.Type = Bit::Event::MouseButtonPressed;
                    Event.Button = E.xbutton.button;
                    Event.MousePosition = Bit::Vector2_si32( E.xbutton.x, E.xbutton.y );
                    m_EventQueue.push_back( Event );
		        }
		        break;
                case ButtonRelease:
		        {
		            Bit::Event Event;
                    Event.Type = Bit::Event::MouseButtonReleased;
                    Event.Button = E.xbutton.button;
                    Event.MousePosition = Bit::Vector2_si32( E.xbutton.x, E.xbutton.y );
                    m_EventQueue.push_back( Event );
		        }
		        break;*/
		        default:
		        {
                    // ...
		        }
		        break;
		    }
		}

		return BIT_OK;
	}

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


}

#endif
