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

#ifdef BIT_PLATFORM_WIN32

#include <windows.h>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
    KeyboardWin32::KeyboardWin32( )/* :
        m_pDisplay( BIT_NULL )*/
    {
/*
        // Connect to X server
        m_pDisplay = XOpenDisplay( NULL );
*/
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
        m_KeyTranslationsBit[ Key_A ] = 'A';
        m_KeyTranslationsBit[ Key_B ] = 'B';
        m_KeyTranslationsBit[ Key_C ] = 'C';
        m_KeyTranslationsBit[ Key_D ] = 'D';
        m_KeyTranslationsBit[ Key_E ] = 'E';
        m_KeyTranslationsBit[ Key_F ] = 'F';
        m_KeyTranslationsBit[ Key_G ] = 'G';
        m_KeyTranslationsBit[ Key_H ] = 'H';
        m_KeyTranslationsBit[ Key_I ] = 'I';
        m_KeyTranslationsBit[ Key_J ] = 'J';
        m_KeyTranslationsBit[ Key_K ] = 'K';
        m_KeyTranslationsBit[ Key_L ] = 'L';
        m_KeyTranslationsBit[ Key_M ] = 'M';
        m_KeyTranslationsBit[ Key_N ] = 'N';
        m_KeyTranslationsBit[ Key_O ] = 'O';
        m_KeyTranslationsBit[ Key_P ] = 'P';
        m_KeyTranslationsBit[ Key_Q ] = 'Q';
        m_KeyTranslationsBit[ Key_R ] = 'R';
        m_KeyTranslationsBit[ Key_S ] = 'S';
        m_KeyTranslationsBit[ Key_T ] = 'T';
        m_KeyTranslationsBit[ Key_U ] = 'U';
        m_KeyTranslationsBit[ Key_V ] = 'V';
        m_KeyTranslationsBit[ Key_W ] = 'W';
        m_KeyTranslationsBit[ Key_X ] = 'X';
        m_KeyTranslationsBit[ Key_Y ] = 'Y';
        m_KeyTranslationsBit[ Key_Z ] = 'Z';

        m_KeyTranslationsBit[ Key_0 ] = '0';
        m_KeyTranslationsBit[ Key_1 ] = '1';
        m_KeyTranslationsBit[ Key_2 ] = '2';
        m_KeyTranslationsBit[ Key_3 ] = '3';
        m_KeyTranslationsBit[ Key_4 ] = '4';
        m_KeyTranslationsBit[ Key_5 ] = '5';
        m_KeyTranslationsBit[ Key_6 ] = '6';
        m_KeyTranslationsBit[ Key_7 ] = '7';
        m_KeyTranslationsBit[ Key_8 ] = '8';
        m_KeyTranslationsBit[ Key_9 ] = '9';

        m_KeyTranslationsBit[ Key_F1 ] = VK_F1;
        m_KeyTranslationsBit[ Key_F2 ] = VK_F2;
        m_KeyTranslationsBit[ Key_F3 ] = VK_F3;
        m_KeyTranslationsBit[ Key_F4 ] = VK_F4;
        m_KeyTranslationsBit[ Key_F5 ] = VK_F5;
        m_KeyTranslationsBit[ Key_F6 ] = VK_F6;
        m_KeyTranslationsBit[ Key_F7 ] = VK_F7;
        m_KeyTranslationsBit[ Key_F8 ] = VK_F8;
        m_KeyTranslationsBit[ Key_F9 ] = VK_F9;
        m_KeyTranslationsBit[ Key_F10 ] = VK_F10;
        m_KeyTranslationsBit[ Key_F11 ] = VK_F11;
        m_KeyTranslationsBit[ Key_F12 ] = VK_F12;

        m_KeyTranslationsBit[ Key_Down ] = VK_DOWN;
        m_KeyTranslationsBit[ Key_Left ] = VK_LEFT;
        m_KeyTranslationsBit[ Key_Right ] = VK_RIGHT;
        m_KeyTranslationsBit[ Key_Up ] = VK_UP;

        //m_KeyTranslationsBit[ Key_A_Diaeresis ]       = XK_Adiaeresis;
        //m_KeyTranslationsBit[ Key_A_Ring ]            = XK_Aring;
        m_KeyTranslationsBit[ Key_Alt_L ]             = VK_LMENU;
        m_KeyTranslationsBit[ Key_Alt_R ]             = VK_RMENU;
        //m_KeyTranslationsBit[ Key_Ampersand ]         = XK_ampersand;
        //m_KeyTranslationsBit[ Key_Apostrophe ]        = XK_apostrophe;
        //m_KeyTranslationsBit[ Key_Asterisk ]          = XK_asterisk;
        m_KeyTranslationsBit[ Key_BackSlash ]         = VK_OEM_5;
        m_KeyTranslationsBit[ Key_Backspace ]         = VK_BACK;
        //m_KeyTranslationsBit[ Key_Bar ]               = XK_bar;
        //m_KeyTranslationsBit[ Key_Brace_Left ]        = XK_braceleft;
        //m_KeyTranslationsBit[ Key_Brace_Right ]       = XK_braceright;
        m_KeyTranslationsBit[ Key_Bracket_L ]         = VK_OEM_4;
        m_KeyTranslationsBit[ Key_Bracket_R ]         = VK_OEM_6;
        m_KeyTranslationsBit[ Key_Break ]             = VK_PAUSE;
        m_KeyTranslationsBit[ Key_Capslock ]          = VK_CAPITAL;
        //m_KeyTranslationsBit[ Key_Colon ]             = XK_colon;
        m_KeyTranslationsBit[ Key_Control_L ]         = VK_LCONTROL;
        m_KeyTranslationsBit[ Key_Control_R ]         = VK_RCONTROL;
        m_KeyTranslationsBit[ Key_Comma ]             = VK_OEM_COMMA;
        m_KeyTranslationsBit[ Key_Delete ]            = VK_DELETE;
        //m_KeyTranslationsBit[ Key_Dollar ]            = XK_dollar;
        m_KeyTranslationsBit[ Key_End ]               = VK_END;
        m_KeyTranslationsBit[ Key_Equal ]             = VK_OEM_PLUS;
        m_KeyTranslationsBit[ Key_Escape ]            = VK_ESCAPE;
        //m_KeyTranslationsBit[ Key_Exclam ]            = XK_exclam;
        //m_KeyTranslationsBit[ Key_Greater ]           = XK_greater;
        m_KeyTranslationsBit[ Key_Home ]              = VK_HOME;
        m_KeyTranslationsBit[ Key_Insert ]            = VK_INSERT;
        //m_KeyTranslationsBit[ Key_Less ]              = XK_less;
        m_KeyTranslationsBit[ Key_Minus ]             = VK_OEM_MINUS;
        //m_KeyTranslationsBit[ Key_Number ]            = XK_numbersign;
        m_KeyTranslationsBit[ Key_NumLock ]           = VK_NUMLOCK;
        //m_KeyTranslationsBit[ Key_O_Diaeresis ]       = XK_Odiaeresis;
        m_KeyTranslationsBit[ Key_PageDown ]          = VK_NEXT;
        m_KeyTranslationsBit[ Key_PageUp ]            = VK_PRIOR;
        //m_KeyTranslationsBit[ Key_Parenthesis_Left ]  = XK_parenleft;
        //m_KeyTranslationsBit[ Key_Parenthesis_Right ] = XK_parenright;
        //m_KeyTranslationsBit[ Key_Percent ]           = XK_percent;
        m_KeyTranslationsBit[ Key_Period ]            = VK_OEM_PERIOD;
        m_KeyTranslationsBit[ Key_Plus ]              = VK_ADD;
        m_KeyTranslationsBit[ Key_Print ]             = VK_SNAPSHOT;
        //m_KeyTranslationsBit[ Key_Question ]          = XK_question;
        m_KeyTranslationsBit[ Key_Quote ]             = VK_OEM_7;
        m_KeyTranslationsBit[ Key_Return ]            = VK_RETURN;
        m_KeyTranslationsBit[ Key_ScrollLock ]        = VK_SCROLL;
        //m_KeyTranslationsBit[ Key_Section ]           = XK_section;
        m_KeyTranslationsBit[ Key_SemiColon ]         = VK_OEM_1;
        m_KeyTranslationsBit[ Key_Shift_L ]           = VK_LSHIFT ;
        m_KeyTranslationsBit[ Key_Shift_R ]           = VK_RSHIFT ;
        m_KeyTranslationsBit[ Key_Slash ]             = VK_OEM_2;
        m_KeyTranslationsBit[ Key_Space ]             = VK_SPACE;
        m_KeyTranslationsBit[ key_Super_L ]           = VK_LWIN;
        m_KeyTranslationsBit[ key_Super_R ]           = VK_RWIN ;
        m_KeyTranslationsBit[ Key_Tab ]               = VK_TAB;
        m_KeyTranslationsBit[ Key_Tilde ]             = VK_OEM_3;
        //m_KeyTranslationsBit[ Key_Underscore ]        = XK_underscore;




        // ///////////////////////////////////////////////////////
        // Set our SYSTEM key translations
        m_KeyTranslationsSystem[ 'A' ] = Key_A;
        m_KeyTranslationsSystem[ 'B' ] = Key_B;
        m_KeyTranslationsSystem[ 'C' ] = Key_C;
        m_KeyTranslationsSystem[ 'D' ] = Key_D;
        m_KeyTranslationsSystem[ 'E' ] = Key_E;
        m_KeyTranslationsSystem[ 'F' ] = Key_F;
        m_KeyTranslationsSystem[ 'G' ] = Key_G;
        m_KeyTranslationsSystem[ 'H' ] = Key_H;
        m_KeyTranslationsSystem[ 'I' ] = Key_I;
        m_KeyTranslationsSystem[ 'J' ] = Key_J;
        m_KeyTranslationsSystem[ 'K' ] = Key_K;
        m_KeyTranslationsSystem[ 'L' ] = Key_L;
        m_KeyTranslationsSystem[ 'M' ] = Key_M;
        m_KeyTranslationsSystem[ 'N' ] = Key_N;
        m_KeyTranslationsSystem[ 'O' ] = Key_O;
        m_KeyTranslationsSystem[ 'P' ] = Key_P;
        m_KeyTranslationsSystem[ 'Q' ] = Key_Q;
        m_KeyTranslationsSystem[ 'R' ] = Key_R;
        m_KeyTranslationsSystem[ 'S' ] = Key_S;
        m_KeyTranslationsSystem[ 'T' ] = Key_T;
        m_KeyTranslationsSystem[ 'U' ] = Key_U;
        m_KeyTranslationsSystem[ 'V' ] = Key_V;
        m_KeyTranslationsSystem[ 'W' ] = Key_W;
        m_KeyTranslationsSystem[ 'X' ] = Key_X;
        m_KeyTranslationsSystem[ 'Y' ] = Key_Y;
        m_KeyTranslationsSystem[ 'Z' ] = Key_Z;

        m_KeyTranslationsSystem[ '0' ] = Key_0;
        m_KeyTranslationsSystem[ '1' ] = Key_1;
        m_KeyTranslationsSystem[ '2' ] = Key_2;
        m_KeyTranslationsSystem[ '3' ] = Key_3;
        m_KeyTranslationsSystem[ '4' ] = Key_4;
        m_KeyTranslationsSystem[ '5' ] = Key_5;
        m_KeyTranslationsSystem[ '6' ] = Key_6;
        m_KeyTranslationsSystem[ '7' ] = Key_7;
        m_KeyTranslationsSystem[ '8' ] = Key_8;
        m_KeyTranslationsSystem[ '9' ] = Key_9;

        m_KeyTranslationsSystem[ VK_F1 ] = Key_F1;
        m_KeyTranslationsSystem[ VK_F2 ] = Key_F2;
        m_KeyTranslationsSystem[ VK_F3 ] = Key_F3;
        m_KeyTranslationsSystem[ VK_F4 ] = Key_F4;
        m_KeyTranslationsSystem[ VK_F5 ] = Key_F5;
        m_KeyTranslationsSystem[ VK_F6 ] = Key_F6;
        m_KeyTranslationsSystem[ VK_F7 ] = Key_F7;
        m_KeyTranslationsSystem[ VK_F8 ] = Key_F8;
        m_KeyTranslationsSystem[ VK_F9 ] = Key_F9;
        m_KeyTranslationsSystem[ VK_F10 ] = Key_F10;
        m_KeyTranslationsSystem[ VK_F11 ] = Key_F11;
        m_KeyTranslationsSystem[ VK_F12 ] = Key_F12;

        m_KeyTranslationsSystem[ VK_DOWN ] = Key_Down;
        m_KeyTranslationsSystem[ VK_LEFT ] = Key_Left;
        m_KeyTranslationsSystem[ VK_RIGHT ] = Key_Right;
        m_KeyTranslationsSystem[ VK_UP ] = Key_Up;
/*
        m_KeyTranslationsSystem[ XK_Adiaeresis ]        = Key_A_Diaeresis;
        m_KeyTranslationsSystem[ XK_Aring ]             = Key_A_Ring;
        m_KeyTranslationsSystem[ XK_Alt_L ]             = Key_Alt_L;
        m_KeyTranslationsSystem[ XK_Alt_R ]             = Key_Alt_R;
        m_KeyTranslationsSystem[ XK_ampersand ]         = Key_Ampersand;
        m_KeyTranslationsSystem[ XK_apostrophe ]        = Key_Apostrophe;
        m_KeyTranslationsSystem[ XK_asterisk ]          = Key_Asterisk;
        m_KeyTranslationsSystem[ XK_backslash ]         = Key_BackSlash;
        m_KeyTranslationsSystem[ XK_BackSpace ]         = Key_Backspace;
        m_KeyTranslationsSystem[ XK_bar ]               = Key_Bar;
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
        m_KeyTranslationsSystem[ XK_dollar ]            = Key_Dollar;
        m_KeyTranslationsSystem[ XK_End ]               = Key_End;
        m_KeyTranslationsSystem[ XK_equal ]             = Key_Equal;
        m_KeyTranslationsSystem[ XK_Escape ]            = Key_Escape;
        m_KeyTranslationsSystem[ XK_exclam ]            = Key_Exclam;
        m_KeyTranslationsSystem[ XK_greater ]           = Key_Greater;
        m_KeyTranslationsSystem[ XK_Home ]              = Key_Home;
        m_KeyTranslationsSystem[ XK_Insert ]            = Key_Insert;
        m_KeyTranslationsSystem[ XK_less ]              = Key_Less;
        m_KeyTranslationsSystem[ XK_minus ]             = Key_Minus;
        m_KeyTranslationsSystem[ XK_numbersign ]        = Key_Number;
        m_KeyTranslationsSystem[ XK_Num_Lock ]          = Key_NumLock;
        m_KeyTranslationsSystem[ XK_Odiaeresis ]        = Key_O_Diaeresis;
        m_KeyTranslationsSystem[ XK_Page_Down ]         = Key_PageDown;
        m_KeyTranslationsSystem[ XK_Page_Up ]           = Key_PageUp;
        m_KeyTranslationsSystem[ XK_parenleft ]         = Key_Parenthesis_Left;
        m_KeyTranslationsSystem[ XK_parenright ]        = Key_Parenthesis_Right;
        m_KeyTranslationsSystem[ XK_percent ]           = Key_Percent;
        m_KeyTranslationsSystem[ XK_period ]            = Key_Period;
        m_KeyTranslationsSystem[ XK_plus ]              = Key_Plus;
        m_KeyTranslationsSystem[ XK_Print ]             = Key_Print;
        m_KeyTranslationsSystem[ XK_question ]          = Key_Question;
        m_KeyTranslationsSystem[ XK_quotedbl ]          = Key_Quote;
        m_KeyTranslationsSystem[ XK_Return ]            = Key_Return;
        m_KeyTranslationsSystem[ XK_Scroll_Lock ]       = Key_ScrollLock;
        m_KeyTranslationsSystem[ XK_section ]           = Key_Section;
        m_KeyTranslationsSystem[ XK_semicolon ]         = Key_SemiColon;
        m_KeyTranslationsSystem[ XK_Shift_L ]           = Key_Shift_L;
        m_KeyTranslationsSystem[ XK_Shift_R ]           = Key_Shift_R;
        m_KeyTranslationsSystem[ XK_slash ]             = Key_Slash;
        m_KeyTranslationsSystem[ XK_space ]             = Key_Space;
        m_KeyTranslationsSystem[ XK_Super_L ]           = key_Super_L;
        m_KeyTranslationsSystem[ XK_Super_R ]           = key_Super_R;
        m_KeyTranslationsSystem[ XK_Tab ]               = Key_Tab;
        m_KeyTranslationsSystem[ XK_dead_grave ]        = Key_Tilde;
        m_KeyTranslationsSystem[ XK_underscore ]        = Key_Underscore;
*/


    }

    KeyboardWin32::~KeyboardWin32( )
    {
        // Close the connection with X server
        /*if( m_pDisplay )
        {
            XCloseDisplay( m_pDisplay );
        }*/
    }

    // Public general functions
    void KeyboardWin32::Update( )
    {
        /*if( m_pDisplay == BIT_NULL )
        {
            return;
        }*/

        for( BIT_MEMSIZE i = 0; i < m_ChangedKeys.size( ); i++ )
        {
           m_PreviousKeyState[ m_ChangedKeys[ i ] ] = m_CurrentKeyState[ m_ChangedKeys[ i ]  ];
        }
        m_ChangedKeys.clear( );
    }

    // Key translation function for platform keys
    Keyboard::eKey KeyboardWin32::TranslateKeyToBitKey( const BIT_UINT16 p_Key )
    {
        return m_KeyTranslationsSystem[ p_Key ];
    }

    BIT_UINT16 KeyboardWin32::TranslateKeyToSystemKey( const eKey p_Key )
    {
        return m_KeyTranslationsBit[ static_cast< BIT_UINT16 >( p_Key ) ];
    }

    // Get state functions
    BIT_BOOL KeyboardWin32::KeyIsDown( const eKey p_Key )
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

    BIT_BOOL KeyboardWin32::KeyIsUp( const eKey p_Key )
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

    BIT_BOOL KeyboardWin32::KeyIsJustPressed( const eKey p_Key )
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

    BIT_BOOL KeyboardWin32::KeyIsJustReleased( const eKey p_Key )
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

    BIT_BOOL KeyboardWin32::GetCurrentKeyState( const eKey p_Key )
    {
        return m_CurrentKeyState[ p_Key ];
    }

    BIT_BOOL KeyboardWin32::GetPreviousKeyState( const eKey p_Key )
    {
         return m_PreviousKeyState[ p_Key ];
    }

    // Set state functions
    void KeyboardWin32::SetCurrentKeyState( const eKey p_Key, const BIT_BOOL p_State )
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

    void KeyboardWin32::SetPreviousKeyState( const eKey p_Key, const BIT_BOOL p_State )
    {
        m_PreviousKeyState[ p_Key ] = p_State;
    }

    // Private functions
    BIT_BOOL KeyboardWin32::GetKeyStatus( const eKey p_Key )
    {
		BIT_UINT32 KeyTranslation = m_KeyTranslationsBit[ p_Key ];
		return ( GetAsyncKeyState( KeyTranslation ) & 0x8000 ) != 0;
    }

}

#endif
