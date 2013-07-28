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
        m_KeyTranslationsBit[ Key_T ] = XK_y;
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

        m_KeyTranslationsBit[ Key_A_Diaeresis ]       = XK_Adiaeresis;
        m_KeyTranslationsBit[ Key_A_Ring ]            = XK_Aring;
        m_KeyTranslationsBit[ Key_Alt_L ]             = XK_Alt_L;
        m_KeyTranslationsBit[ Key_Alt_R ]             = XK_Alt_R;
        m_KeyTranslationsBit[ Key_Ampersand ]         = XK_ampersand;
        m_KeyTranslationsBit[ Key_Apostrophe ]        = XK_apostrophe;
        m_KeyTranslationsBit[ Key_Asterisk ]          = XK_asterisk;
        m_KeyTranslationsBit[ Key_BackSlash ]         = XK_backslash;
        m_KeyTranslationsBit[ Key_Backspace ]         = XK_BackSpace;
        m_KeyTranslationsBit[ Key_Bar ]               = XK_bar;
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
        m_KeyTranslationsBit[ Key_Dollar ]            = XK_dollar;
        m_KeyTranslationsBit[ Key_End ]               = XK_End;
        m_KeyTranslationsBit[ Key_Equal ]             = XK_equal;
        m_KeyTranslationsBit[ Key_Escape ]            = XK_Escape;
        m_KeyTranslationsBit[ Key_Exclam ]            = XK_exclam;
        m_KeyTranslationsBit[ Key_Greater ]           = XK_greater;
        m_KeyTranslationsBit[ Key_Home ]              = XK_Home;
        m_KeyTranslationsBit[ Key_Insert ]            = XK_Insert;
        m_KeyTranslationsBit[ Key_Less ]              = XK_less;
        m_KeyTranslationsBit[ Key_Minus ]             = XK_minus;
        m_KeyTranslationsBit[ Key_Number ]            = XK_numbersign;
        m_KeyTranslationsBit[ Key_NumLock ]           = XK_Num_Lock;
        m_KeyTranslationsBit[ Key_O_Diaeresis ]       = XK_Odiaeresis;
        m_KeyTranslationsBit[ Key_PageDown ]          = XK_Page_Down;
        m_KeyTranslationsBit[ Key_PageUp ]            = XK_Page_Up;
        m_KeyTranslationsBit[ Key_Parenthesis_Left ]  = XK_parenleft;
        m_KeyTranslationsBit[ Key_Parenthesis_Right ] = XK_parenright;
        m_KeyTranslationsBit[ Key_Percent ]           = XK_percent;
        m_KeyTranslationsBit[ Key_Period ]            = XK_period;
        m_KeyTranslationsBit[ Key_Plus ]              = XK_plus;
        m_KeyTranslationsBit[ Key_Print ]             = XK_Print;
        m_KeyTranslationsBit[ Key_Question ]          = XK_question;
        m_KeyTranslationsBit[ Key_Quote ]             = XK_quotedbl;
        m_KeyTranslationsBit[ Key_Return ]            = XK_Return;
        m_KeyTranslationsBit[ Key_ScrollLock ]        = XK_Scroll_Lock;
        m_KeyTranslationsBit[ Key_Section ]           = XK_section;
        m_KeyTranslationsBit[ Key_SemiColon ]         = XK_semicolon;
        m_KeyTranslationsBit[ Key_Shift_L ]           = XK_Shift_L;
        m_KeyTranslationsBit[ Key_Shift_R ]           = XK_Shift_R;
        m_KeyTranslationsBit[ Key_Slash ]             = XK_slash;
        m_KeyTranslationsBit[ Key_Space ]             = XK_space;
        m_KeyTranslationsBit[ key_Super_L ]           = XK_Super_L;
        m_KeyTranslationsBit[ key_Super_R ]           = XK_Super_R;
        m_KeyTranslationsBit[ Key_Tab ]               = XK_Tab;
        m_KeyTranslationsBit[ Key_Tilde ]             = XK_dead_grave;
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
        m_KeyTranslationsSystem[ XK_y ] = Key_T;
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

    }

    KeyboardLinux::~KeyboardLinux( )
    {
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
        /*switch( p_Key )
        {
            case XK_w: return Key_W;
            case XK_a: return Key_A;
            //case XK_S: return Key_W;
            case XK_d: return Key_D;
            default: return Keyboard::Key_None;
        }

        return Keyboard::Key_None;*/
    }

    BIT_UINT16 KeyboardLinux::TranslateKeyToSystemKey( const eKey p_Key )
    {
        return m_KeyTranslationsBit[ static_cast< BIT_UINT16 >( p_Key ) ];
    }

    // Get state functions
    BIT_BOOL KeyboardLinux::KeyIsDown( eKey p_Key )
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

    BIT_BOOL KeyboardLinux::KeyIsUp( eKey p_Key )
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

    BIT_BOOL KeyboardLinux::KeyIsJustPressed( eKey p_Key )
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

    BIT_BOOL KeyboardLinux::KeyIsJustReleased( eKey p_Key )
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

    // Private functions
    BIT_BOOL KeyboardLinux::GetKeyStatus( eKey p_Key )
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
