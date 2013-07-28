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

        // Clear the key translations
        // NOT REQUIRED WHEN ALL THE KEY CODES ARE SET
        for( BIT_MEMSIZE i = 0; i < Keyboard::Key_Count; i++ )
        {
            m_KeyTranslations[ i ] = 0;
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

        // Set our key translations
        m_KeyTranslations[ Key_A ] = XK_A;
        m_KeyTranslations[ Key_B ] = XK_B;
        m_KeyTranslations[ Key_C ] = XK_C;
        m_KeyTranslations[ Key_D ] = XK_D;
        m_KeyTranslations[ Key_E ] = XK_E;
        m_KeyTranslations[ Key_F ] = XK_F;
        m_KeyTranslations[ Key_G ] = XK_G;
        m_KeyTranslations[ Key_H ] = XK_H;
        m_KeyTranslations[ Key_I ] = XK_I;
        m_KeyTranslations[ Key_J ] = XK_J;
        m_KeyTranslations[ Key_K ] = XK_K;
        m_KeyTranslations[ Key_L ] = XK_L;
        m_KeyTranslations[ Key_M ] = XK_M;
        m_KeyTranslations[ Key_N ] = XK_N;
        m_KeyTranslations[ Key_O ] = XK_O;
        m_KeyTranslations[ Key_P ] = XK_P;
        m_KeyTranslations[ Key_Q ] = XK_Q;
        m_KeyTranslations[ Key_R ] = XK_R;
        m_KeyTranslations[ Key_S ] = XK_S;
        m_KeyTranslations[ Key_T ] = XK_T;
        m_KeyTranslations[ Key_U ] = XK_U;
        m_KeyTranslations[ Key_V ] = XK_V;
        m_KeyTranslations[ Key_W ] = XK_W;
        m_KeyTranslations[ Key_X ] = XK_X;
        m_KeyTranslations[ Key_Y ] = XK_Y;
        m_KeyTranslations[ Key_Z ] = XK_Z;

        m_KeyTranslations[ Key_0 ] = XK_0;
        m_KeyTranslations[ Key_1 ] = XK_1;
        m_KeyTranslations[ Key_2 ] = XK_2;
        m_KeyTranslations[ Key_3 ] = XK_3;
        m_KeyTranslations[ Key_4 ] = XK_4;
        m_KeyTranslations[ Key_5 ] = XK_5;
        m_KeyTranslations[ Key_6 ] = XK_6;
        m_KeyTranslations[ Key_7 ] = XK_7;
        m_KeyTranslations[ Key_8 ] = XK_8;
        m_KeyTranslations[ Key_9 ] = XK_9;

        m_KeyTranslations[ Key_F1 ] = XK_F1;
        m_KeyTranslations[ Key_F2 ] = XK_F2;
        m_KeyTranslations[ Key_F3 ] = XK_F3;
        m_KeyTranslations[ Key_F4 ] = XK_F4;
        m_KeyTranslations[ Key_F5 ] = XK_F5;
        m_KeyTranslations[ Key_F6 ] = XK_F6;
        m_KeyTranslations[ Key_F7 ] = XK_F7;
        m_KeyTranslations[ Key_F8 ] = XK_F8;
        m_KeyTranslations[ Key_F9 ] = XK_F9;
        m_KeyTranslations[ Key_F10 ] = XK_F10;
        m_KeyTranslations[ Key_F11 ] = XK_F11;
        m_KeyTranslations[ Key_F12 ] = XK_F12;

        m_KeyTranslations[ Key_Down ] = XK_Down;
        m_KeyTranslations[ Key_Left ] = XK_Left;
        m_KeyTranslations[ Key_Right ] = XK_Right;
        m_KeyTranslations[ Key_Up ] = XK_Up;

        m_KeyTranslations[ Key_Alt_L ] = XK_Alt_L;
        m_KeyTranslations[ Key_Alt_R ] = XK_Alt_R;
        m_KeyTranslations[ Key_Backspace ] = XK_BackSpace;
        m_KeyTranslations[ Key_Bracket_L ] = XK_bracketleft;
        m_KeyTranslations[ Key_Bracket_R ] = XK_bracketright;
        m_KeyTranslations[ Key_Break ] = XK_Break;
        m_KeyTranslations[ Key_Capslock ] = XK_Caps_Lock;
        m_KeyTranslations[ Key_Control_L ] = XK_Control_L;
        m_KeyTranslations[ Key_Control_R ] = XK_Control_R;
        m_KeyTranslations[ Key_Delete ] = XK_Delete;
        m_KeyTranslations[ Key_End ] = XK_End;
        m_KeyTranslations[ key_Espace ] = XK_Escape;
        m_KeyTranslations[ Key_Home ] = XK_Home;
        m_KeyTranslations[ Key_Insert ] = XK_Insert;
        m_KeyTranslations[ Key_PageDown ] = XK_Page_Down;
        m_KeyTranslations[ Key_PageUp ] = XK_Page_Up;
        m_KeyTranslations[ Key_Print ] = XK_Print;
        m_KeyTranslations[ Key_Return ] = XK_Return;
        m_KeyTranslations[ Key_ScrollLock ] = XK_Scroll_Lock;
        m_KeyTranslations[ Key_Section] = XK_section;
        m_KeyTranslations[ Key_Shift_L ] = XK_Shift_L;
        m_KeyTranslations[ Key_Shift_R ] = XK_Shift_R;
        m_KeyTranslations[ Key_Space ] = XK_space;
        m_KeyTranslations[ key_Super_L ] = XK_Super_L;
        m_KeyTranslations[ key_Super_R ] = XK_Super_R;
        m_KeyTranslations[ Key_Tab ] = XK_Tab;

        // New unsorted keys
        m_KeyTranslations[ Key_Asterisk ] = XK_asterisk;
        m_KeyTranslations[ Key_Plus ] = XK_plus;
        m_KeyTranslations[ Key_Comma ] = XK_comma;
        m_KeyTranslations[ Key_Minus ] = XK_minus;
        m_KeyTranslations[ Key_Period ] = XK_period;
        m_KeyTranslations[ Key_Slash ] = XK_slash;
        m_KeyTranslations[ Key_Colon ] = XK_colon;
        m_KeyTranslations[ Key_SemiColon ] = XK_semicolon;
        m_KeyTranslations[ Key_Less ] = XK_less;
        m_KeyTranslations[ Key_Equal ] = XK_equal;
        m_KeyTranslations[ Key_Greater ] = XK_greater;
        m_KeyTranslations[ Key_Question ] = XK_question;
        m_KeyTranslations[ Key_Underscore ] = XK_underscore;
        m_KeyTranslations[ Key_A_Diaeresis ] = XK_Adiaeresis;
        m_KeyTranslations[ Key_A_Ring ] = XK_Aring;
        m_KeyTranslations[ Key_O_Diaeresis ] = XK_Odiaeresis;

        m_KeyTranslations[ Key_Exclam ] = XK_exclam;       // !
        m_KeyTranslations[ Key_Quote ] = XK_quotedbl;     // "
        m_KeyTranslations[ Key_Number ] = XK_numbersign;   // #
        m_KeyTranslations[ Key_Dollar ] = XK_dollar;       // $
        m_KeyTranslations[ Key_Percent ] = XK_percent;      // %
        m_KeyTranslations[ Key_Ampersand ] = XK_ampersand;    // &
        m_KeyTranslations[ Key_Apostrophe ] = XK_apostrophe;   // ´
        m_KeyTranslations[ Key_Parenthesis_Left ] = XK_parenleft;
        m_KeyTranslations[ Key_Parenthesis_Right ] = XK_parenright;
        m_KeyTranslations[ Key_Brace_Left ] = XK_braceleft;    // }
        m_KeyTranslations[ Key_Bar ] = XK_bar;          // |
        m_KeyTranslations[ Key_Brace_Right ] = XK_braceright;   // {
        m_KeyTranslations[ Key_Tilde ] = XK_asciitilde;   // ~



//#define XK_exclam                        0x0021  /* U+0021 EXCLAMATION MARK */
//#define XK_quotedbl                      0x0022  /* U+0022 QUOTATION MARK */
//#define XK_numbersign                    0x0023  /* U+0023 NUMBER SIGN */
//#define XK_dollar                        0x0024  /* U+0024 DOLLAR SIGN */
//#define XK_percent                       0x0025  /* U+0025 PERCENT SIGN */
//#define XK_ampersand                     0x0026  /* U+0026 AMPERSAND */
//#define XK_apostrophe                    0x0027  /* U+0027 APOSTROPHE */
//#define XK_parenleft                     0x0028  /* U+0028 LEFT PARENTHESIS */
//#define XK_parenright                    0x0029  /* U+0029 RIGHT PARENTHESIS */
//#define XK_asterisk                      0x002a  /* U+002A ASTERISK */
//
//#define XK_braceleft                     0x007b  /* U+007B LEFT CURLY BRACKET */
//#define XK_bar                           0x007c  /* U+007C VERTICAL LINE */
//#define XK_braceright                    0x007d  /* U+007D RIGHT CURLY BRACKET */
//#define XK_asciitilde                    0x007e  /* U+007E TILDE */
//
//


        //    #define XK_Home                          0xff50
        //    #define XK_Left                          0xff51  /* Move left, left arrow */
       //     #define XK_Up                            0xff52  /* Move up, up arrow */
        //    #define XK_Right                         0xff53  /* Move right, right arrow */
        //    #define XK_Down                          0xff54  /* Move down, down arrow */
         //   #define XK_Prior                         0xff55  /* Prior, previous */
          //  #define XK_Page_Up                       0xff55
       //     #define XK_Next                          0xff56  /* Next */
       //     #define XK_Page_Down                     0xff56
       //     #define XK_End                           0xff57  /* EOL */
         //   #define XK_Begin                         0xff58  /* BOL */


            /* Misc functions */

     //       #define XK_Num_Lock                      0xff7f



        /*

        // Arrow keys



        // Media keys
        Key_Mute,
        Key_Next,
        Key_Play,
        Key_Previous,
        Key_Stop,
        Key_VolumeHigh,
        Key_VolumeLow,

        */
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
    Keyboard::eKey KeyboardLinux::TranslateKey( const BIT_UINT32 p_Key )
    {
        return Keyboard::Key_None;
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



        KeySym keyTranslation = 0;// m_KeyTranslations[ p_Key ];
        switch( p_Key )
        {
            case Key_A:                 keyTranslation = XK_A;              break;
            case Key_B:                 keyTranslation = XK_B;              break;
            case Key_C:                 keyTranslation = XK_C;              break;
            case Key_D:                 keyTranslation = XK_D;              break;
            case Key_E:                 keyTranslation = XK_E;              break;
            case Key_F:                 keyTranslation = XK_F;              break;
            case Key_G:                 keyTranslation = XK_G;              break;
            case Key_H:                 keyTranslation = XK_H;              break;
            case Key_I:                 keyTranslation = XK_I;              break;
            case Key_J:                 keyTranslation = XK_J;              break;
            case Key_K:                 keyTranslation = XK_K;              break;
            case Key_L:                 keyTranslation = XK_L;              break;
            case Key_M:                 keyTranslation = XK_M;              break;
            case Key_N:                 keyTranslation = XK_N;              break;
            case Key_O:                 keyTranslation = XK_O;              break;
            case Key_P:                 keyTranslation = XK_P;              break;
            case Key_Q:                 keyTranslation = XK_Q;              break;
            case Key_R:                 keyTranslation = XK_R;              break;
            case Key_S:                 keyTranslation = XK_S;              break;
            case Key_T:                 keyTranslation = XK_T;              break;
            case Key_U:                 keyTranslation = XK_U;              break;
            case Key_V:                 keyTranslation = XK_V;              break;
            case Key_W:                 keyTranslation = XK_W;              break;
            case Key_X:                 keyTranslation = XK_X;              break;
            case Key_Y:                 keyTranslation = XK_Y;              break;
            case Key_Z:                 keyTranslation = XK_Z;              break;

            case Key_0:                 keyTranslation = XK_0;              break;
            case Key_1:                 keyTranslation = XK_1;              break;
            case Key_2:                 keyTranslation = XK_2;              break;
            case Key_3:                 keyTranslation = XK_3;              break;
            case Key_4:                 keyTranslation = XK_4;              break;
            case Key_5:                 keyTranslation = XK_5;              break;
            case Key_6:                 keyTranslation = XK_6;              break;
            case Key_7:                 keyTranslation = XK_7;              break;
            case Key_8:                 keyTranslation = XK_8;              break;
            case Key_9:                 keyTranslation = XK_9;              break;

            case Key_F1:                keyTranslation = XK_F1;             break;
            case Key_F2:                keyTranslation = XK_F2;             break;
            case Key_F3:                keyTranslation = XK_F3;             break;
            case Key_F4:                keyTranslation = XK_F4;             break;
            case Key_F5:                keyTranslation = XK_F5;             break;
            case Key_F6:                keyTranslation = XK_F6;             break;
            case Key_F7:                keyTranslation = XK_F7;             break;
            case Key_F8:                keyTranslation = XK_F8;             break;
            case Key_F9:                keyTranslation = XK_F9;             break;
            case Key_F10:               keyTranslation = XK_F10;            break;
            case Key_F11:               keyTranslation = XK_F11;            break;
            case Key_F12:               keyTranslation = XK_F12;            break;

            case Key_Down:              keyTranslation = XK_Down;           break;
            case Key_Left:              keyTranslation = XK_Left;           break;
            case Key_Right:             keyTranslation = XK_Right;          break;
            case Key_Up:                keyTranslation = XK_Up;             break;

            case Key_A_Diaeresis:       keyTranslation = XK_Adiaeresis;     break;
            case Key_A_Ring:            keyTranslation = XK_Aring;          break;
            case Key_Alt_L:             keyTranslation = XK_Alt_L;          break;
            case Key_Alt_R:             keyTranslation = XK_Alt_R;          break;
            case Key_Ampersand:         keyTranslation = XK_ampersand;      break;
            case Key_Apostrophe:        keyTranslation = XK_apostrophe;     break;
            case Key_Asterisk:          keyTranslation = XK_asterisk;       break;
            case Key_BackSlash:         keyTranslation = XK_backslash;      break;
            case Key_Backspace:         keyTranslation = XK_BackSpace;      break;
            case Key_Bar:               keyTranslation = XK_bar;            break;
            case Key_Brace_Left:        keyTranslation = XK_braceleft;      break;
            case Key_Brace_Right:       keyTranslation = XK_braceright;     break;
            case Key_Bracket_L:         keyTranslation = XK_bracketleft;    break;
            case Key_Bracket_R:         keyTranslation = XK_bracketright;   break;
            case Key_Break:             keyTranslation = XK_Break;          break;
            case Key_Capslock:          keyTranslation = XK_Caps_Lock;      break;
            case Key_Colon:             keyTranslation = XK_colon;          break;
            case Key_Control_L:         keyTranslation = XK_Control_L;      break;
            case Key_Control_R:         keyTranslation = XK_Control_R;      break;
            case Key_Comma:             keyTranslation = XK_comma;          break;
            case Key_Delete:            keyTranslation = XK_Delete;         break;
            case Key_Dollar:            keyTranslation = XK_dollar;         break;
            case Key_End:               keyTranslation = XK_End;            break;
            case Key_Equal:             keyTranslation = XK_equal;          break;
            case key_Espace:            keyTranslation = XK_Escape;         break;
            case Key_Exclam:            keyTranslation = XK_exclam;         break;
            case Key_Greater:           keyTranslation = XK_greater;        break;
            case Key_Home:              keyTranslation = XK_Home;           break;
            case Key_Insert:            keyTranslation = XK_Insert;         break;
            case Key_Less:              keyTranslation = XK_less;           break;
            case Key_Minus:             keyTranslation = XK_minus;          break;
            case Key_Number:            keyTranslation = XK_numbersign;     break;
            case Key_O_Diaeresis:       keyTranslation = XK_Odiaeresis;     break;
            case Key_PageDown:          keyTranslation = XK_Page_Down;      break;
            case Key_PageUp:            keyTranslation = XK_Page_Up;        break;
            case Key_Parenthesis_Left:  keyTranslation = XK_parenleft;      break;
            case Key_Parenthesis_Right: keyTranslation = XK_parenright;     break;
            case Key_Percent:           keyTranslation = XK_percent;        break;
            case Key_Period:            keyTranslation = XK_period;         break;
            case Key_Plus:              keyTranslation = XK_plus;           break;
            case Key_Print:             keyTranslation = XK_Print;          break;
            case Key_Question:          keyTranslation = XK_question;       break;
            case Key_Quote:             keyTranslation = XK_quotedbl;       break;
            case Key_Return:            keyTranslation = XK_Return;         break;
            case Key_ScrollLock:        keyTranslation = XK_Scroll_Lock;    break;
            case Key_Section:           keyTranslation = XK_section;        break;
            case Key_SemiColon:         keyTranslation = XK_semicolon;      break;
            case Key_Shift_L:           keyTranslation = XK_Shift_L;        break;
            case Key_Shift_R:           keyTranslation = XK_Shift_R;        break;
            case Key_Slash:             keyTranslation = XK_slash;          break;
            case Key_Space:             keyTranslation = XK_space;          break;
            case key_Super_L:           keyTranslation = XK_Super_L;        break;
            case key_Super_R:           keyTranslation = XK_Super_R;        break;
            case Key_Tab:               keyTranslation = XK_Tab;            break;
            case Key_Tilde:             keyTranslation = XK_dead_grave;     break;
            case Key_Underscore:        keyTranslation = XK_underscore;     break;

            default: return BIT_FALSE;
        }




        // Get the case key code
        KeyCode Code = XKeysymToKeycode( m_pDisplay, keyTranslation );

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
