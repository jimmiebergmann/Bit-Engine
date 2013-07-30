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

#ifndef __BIT_SYSTEM_MOUSE_LINUX_HPP__
#define __BIT_SYSTEM_MOUSE_LINUX_HPP__

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Mouse.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <vector>

namespace Bit
{

    class BIT_API MouseLinux : public Mouse
    {

    public:

        // Constructor/destructor
        MouseLinux( );
        virtual ~MouseLinux( );

        // Public general functions
        virtual void Update( );

        // Button translation function for platform keys
        virtual eButton TranslateButtonToBitKey( const BIT_UINT16 p_Button );
        virtual BIT_UINT16 TranslateButtonToSystemKey( const eButton p_Button );

        // Get state functions
        virtual BIT_BOOL ButtonIsDown( eButton p_Button );
        virtual BIT_BOOL ButtonIsUp( eButton p_Button );
        virtual BIT_BOOL ButtonIsJustPressed( eButton p_Button );
        virtual BIT_BOOL ButtonIsJustReleased( eButton p_Button );

    private:

        // Private functions
        BIT_BOOL GetButtonStatus( const eButton p_Button );

        // Private variables
        ::Display * m_pDisplay;
        BIT_BOOL m_CurrentButtonState[ Button_Count ];
        BIT_BOOL m_PreviousButtonState[ Button_Count ];
        std::vector< eButton > m_ChangedButtons;
        BIT_UINT32 m_ButtonTranslationsBit[ Button_Count ];
        eButton m_ButtonTranslationsSystem[ 8 ];
      /*  KeySym m_KeyTranslationsBit[ Key_Count ];
        eKey m_KeyTranslationsSystem[ 65536 ];
*/

        // Private statics
        static const BIT_UINT32 s_ReservedButtonCount = 16;

    };

}

#endif

#endif
