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

    }

    // Key translation function for platform keys
    Keyboard::eKey KeyboardLinux::TranslateKey( const BIT_UINT32 p_Key )
    {
        return Keyboard::Key_None;
    }

    // Get state functions
    BIT_BOOL KeyboardLinux::KeyIsDown( eKey p_Key )
    {
        return GetKeyStatus( p_Key );
    }

    BIT_BOOL KeyboardLinux::KeyIsUp( eKey p_Key )
    {
        return !GetKeyStatus( p_Key );
    }

    BIT_BOOL KeyboardLinux::KeyIsJustPressed( eKey p_Key )
    {
        return BIT_FALSE;
    }

    BIT_BOOL KeyboardLinux::KeyIsJustReleased( eKey p_Key )
    {
        return BIT_FALSE;
    }

    // Private functions
    BIT_BOOL KeyboardLinux::GetKeyStatus( eKey p_Key )
    {
        // Make sure that the connection to X server is established.
        if( m_pDisplay == BIT_NULL )
        {
            return BIT_FALSE;;
        }

        // Transalte the eKey to a KeySym
        // ..
        KeySym Key = XK_A;

        // Get the key code
        KeyCode Code = XKeysymToKeycode( m_pDisplay, Key );

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
