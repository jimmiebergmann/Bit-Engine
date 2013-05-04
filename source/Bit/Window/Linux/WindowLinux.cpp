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
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

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
		const std::string p_Title )
	{
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
        WindowAttributes.event_mask =   KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                                        EnterWindowMask | LeaveWindowMask | PointerMotionMask | VisibilityChangeMask |
                                        FocusChangeMask;

        // Create the window
        m_Window = XCreateWindow( m_pDisplay,
                                 DefaultRootWindow( m_pDisplay ),
                                 0, 0, p_Size.x, p_Size.y,
                                 0,
                                 CopyFromParent,
                                 CopyFromParent,
                                 DefaultVisual( m_pDisplay, m_Screen ),
                                 0,
                                 &WindowAttributes );

        // Set the window title
        XStoreName( m_pDisplay, m_Window, p_Title.c_str( ) );


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
                //case blablabla:
		        //break;

		        default:
		        break;
		    }
		}

		return BIT_OK;
	}

	// Set functions
	BIT_UINT32 WindowLinux::SetTitle( std::string p_Title )
	{
        if( m_pDisplay == BIT_NULL )
	    {
	        bitTrace( "CAn not set the title ");
	        return BIT_ERROR;
	    }

        // Set the window title
        XStoreName( m_pDisplay, m_Window, p_Title.c_str( ) );
        return BIT_OK;
	}


	// Private functions



}
