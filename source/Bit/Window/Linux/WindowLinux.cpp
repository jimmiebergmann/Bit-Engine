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
#include <Bit/Window.hpp>

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
         m_Screen( 0 )
	{
	}

	// Public functions
	BIT_UINT32 WindowLinux::Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title, const BIT_UINT32 p_Style )
	{












/*

    if(!(m_pDisplay=XOpenDisplay(NULL)))
    {
      bitTrace("ERROR: could not open display\n");
      return 1;
    }

  m_Screen = DefaultScreen(m_pDisplay);*/
  //::Window rootwind = RootWindow(m_pDisplay, m_Screen);
 /* Colormap cmap = DefaultColormap(m_pDisplay, m_Screen);
  Atom wmDeleteMessage = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", False);

  int blackColor = BlackPixel(m_pDisplay, m_Screen);
  int whiteColor = WhitePixel(m_pDisplay, m_Screen);

    m_Window= XCreateSimpleWindow( m_pDisplay, RootWindow( m_pDisplay, m_Screen ), 0, 0, 200, 100, 0, blackColor, blackColor);
  XMapWindow(m_pDisplay, m_Window);*/
 // XSetWMProtocols(m_pDisplay, m_Window, &wmDeleteMessage, 1);
  /*bool running = true;
  while(running)
  {
    XEvent e;
    XNextEvent(m_pDisplay, &e);
    switch  (e.type)
    {
      case ClientMessage:
        if(e.xclient.data.l[0] == wmDeleteMessage)
        {
           bitTrace("Shutting down");
          //XDestroyWindow(m_pDisplay,e.xclient.window);
          running=false;
          break;
        }
        break;
    }
  }

    XCloseDisplay( m_pDisplay);

*/









        // open a connection with X server
	    if( ( m_pDisplay = XOpenDisplay( BIT_NULL ) ) == BIT_NULL )
        {
            bitTrace( "[WindowLinux::Create] Can not connect to X server." );
            return BIT_ERROR;
        }

	    // Get the screen
	    m_Screen = DefaultScreen( m_pDisplay );







        // LIST OF THE MASKS WE CAN USE
        /*
        #define NoEventMask			0L
        #define KeyPressMask			(1L<<0)
        #define KeyReleaseMask			(1L<<1)
        #define ButtonPressMask			(1L<<2)
        #define ButtonReleaseMask		(1L<<3)
        #define EnterWindowMask			(1L<<4)
        #define LeaveWindowMask			(1L<<5)
        #define PointerMotionMask		(1L<<6)
        #define PointerMotionHintMask		(1L<<7)
        #define Button1MotionMask		(1L<<8)
        #define Button2MotionMask		(1L<<9)
        #define Button3MotionMask		(1L<<10)
        #define Button4MotionMask		(1L<<11)
        #define Button5MotionMask		(1L<<12)
        #define ButtonMotionMask		(1L<<13)
        #define KeymapStateMask			(1L<<14)
        #define ExposureMask			(1L<<15)
        #define VisibilityChangeMask		(1L<<16)
        #define StructureNotifyMask		(1L<<17)
        #define ResizeRedirectMask		(1L<<18)
        #define SubstructureNotifyMask		(1L<<19)
        #define SubstructureRedirectMask	(1L<<20)
        #define FocusChangeMask			(1L<<21)
        #define PropertyChangeMask		(1L<<22)
        #define ColormapChangeMask		(1L<<23)
        #define OwnerGrabButtonMask		(1L<<24)
        */

        // Creat the window attricutes
        XSetWindowAttributes WindowAttributes;
        WindowAttributes.border_pixel = 0;
        WindowAttributes.border_pixmap = None;
        WindowAttributes.event_mask =   KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                                        EnterWindowMask | LeaveWindowMask | PointerMotionMask | VisibilityChangeMask |
                                        FocusChangeMask | ExposureMask | StructureNotifyMask;

        // Create the window
        m_Window = XCreateWindow( m_pDisplay,
                                 DefaultRootWindow( m_pDisplay ),
                                 0, 0, p_Size.x, p_Size.y,
                                 0,
                                 DefaultDepth( m_pDisplay, m_Screen ),
                                 InputOutput,
                                 DefaultVisual( m_pDisplay, m_Screen ),
                                 CWBorderPixel | CWEventMask,
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

            /*
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
            */

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

		return BIT_OK;
	}

	BIT_UINT32 WindowLinux::Destroy( )
	{
	    XDestroyWindow( m_pDisplay, m_Window );
	    //XFreeColormap( m_pDisplay, m_ColorMap );

	    // Clost the display
	    if( m_pDisplay )
	    {
	        XCloseDisplay( m_pDisplay );
            m_pDisplay = BIT_NULL;
	    }

		return BIT_ERROR;
	}

	BIT_UINT32 WindowLinux::DoEvents( )
	{


        // !NOTE!
		// Experimental code!
		if( m_pDisplay == BIT_NULL )
		{
		    return BIT_ERROR;
		}


        // Declare an x server event.
		XEvent Event;

		// Loop through all the events
		while( XPending( m_pDisplay ) > 0 )
		{
		    // Get the next event
		    XNextEvent( m_pDisplay, &Event );

		    switch( Event.type )
		    {

                case ClientMessage:
		        {
		            if( *XGetAtomName( m_pDisplay, Event.xclient.message_type ) == *"WM_PROTOCOLS" )
		            {
                        return BIT_ERROR;
		            }
		        }
		        break;
                //case blablabla:
		        //break;
              /*  case DestroyNotify:
                {
                    bitTrace( "Destroying window! 1" );
                    XDestroyWindow( m_pDisplay, m_Window );
                }
                break;

		        case ClientMessage:
		        {
		            bitTrace( "Destroying window! 2" );
                    if( (::Atom)Event.xclient.data.l[ 0 ] == XInternAtom( m_pDisplay, "WM_DELETE_WINDOW", false ) )
                    {
                        bitTrace( "Destroying window!  3" );
                        XDestroyWindow( m_pDisplay, Event.xclient.window );
                    }
		        }
		        break;
*/
		        default:
		        break;
		    }
		}

		return BIT_OK;
	}

	// Set functions
	BIT_UINT32 WindowLinux::SetTitle( std::string p_Title )
	{
        // Set the window title
        XStoreName( m_pDisplay, m_Window, p_Title.c_str( ) );
        return BIT_OK;
	}


	// Private functions



}
