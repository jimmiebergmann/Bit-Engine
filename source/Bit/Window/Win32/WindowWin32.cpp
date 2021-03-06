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
/*
#include <Bit/Window/Win32/WindowWin32.hpp>

#ifdef BIT_PLATFORM_WIN32

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>


namespace Bit
{

	// Constructors/destructors
	WindowWin32::WindowWin32( ) :
		m_DeviceContext( BIT_NULL ),
		m_Window( BIT_NULL ),
		m_RegisteredClass( BIT_FALSE ),
        m_pKeyboard( BIT_NULL ),
        m_pMouse( BIT_NULL )
	{
		m_Open = BIT_FALSE;
		m_Focused = BIT_FALSE;

		// Create the keyboard and mouse
        m_pKeyboard = new KeyboardWin32( );
        m_pMouse = new MouseWin32( );

        // Clear and reserve space for the input vecotrs
        m_PressedKeys.clear( );
        m_PressedKeys.reserve( KeyboardWin32::s_ReservedKeyCount );
        m_PressedButtons.clear( );
        m_PressedButtons.reserve( MouseWin32::s_ReservedButtonCount );
	}

	WindowWin32::~WindowWin32( )
	{
		Close( );

		// Delete the keyboard adn mouse
	    if( m_pKeyboard )
	    {
	        delete m_pKeyboard;
	        m_pKeyboard = BIT_NULL;
	    }

	    if( m_pMouse )
	    {
	        delete m_pMouse;
	        m_pMouse = BIT_NULL;
	    }
	}

	// Public functions
	BIT_UINT32 WindowWin32::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits )
	{
		return Open( p_Size, p_Bits, "Bit Engine Application", Bit::Window::Style_All );
	}

	BIT_UINT32 WindowWin32::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits, const std::string p_Title )
	{
		return Open( p_Size, p_Bits, p_Title, Bit::Window::Style_All );
	}

	BIT_UINT32 WindowWin32::Open( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title, const BIT_UINT32 p_Style )
	{
		// Convert the title into the right format
		std::wstring STemp = StringToWideString( p_Title ); // Temporary buffer is required
		LPCWSTR ConvertedTitle = STemp.c_str();

		// Let's fill a window class with information
		WNDCLASS WinClass;

		HINSTANCE WinInstance = GetModuleHandle( BIT_NULL ); // Grab any old handle
		WinClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WinClass.lpfnWndProc = ( WNDPROC )WindowWin32::WindowProcStatic;
		WinClass.cbClsExtra = 0;
		WinClass.cbWndExtra = 0;
		WinClass.hInstance = WinInstance;
		WinClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
		WinClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		WinClass.hbrBackground = NULL;
		WinClass.lpszClassName = L"Win32 Window";
		WinClass.lpszMenuName = NULL;

		// Register the window class
		if( !RegisterClass( &WinClass ) )
		{
			bitTrace( BIT_NULL, "[WindowWin32::Create] Can not register the window class.\n" );
			return BIT_ERROR;
		}
		// The class is now registered.
		m_RegisteredClass = BIT_TRUE;

		// Let's set the windows style
		RECT WindowRect;
		DWORD	ExStyle;
		DWORD	Style;

		HWND DesktopWindow = GetDesktopWindow( );
		HDC DesktopDC = GetDC( DesktopWindow );
		ReleaseDC( DesktopWindow, DesktopDC );

		WindowRect.left = ( long )0;
		WindowRect.right = ( long )p_Size.x;
		WindowRect.top = ( long )0;
		WindowRect.bottom = ( long )p_Size.y;

		m_Style = 0; // Do not directly copy the param style, clear out errors
		ExStyle = 0;
		Style = 0;

		// Set the window decoration style
		if( p_Style == Bit::Window::Style_All )
		{
			m_Style |= Bit::Window::Style_All;
			ExStyle = WS_EX_APPWINDOW;
			Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER;
		}
		else
		{
			if( p_Style & Bit::Window::Style_Minimize )
			{
				m_Style |= Bit::Window::Style_Minimize;
				Style |=  WS_MINIMIZEBOX;
			}

			if( p_Style & Bit::Window::Style_Resize )
			{
				m_Style |= Bit::Window::Style_Resize;
				Style |= WS_MAXIMIZEBOX | WS_SIZEBOX;
			}

			if( p_Style & Bit::Window::Style_TitleBar )
			{
				m_Style |= Bit::Window::Style_TitleBar;
				Style |= WS_CAPTION | WS_SYSMENU | WS_BORDER;
			}
		}

		// Apply yhe style
		AdjustWindowRectEx( &WindowRect, Style, FALSE, ExStyle );

		// Let's create the window.
		m_Window = CreateWindowEx(
			ExStyle,
			L"Win32 Window",
			ConvertedTitle,

			WS_CLIPSIBLINGS |
			WS_CLIPCHILDREN |
			Style,
			CW_USEDEFAULT, CW_USEDEFAULT,

			WindowRect.right - WindowRect.left,
			WindowRect.bottom - WindowRect.top,
			NULL,
			NULL,
			WinInstance,
			this );

		// Did the window creation succeed?
		if( m_Window == BIT_NULL )
		{
			bitTrace(BIT_NULL, "[WindowWin32::Create] Failed to create window.\n");
			return BIT_ERROR;
		}

		// Did we disable the [X] button?
		// We have to apply this property after the window creation. So here it comes.
		if( !( p_Style & Bit::Window::Style_Close ) && ( m_Style != Bit::Window::Style_All ) )
		{
			UINT dwExtra = MF_DISABLED | MF_GRAYED;
			HMENU hMenu = GetSystemMenu( m_Window, false );
			EnableMenuItem( hMenu, SC_CLOSE, MF_BYCOMMAND | dwExtra );
		}
		else
		{
			m_Style |= Bit::Window::Style_Close;
		}

		// Get the device context
		m_DeviceContext = GetDC( m_Window );

		// Now, show the window
		ShowWindow( m_Window, SW_SHOW );
		SetForegroundWindow( m_Window );
		SetFocus( m_Window );

		// Finally set the base class attributes
		m_Open = BIT_TRUE;
		m_Size = p_Size;
		m_Bits = p_Bits;
		m_Title = p_Title;
		m_Style = p_Style;

		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::Close( )
	{
		// Show the cursor
		::ShowCursor( BIT_TRUE );

		// Get the module handler.
		HINSTANCE Hinstance = GetModuleHandle( BIT_NULL );

		// Change back to normal display settings.
		ChangeDisplaySettings( NULL, 0 );

		// Make current to null.
		/// wglMakeCurrent( BIT_NULL, BIT_NULL );

		if(m_DeviceContext && !ReleaseDC( m_Window, m_DeviceContext ))
		{
			bitTrace( "[WindowWin32::Destroy] Releasing device context failed.\n" );
		}
		if(m_Window && !DestroyWindow( m_Window ) )
		{
			bitTrace( "[WindowWin32::Destroy] Destroying window failed.\n" );
		}

		if ( m_RegisteredClass && !UnregisterClass( L"Win32 Window", Hinstance ) )
		{
			bitTrace( "[WindowWin32::Destroy] Unregistering class failed.\n" );
		}

		// Null the attributes
		m_Open = BIT_FALSE;
		m_DeviceContext = NULL;
		m_Window = NULL;
		m_RegisteredClass = BIT_FALSE;

		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::Update( )
	{
		// Clear the event queue
		m_EventQueue.clear( );

		// Update the keyboard and mouse events
        m_pKeyboard->Update( );
        m_pMouse->Update( );

		// Go through all the window event messages
		MSG Message;
		while( PeekMessage( &Message, NULL, NULL, NULL, PM_REMOVE ))
		{
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}

		// Add additional keyboard and mouse events.
		// We need to fill up some event gaps.
		// Add the event to the input vector if it's not already in the list
		Bit::Event Event;
        for( BIT_MEMSIZE i = 0; i < m_PressedKeys.size( ); i++ )
        {
            Event.Type = Bit::Event::KeyPressed;
            Event.Key = m_PressedKeys[ i ];
            m_EventQueue.push_back( Event );
			m_pKeyboard->SetCurrentKeyState( m_PressedKeys[ i ], BIT_TRUE );
        }
        for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
        {
            Event.Type = Bit::Event::ButtonPressed;
            Event.Button = m_PressedButtons[ i ];
            m_EventQueue.push_back( Event );
			m_pMouse->SetCurrentButtonState( m_PressedButtons[ i ], BIT_TRUE );
        }

		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::Show( const BIT_BOOL p_State )
	{
		::ShowWindow( m_Window, p_State );
		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::ShowCursor( const BIT_BOOL p_State )
	{
		::ShowCursor( p_State );
		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::SetCursorPosition( Vector2_si32 p_Position )
	{
		SetCursorPos( p_Position.x, p_Position.y );
		return BIT_OK;
	}

	Vector2_si32 WindowWin32::GetCursorScreenPosition( )
	{
		POINT Point;
		GetCursorPos( &Point );
		return Vector2_si32( Point.x, Point.y );
	}

	// Set functions
	BIT_UINT32 WindowWin32::SetTitle( std::string p_Title )
	{
		// Convert the title into the right format
		std::wstring STemp = StringToWideString( p_Title ); // Temporary buffer is required
		LPCWSTR ConvertedTitle = STemp.c_str();

		// Change the title
		BIT_UINT32 Status = ( SetWindowText( m_Window, ConvertedTitle ) ? BIT_OK : BIT_ERROR );

		if( Status == BIT_OK )
		{
			m_Title = p_Title;
		}

		return Status;
	}

	// Get functions
	HDC WindowWin32::GetDeviceContext( ) const
	{
		return m_DeviceContext;
	}

	// Private functions
	LRESULT WindowWin32::WindowProcStatic( HWND p_HWND, UINT p_Message,
		WPARAM p_WParam, LPARAM p_LParam )
	{
		if ( p_Message == WM_NCCREATE)
		{
			SetWindowLongPtr( p_HWND, GWL_USERDATA, (LONG_PTR)( (LPCREATESTRUCT)p_LParam)->lpCreateParams );
		}
		else
		{
			WindowWin32 * pWnd = ( WindowWin32* )GetWindowLongPtr( p_HWND, GWL_USERDATA );

			if (pWnd != NULL)
			{
				return pWnd->WindowProc( p_HWND, p_Message, p_WParam, p_LParam );
			}
		}

		return DefWindowProc( p_HWND, p_Message, p_WParam, p_LParam );
	}

	LRESULT WindowWin32::WindowProc( HWND p_HWND, UINT p_Message,
		WPARAM p_WParam, LPARAM p_LParam )
	{
		// Declare our event
		Bit::Event Event;

		switch(p_Message)
		{
			case WM_CLOSE:
			{
				Event.Type = Bit::Event::Closed;
				m_EventQueue.push_back( Event );

				// Make sure to close the window as well.
				Close( );
			}
			break;
			case WM_SETFOCUS:
			{
				m_Focused = BIT_TRUE;
				Event.Type = Bit::Event::GainedFocus;
				m_EventQueue.push_back( Event );
			}
			break;
			case WM_KILLFOCUS:
			{
				m_Focused = BIT_FALSE;
				Event.Type = Bit::Event::LostFocus;
				m_EventQueue.push_back( Event );
			}
			break;
			case WM_SIZE:
			{
				Event.Type = Bit::Event::Resized;
				Event.Size = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
				m_EventQueue.push_back( Event );
			}
			break;
			case WM_MOVE:
			{
				Event.Type = Bit::Event::Moved;
				m_Position = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
				Event.Position = m_Position;
				m_EventQueue.push_back( Event );
			}
			break;
			case WM_KEYDOWN:
			{
				Keyboard::eKey Key = Keyboard::Key_None;

				switch( p_WParam )
				{
					case VK_SHIFT:
					{
						UINT scancode = MapVirtualKey( VK_RSHIFT, MAPVK_VK_TO_VSC );
						Key = ( ( (p_LParam & 0x01FF0000 ) >> 16 ) == scancode) ? Keyboard::Key_Shift_R : Keyboard::Key_Shift_L;
					}
					break;
					case VK_MENU: // Alt key
					{
						Key = ( HIWORD( p_LParam ) & KF_EXTENDED ) ? Keyboard::Key_Alt_R : Keyboard::Key_Alt_L;
					}
					break;
					case VK_CONTROL:
					{
						if( GetAsyncKeyState( VK_RMENU ) == 0 )
						{
							Key = ( HIWORD( p_LParam ) & KF_EXTENDED ) ? Keyboard::Key_Control_R : Keyboard::Key_Control_L;
						}
					}
					break;
					// No special key was handled, check the regualar keys.
					default:
					{
						// Get the right key code from the keyboard class.
						Key = m_pKeyboard->TranslateKeyToBitKey( static_cast< BIT_UINT16 >( LOWORD( p_WParam ) ) );
					}
					break;
				}

                if( Key != Keyboard::Key_None )
                {
                    // Was the key just pressed? (KeyJustPressed event)
                    if( !m_pKeyboard->GetPreviousKeyState( Key ) )
                    {
                        Event.Type = Bit::Event::KeyJustPressed;
                        Event.Key = Key;
                        m_EventQueue.push_back( Event );

                        // Add the event to the input vector if it's not already in the list
                        BIT_BOOL NotInList = BIT_TRUE;
                        for( BIT_MEMSIZE i = 0; i < m_PressedKeys.size( ); i++ )
                        {
                            if( Key == m_PressedKeys[ i ] )
                            {
                                NotInList = BIT_FALSE;
                                break;
                            }
                        }
                        if( NotInList )
                        {
                            m_PressedKeys.push_back( Key );
                        }

                    }

                    // Set the keyboard state
                    m_pKeyboard->SetCurrentKeyState( Key, BIT_TRUE );
                }
			}
			break;
			case WM_KEYUP:
			{
                Keyboard::eKey Key = Keyboard::Key_None;

				// Under Win32, we need to check some extra checks for some certain keys. (sigh)
				switch( p_WParam )
				{
					case VK_SHIFT:
					{
						UINT scancode = MapVirtualKey( VK_RSHIFT, MAPVK_VK_TO_VSC );
						Key = ( ( (p_LParam & 0x01FF0000 ) >> 16 ) == scancode) ? Keyboard::Key_Shift_R : Keyboard::Key_Shift_L;
					}
					break;
					case VK_MENU: // Alt key
					{
						Key = ( HIWORD( p_LParam ) & KF_EXTENDED ) ? Keyboard::Key_Alt_R : Keyboard::Key_Alt_L;
					}
					break;
					case VK_CONTROL:
					{
						Key = ( HIWORD( p_LParam ) & KF_EXTENDED ) ? Keyboard::Key_Control_R : Keyboard::Key_Control_L;
					}
					break;
					// No special key was handled, check the regualar keys.
					default:
					{
						// Get the right key code from the keyboard class.
						 Key = m_pKeyboard->TranslateKeyToBitKey( static_cast< BIT_UINT16 >( LOWORD( p_WParam ) ) );
					}
					break;
				}

                if( Key != Keyboard::Key_None )
				{
                    Event.Type = Bit::Event::KeyJustReleased;
                    Event.Key = Key;
                    m_EventQueue.push_back( Event );

                    // Set the keyboard state
                    m_pKeyboard->SetCurrentKeyState( Key, BIT_FALSE );

                    // Remove the key from the input vector
                    for( BIT_MEMSIZE i = 0; i < m_PressedKeys.size( ); i++ )
                    {
                        if( Key == m_PressedKeys[ i ] )
                        {
                            m_PressedKeys.erase( m_PressedKeys.begin( ) + i );
                            break;
                        }
                    }
				}
			}
			break;
			case WM_MOUSEMOVE:
			{
				Event.Type = Bit::Event::MouseMoved;
				Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
				m_EventQueue.push_back( Event );
			}
			break;

			case WM_LBUTTONDOWN:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_1;

                // Was the button just pressed? (ButtonJustPressed event)
                if( !m_pMouse->GetPreviousButtonState( Button ) )
                {
                    Event.Type = Bit::Event::ButtonJustPressed;
                    Event.Button = Button;
                    Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                    m_EventQueue.push_back( Event );

                    // Add the event to the input vector if it's not already in the list
                    BIT_BOOL NotInList = BIT_TRUE;
                    for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                    {
                        if( Button == m_PressedButtons[ i ] )
                        {
                            NotInList = BIT_FALSE;
                            break;
                        }
                    }
                    if( NotInList )
                    {
                        m_PressedButtons.push_back( Button );
                    }
				}

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_TRUE );
			}
			break;
			case WM_MBUTTONDOWN:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_2;

                // Was the button just pressed? (ButtonJustPressed event)
                if( !m_pMouse->GetPreviousButtonState( Button ) )
                {
                    Event.Type = Bit::Event::ButtonJustPressed;
                    Event.Button = Button;
                    Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                    m_EventQueue.push_back( Event );

                    // Add the event to the input vector if it's not already in the list
                    BIT_BOOL NotInList = BIT_TRUE;
                    for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                    {
                        if( Button == m_PressedButtons[ i ] )
                        {
                            NotInList = BIT_FALSE;
                            break;
                        }
                    }
                    if( NotInList )
                    {
                        m_PressedButtons.push_back( Button );
                    }
				}

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_TRUE );
			}
			break;
			case WM_RBUTTONDOWN:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_3;

                // Was the button just pressed? (ButtonJustPressed event)
                if( !m_pMouse->GetPreviousButtonState( Button ) )
                {
                    Event.Type = Bit::Event::ButtonJustPressed;
                    Event.Button = Button;
                    Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                    m_EventQueue.push_back( Event );

                    // Add the event to the input vector if it's not already in the list
                    BIT_BOOL NotInList = BIT_TRUE;
                    for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                    {
                        if( Button == m_PressedButtons[ i ] )
                        {
                            NotInList = BIT_FALSE;
                            break;
                        }
                    }
                    if( NotInList )
                    {
                        m_PressedButtons.push_back( Button );
                    }
				}

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_TRUE );
			}
			break;

			case WM_LBUTTONUP:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_1;
                Event.Type = Bit::Event::ButtonJustReleased;
                Event.Button = Button;
                Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                m_EventQueue.push_back( Event );

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_FALSE );

                // Remove the key from the input vector
                for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                {
                    if( Button == m_PressedButtons[ i ] )
                    {
                        m_PressedButtons.erase( m_PressedButtons.begin( ) + i );
                        break;
                    }
                }
			}
			break;
			case WM_MBUTTONUP:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_2;
                Event.Type = Bit::Event::ButtonJustReleased;
                Event.Button = Button;
                Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                m_EventQueue.push_back( Event );

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_FALSE );

                // Remove the key from the input vector
                for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                {
                    if( Button == m_PressedButtons[ i ] )
                    {
                        m_PressedButtons.erase( m_PressedButtons.begin( ) + i );
                        break;
                    }
                }
			}
			break;
			case WM_RBUTTONUP:
			{
				const Mouse::eButton Button = Bit::Mouse::Button_3;
                Event.Type = Bit::Event::ButtonJustReleased;
                Event.Button = Button;
                Event.MousePosition = Bit::Vector2_si32( LOWORD( p_LParam ), HIWORD( p_LParam ) );
                m_EventQueue.push_back( Event );

                // Set the mouse state
                m_pMouse->SetCurrentButtonState( Button, BIT_FALSE );

                // Remove the key from the input vector
                for( BIT_MEMSIZE i = 0; i < m_PressedButtons.size( ); i++ )
                {
                    if( Button == m_PressedButtons[ i ] )
                    {
                        m_PressedButtons.erase( m_PressedButtons.begin( ) + i );
                        break;
                    }
                }
			}
			break;

			// No matching cases?
			default:
			{
				// Call the input callback function
				//return m_Input.Callback( p_HWND, p_Message, p_WParam, p_LParam );
			}
			break;
		}

		return DefWindowProc( p_HWND, p_Message, p_WParam, p_LParam );
	}

	std::wstring WindowWin32::StringToWideString( const std::string & p_String )
	{
		int len;
		int slength = (int)p_String.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, p_String.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, p_String.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete [] buf;
		return r;
	}

}

#endif
*/