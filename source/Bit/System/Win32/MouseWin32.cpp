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

#include <Bit/System/Win32/MouseWin32.hpp>

#ifdef BIT_PLATFORM_WINDOWS
#include <Windows.h>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
/*
    // Global static varialbes
    const static BIT_UINT32 s_ButtonMask = 0x0700;
*/

    // Constructor/destructor
    MouseWin32::MouseWin32( )
    {
		// Clear the button states
		for( SizeType i = 0; i < s_ButtonCount; i++ )
		{
			m_CurrentButtonState[ i ] = false;
			m_PreviousButtonState[ i ] = false;
		}

		// Reserve the changed keys vector
		m_ChangedButtons.clear( );
		m_ChangedButtons.resize( s_ButtonCount );


		// Set the BIT button translations
		m_ButtonTranslationsBitToWin32[ Left ] = VK_LBUTTON;
		m_ButtonTranslationsBitToWin32[ Middle ] = VK_MBUTTON;
		m_ButtonTranslationsBitToWin32[ Right ] = VK_RBUTTON;
		m_ButtonTranslationsBitToWin32[ 3 ] = 0;

		//  Set the SYSTEM button translations
		m_ButtonTranslationsWin32ToBit[ 0 ] = None;
		m_ButtonTranslationsWin32ToBit[ VK_LBUTTON ] = Left;
		m_ButtonTranslationsWin32ToBit[ VK_RBUTTON ] = Right;
		m_ButtonTranslationsWin32ToBit[ 3 ] = None;
		m_ButtonTranslationsWin32ToBit[ VK_MBUTTON ] = Middle;
		
    }

    MouseWin32::~MouseWin32( )
    {
    }

     // Public general functions
    void MouseWin32::Update( )
    {
		for( SizeType i = 0; i < m_ChangedButtons.size( ); i++ )
		{
			m_PreviousButtonState[ m_ChangedButtons[ i ] ] = m_CurrentButtonState[ m_ChangedButtons[ i ]  ];
		}
		m_ChangedButtons.clear( );
    }

	// Button translation function for platform keys
	MouseWin32::eButton MouseWin32::TranslateButtonToBitKey( const Uint16 p_Button )
	{
		return m_ButtonTranslationsWin32ToBit[ p_Button ];
	}

	Uint16 MouseWin32::TranslateButtonToSystemKey( const eButton p_Button )
	{
		return m_ButtonTranslationsBitToWin32[ static_cast< SizeType >( p_Button ) ];
	}

	// Get state functions
	Vector2i32 MouseWin32::GetPosition( ) const
	{
		POINT position;
		if( GetCursorPos( &position ) )
		{
			return Vector2i32(	static_cast<Int32>( position.x ),
								static_cast<Int32>( position.y ) );
		}

		return Vector2i32( 0, 0 );
	}

	Bool MouseWin32::ButtonIsDown( const eButton p_Button )
	{
		const Bool ButtonState = GetButtonStatus( p_Button );

		if( m_CurrentButtonState[ p_Button ] != ButtonState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedButtons.size( ) < s_ButtonCount )
			{
				m_ChangedButtons.push_back( p_Button );
			}
		}

		return ( m_CurrentButtonState[ p_Button ] = ButtonState );
	}

	Bool MouseWin32::ButtonIsUp( const eButton p_Button )
	{
		const Bool ButtonState = GetButtonStatus( p_Button );

		if( m_CurrentButtonState[ p_Button ] != ButtonState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedButtons.size( ) < s_ButtonCount )
			{
				m_ChangedButtons.push_back( p_Button );
			}
		}

		return !( m_CurrentButtonState[ p_Button ] = ButtonState );
	}

	Bool MouseWin32::ButtonIsJustPressed( const eButton p_Button )
	{
		const Bool ButtonState = GetButtonStatus( p_Button );

		if( m_CurrentButtonState[ p_Button ] != ButtonState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedButtons.size( ) < s_ButtonCount )
			{
				m_ChangedButtons.push_back( p_Button );
			}
		}

		return ( m_CurrentButtonState[ p_Button ] = ButtonState )  && !m_PreviousButtonState[ p_Button ];
	}

	Bool MouseWin32::ButtonIsJustReleased( const eButton p_Button )
	{
		const Bool ButtonState = GetButtonStatus( p_Button );

		if( m_CurrentButtonState[ p_Button ] != ButtonState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedButtons.size( ) < s_ButtonCount )
			{
				m_ChangedButtons.push_back( p_Button );
			}
		}

		return !( m_CurrentButtonState[ p_Button ] = ButtonState )  && m_PreviousButtonState[ p_Button ];
	}

	Bool MouseWin32::GetCurrentButtonState( const eButton p_Button )
	{
		return m_CurrentButtonState[ p_Button ];
	}

	Bool MouseWin32::GetPreviousButtonState( const eButton p_Button )
	{
		return m_PreviousButtonState[ p_Button ];
	}

	// Set state functions
	void MouseWin32::SetCurrentButtonState( const eButton p_Button, const Bool p_State )
	{
		// Push the key to the changed key vector.
		if( m_CurrentButtonState[ p_Button ] != p_State )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedButtons.size( ) < s_ButtonCount )
			{
				m_ChangedButtons.push_back( p_Button );
			}
		}

		// Set the current state
		m_CurrentButtonState[ p_Button ] = p_State;
	}

	void MouseWin32::SetPreviousButtonState( const eButton p_Button, const Bool p_State )
	{
		m_PreviousButtonState[ p_Button ] = p_State;
	}

	// Private functions
	Bool MouseWin32::GetButtonStatus( const eButton p_Button )
	{
		return static_cast<Uint16>( GetAsyncKeyState( TranslateButtonToSystemKey( p_Button ) ) ) > 0;
	}

}

#endif
