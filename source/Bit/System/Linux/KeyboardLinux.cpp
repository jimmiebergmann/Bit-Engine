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

#include <Bit/System/Linux/KeyboardLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	KeyboardLinux::KeyboardLinux( ) :
        m_pDisplay( NULL )
	{
	    // Connect to X server
        m_pDisplay = XOpenDisplay( NULL );

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
/*
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

        */
	}

	KeyboardLinux::~KeyboardLinux( )
	{
	}

	// Public general functions
	void KeyboardLinux::Update( )
	{
		for( SizeType i = 0; i < m_ChangedKeys.size( ); i++ )
		{
			m_PreviousKeyState[ m_ChangedKeys[ i ] ] = m_CurrentKeyState[ m_ChangedKeys[ i ]  ];
		}
		m_ChangedKeys.clear( );
	}

	// Key translation function for platform keys
	KeyboardLinux::eKey KeyboardLinux::TranslateKeyToBitKey( const Uint16 p_Key )
	{
		return m_KeyTranslationsWin32ToBit[ p_Key ];
	}
	Uint16 KeyboardLinux::TranslateKeyToSystemKey( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return 0;
		}

		return m_KeyTranslationsBitToWin32[ static_cast< SizeType >( p_Key ) ];
	}

	// Get state functions
	Bool KeyboardLinux::KeyIsDown( const eKey p_Key )
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

	Bool KeyboardLinux::KeyIsUp( const eKey p_Key )
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

	Bool KeyboardLinux::KeyIsJustPressed( const eKey p_Key )
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

	Bool KeyboardLinux::KeyIsJustReleased( const eKey p_Key )
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
	Bool KeyboardLinux::GetCurrentKeyState( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		return m_CurrentKeyState[ p_Key ];
	}
	Bool KeyboardLinux::GetPreviousKeyState( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return false;
		}

		return m_PreviousKeyState[ p_Key ];
	}

	void KeyboardLinux::SetCurrentKeyState( const eKey p_key, const Bool p_State )
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

    void KeyboardLinux::SetPreviousKeyState( const eKey p_key, const Bool p_State )
	{
		if( p_key == Unknown )
		{
			return;
		}

		m_PreviousKeyState[ p_key ] = p_State;
	}

    Bool KeyboardLinux::GetKeyStatus( const eKey p_key )
	{
		/*Uint16 a = TranslateKeyToSystemKey( p_key );
		return static_cast<Uint16>( GetAsyncKeyState( TranslateKeyToSystemKey( p_key )) & 0x8000 ) != 0;*/


		// Make sure that the connection to X server is established.
        if( m_pDisplay == NULL )
        {
            return false;
        }

        // Transalte the eKey to a KeySym
        KeySym keyTranslation = static_cast<KeySym>( TranslateKeyToSystemKey( p_key ) );

        // Get the case key code
        KeyCode code = XKeysymToKeycode( m_pDisplay, keyTranslation );

        // Get the status for all keys
        // Can we optimize this by calling this function once for each loop?
        char keymap[ 32 ];
        XQueryKeymap( m_pDisplay, keymap );

        // Get the current state of the key we are checking
        Bool state = ( keymap[ code / 8 ] & ( 1 << ( code % 8 ) ) ) != 0;
        return state;
	}

}

#endif





