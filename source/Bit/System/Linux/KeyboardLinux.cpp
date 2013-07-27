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

        m_KeyTranslations[ Key_Alt ] = XK_Alt_L;
        m_KeyTranslations[ Key_Backspace ] = XK_BackSpace;
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
        m_KeyTranslations[ Key_Shift_L ] = XK_Shift_L;
        m_KeyTranslations[ Key_Shift_R ] = XK_Shift_R;
        m_KeyTranslations[ Key_Space ] = XK_space;
        m_KeyTranslations[ key_Super_L ] = XK_Super_L;
        m_KeyTranslations[ key_Super_R ] = XK_Super_R;
        m_KeyTranslations[ Key_Tab ] = XK_Tab;




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

       //     #define XK_Select                        0xff60  /* Select, mark */
      //      #define XK_Print                         0xff61
       //     #define XK_Execute                       0xff62  /* Execute, run, do */
      //      #define XK_Insert                        0xff63  /* Insert, insert here */
       //     #define XK_Undo                          0xff65
       //     #define XK_Redo                          0xff66  /* Redo, again */
      //      #define XK_Menu                          0xff67
      //      #define XK_Find                          0xff68  /* Find, search */
      //      #define XK_Cancel                        0xff69  /* Cancel, stop, abort, exit */
      //      #define XK_Help                          0xff6a  /* Help */
      //      #define XK_Break                         0xff6b
      //      #define XK_Mode_switch                   0xff7e  /* Character set switch */
     //       #define XK_script_switch                 0xff7e  /* Alias for mode_switch */
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
        // ..
        KeySym keyTranslation = m_KeyTranslations[ p_Key ];

        // Get the key code
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
