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

#ifndef __BIT_SYSTEM_KEYBOARD_HPP__
#define __BIT_SYSTEM_KEYBOARD_HPP__

#include <Bit/DataTypes.hpp>

namespace Bit
{

    class BIT_API Keyboard
    {

    public:

        // Public enum
        enum eKey
        {
            // Alphabet keys
            Key_A,
            Key_B,
            Key_C,
            Key_D,
            Key_E,
            Key_F,
            Key_G,
            Key_H,
            Key_I,
            Key_J,
            Key_K,
            Key_L,
            Key_M,
            Key_N,
            Key_O,
            Key_P,
            Key_Q,
            Key_R,
            Key_S,
            Key_T,
            Key_U,
            Key_V,
            Key_W,
            Key_X,
            Key_Y,
            Key_Z,

            // Numeric keys
            Key_0,
            Key_1,
            Key_2,
            Key_3,
            Key_4,
            Key_5,
            Key_6,
            Key_7,
            Key_8,
            Key_9,

            // Function Keys
            Key_F1,
            Key_F2,
            Key_F3,
            Key_F4,
            Key_F5,
            Key_F6,
            Key_F7,
            Key_F8,
            Key_F9,
            Key_F10,
            Key_F11,
            Key_F12,

            // Arrow keys
            Key_Down,
            Key_Left,
            Key_Right,
            Key_Up,

            // System keys
            Key_A_Diaeresis,
            Key_A_Ring,
            Key_Alt_L,
            Key_Alt_R,
            Key_Ampersand,
            Key_Apostrophe,
            Key_Asterisk,
            Key_BackSlash,
            Key_Backspace,
            Key_Bar,
            Key_Brace_Left,
            Key_Brace_Right,
            Key_Bracket_L,
            Key_Bracket_R,
            Key_Break,
            Key_Capslock,
            Key_Colon,
            Key_Control_L,
            Key_Control_R,
            Key_Comma,
            Key_Delete,
            Key_Dollar,
            Key_End,
            Key_Equal,
            Key_Escape,
            Key_Exclam,
            Key_Greater,
            Key_Home,
            Key_Insert,
            Key_Less,
            Key_Minus,
            Key_Number,
            Key_NumLock,
            Key_O_Diaeresis,
            Key_PageDown,
            Key_PageUp,
            Key_Parenthesis_Left,
            Key_Parenthesis_Right,
            Key_Percent,
            Key_Period,
            Key_Plus,
            Key_Print,
            Key_Question,
            Key_Quote,
            Key_Return,
            Key_ScrollLock,
            Key_Section,
            Key_SemiColon,
            Key_Shift_L,
            Key_Shift_R,
            Key_Slash,
            Key_Space,
            key_Super_L,
            key_Super_R,
            Key_Tab,
            Key_Tilde,
            Key_Underscore,

/*
            // Media keys ( NOT IMPLEMENTED YET )
            Key_Mute,
            Key_Next,
            Key_Play,
            Key_Previous,
            Key_Stop,
            Key_VolumeHigh,
            Key_VolumeLow,
*/

            // Key counter (not a key)
            Key_Count,
            Key_None
        };

        // Constructors/destructors
		virtual ~Keyboard( ) {}

        // Public general functions
        virtual void Update( ) = 0;

        // Key translation function for platform keys
        virtual eKey TranslateKeyToBitKey( const BIT_UINT16 p_Key ) = 0;
        virtual BIT_UINT16 TranslateKeyToSystemKey( const eKey p_Key ) = 0;

        // Get state functions
        virtual BIT_BOOL KeyIsDown( const eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsUp( const eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsJustPressed( const eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsJustReleased( const eKey p_Key ) = 0;

    };

    // Use this function for keyboard creation!
	// Function for Keyboard allocation
	BIT_API Keyboard * CreateKeyboard( );

}

#endif
