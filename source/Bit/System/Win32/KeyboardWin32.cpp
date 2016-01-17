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

#include <Bit/System/Win32/KeyboardWin32.hpp>

#ifdef BIT_PLATFORM_WINDOWS


#include <windows.h>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	KeyboardWin32::KeyboardWin32( )
	{
		// Clear the key states
		for( SizeType i = 0; i < KeyCount; i++ )
		{
			m_CurrentKeyState[ i ] = false;
			m_PreviousKeyState[ i ] = false;

			// Initialize the translation array for bit to win32 keys
			m_KeyTranslationsBitToWin32[ i ] = 0;
		}

        // Clear the system key translations
        for( SizeType i = 0; i < 65536; i++ )
        {
            m_KeyTranslationsWin32ToBit[ i ] = Unknown;
        }

		// Reserve the changed keys vector
		m_ChangedKeys.clear( );
		m_ChangedKeys.resize( KeyCount );

		 // Set our BIT key translations
        m_KeyTranslationsBitToWin32[ A ] = 'A';
        m_KeyTranslationsBitToWin32[ B ] = 'B';
        m_KeyTranslationsBitToWin32[ C ] = 'C';
        m_KeyTranslationsBitToWin32[ D ] = 'D';
        m_KeyTranslationsBitToWin32[ E ] = 'E';
        m_KeyTranslationsBitToWin32[ F ] = 'F';
        m_KeyTranslationsBitToWin32[ G ] = 'G';
        m_KeyTranslationsBitToWin32[ H ] = 'H';
        m_KeyTranslationsBitToWin32[ I ] = 'I';
        m_KeyTranslationsBitToWin32[ J ] = 'J';
        m_KeyTranslationsBitToWin32[ K ] = 'K';
        m_KeyTranslationsBitToWin32[ L ] = 'L';
        m_KeyTranslationsBitToWin32[ M ] = 'M';
        m_KeyTranslationsBitToWin32[ N ] = 'N';
        m_KeyTranslationsBitToWin32[ O ] = 'O';
        m_KeyTranslationsBitToWin32[ P ] = 'P';
        m_KeyTranslationsBitToWin32[ Q ] = 'Q';
        m_KeyTranslationsBitToWin32[ R ] = 'R';
        m_KeyTranslationsBitToWin32[ S ] = 'S';
        m_KeyTranslationsBitToWin32[ T ] = 'T';
        m_KeyTranslationsBitToWin32[ U ] = 'U';
        m_KeyTranslationsBitToWin32[ V ] = 'V';
        m_KeyTranslationsBitToWin32[ W ] = 'W';
        m_KeyTranslationsBitToWin32[ X ] = 'X';
        m_KeyTranslationsBitToWin32[ Y ] = 'Y';
        m_KeyTranslationsBitToWin32[ Z ] = 'Z';

        m_KeyTranslationsBitToWin32[ Num0 ] = '0';
        m_KeyTranslationsBitToWin32[ Num1 ] = '1';
        m_KeyTranslationsBitToWin32[ Num2 ] = '2';
        m_KeyTranslationsBitToWin32[ Num3 ] = '3';
        m_KeyTranslationsBitToWin32[ Num4 ] = '4';
        m_KeyTranslationsBitToWin32[ Num5 ] = '5';
        m_KeyTranslationsBitToWin32[ Num6 ] = '6';
        m_KeyTranslationsBitToWin32[ Num7 ] = '7';
        m_KeyTranslationsBitToWin32[ Num8 ] = '8';
        m_KeyTranslationsBitToWin32[ Num9 ] = '9';

        m_KeyTranslationsBitToWin32[ F1 ]	= VK_F1;
        m_KeyTranslationsBitToWin32[ F2 ]	= VK_F2;
        m_KeyTranslationsBitToWin32[ F3 ]	= VK_F3;
        m_KeyTranslationsBitToWin32[ F4 ]	= VK_F4;
        m_KeyTranslationsBitToWin32[ F5 ]	= VK_F5;
        m_KeyTranslationsBitToWin32[ F6 ]	= VK_F6;
        m_KeyTranslationsBitToWin32[ F7 ]	= VK_F7;
        m_KeyTranslationsBitToWin32[ F8 ]	= VK_F8;
        m_KeyTranslationsBitToWin32[ F9 ]	= VK_F9;
        m_KeyTranslationsBitToWin32[ F10 ]	= VK_F10;
        m_KeyTranslationsBitToWin32[ F11 ]	= VK_F11;
        m_KeyTranslationsBitToWin32[ F12 ]	= VK_F12;

        m_KeyTranslationsBitToWin32[ Down ]		= VK_DOWN;
        m_KeyTranslationsBitToWin32[ Left ]		= VK_LEFT;
        m_KeyTranslationsBitToWin32[ Right ]	= VK_RIGHT;
        m_KeyTranslationsBitToWin32[ Up ]		= VK_UP;

		m_KeyTranslationsBitToWin32[ AltLeft ]			= VK_LMENU;
		m_KeyTranslationsBitToWin32[ AltRight ]			= VK_RMENU;
		///m_KeyTranslationsBitToWin32[ BackSlash ]		= VK_OEM_5;
		m_KeyTranslationsBitToWin32[ Backspace ]		= VK_BACK;
		//m_KeyTranslationsBitToWin32[ BraceLeft ]		= XK_braceleft;
		//m_KeyTranslationsBitToWin32[ BraceRight ]		= XK_braceright;
		m_KeyTranslationsBitToWin32[ BracketLeft ]		= VK_OEM_4;
		m_KeyTranslationsBitToWin32[ BracketRight ]		= VK_OEM_6;
		m_KeyTranslationsBitToWin32[ Break ]			= VK_PAUSE;
		m_KeyTranslationsBitToWin32[ Capslock ]			= VK_CAPITAL;
		//m_KeyTranslationsBitToWin32[ Key_Colon ]		= XK_colon;
		m_KeyTranslationsBitToWin32[ ControlLeft ]		= VK_LCONTROL;
		m_KeyTranslationsBitToWin32[ ControlRight ]		= VK_RCONTROL;
		m_KeyTranslationsBitToWin32[ Comma ]			= VK_OEM_COMMA;
		m_KeyTranslationsBitToWin32[ Delete ]			= VK_DELETE;
		m_KeyTranslationsBitToWin32[ End ]				= VK_END;
		///m_KeyTranslationsBitToWin32[ Equal ]			= VK_OEM_PLUS;
		m_KeyTranslationsBitToWin32[ Escape ]			= VK_ESCAPE;
		//m_KeyTranslationsBitToWin32[ Greater ]		= XK_greater;
		m_KeyTranslationsBitToWin32[ Home ]				= VK_HOME;
		m_KeyTranslationsBitToWin32[ Insert ]			= VK_INSERT;
		//m_KeyTranslationsBitToWin32[ Key_Less ]		= XK_less;
		m_KeyTranslationsBitToWin32[ Minus ]			= VK_OEM_MINUS;
		m_KeyTranslationsBitToWin32[ NumLock ]			= VK_NUMLOCK;
		m_KeyTranslationsBitToWin32[ PageDown ]			= VK_NEXT;
		m_KeyTranslationsBitToWin32[ PageUp ]			= VK_PRIOR;
		m_KeyTranslationsBitToWin32[ Period ]			= VK_OEM_PERIOD;
		m_KeyTranslationsBitToWin32[ Plus ]				= VK_ADD;
		m_KeyTranslationsBitToWin32[ Print ]			= VK_SNAPSHOT;
		///m_KeyTranslationsBitToWin32[ Quote ]			= VK_OEM_7;
		m_KeyTranslationsBitToWin32[ Return ]			= VK_RETURN;
		m_KeyTranslationsBitToWin32[ ScrollLock ]		= VK_SCROLL;
		m_KeyTranslationsBitToWin32[ SemiColon ]		= VK_OEM_1;
		m_KeyTranslationsBitToWin32[ ShiftLeft ]		= VK_LSHIFT;
		m_KeyTranslationsBitToWin32[ ShiftRight ]		= VK_RSHIFT;
		///m_KeyTranslationsBitToWin32[ Key_Slash ]		= VK_OEM_2;
		m_KeyTranslationsBitToWin32[ Space ]			= VK_SPACE;
		m_KeyTranslationsBitToWin32[ SuperLeft ]		= VK_LWIN;
		m_KeyTranslationsBitToWin32[ SuperRight ]		= VK_RWIN ;
		m_KeyTranslationsBitToWin32[ Tab ]				= VK_TAB;
		///m_KeyTranslationsBitToWin32[ Tilde ]			= VK_OEM_3;
		//m_KeyTranslationsBitToWin32[ Underscore ]		= XK_underscore;



		 // Set our SYSTEM key translations
        m_KeyTranslationsWin32ToBit[ 'A' ] = A;
        m_KeyTranslationsWin32ToBit[ 'B' ] = B;
        m_KeyTranslationsWin32ToBit[ 'C' ] = C;
        m_KeyTranslationsWin32ToBit[ 'D' ] = D;
        m_KeyTranslationsWin32ToBit[ 'E' ] = E;
        m_KeyTranslationsWin32ToBit[ 'F' ] = F;
        m_KeyTranslationsWin32ToBit[ 'G' ] = G;
        m_KeyTranslationsWin32ToBit[ 'H' ] = H;
        m_KeyTranslationsWin32ToBit[ 'I' ] = I;
        m_KeyTranslationsWin32ToBit[ 'J' ] = J;
        m_KeyTranslationsWin32ToBit[ 'K' ] = K;
        m_KeyTranslationsWin32ToBit[ 'L' ] = L;
        m_KeyTranslationsWin32ToBit[ 'M' ] = M;
        m_KeyTranslationsWin32ToBit[ 'N' ] = N;
        m_KeyTranslationsWin32ToBit[ 'O' ] = O;
        m_KeyTranslationsWin32ToBit[ 'P' ] = P;
        m_KeyTranslationsWin32ToBit[ 'Q' ] = Q;
        m_KeyTranslationsWin32ToBit[ 'R' ] = R;
        m_KeyTranslationsWin32ToBit[ 'S' ] = S;
        m_KeyTranslationsWin32ToBit[ 'T' ] = T;
        m_KeyTranslationsWin32ToBit[ 'U' ] = U;
        m_KeyTranslationsWin32ToBit[ 'V' ] = V;
        m_KeyTranslationsWin32ToBit[ 'W' ] = W;
        m_KeyTranslationsWin32ToBit[ 'X' ] = X;
        m_KeyTranslationsWin32ToBit[ 'Y' ] = Y;
        m_KeyTranslationsWin32ToBit[ 'Z' ] = Z;

        m_KeyTranslationsWin32ToBit[ '0' ] = Num0;
        m_KeyTranslationsWin32ToBit[ '1' ] = Num1;
        m_KeyTranslationsWin32ToBit[ '2' ] = Num2;
        m_KeyTranslationsWin32ToBit[ '3' ] = Num3;
        m_KeyTranslationsWin32ToBit[ '4' ] = Num4;
        m_KeyTranslationsWin32ToBit[ '5' ] = Num5;
        m_KeyTranslationsWin32ToBit[ '6' ] = Num6;
        m_KeyTranslationsWin32ToBit[ '7' ] = Num7;
        m_KeyTranslationsWin32ToBit[ '8' ] = Num8;
        m_KeyTranslationsWin32ToBit[ '9' ] = Num9;

        m_KeyTranslationsWin32ToBit[ VK_F1 ]	= F1;
        m_KeyTranslationsWin32ToBit[ VK_F2 ]	= F2;
        m_KeyTranslationsWin32ToBit[ VK_F3 ]	= F3;
        m_KeyTranslationsWin32ToBit[ VK_F4 ]	= F4;
        m_KeyTranslationsWin32ToBit[ VK_F5 ]	= F5;
        m_KeyTranslationsWin32ToBit[ VK_F6 ]	= F6;
        m_KeyTranslationsWin32ToBit[ VK_F7 ]	= F7;
        m_KeyTranslationsWin32ToBit[ VK_F8 ]	= F8;
        m_KeyTranslationsWin32ToBit[ VK_F9 ]	= F9;
        m_KeyTranslationsWin32ToBit[ VK_F10 ]	= F10;
        m_KeyTranslationsWin32ToBit[ VK_F11 ]	= F11;
        m_KeyTranslationsWin32ToBit[ VK_F12 ]	= F12;

        m_KeyTranslationsWin32ToBit[ VK_DOWN ]	= Down;
        m_KeyTranslationsWin32ToBit[ VK_LEFT ]	= Left;
        m_KeyTranslationsWin32ToBit[ VK_RIGHT ] = Right;
        m_KeyTranslationsWin32ToBit[ VK_UP ]	= Up;


		m_KeyTranslationsWin32ToBit[ VK_LMENU ]             = AltLeft;
        m_KeyTranslationsWin32ToBit[ VK_RMENU ]             = AltRight;
        ///m_KeyTranslationsWin32ToBit[ Key_BackSlash ]     = VK_OEM_5;
        m_KeyTranslationsWin32ToBit[ VK_BACK ]              = Backspace ;
        //m_KeyTranslationsWin32ToBit[ Key_Brace_Left ]     = XK_braceleft;
        //m_KeyTranslationsWin32ToBit[ Key_Brace_Right ]    = XK_braceright;
        m_KeyTranslationsWin32ToBit[ VK_OEM_4 ]             = BracketLeft;
        m_KeyTranslationsWin32ToBit[ VK_OEM_6 ]             = BracketRight;
        m_KeyTranslationsWin32ToBit[ VK_PAUSE ]             = Break;
        m_KeyTranslationsWin32ToBit[ VK_CAPITAL ]           = Capslock;
        //m_KeyTranslationsWin32ToBit[ Key_Colon ]          = XK_colon;
        m_KeyTranslationsWin32ToBit[ VK_LCONTROL ]          = ControlLeft;
        m_KeyTranslationsWin32ToBit[ VK_RCONTROL ]          = ControlRight;
        m_KeyTranslationsWin32ToBit[ VK_OEM_COMMA ]         = Comma;
        m_KeyTranslationsWin32ToBit[ VK_DELETE ]            = Delete;
        m_KeyTranslationsWin32ToBit[ VK_END ]               = End;
        ///m_KeyTranslationsWin32ToBit[ Key_Equal ]         = VK_OEM_PLUS;
        m_KeyTranslationsWin32ToBit[ VK_ESCAPE ]            = Escape;
        //m_KeyTranslationsWin32ToBit[ Key_Greater ]        = XK_greater;
        m_KeyTranslationsWin32ToBit[ VK_HOME ]              = Home;
        m_KeyTranslationsWin32ToBit[ VK_INSERT ]            = Insert;
        //m_KeyTranslationsWin32ToBit[ Key_Less ]           = XK_less;
        m_KeyTranslationsWin32ToBit[ VK_OEM_MINUS ]         = Minus;
        m_KeyTranslationsWin32ToBit[ VK_NUMLOCK ]           = NumLock;
        m_KeyTranslationsWin32ToBit[ VK_NEXT ]              = PageDown;
        m_KeyTranslationsWin32ToBit[ VK_PRIOR ]				= PageUp;
        m_KeyTranslationsWin32ToBit[ VK_OEM_PERIOD ]        = Period;
        m_KeyTranslationsWin32ToBit[ VK_ADD ]				= Plus;
        m_KeyTranslationsWin32ToBit[ VK_SNAPSHOT ]          = Print;
        ///m_KeyTranslationsWin32ToBit[ Key_Quote ]         = VK_OEM_7;
        m_KeyTranslationsWin32ToBit[ VK_RETURN ]            = Return;
        m_KeyTranslationsWin32ToBit[ VK_SCROLL ]			= ScrollLock;
        m_KeyTranslationsWin32ToBit[ VK_OEM_1 ]				= SemiColon;
        m_KeyTranslationsWin32ToBit[ VK_LSHIFT ]			= ShiftLeft;
        m_KeyTranslationsWin32ToBit[ VK_RSHIFT ]			= ShiftRight;
        ///m_KeyTranslationsWin32ToBit[ Key_Slash ]         = VK_OEM_2;
        m_KeyTranslationsWin32ToBit[ VK_SPACE ]             = Space;
        m_KeyTranslationsWin32ToBit[ VK_LWIN ]				= SuperLeft;
        m_KeyTranslationsWin32ToBit[ VK_RWIN ]				= SuperRight;
        m_KeyTranslationsWin32ToBit[ VK_TAB ]               = Tab;
        ///m_KeyTranslationsWin32ToBit[ Key_Tilde ]         = VK_OEM_3;
        //m_KeyTranslationsWin32ToBit[ Key_Underscore ]     = XK_underscore;
	}

	KeyboardWin32::~KeyboardWin32( )
	{
	}

	// Public general functions
	void KeyboardWin32::Update( )
	{
		for( SizeType i = 0; i < m_ChangedKeys.size( ); i++ )
		{
			m_PreviousKeyState[ m_ChangedKeys[ i ] ] = m_CurrentKeyState[ m_ChangedKeys[ i ]  ];
		}
		m_ChangedKeys.clear( );
	}

	// Key translation function for platform keys
	KeyboardWin32::eKey KeyboardWin32::TranslateKeyToBitKey( const Uint16 p_Key )
	{
		return m_KeyTranslationsWin32ToBit[ p_Key ];
	}
	Uint16 KeyboardWin32::TranslateKeyToSystemKey( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return 0;
		}

		return m_KeyTranslationsBitToWin32[ static_cast< SizeType >( p_Key ) ];
	}

	// Get state functions
	Bool KeyboardWin32::KeyIsDown( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		const Bool buttonState = GetKeyStatus( p_Key );

		if( m_CurrentKeyState[ p_Key ] != buttonState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedKeys.size( ) < KeyCount )
			{
				m_ChangedKeys.push_back( p_Key );
			}
		}

		return ( m_CurrentKeyState[ p_Key ] = buttonState );
	}

	Bool KeyboardWin32::KeyIsUp( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		const Bool keyState = GetKeyStatus( p_Key );

		if( m_CurrentKeyState[ p_Key ] != keyState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedKeys.size( ) < KeyCount )
			{
				m_ChangedKeys.push_back( p_Key );
			}
		}

		return !( m_CurrentKeyState[ p_Key ] = keyState );
	}

	Bool KeyboardWin32::KeyIsJustPressed( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		const Bool keyState = GetKeyStatus( p_Key );

		if( m_CurrentKeyState[ p_Key ] != keyState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedKeys.size( ) < KeyCount )
			{
				m_ChangedKeys.push_back( p_Key );
			}
		}

		return ( m_CurrentKeyState[ p_Key ] = keyState )  && !m_PreviousKeyState[ p_Key ];
	}

	Bool KeyboardWin32::KeyIsJustReleased( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		const Bool keyState = GetKeyStatus( p_Key );

		if( m_CurrentKeyState[ p_Key ] != keyState )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedKeys.size( ) < KeyCount )
			{
				m_ChangedKeys.push_back( p_Key );
			}
		}

		return !( m_CurrentKeyState[ p_Key ] = keyState )  && m_PreviousKeyState[ p_Key ];
	}

	// Private functions
	Bool KeyboardWin32::GetCurrentKeyState( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		return m_CurrentKeyState[ p_Key ];
	}
	Bool KeyboardWin32::GetPreviousKeyState( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		return m_PreviousKeyState[ p_Key ];
	}

	void KeyboardWin32::SetCurrentKeyState( const eKey p_key, const Bool p_State )
	{
		if( p_key == Unknown )
		{
			return;
		}

		// Push the key to the changed key vector.
		if( m_CurrentKeyState[ p_key ] != p_State )
		{
			// Push back the reserved key if we have space for the changed key
			if( m_ChangedKeys.size( ) < KeyCount )
			{
				m_ChangedKeys.push_back( p_key );
			}
		}

		// Set the current state
		m_CurrentKeyState[ p_key ] = p_State;
	}

    void KeyboardWin32::SetPreviousKeyState( const eKey p_key, const Bool p_State )
	{
		if( p_key == Unknown )
		{
			return;
		}

		m_PreviousKeyState[ p_key ] = p_State;
	}
	
    Bool KeyboardWin32::GetKeyStatus( const eKey p_key )
	{
		Uint16 a = TranslateKeyToSystemKey( p_key );
		return static_cast<Uint16>( GetAsyncKeyState( TranslateKeyToSystemKey( p_key )) & 0x8000 ) != 0;
	}

}

#endif
