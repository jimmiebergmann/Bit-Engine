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
			m_KeyTranslationsBitToSystem[ i ] = 0;
		}

        // Clear the system key translations
        for( SizeType i = 0; i < 65536; i++ )
        {
            m_KeyTranslationsSystemToBit[ i ] = Unknown;
        }

		// Reserve the changed keys vector
		m_ChangedKeys.clear( );
		m_ChangedKeys.resize( KeyCount );

        // Set our BIT key translations
        m_KeyTranslationsBitToSystem[ A ] = XK_a;
        m_KeyTranslationsBitToSystem[ B ] = XK_b;
        m_KeyTranslationsBitToSystem[ C ] = XK_c;
        m_KeyTranslationsBitToSystem[ D ] = XK_d;
        m_KeyTranslationsBitToSystem[ E ] = XK_e;
        m_KeyTranslationsBitToSystem[ F ] = XK_f;
        m_KeyTranslationsBitToSystem[ G ] = XK_g;
        m_KeyTranslationsBitToSystem[ H ] = XK_h;
        m_KeyTranslationsBitToSystem[ I ] = XK_i;
        m_KeyTranslationsBitToSystem[ J ] = XK_j;
        m_KeyTranslationsBitToSystem[ K ] = XK_k;
        m_KeyTranslationsBitToSystem[ L ] = XK_l;
        m_KeyTranslationsBitToSystem[ M ] = XK_m;
        m_KeyTranslationsBitToSystem[ N ] = XK_n;
        m_KeyTranslationsBitToSystem[ O ] = XK_o;
        m_KeyTranslationsBitToSystem[ P ] = XK_p;
        m_KeyTranslationsBitToSystem[ Q ] = XK_q;
        m_KeyTranslationsBitToSystem[ R ] = XK_r;
        m_KeyTranslationsBitToSystem[ S ] = XK_s;
        m_KeyTranslationsBitToSystem[ T ] = XK_t;
        m_KeyTranslationsBitToSystem[ U ] = XK_u;
        m_KeyTranslationsBitToSystem[ V ] = XK_v;
        m_KeyTranslationsBitToSystem[ W ] = XK_w;
        m_KeyTranslationsBitToSystem[ X ] = XK_x;
        m_KeyTranslationsBitToSystem[ Y ] = XK_y;
        m_KeyTranslationsBitToSystem[ Z ] = XK_z;

        m_KeyTranslationsBitToSystem[ Num0 ] = XK_0;
        m_KeyTranslationsBitToSystem[ Num1 ] = XK_1;
        m_KeyTranslationsBitToSystem[ Num2 ] = XK_2;
        m_KeyTranslationsBitToSystem[ Num3 ] = XK_3;
        m_KeyTranslationsBitToSystem[ Num4 ] = XK_4;
        m_KeyTranslationsBitToSystem[ Num5 ] = XK_5;
        m_KeyTranslationsBitToSystem[ Num6 ] = XK_6;
        m_KeyTranslationsBitToSystem[ Num7 ] = XK_7;
        m_KeyTranslationsBitToSystem[ Num8 ] = XK_8;
        m_KeyTranslationsBitToSystem[ Num9 ] = XK_9;

        m_KeyTranslationsBitToSystem[ F1 ] = XK_F1;
        m_KeyTranslationsBitToSystem[ F2 ] = XK_F2;
        m_KeyTranslationsBitToSystem[ F3 ] = XK_F3;
        m_KeyTranslationsBitToSystem[ F4 ] = XK_F4;
        m_KeyTranslationsBitToSystem[ F5 ] = XK_F5;
        m_KeyTranslationsBitToSystem[ F6 ] = XK_F6;
        m_KeyTranslationsBitToSystem[ F7 ] = XK_F7;
        m_KeyTranslationsBitToSystem[ F8 ] = XK_F8;
        m_KeyTranslationsBitToSystem[ F9 ] = XK_F9;
        m_KeyTranslationsBitToSystem[ F10 ] = XK_F10;
        m_KeyTranslationsBitToSystem[ F11 ] = XK_F11;
        m_KeyTranslationsBitToSystem[ F12 ] = XK_F12;

        m_KeyTranslationsBitToSystem[ Down ]    = XK_Down;
        m_KeyTranslationsBitToSystem[ Left ]    = XK_Left;
        m_KeyTranslationsBitToSystem[ Right ]   = XK_Right;
        m_KeyTranslationsBitToSystem[ Up ]      = XK_Up;

        m_KeyTranslationsBitToSystem[ AltLeft ]           = XK_Alt_L;
        m_KeyTranslationsBitToSystem[ AltRight ]          = XK_Alt_R;
        m_KeyTranslationsBitToSystem[ Backspace ]         = XK_BackSpace;
        m_KeyTranslationsBitToSystem[ BraceLeft ]         = XK_braceleft;
        m_KeyTranslationsBitToSystem[ BraceRight ]        = XK_braceright;
        m_KeyTranslationsBitToSystem[ BracketLeft ]       = XK_bracketleft;
        m_KeyTranslationsBitToSystem[ BracketRight ]      = XK_bracketright;
        m_KeyTranslationsBitToSystem[ Break ]             = XK_Break;
        m_KeyTranslationsBitToSystem[ Capslock ]          = XK_Caps_Lock;
        m_KeyTranslationsBitToSystem[ Colon ]             = XK_colon;
        m_KeyTranslationsBitToSystem[ ControlLeft ]       = XK_Control_L;
        m_KeyTranslationsBitToSystem[ ControlRight ]      = XK_Control_R;
        m_KeyTranslationsBitToSystem[ Comma ]             = XK_comma;
        m_KeyTranslationsBitToSystem[ Delete ]            = XK_Delete;
        m_KeyTranslationsBitToSystem[ End ]               = XK_End;
        m_KeyTranslationsBitToSystem[ Escape ]            = XK_Escape;
        m_KeyTranslationsBitToSystem[ Greater ]           = XK_greater;
        m_KeyTranslationsBitToSystem[ Home ]              = XK_Home;
        m_KeyTranslationsBitToSystem[ Insert ]            = XK_Insert;
        m_KeyTranslationsBitToSystem[ Less ]              = XK_less;
        m_KeyTranslationsBitToSystem[ Minus ]             = XK_minus;
        m_KeyTranslationsBitToSystem[ NumLock ]           = XK_Num_Lock;
        m_KeyTranslationsBitToSystem[ PageDown ]          = XK_Page_Down;
        m_KeyTranslationsBitToSystem[ PageUp ]            = XK_Page_Up;
        m_KeyTranslationsBitToSystem[ Period ]            = XK_period;
        m_KeyTranslationsBitToSystem[ Plus ]              = XK_plus;
        m_KeyTranslationsBitToSystem[ Print ]             = XK_Print;
        m_KeyTranslationsBitToSystem[ Return ]            = XK_Return;
        m_KeyTranslationsBitToSystem[ ScrollLock ]        = XK_Scroll_Lock;
        m_KeyTranslationsBitToSystem[ SemiColon ]         = XK_semicolon;
        m_KeyTranslationsBitToSystem[ ShiftLeft ]         = XK_Shift_L;
        m_KeyTranslationsBitToSystem[ ShiftRight ]        = XK_Shift_R;
        m_KeyTranslationsBitToSystem[ Space ]             = XK_space;
        m_KeyTranslationsBitToSystem[ SuperLeft ]         = XK_Super_L;
        m_KeyTranslationsBitToSystem[ SuperRight ]        = XK_Super_R;
        m_KeyTranslationsBitToSystem[ Tab ]               = XK_Tab;
        m_KeyTranslationsBitToSystem[ Underscore ]        = XK_underscore;


		 // Set our BIT key translations
        m_KeyTranslationsSystemToBit[ XK_a ] = A;
        m_KeyTranslationsSystemToBit[ XK_b ] = B;
        m_KeyTranslationsSystemToBit[ XK_c ] = C;
        m_KeyTranslationsSystemToBit[ XK_d ] = D;
        m_KeyTranslationsSystemToBit[ XK_e ] = E;
        m_KeyTranslationsSystemToBit[ XK_f ] = F;
        m_KeyTranslationsSystemToBit[ XK_g ] = G;
        m_KeyTranslationsSystemToBit[ XK_h ] = H;
        m_KeyTranslationsSystemToBit[ XK_i ] = I;
        m_KeyTranslationsSystemToBit[ XK_j ] = J;
        m_KeyTranslationsSystemToBit[ XK_k ] = K;
        m_KeyTranslationsSystemToBit[ XK_l ] = L;
        m_KeyTranslationsSystemToBit[ XK_m ] = M;
        m_KeyTranslationsSystemToBit[ XK_n ] = N;
        m_KeyTranslationsSystemToBit[ XK_o ] = O;
        m_KeyTranslationsSystemToBit[ XK_p ] = P;
        m_KeyTranslationsSystemToBit[ XK_q ] = Q;
        m_KeyTranslationsSystemToBit[ XK_r ] = R;
        m_KeyTranslationsSystemToBit[ XK_s ] = S;
        m_KeyTranslationsSystemToBit[ XK_t ] = T;
        m_KeyTranslationsSystemToBit[ XK_u ] = U;
        m_KeyTranslationsSystemToBit[ XK_v ] = V;
        m_KeyTranslationsSystemToBit[ XK_w ] = W;
        m_KeyTranslationsSystemToBit[ XK_x ] = X;
        m_KeyTranslationsSystemToBit[ XK_y ] = Y;
        m_KeyTranslationsSystemToBit[ XK_z ] = Z;

        m_KeyTranslationsSystemToBit[ XK_0 ] = Num0;
        m_KeyTranslationsSystemToBit[ XK_1 ] = Num1;
        m_KeyTranslationsSystemToBit[ XK_2 ] = Num2;
        m_KeyTranslationsSystemToBit[ XK_3 ] = Num3;
        m_KeyTranslationsSystemToBit[ XK_4 ] = Num4;
        m_KeyTranslationsSystemToBit[ XK_5 ] = Num5;
        m_KeyTranslationsSystemToBit[ XK_6 ] = Num6;
        m_KeyTranslationsSystemToBit[ XK_7 ] = Num7;
        m_KeyTranslationsSystemToBit[ XK_8 ] = Num8;
        m_KeyTranslationsSystemToBit[ XK_9 ] = Num9;

        m_KeyTranslationsSystemToBit[ XK_F1 ] = F1;
        m_KeyTranslationsSystemToBit[ XK_F2 ] = F2;
        m_KeyTranslationsSystemToBit[ XK_F3 ] = F3;
        m_KeyTranslationsSystemToBit[ XK_F4 ] = F4;
        m_KeyTranslationsSystemToBit[ XK_F5 ] = F5;
        m_KeyTranslationsSystemToBit[ XK_F6 ] = F6;
        m_KeyTranslationsSystemToBit[ XK_F7 ] = F7;
        m_KeyTranslationsSystemToBit[ XK_F8 ] = F8;
        m_KeyTranslationsSystemToBit[ XK_F9 ] = F9;
        m_KeyTranslationsSystemToBit[ XK_F10 ] = F10;
        m_KeyTranslationsSystemToBit[ XK_F11 ] = F11;
        m_KeyTranslationsSystemToBit[ XK_F12 ] = F12;

        m_KeyTranslationsSystemToBit[ XK_Down ] = Down;
        m_KeyTranslationsSystemToBit[ XK_Left ] = Left;
        m_KeyTranslationsSystemToBit[ XK_Right ] = Right;
        m_KeyTranslationsSystemToBit[ XK_Up ] = Up;

        m_KeyTranslationsSystemToBit[ XK_Alt_L ] = AltLeft;
        m_KeyTranslationsSystemToBit[ XK_Alt_R ] = AltRight;
        m_KeyTranslationsSystemToBit[ XK_BackSpace ] = Backspace;
        m_KeyTranslationsSystemToBit[ XK_braceleft ] = BraceLeft;
        m_KeyTranslationsSystemToBit[ XK_braceright ] = BraceRight;
        m_KeyTranslationsSystemToBit[ XK_bracketleft ] = BracketLeft;
        m_KeyTranslationsSystemToBit[ XK_bracketright ] = BracketRight;
        m_KeyTranslationsSystemToBit[ XK_Break ] = Break;
        m_KeyTranslationsSystemToBit[ XK_Caps_Lock ] = Capslock;
        m_KeyTranslationsSystemToBit[ XK_colon ] = Colon;
        m_KeyTranslationsSystemToBit[ XK_Control_L ] = ControlLeft;
        m_KeyTranslationsSystemToBit[ XK_Control_R ] = ControlRight;
        m_KeyTranslationsSystemToBit[ XK_comma ] = Comma;
        m_KeyTranslationsSystemToBit[ XK_Delete ] = Delete;
        m_KeyTranslationsSystemToBit[ XK_End ] = End;
        m_KeyTranslationsSystemToBit[ XK_Escape ] = Escape;
        m_KeyTranslationsSystemToBit[ XK_greater ] = Greater;
        m_KeyTranslationsSystemToBit[ XK_Home ] = Home;
        m_KeyTranslationsSystemToBit[ XK_Insert ] = Insert;
        m_KeyTranslationsSystemToBit[ XK_less ] = Less;
        m_KeyTranslationsSystemToBit[ XK_minus ] = Minus;
        m_KeyTranslationsSystemToBit[ XK_Num_Lock ] = NumLock;
        m_KeyTranslationsSystemToBit[ XK_Page_Down ] = PageDown;
        m_KeyTranslationsSystemToBit[ XK_Page_Up ] = PageUp;
        m_KeyTranslationsSystemToBit[ XK_period ] = Period;
        m_KeyTranslationsSystemToBit[ XK_plus ] = Plus;
        m_KeyTranslationsSystemToBit[ XK_Print ] = Print;
        m_KeyTranslationsSystemToBit[ XK_Return ] = Return;
        m_KeyTranslationsSystemToBit[ XK_Scroll_Lock ] =ScrollLock;
        m_KeyTranslationsSystemToBit[ XK_semicolon ] = SemiColon;
        m_KeyTranslationsSystemToBit[ XK_Shift_L ] = ShiftLeft;
        m_KeyTranslationsSystemToBit[ XK_Shift_R ] = ShiftRight;
        m_KeyTranslationsSystemToBit[ XK_space ] = Space;
        m_KeyTranslationsSystemToBit[ XK_Super_L ] = SuperLeft;
        m_KeyTranslationsSystemToBit[ XK_Super_R ] = SuperRight;
        m_KeyTranslationsSystemToBit[ XK_Tab ] = Tab;
        m_KeyTranslationsSystemToBit[ XK_underscore ] = Underscore;
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
		return m_KeyTranslationsSystemToBit[ p_Key ];
	}
	Uint16 KeyboardLinux::TranslateKeyToSystemKey( const eKey p_Key )
	{
		if( p_Key == Unknown )
		{
			return 0;
		}

		return m_KeyTranslationsBitToSystem[ static_cast< SizeType >( p_Key ) ];
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
            m_KeyTranslationsBitToSystem[ i ] = 0;
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
        m_KeyTranslationsBitToSystem[ Key_A ] = XK_a;
        m_KeyTranslationsBitToSystem[ Key_B ] = XK_b;
        m_KeyTranslationsBitToSystem[ Key_C ] = XK_c;
        m_KeyTranslationsBitToSystem[ Key_D ] = XK_d;
        m_KeyTranslationsBitToSystem[ Key_E ] = XK_e;
        m_KeyTranslationsBitToSystem[ Key_F ] = XK_f;
        m_KeyTranslationsBitToSystem[ Key_G ] = XK_g;
        m_KeyTranslationsBitToSystem[ Key_H ] = XK_h;
        m_KeyTranslationsBitToSystem[ Key_I ] = XK_i;
        m_KeyTranslationsBitToSystem[ Key_J ] = XK_j;
        m_KeyTranslationsBitToSystem[ Key_K ] = XK_k;
        m_KeyTranslationsBitToSystem[ Key_L ] = XK_l;
        m_KeyTranslationsBitToSystem[ Key_M ] = XK_m;
        m_KeyTranslationsBitToSystem[ Key_N ] = XK_n;
        m_KeyTranslationsBitToSystem[ Key_O ] = XK_o;
        m_KeyTranslationsBitToSystem[ Key_P ] = XK_p;
        m_KeyTranslationsBitToSystem[ Key_Q ] = XK_q;
        m_KeyTranslationsBitToSystem[ Key_R ] = XK_r;
        m_KeyTranslationsBitToSystem[ Key_S ] = XK_s;
        m_KeyTranslationsBitToSystem[ Key_T ] = XK_t;
        m_KeyTranslationsBitToSystem[ Key_U ] = XK_u;
        m_KeyTranslationsBitToSystem[ Key_V ] = XK_v;
        m_KeyTranslationsBitToSystem[ Key_W ] = XK_w;
        m_KeyTranslationsBitToSystem[ Key_X ] = XK_x;
        m_KeyTranslationsBitToSystem[ Key_Y ] = XK_y;
        m_KeyTranslationsBitToSystem[ Key_Z ] = XK_z;

        m_KeyTranslationsBitToSystem[ Key_0 ] = XK_0;
        m_KeyTranslationsBitToSystem[ Key_1 ] = XK_1;
        m_KeyTranslationsBitToSystem[ Key_2 ] = XK_2;
        m_KeyTranslationsBitToSystem[ Key_3 ] = XK_3;
        m_KeyTranslationsBitToSystem[ Key_4 ] = XK_4;
        m_KeyTranslationsBitToSystem[ Key_5 ] = XK_5;
        m_KeyTranslationsBitToSystem[ Key_6 ] = XK_6;
        m_KeyTranslationsBitToSystem[ Key_7 ] = XK_7;
        m_KeyTranslationsBitToSystem[ Key_8 ] = XK_8;
        m_KeyTranslationsBitToSystem[ Key_9 ] = XK_9;

        m_KeyTranslationsBitToSystem[ Key_F1 ] = XK_F1;
        m_KeyTranslationsBitToSystem[ Key_F2 ] = XK_F2;
        m_KeyTranslationsBitToSystem[ Key_F3 ] = XK_F3;
        m_KeyTranslationsBitToSystem[ Key_F4 ] = XK_F4;
        m_KeyTranslationsBitToSystem[ Key_F5 ] = XK_F5;
        m_KeyTranslationsBitToSystem[ Key_F6 ] = XK_F6;
        m_KeyTranslationsBitToSystem[ Key_F7 ] = XK_F7;
        m_KeyTranslationsBitToSystem[ Key_F8 ] = XK_F8;
        m_KeyTranslationsBitToSystem[ Key_F9 ] = XK_F9;
        m_KeyTranslationsBitToSystem[ Key_F10 ] = XK_F10;
        m_KeyTranslationsBitToSystem[ Key_F11 ] = XK_F11;
        m_KeyTranslationsBitToSystem[ Key_F12 ] = XK_F12;

        m_KeyTranslationsBitToSystem[ Key_Down ] = XK_Down;
        m_KeyTranslationsBitToSystem[ Key_Left ] = XK_Left;
        m_KeyTranslationsBitToSystem[ Key_Right ] = XK_Right;
        m_KeyTranslationsBitToSystem[ Key_Up ] = XK_Up;

        m_KeyTranslationsBitToSystem[ Key_Alt_L ]             = XK_Alt_L;
        m_KeyTranslationsBitToSystem[ Key_Alt_R ]             = XK_Alt_R;
        m_KeyTranslationsBitToSystem[ Key_Backspace ]         = XK_BackSpace;
        m_KeyTranslationsBitToSystem[ Key_Brace_Left ]        = XK_braceleft;
        m_KeyTranslationsBitToSystem[ Key_Brace_Right ]       = XK_braceright;
        m_KeyTranslationsBitToSystem[ Key_Bracket_L ]         = XK_bracketleft;
        m_KeyTranslationsBitToSystem[ Key_Bracket_R ]         = XK_bracketright;
        m_KeyTranslationsBitToSystem[ Key_Break ]             = XK_Break;
        m_KeyTranslationsBitToSystem[ Key_Capslock ]          = XK_Caps_Lock;
        m_KeyTranslationsBitToSystem[ Key_Colon ]             = XK_colon;
        m_KeyTranslationsBitToSystem[ Key_Control_L ]         = XK_Control_L;
        m_KeyTranslationsBitToSystem[ Key_Control_R ]         = XK_Control_R;
        m_KeyTranslationsBitToSystem[ Key_Comma ]             = XK_comma;
        m_KeyTranslationsBitToSystem[ Key_Delete ]            = XK_Delete;
        m_KeyTranslationsBitToSystem[ Key_End ]               = XK_End;
        m_KeyTranslationsBitToSystem[ Key_Escape ]            = XK_Escape;
        m_KeyTranslationsBitToSystem[ Key_Greater ]           = XK_greater;
        m_KeyTranslationsBitToSystem[ Key_Home ]              = XK_Home;
        m_KeyTranslationsBitToSystem[ Key_Insert ]            = XK_Insert;
        m_KeyTranslationsBitToSystem[ Key_Less ]              = XK_less;
        m_KeyTranslationsBitToSystem[ Key_Minus ]             = XK_minus;
        m_KeyTranslationsBitToSystem[ Key_NumLock ]           = XK_Num_Lock;
        m_KeyTranslationsBitToSystem[ Key_PageDown ]          = XK_Page_Down;
        m_KeyTranslationsBitToSystem[ Key_PageUp ]            = XK_Page_Up;
        m_KeyTranslationsBitToSystem[ Key_Period ]            = XK_period;
        m_KeyTranslationsBitToSystem[ Key_Plus ]              = XK_plus;
        m_KeyTranslationsBitToSystem[ Key_Print ]             = XK_Print;
        m_KeyTranslationsBitToSystem[ Key_Return ]            = XK_Return;
        m_KeyTranslationsBitToSystem[ Key_ScrollLock ]        = XK_Scroll_Lock;
        m_KeyTranslationsBitToSystem[ Key_SemiColon ]         = XK_semicolon;
        m_KeyTranslationsBitToSystem[ Key_Shift_L ]           = XK_Shift_L;
        m_KeyTranslationsBitToSystem[ Key_Shift_R ]           = XK_Shift_R;
        m_KeyTranslationsBitToSystem[ Key_Space ]             = XK_space;
        m_KeyTranslationsBitToSystem[ key_Super_L ]           = XK_Super_L;
        m_KeyTranslationsBitToSystem[ key_Super_R ]           = XK_Super_R;
        m_KeyTranslationsBitToSystem[ Key_Tab ]               = XK_Tab;
        m_KeyTranslationsBitToSystem[ Key_Underscore ]        = XK_underscore;


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
        return m_KeyTranslationsBitToSystem[ static_cast< BIT_UINT16 >( p_Key ) ];
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
        KeySym KeyTranslation = m_KeyTranslationsBitToSystem[ p_Key ];

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
