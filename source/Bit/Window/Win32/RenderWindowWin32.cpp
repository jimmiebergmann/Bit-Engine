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

#include <Bit/Window/Win32/RenderWindowWin32.hpp>
#include <sstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

#ifdef BIT_PLATFORM_WINDOWS

static std::wstring StringToWideString( const std::string & p_String )
{
	int len;
	int slength = static_cast<int>( p_String.length() + 1 );
	len = MultiByteToWideChar(CP_ACP, 0, p_String.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, p_String.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete [] buf;
	return r;
}

namespace Bit
{

	RenderWindowWin32::RenderWindowWin32( ) :
		m_Open( false ),
		m_VideoMode( ),
		m_Title( "" ),
		m_Style( static_cast<Uint32>( Style::None ) ),
		m_DeviceContextHandle( NULL ),
		m_WindowHandle( NULL ),
		m_RegisteredWindowClass( false ),
		m_Events( ),
		m_Resizing( false ),
		m_Moving( false ),
		m_Focused( false )
	{
	}

	RenderWindowWin32::RenderWindowWin32( const VideoMode & p_VideoMode, const std::string & p_Title, const Uint32 p_Style ) :
		m_Open( false ),
		m_VideoMode( ),
		m_Title( p_Title ),
		m_Style( static_cast<Uint32>( Style::None ) ),
		m_DeviceContextHandle( NULL ),
		m_WindowHandle( NULL ),
		m_RegisteredWindowClass( false ),
		m_Events( ),
		m_Resizing( false ),
		m_Moving( false ),
		m_Focused( false )
	{
		Open( p_VideoMode, p_Title, p_Style );
	}

	RenderWindowWin32::~RenderWindowWin32( )
	{
		Close( );
	}

	Bool RenderWindowWin32::Open( const VideoMode & p_VideoMode, const std::string & p_Title, const Uint32 p_Style )
	{
		// Convert the title itnro a wide string
		std::wstring convertedTitle = StringToWideString( p_Title ).c_str( );

		// Create an unique window class name
		static Uint32 uniqueNumber = 0;
		std::stringstream ss;
		ss << "BitEngineWindow" << uniqueNumber;
		uniqueNumber++;
		std::wstring className = StringToWideString( ss.str( ).c_str( ) ).c_str( );

		// Create a window class(WNDCLASS - win32)
		WNDCLASS winClass;
		HINSTANCE winInstance = GetModuleHandle( NULL ); // Grab any old handle
		winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		winClass.lpfnWndProc = ( WNDPROC )RenderWindowWin32::WindowProcStatic;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = winInstance;
		winClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
		winClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		winClass.hbrBackground = NULL;
		winClass.lpszClassName = className.c_str( );
		winClass.lpszMenuName = NULL;

		// Register the window class
		if( !RegisterClass( &winClass ) )
		{
			return false;
		}
		// The class is now registered.
		m_WidnowClassName = className;
		m_RegisteredWindowClass = true;

		// Let's set the windows style
		DWORD	exStyle		= 0;
		DWORD	style		= 0;
		RECT	windowRect;
		windowRect.left		= static_cast<LONG>( 0 );
		windowRect.right	= static_cast<LONG>( p_VideoMode.GetSize( ).x );
		windowRect.top		= static_cast<LONG>( 0 );
		windowRect.bottom	= static_cast<LONG>( p_VideoMode.GetSize( ).y );

		// Set the window decoration style
		if( p_Style == Bit::Style::Default )
		{
			exStyle = WS_EX_APPWINDOW;
			style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER;
		}
		else
		{
			if( p_Style & Bit::Style::Minimize )
			{
				style |=  WS_MINIMIZEBOX;
			}

			if( p_Style & Bit::Style::Resize )
			{
				style |= WS_MAXIMIZEBOX | WS_SIZEBOX;
			}

			if( p_Style & Bit::Style::TitleBar )
			{
				style |= WS_CAPTION | WS_SYSMENU | WS_BORDER;
			}
		}

		// Apply yhe style
		AdjustWindowRectEx( &windowRect, style, FALSE, exStyle );

		// Let's create the window.
		m_WindowHandle = CreateWindowEx(
			exStyle,
			className.c_str( ),
			convertedTitle.c_str( ),

			WS_CLIPSIBLINGS |
			WS_CLIPCHILDREN |
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,

			windowRect.right	- windowRect.left,
			windowRect.bottom	- windowRect.top,
			NULL,
			NULL,
			winInstance,
			this );

		// Did the window creation succeed?
		if( m_WindowHandle == NULL )
		{
			std::cout << "[RenderWindowWin32::Create] Failed to create window.\n";
			return false;
		}

		// Did we disable the [X](close) button?
		// We have to apply this property after the window creation.
		// So here it comes.
		if( !( p_Style & Bit::Style::Close ) )
		{
			UINT dwExtra = MF_DISABLED | MF_GRAYED;
			HMENU hMenu = GetSystemMenu( m_WindowHandle, false );
			EnableMenuItem( hMenu, SC_CLOSE, MF_BYCOMMAND | dwExtra );
		}

		// Get the device context
		m_DeviceContextHandle = GetDC( m_WindowHandle );

		// Now, show the window and focus it
		ShowWindow( m_WindowHandle, SW_SHOW );
		SetForegroundWindow( m_WindowHandle );
		SetFocus( m_WindowHandle );

		// Set the rest of the member variables
		m_VideoMode = p_VideoMode;
		m_Title = p_Title;
		m_Style = p_Style;
		m_Open = true;
		m_Focused = true;
		return true;
	}

	void RenderWindowWin32::Close( )
	{
		// Get the module handler.
		HINSTANCE Hinstance = GetModuleHandle( NULL );

		// Change back to normal display settings.
		ChangeDisplaySettings( NULL, 0 );

		// Release the device context
		if( m_DeviceContextHandle && !ReleaseDC( m_WindowHandle, m_DeviceContextHandle ))
		{
			std::cout << "[RenderWindowWin32::Destroy] Releasing device context failed.\n";
		}

		// Destroy the window
		if( m_WindowHandle && !DestroyWindow( m_WindowHandle ) )
		{
			std::cout << "[RenderWindowWin32::Destroy] Destroying window failed.\n";
		}

		// Unregister the window class
		if ( m_RegisteredWindowClass && !UnregisterClass( m_WidnowClassName.c_str( ), Hinstance ) )
		{
			std::cout << "[RenderWindowWin32::Destroy] Unregistering class failed.\n";
		}

		// Reset the attributes
		m_Open =  false;
		m_Focused = false;
		m_VideoMode = VideoMode( );
		m_Title =  "";
		m_Style = static_cast<Uint32>( Style::None );
		m_DeviceContextHandle = NULL;
		m_WindowHandle = NULL;
		m_RegisteredWindowClass =  false;

		// Clear all the events
		ClearEvents( );
	}

	void RenderWindowWin32::Update( )
	{
		// Clear all the events
		ClearEvents( );
		m_Mouse.Update( );
		m_Keyboard.Update( );

		// Reset the resizing / moving flags
		m_Resizing = false;
		m_Moving = false;

		// Go through all the window event messages
		MSG message;
		while( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ))
		{
			// A modal function is being called when you press the alt key,
			// fix this by ignoring the alt(menu) key event.
			if( message.message == WM_SYSCOMMAND &&
				message.wParam == SC_KEYMENU )
			{
				break;
			}

			// Translate the dispatch the message
			// This will call the WindowProcStatic function
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}

	Bool RenderWindowWin32::PollEvent( Event & p_Event )
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

	void RenderWindowWin32::SetTitle( const std::string & p_Title )
	{
		// Convert the title into the right format
		std::wstring convertedTitle = StringToWideString( p_Title ).c_str();
		//LPCWSTR convertedTitle = StringToWideString( p_Title ).c_str( );

		// Change the title
		Bool status = ( SetWindowText( m_WindowHandle, convertedTitle.c_str( ) ) ? true : false );

		if( status == true )
		{
			m_Title = p_Title;
		}
	}

	Bool RenderWindowWin32::IsOpen( ) const
	{
		return m_Open;
	}

	Bool RenderWindowWin32::IsFocused( ) const
	{
		return m_Focused;
	}

	const VideoMode & RenderWindowWin32::GetVideoMode( ) const
	{
		return m_VideoMode;
	}

	const std::string & RenderWindowWin32::GetTitle( ) const
	{
		return m_Title;
	}

	HDC RenderWindowWin32::GetDeviceContextHandle( ) const
	{
		return m_DeviceContextHandle;
	}

	// Private functions
	void RenderWindowWin32::ClearEvents( )
	{
		while( m_Events.empty( ) == false )
		{
			m_Events.pop( );
		}
	}

	LRESULT RenderWindowWin32::WindowProcStatic(	HWND p_HWND, UINT p_Message,
													WPARAM p_WParam, LPARAM p_LParam )
	{
		if ( p_Message == WM_NCCREATE)
		{
			SetWindowLongPtr( p_HWND, GWL_USERDATA, (LONG_PTR)( (LPCREATESTRUCT)p_LParam)->lpCreateParams );
		}
		else
		{
			RenderWindowWin32 * pWnd = ( RenderWindowWin32* )GetWindowLongPtr( p_HWND, GWL_USERDATA );

			if (pWnd != NULL)
			{
				return pWnd->WindowProc( p_HWND, p_Message, p_WParam, p_LParam );
			}
		}

		return DefWindowProc( p_HWND, p_Message, p_WParam, p_LParam );
	}

	LRESULT RenderWindowWin32::WindowProc(	HWND p_HWND, UINT p_Message,
											WPARAM p_WParam, LPARAM p_LParam )
	{
		Event e;

		switch(p_Message)
		{
			case WM_CLOSE:
			{
				// Push close event
				e.Type = Event::Closed;
				m_Events.push( e );

				// Do not forward the event.
				return 0;
			}
			break;
			case WM_SETFOCUS:
			{
				// Set the focus flag
				m_Focused = true;

				// Push gained focus event
				e.Type = Event::GainedFocus;
				m_Events.push( e );
				
			}
			break;
			case WM_KILLFOCUS:
			{
				// Set the focus flag
				m_Focused = false;

				// Push gained lost event
				e.Type = Event::LostFocus;
				m_Events.push( e );
			}
			break;
			/*case WM_ENTERSIZEMOVE:
			{
			}
			break;*/
			case WM_EXITSIZEMOVE:
			{
				// Get the widnow rect
				RECT rect;
				if( GetWindowRect( m_WindowHandle, &rect ) )
				{
					// Are we resizing the window?
					if( m_Resizing )
					{
						// Get the size of the window
						Vector2i32 size(	static_cast<Int32>( rect.right - rect.left ),
											static_cast<Int32>( rect.bottom - rect.top ));

						// Push resize event
						e.Type = Event::Resized;
						e.Size = size;
						m_Events.push( e );

					}

					// Are we moving the window?
					if( m_Moving )
					{
						// Get the position of the window
						Vector2i32 position(	static_cast<Int32>( rect.left ),
												static_cast<Int32>( rect.top ));

						// Push move event
						e.Type = Event::Moved;
						e.Position = position;
						m_Events.push( e );
					}
				}
			}
			break;
			case WM_SIZE:
			{
				// Is this the first time we resize the window in a while?
				if( m_Resizing == false )
				{
					Vector2i32 size(	static_cast<Int32>( LOWORD( p_LParam ) ),
										static_cast<Int32>( HIWORD( p_LParam ) ));

					// Is the window really resizing?
					// The WM_SIZE event is sent when you move the window as well.
					if( size.x > 0 || size.y > 0 )
					{
						m_Resizing = true;
					}
				}
			}
			break;
			case WM_MOVE:
			{
				// Is this the first time we move the window in a while?
				if( m_Moving == false )
				{
					m_Moving = true;
				}
			}
			break;
			case WM_KEYDOWN:
			{

				// Get the key as a bit key
				Keyboard::eKey key = m_Keyboard.TranslateKeyToBitKey( static_cast<Uint16>( LOWORD( p_WParam ) ) );

				// Add key just pressed event
				Bool status = m_Keyboard.GetCurrentKeyState( key );
				if( status == false )
				{
					Event e2;
					e2.Type = Event::KeyJustPressed;
					e2.Key = key;
					m_Events.push( e2 );
				}

				// Add mouse key event
				e.Type = Event::KeyPressed;
				e.Key = key;
				m_Events.push( e );
			}
			break;
			case WM_KEYUP:
			{
				// Get the key as a bit key
				Keyboard::eKey key = m_Keyboard.TranslateKeyToBitKey( static_cast<Uint16>( LOWORD( p_WParam ) ) );

				e.Type = Event::KeyJustReleased;
				e.Key = key;
				m_Events.push( e );
			}
			break;
			case WM_MOUSEMOVE:
			{
				// Push mouse move event
				e.Type = Event::MouseMoved;
				m_Events.push( e );
			}
			break;
			case WM_LBUTTONDOWN:
			{ 
				// Add mouse just pressed event
				Bool status = false;//m_Mouse.GetCurrentButtonState( Mouse::Left );
				if( status == false )
				{
					Event e2;
					e2.Type = Event::MouseJustPressed;
					e2.Button = Mouse::Left;
					m_Events.push( e2 );
				}

				// Add mouse pressed event
				e.Type = Event::MousePressed;
				e.Button = Mouse::Left;
				m_Events.push( e );
			}
			break;
			case WM_MBUTTONDOWN:
			{
				// Add mouse just pressed event
				Bool status = false;//m_Mouse.GetCurrentButtonState( Mouse::Middle );
				if( status == false )
				{
					Event e2;
					e2.Type = Event::MouseJustPressed;
					e2.Button = Mouse::Middle;
					m_Events.push( e2 );
				}

				// Add mouse pressed event
				e.Type = Event::MousePressed;
				e.Button = Mouse::Middle;
				m_Events.push( e );
			}
			break;
			case WM_RBUTTONDOWN:
			{
				// Add mouse just pressed event
				Bool status = false;//m_Mouse.GetCurrentButtonState( Mouse::Right );
				if( status == false )
				{
					Event e2;
					e2.Type = Event::MouseJustPressed;
					e2.Button = Mouse::Right;
					m_Events.push( e2 );
				}

				// Add mouse pressed event
				e.Type = Event::MousePressed;
				e.Button = Mouse::Right;
				m_Events.push( e );
			}
			break;
			case WM_LBUTTONUP:
			{
				e.Type = Event::MouseJustReleased;
				e.Button = Mouse::Left;
				m_Events.push( e );
			}
			break;
			case WM_MBUTTONUP:
			{
				e.Type = Event::MouseJustReleased;
				e.Button = Mouse::Middle;
				m_Events.push( e );
			}
			break;
			case WM_RBUTTONUP:
			{
				e.Type = Event::MouseJustReleased;
				e.Button = Mouse::Right;
				m_Events.push( e );
			}
			break;
			default:
			{
			}
			break;
		}

		return DefWindowProc( p_HWND, p_Message, p_WParam, p_LParam );
	}

}

#endif