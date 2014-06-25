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

#ifndef BIT_SYSTEM_MOUSE_LINUX_HPP
#define BIT_SYSTEM_MOUSE_LINUX_HPP

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Mouse.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <vector>

// Make sure to undefine Bool, defined in Xlib.h
#undef Bool

namespace Bit
{

    class BIT_API MouseLinux : public Private::MouseBase
    {

    public:

        	// Friend classes
		friend class RenderWindowLinux;

        // Constructor/destructor
        MouseLinux( );
        ~MouseLinux( );

        // Public general functions
        virtual void Update( );

        // Button translation function for platform keys
		virtual eButton TranslateButtonToBitButton( const Uint16 p_Button );
        virtual Uint16 TranslateButtonToSystemButton( const eButton p_Button );

        // Get state functions
		virtual Vector2i32 GetPosition( ) const;
		virtual Bool ButtonIsDown( const eButton p_Button );
		virtual Bool ButtonIsUp( const eButton p_Button );
		virtual Bool ButtonIsJustPressed( const eButton p_Button );
		virtual Bool ButtonIsJustReleased( const eButton p_Button );

    private:

		// Set state functions
		virtual Bool GetCurrentButtonState( const eButton p_Button );
		virtual Bool GetPreviousButtonState( const eButton p_Button );
        void SetCurrentButtonState( const eButton p_Button, const Bool p_State );
        void SetPreviousButtonState( const eButton p_Button, const Bool p_State );

        // Private functions
        Bool GetButtonStatus( const eButton p_Button );

        // Private variables
        ::Display * m_pDisplay;
        Bool m_CurrentButtonState[ ButtonCount ];					///< Current states of all buttons
        Bool m_PreviousButtonState[ ButtonCount ];					///< Previous states of all buttons
        std::vector< eButton > m_ChangedButtons;					///< Vector of all buttons that were changed since last Update( ) call.
        Uint16 m_ButtonTranslationsBitToWin32[ ButtonCount + 1 ];	///< Translation table from bit keys to win32 keys
        eButton m_ButtonTranslationsWin32ToBit[ ButtonCount + 2 ];	///< Translation table from system keys to bit keys

    };

}

#endif

#endif