/*



#include <Bit/System/Linux/KeyboardLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
    KeyboardLinux::KeyboardLinux( ) :
        m_pDisplay( BIT_NULL )
    {

        // Connect to X server
        m_pDisplay = XOpenDisplay( NULL );

        // Clear the bit key translations
        // NOT REQUIRED WHEN ALL THE KEY CODES ARE SET
        for( BIT_MEMSIZE i = 0; i < Keyboard::Key_Count; i++ )
        {
            m_KeyTranslationsBit[ i ] = 0;
        }

        // Clear the system key translations
        for( BIT_MEMSIZE i = 0; i < 65536; i++ )
        {
            m_KeyTranslationsSystem[ i ] = Key_None;
        }

        // Clear the key states
        for( BIT_MEMSIZE i = 0; i < Keyboard::Key_Count; i++ )
        {
            m_CurrentKeyState[ i ] = BIT_FALSE;
            m_PreviousKeyState[ i ] = BIT_FALSE;
        }

        // Reserve the changed keys vector
        m_ChangedKeys.clear( );
        m_ChangedKeys.resize( s_ReservedKeyCount );

        // ///////////////////////////////////////////////////////
        // Set our BIT key translations
        m_KeyTranslationsBit[ Key_A ] = XK_a;
        m_KeyTranslationsBit[ Key_B ] = XK_b;
        m_KeyTranslationsBit[ Key_C ] = XK_c;
        m_KeyTranslationsBit[ Key_D ] = XK_d;
        m_KeyTranslationsBit[ Key_E ] = XK_e;
        m_KeyTranslationsBit[ Key_F ] = XK_f;
        m_KeyTranslationsBit[ Key_G ] = XK_g;
        m_KeyTranslationsBit[ Key_H ] = XK_h;
        m_KeyTranslationsBit[ Key_I ] = XK_i;
        m_KeyTranslationsBit[ Key_J ] = XK_j;
        m_KeyTranslationsBit[ Key_K ] = XK_k;
        m_KeyTranslationsBit[ Key_L ] = XK_l;
        m_KeyTranslationsBit[ Key_M ] = XK_m;
        m_KeyTranslationsBit[ Key_N ] = XK_n;
        m_KeyTranslationsBit[ Key_O ] = XK_o;
        m_KeyTranslationsBit[ Key_P ] = XK_p;
        m_KeyTranslationsBit[ Key_Q ] = XK_q;
        m_KeyTranslationsBit[ Key_R ] = XK_r;
        m_KeyTranslationsBit[ Key_S ] = XK_s;
        m_KeyTranslationsBit[ Key_T ] = XK_t;
        m_KeyTranslationsBit[ Key_U ] = XK_u;
        m_KeyTranslationsBit[ Key_V ] = XK_v;
        m_KeyTranslationsBit[ Key_W ] = XK_w;
        m_KeyTranslationsBit[ Key_X ] = XK_x;
        m_KeyTranslationsBit[ Key_Y ] = XK_y;
        m_KeyTranslationsBit[ Key_Z ] = XK_z;

        m_KeyTranslationsBit[ Key_0 ] = XK_0;
        m_KeyTranslationsBit[ Key_1 ] = XK_1;
        m_KeyTranslationsBit[ Key_2 ] = XK_2;
        m_KeyTranslationsBit[ Key_3 ] = XK_3;
        m_KeyTranslationsBit[ Key_4 ] = XK_4;
        m_KeyTranslationsBit[ Key_5 ] = XK_5;
        m_KeyTranslationsBit[ Key_6 ] = XK_6;
        m_KeyTranslationsBit[ Key_7 ] = XK_7;
        m_KeyTranslationsBit[ Key_8 ] = XK_8;
        m_KeyTranslationsBit[ Key_9 ] = XK_9;

        m_KeyTranslationsBit[ Key_F1 ] = XK_F1;
        m_KeyTranslationsBit[ Key_F2 ] = XK_F2;
        m_KeyTranslationsBit[ Key_F3 ] = XK_F3;
        m_KeyTranslationsBit[ Key_F4 ] = XK_F4;
        m_KeyTranslationsBit[ Key_F5 ] = XK_F5;
        m_KeyTranslationsBit[ Key_F6 ] = XK_F6;
        m_KeyTranslationsBit[ Key_F7 ] = XK_F7;
        m_KeyTranslationsBit[ Key_F8 ] = XK_F8;
        m_KeyTranslationsBit[ Key_F9 ] = XK_F9;
        m_KeyTranslationsBit[ Key_F10 ] = XK_F10;
        m_KeyTranslationsBit[ Key_F11 ] = XK_F11;
        m_KeyTranslationsBit[ Key_F12 ] = XK_F12;

        m_KeyTranslationsBit[ Key_Down ] = XK_Down;
        m_KeyTranslationsBit[ Key_Left ] = XK_Left;
        m_KeyTranslationsBit[ Key_Right ] = XK_Right;
        m_KeyTranslationsBit[ Key_Up ] = XK_Up;

        m_KeyTranslationsBit[ Key_Alt_L ]             = XK_Alt_L;
        m_KeyTranslationsBit[ Key_Alt_R ]             = XK_Alt_R;
        m_KeyTranslationsBit[ Key_Backspace ]         = XK_BackSpace;
        m_KeyTranslationsBit[ Key_Brace_Left ]        = XK_braceleft;
        m_KeyTranslationsBit[ Key_Brace_Right ]       = XK_braceright;
        m_KeyTranslationsBit[ Key_Bracket_L ]         = XK_bracketleft;
        m_KeyTranslationsBit[ Key_Bracket_R ]         = XK_bracketright;
        m_KeyTranslationsBit[ Key_Break ]             = XK_Break;
        m_KeyTranslationsBit[ Key_Capslock ]          = XK_Caps_Lock;
        m_KeyTranslationsBit[ Key_Colon ]             = XK_colon;
        m_KeyTranslationsBit[ Key_Control_L ]         = XK_Control_L;
        m_KeyTranslationsBit[ Key_Control_R ]         = XK_Control_R;
        m_KeyTranslationsBit[ Key_Comma ]             = XK_comma;
        m_KeyTranslationsBit[ Key_Delete ]            = XK_Delete;
        m_KeyTranslationsBit[ Key_End ]               = XK_End;
        m_KeyTranslationsBit[ Key_Escape ]            = XK_Escape;
        m_KeyTranslationsBit[ Key_Greater ]           = XK_greater;
        m_KeyTranslationsBit[ Key_Home ]              = XK_Home;
        m_KeyTranslationsBit[ Key_Insert ]            = XK_Insert;
        m_KeyTranslationsBit[ Key_Less ]              = XK_less;
        m_KeyTranslationsBit[ Key_Minus ]             = XK_minus;
        m_KeyTranslationsBit[ Key_NumLock ]           = XK_Num_Lock;
        m_KeyTranslationsBit[ Key_PageDown ]          = XK_Page_Down;
        m_KeyTranslationsBit[ Key_PageUp ]            = XK_Page_Up;
        m_KeyTranslationsBit[ Key_Period ]            = XK_period;
        m_KeyTranslationsBit[ Key_Plus ]              = XK_plus;
        m_KeyTranslationsBit[ Key_Print ]             = XK_Print;
        m_KeyTranslationsBit[ Key_Return ]            = XK_Return;
        m_KeyTranslationsBit[ Key_ScrollLock ]        = XK_Scroll_Lock;
        m_KeyTranslationsBit[ Key_SemiColon ]         = XK_semicolon;
        m_KeyTranslationsBit[ Key_Shift_L ]           = XK_Shift_L;
        m_KeyTranslationsBit[ Key_Shift_R ]           = XK_Shift_R;
        m_KeyTranslationsBit[ Key_Space ]             = XK_space;
        m_KeyTranslationsBit[ key_Super_L ]           = XK_Super_L;
        m_KeyTranslationsBit[ key_Super_R ]           = XK_Super_R;
        m_KeyTranslationsBit[ Key_Tab ]               = XK_Tab;
        m_KeyTranslationsBit[ Key_Underscore ]        = XK_underscore;


        // ///////////////////////////////////////////////////////
        // Set our SYSTEM key translations
        m_KeyTranslationsSystem[ XK_a ] = Key_A;
        m_KeyTranslationsSystem[ XK_b ] = Key_B;
        m_KeyTranslationsSystem[ XK_c ] = Key_C;
        m_KeyTranslationsSystem[ XK_d ] = Key_D;
        m_KeyTranslationsSystem[ XK_e ] = Key_E;
        m_KeyTranslationsSystem[ XK_f ] = Key_F;
        m_KeyTranslationsSystem[ XK_g ] = Key_G;
        m_KeyTranslationsSystem[ XK_h ] = Key_H;
        m_KeyTranslationsSystem[ XK_i ] = Key_I;
        m_KeyTranslationsSystem[ XK_j ] = Key_J;
        m_KeyTranslationsSystem[ XK_k ] = Key_K;
        m_KeyTranslationsSystem[ XK_l ] = Key_L;
        m_KeyTranslationsSystem[ XK_m ] = Key_M;
        m_KeyTranslationsSystem[ XK_n ] = Key_N;
        m_KeyTranslationsSystem[ XK_o ] = Key_O;
        m_KeyTranslationsSystem[ XK_p ] = Key_P;
        m_KeyTranslationsSystem[ XK_q ] = Key_Q;
        m_KeyTranslationsSystem[ XK_r ] = Key_R;
        m_KeyTranslationsSystem[ XK_s ] = Key_S;
        m_KeyTranslationsSystem[ XK_t ] = Key_T;
        m_KeyTranslationsSystem[ XK_u ] = Key_U;
        m_KeyTranslationsSystem[ XK_v ] = Key_V;
        m_KeyTranslationsSystem[ XK_w ] = Key_W;
        m_KeyTranslationsSystem[ XK_x ] = Key_X;
        m_KeyTranslationsSystem[ XK_y ] = Key_Y;
        m_KeyTranslationsSystem[ XK_z ] = Key_Z;

        m_KeyTranslationsSystem[ XK_0  ] = Key_0;
        m_KeyTranslationsSystem[ XK_1  ] = Key_1;
        m_KeyTranslationsSystem[ XK_2  ] = Key_2;
        m_KeyTranslationsSystem[ XK_3  ] = Key_3;
        m_KeyTranslationsSystem[ XK_4  ] = Key_4;
        m_KeyTranslationsSystem[ XK_5  ] = Key_5;
        m_KeyTranslationsSystem[ XK_6  ] = Key_6;
        m_KeyTranslationsSystem[ XK_7  ] = Key_7;
        m_KeyTranslationsSystem[ XK_8  ] = Key_8;
        m_KeyTranslationsSystem[ XK_9  ] = Key_9;

        m_KeyTranslationsSystem[ XK_F1 ] = Key_F1;
        m_KeyTranslationsSystem[ XK_F2 ] = Key_F2;
        m_KeyTranslationsSystem[ XK_F3 ] = Key_F3;
        m_KeyTranslationsSystem[ XK_F4 ] = Key_F4;
        m_KeyTranslationsSystem[ XK_F5 ] = Key_F5;
        m_KeyTranslationsSystem[ XK_F6 ] = Key_F6;
        m_KeyTranslationsSystem[ XK_F7 ] = Key_F7;
        m_KeyTranslationsSystem[ XK_F8 ] = Key_F8;
        m_KeyTranslationsSystem[ XK_F9 ] = Key_F9;
        m_KeyTranslationsSystem[ XK_F10 ] = Key_F10;
        m_KeyTranslationsSystem[ XK_F11 ] = Key_F11;
        m_KeyTranslationsSystem[ XK_F12 ] = Key_F12;

        m_KeyTranslationsSystem[ XK_Down ] = Key_Down;
        m_KeyTranslationsSystem[ XK_Left ] = Key_Left;
        m_KeyTranslationsSystem[ XK_Right ] = Key_Right;
        m_KeyTranslationsSystem[ XK_Up ] = Key_Up;

        m_KeyTranslationsSystem[ XK_Alt_L ]             = Key_Alt_L;
        m_KeyTranslationsSystem[ XK_Alt_R ]             = Key_Alt_R;
        m_KeyTranslationsSystem[ XK_BackSpace ]         = Key_Backspace;
        m_KeyTranslationsSystem[ XK_braceleft ]         = Key_Brace_Left;
        m_KeyTranslationsSystem[ XK_braceright ]        = Key_Brace_Right;
        m_KeyTranslationsSystem[ XK_bracketleft ]       = Key_Bracket_L;
        m_KeyTranslationsSystem[ XK_bracketright ]      = Key_Bracket_R;
        m_KeyTranslationsSystem[ XK_Break ]             = Key_Break;
        m_KeyTranslationsSystem[ XK_Caps_Lock ]         = Key_Capslock;
        m_KeyTranslationsSystem[ XK_colon ]             = Key_Colon;
        m_KeyTranslationsSystem[ XK_Control_L ]         = Key_Control_L;
        m_KeyTranslationsSystem[ XK_Control_R ]         = Key_Control_R;
        m_KeyTranslationsSystem[ XK_comma ]             = Key_Comma;
        m_KeyTranslationsSystem[ XK_Delete ]            = Key_Delete;
        m_KeyTranslationsSystem[ XK_End ]               = Key_End;
        m_KeyTranslationsSystem[ XK_Escape ]            = Key_Escape;
        m_KeyTranslationsSystem[ XK_greater ]           = Key_Greater;
        m_KeyTranslationsSystem[ XK_Home ]              = Key_Home;
        m_KeyTranslationsSystem[ XK_Insert ]            = Key_Insert;
        m_KeyTranslationsSystem[ XK_less ]              = Key_Less;
        m_KeyTranslationsSystem[ XK_minus ]             = Key_Minus;
        m_KeyTranslationsSystem[ XK_Num_Lock ]          = Key_NumLock;
        m_KeyTranslationsSystem[ XK_Page_Down ]         = Key_PageDown;
        m_KeyTranslationsSystem[ XK_Page_Up ]           = Key_PageUp;
        m_KeyTranslationsSystem[ XK_period ]            = Key_Period;
        m_KeyTranslationsSystem[ XK_plus ]              = Key_Plus;
        m_KeyTranslationsSystem[ XK_Print ]             = Key_Print;
        m_KeyTranslationsSystem[ XK_Return ]            = Key_Return;
        m_KeyTranslationsSystem[ XK_Scroll_Lock ]       = Key_ScrollLock;
        m_KeyTranslationsSystem[ XK_semicolon ]         = Key_SemiColon;
        m_KeyTranslationsSystem[ XK_Shift_L ]           = Key_Shift_L;
        m_KeyTranslationsSystem[ XK_Shift_R ]           = Key_Shift_R;
        m_KeyTranslationsSystem[ XK_space ]             = Key_Space;
        m_KeyTranslationsSystem[ XK_Super_L ]           = key_Super_L;
        m_KeyTranslationsSystem[ XK_Super_R ]           = key_Super_R;
        m_KeyTranslationsSystem[ XK_Tab ]               = Key_Tab;
        m_KeyTranslationsSystem[ XK_underscore ]        = Key_Underscore;

    }

    KeyboardLinux::~KeyboardLinux( )
    {
        // Close the connection with X server
        if( m_pDisplay )
        {
            XCloseDisplay( m_pDisplay );
        }
    }

    // Public general functions
    void KeyboardLinux::Update( )
    {
        if( m_pDisplay == BIT_NULL )
        {
            return;
        }

        for( BIT_MEMSIZE i = 0; i < m_ChangedKeys.size( ); i++ )
        {
           m_PreviousKeyState[ m_ChangedKeys[ i ] ] = m_CurrentKeyState[ m_ChangedKeys[ i ]  ];
        }
        m_ChangedKeys.clear( );
    }

    // Key translation function for platform keys
    Keyboard::eKey KeyboardLinux::TranslateKeyToBitKey( const BIT_UINT16 p_Key )
    {
        return m_KeyTranslationsSystem[ p_Key ];
    }

    BIT_UINT16 KeyboardLinux::TranslateKeyToSystemKey( const eKey p_Key )
    {
        return m_KeyTranslationsBit[ static_cast< BIT_UINT16 >( p_Key ) ];
    }

    // Get state functions
    BIT_BOOL KeyboardLinux::KeyIsDown( const eKey p_Key )
    {
        const BIT_BOOL KeyState = GetKeyStatus( p_Key );

        if( m_CurrentKeyState[ p_Key ] != KeyState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedKeys.size( ) < s_ReservedKeyCount )
            {
                m_ChangedKeys.push_back( p_Key );
            }
        }

        return ( m_CurrentKeyState[ p_Key ] = KeyState );
    }

    BIT_BOOL KeyboardLinux::KeyIsUp( const eKey p_Key )
    {
        const BIT_BOOL KeyState = GetKeyStatus( p_Key );

        if( m_CurrentKeyState[ p_Key ] != KeyState )
        {
           // Push back the reserved key if we have space for the changed key
            if( m_ChangedKeys.size( ) < s_ReservedKeyCount )
            {
                m_ChangedKeys.push_back( p_Key );
            }
        }

        return !( m_CurrentKeyState[ p_Key ] = KeyState );
    }

    BIT_BOOL KeyboardLinux::KeyIsJustPressed( const eKey p_Key )
    {
        const BIT_BOOL KeyState = GetKeyStatus( p_Key );

        if( m_CurrentKeyState[ p_Key ] != KeyState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedKeys.size( ) < s_ReservedKeyCount )
            {
                m_ChangedKeys.push_back( p_Key );
            }
        }

        return ( m_CurrentKeyState[ p_Key ] = KeyState )  && !m_PreviousKeyState[ p_Key ];
    }

    BIT_BOOL KeyboardLinux::KeyIsJustReleased( const eKey p_Key )
    {
        const BIT_BOOL KeyState = GetKeyStatus( p_Key );

        if( m_CurrentKeyState[ p_Key ] != KeyState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedKeys.size( ) < s_ReservedKeyCount )
            {
                m_ChangedKeys.push_back( p_Key );
            }
        }

        return !( m_CurrentKeyState[ p_Key ] = KeyState )  && m_PreviousKeyState[ p_Key ];
    }

    BIT_BOOL KeyboardLinux::GetCurrentKeyState( const eKey p_Key )
    {
        return m_CurrentKeyState[ p_Key ];
    }

    BIT_BOOL KeyboardLinux::GetPreviousKeyState( const eKey p_Key )
    {
         return m_PreviousKeyState[ p_Key ];
    }

    // Set state functions
    void KeyboardLinux::SetCurrentKeyState( const eKey p_Key, const BIT_BOOL p_State )
    {
        // Push the key to the changed key vector.
        if( m_CurrentKeyState[ p_Key ] != p_State )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedKeys.size( ) < s_ReservedKeyCount )
            {
                m_ChangedKeys.push_back( p_Key );
            }
        }

        // Set the current state
        m_CurrentKeyState[ p_Key ] = p_State;
    }

    void KeyboardLinux::SetPreviousKeyState( const eKey p_Key, const BIT_BOOL p_State )
    {
        m_PreviousKeyState[ p_Key ] = p_State;
    }

    // Private functions
    BIT_BOOL KeyboardLinux::GetKeyStatus( const eKey p_Key )
    {
        // Make sure that the connection to X server is established.
        if( m_pDisplay == BIT_NULL )
        {
            return BIT_FALSE;
        }

        // Transalte the eKey to a KeySym
        KeySym KeyTranslation = m_KeyTranslationsBit[ p_Key ];

        // Get the case key code
        KeyCode Code = XKeysymToKeycode( m_pDisplay, KeyTranslation );

        // Get the status for all keys
        // Can we optimize this by calling this function once for each loop?
        char Keymap[ 32 ];
        XQueryKeymap( m_pDisplay, Keymap );

        // Get the current state of the key we are checking
        BIT_BOOL State = ( Keymap[ Code / 8 ] & ( 1 << ( Code % 8 ) ) ) != 0;
        return State;
    }

}

#endif


*/
