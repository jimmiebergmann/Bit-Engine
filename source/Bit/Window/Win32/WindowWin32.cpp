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

#include <Bit/Window/Win32/WindowWin32.hpp>
#include <Bit/System/Debugger.hpp>

namespace Bit
{

	// Constructors/destructors
	WindowWin32::WindowWin32( ) :
		m_DeviceContext( BIT_NULL ),
		m_Window( BIT_NULL ),
		m_RegisteredClass( BIT_FALSE )
	{
	}

	// Public functions
	BIT_UINT32 WindowWin32::Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height, const BIT_UINT32 p_Bits,
		const std::string p_Title )
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
		WindowRect.right = ( long )p_Width;
		WindowRect.top = ( long )0;
		WindowRect.bottom = ( long )p_Height;

		ExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

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

		// Get the device context
		m_DeviceContext = GetDC( m_Window );

		// Now, show the window
		ShowWindow( m_Window, SW_SHOW );
		SetForegroundWindow( m_Window );
		SetFocus( m_Window );

		// Finally set the base class attributes
		m_Width = p_Width;
		m_Height = p_Height;
		m_Bits = p_Bits;
		m_Title = p_Title;

		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::Destroy( )
	{
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
		m_DeviceContext = NULL;
		m_Window = NULL;
		m_RegisteredClass = BIT_FALSE;

		return BIT_OK;
	}

	BIT_UINT32 WindowWin32::DoEvents( )
	{
		m_EventQueue.clear( );

		MSG Message;
		while( PeekMessage( &Message, NULL, NULL, NULL, PM_REMOVE ))
		{
			TranslateMessage( &Message );
			DispatchMessage( &Message );

		}

		return BIT_OK;
	}

	// Set functions
	BIT_UINT32 WindowWin32::SetTitle( std::string p_Title )
	{
		// Convert the title into the right format
		std::wstring STemp = StringToWideString( p_Title ); // Temporary buffer is required
		LPCWSTR ConvertedTitle = STemp.c_str();

		// Change the title
		return ( SetWindowText( m_Window, ConvertedTitle ) ? BIT_OK : BIT_ERROR );
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
		switch(p_Message)
		{
			case(WM_CLOSE):
				{
					//m_EventQueue.push( Event( Window::Close ) );
				}
				break;
			case(WM_SETFOCUS):
				{
					//m_EventQueue.push( Event( Window::GainedFocus ) );
				}
				break;
			case(WM_KILLFOCUS):
				{
					//m_EventQueue.push( Event( Window::LostFocus ) );
				}
				break;
			case(WM_SIZE):
				{
					//m_EventQueue.push( Event( Window::Resize ) );
				}
				break;
			case(WM_MOVE):
				{
					//m_EventQueue.push( Event( Window::Move ) );
				}
				break;

			// No matching cases?
			default:
			{
				// Call the input callback function
				//return m_Input.Callback( p_HWND, p_Message, p_WParam, p_LParam );
			}
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