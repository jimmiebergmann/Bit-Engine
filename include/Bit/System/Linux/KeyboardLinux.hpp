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

#ifndef BIT_SYSTEM_KEYBOARD_LINUX_HPP
#define BIT_SYSTEM_KEYBOARD_LINUX_HPP

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Keyboard.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <vector>

namespace Bit
{

    class BIT_API KeyboardLinux : public Keyboard
    {

    public:

        // Constructor/destructor
        KeyboardLinux( );
        virtual ~KeyboardLinux( );

         // Public general functions
        virtual void Update( );

        // Key translation function for platform keys
        virtual eKey TranslateKeyToBitKey( const BIT_UINT16 p_Key );
        virtual BIT_UINT16 TranslateKeyToSystemKey( const eKey p_Key );

        // Get state functions
        virtual BIT_BOOL KeyIsDown( const eKey p_Key );
        virtual BIT_BOOL KeyIsUp( const eKey p_Key );
        virtual BIT_BOOL KeyIsJustPressed( const eKey p_Key );
        virtual BIT_BOOL KeyIsJustReleased( const eKey p_Key );
        BIT_BOOL GetCurrentKeyState( const eKey p_Key );
        BIT_BOOL GetPreviousKeyState( const eKey p_Key );

        // Set state functions
        void SetCurrentKeyState( const eKey p_Key, const BIT_BOOL p_State );
        void SetPreviousKeyState( const eKey p_Key, const BIT_BOOL p_State );

        // Public static varialbes
        static const BIT_UINT32 s_ReservedKeyCount = 64;

    private:

        // Private functions
        BIT_BOOL GetKeyStatus( const eKey p_Key );

        // Private variables
        ::Display * m_pDisplay;
        BIT_BOOL m_CurrentKeyState[ Key_Count ];
        BIT_BOOL m_PreviousKeyState[ Key_Count ];
        std::vector< eKey > m_ChangedKeys;
        KeySym m_KeyTranslationsBit[ Key_Count ];
        eKey m_KeyTranslationsSystem[ 65536 ];

    };

}

#endif

#endif
