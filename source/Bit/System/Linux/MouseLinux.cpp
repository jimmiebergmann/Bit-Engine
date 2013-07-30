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

#include <Bit/System/Linux/MouseLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

    // Global static varialbes
    const static BIT_UINT32 s_ButtonMask = 0x0700;


    // Constructor/destructor
    MouseLinux::MouseLinux( )
    {
        // Connect to X server
        m_pDisplay = XOpenDisplay( NULL );

        // Clear the bit button translations
        // NOT REQUIRED WHEN ALL THE BUTTON CODES ARE SET
        for( BIT_MEMSIZE i = 0; i < Mouse::Button_Count; i++ )
        {
            m_ButtonTranslationsBit[ i ] = 0;
        }

        // Clear the system button translations
        for( BIT_MEMSIZE i = 0; i < 8; i++ )
        {
            m_ButtonTranslationsSystem[ i ] = Button_None;
        }

        // Clear the button states
        for( BIT_MEMSIZE i = 0; i < Mouse::Button_Count; i++ )
        {
            m_CurrentButtonState[ i ] = BIT_FALSE;
            m_PreviousButtonState[ i ] = BIT_FALSE;
        }

        // Reserve the changed keys vector
        m_ChangedButtons.clear( );
        m_ChangedButtons.resize( s_ReservedButtonCount );


        // Set the BIT button translations
        m_ButtonTranslationsBit[ Button_1 ] = 0x0100;
        m_ButtonTranslationsBit[ Button_2 ] = 0x0200;
        m_ButtonTranslationsBit[ Button_3 ] = 0x0400;

        //  Set the SYSTEM button translations
        m_ButtonTranslationsSystem[ 1 ] = Button_1;
        m_ButtonTranslationsSystem[ 2 ] = Button_2;
        m_ButtonTranslationsSystem[ 3 ] = Button_3;
    }
    MouseLinux::~MouseLinux( )
    {
        // Close the connection with X server
        if( m_pDisplay )
        {
            XCloseDisplay( m_pDisplay );
        }
    }

     // Public general functions
    void MouseLinux::Update( )
    {
        if( m_pDisplay == BIT_NULL )
        {
            return;
        }

        for( BIT_MEMSIZE i = 0; i < m_ChangedButtons.size( ); i++ )
        {
           m_PreviousButtonState[ m_ChangedButtons[ i ] ] = m_CurrentButtonState[ m_ChangedButtons[ i ]  ];
        }
        m_ChangedButtons.clear( );
    }

    // Button translation function for platform keys
    Mouse::eButton MouseLinux::TranslateButtonToBitKey( const BIT_UINT16 p_Button )
    {
        return m_ButtonTranslationsSystem[ p_Button ];
    }

    BIT_UINT16 MouseLinux::TranslateButtonToSystemKey( const eButton p_Button )
    {
         return m_ButtonTranslationsBit[ static_cast< BIT_UINT16 >( p_Button ) ];
    }

    // Get state functions
    BIT_BOOL MouseLinux::ButtonIsDown( const eButton p_Button )
    {
        const BIT_BOOL ButtonState = GetButtonStatus( p_Button );

        if( m_CurrentButtonState[ p_Button ] != ButtonState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedButtons.size( ) < s_ReservedButtonCount )
            {
                m_ChangedButtons.push_back( p_Button );
            }
        }

        return ( m_CurrentButtonState[ p_Button ] = ButtonState );
    }

    BIT_BOOL MouseLinux::ButtonIsUp( const eButton p_Button )
    {
        const BIT_BOOL ButtonState = GetButtonStatus( p_Button );

        if( m_CurrentButtonState[ p_Button ] != ButtonState )
        {
           // Push back the reserved key if we have space for the changed key
            if( m_ChangedButtons.size( ) < s_ReservedButtonCount )
            {
                m_ChangedButtons.push_back( p_Button );
            }
        }

        return !( m_CurrentButtonState[ p_Button ] = ButtonState );
    }

    BIT_BOOL MouseLinux::ButtonIsJustPressed( const eButton p_Button )
    {
        const BIT_BOOL ButtonState = GetButtonStatus( p_Button );

        if( m_CurrentButtonState[ p_Button ] != ButtonState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedButtons.size( ) < s_ReservedButtonCount )
            {
                m_ChangedButtons.push_back( p_Button );
            }
        }

        return ( m_CurrentButtonState[ p_Button ] = ButtonState )  && !m_PreviousButtonState[ p_Button ];
    }

    BIT_BOOL MouseLinux::ButtonIsJustReleased( const eButton p_Button )
    {
         const BIT_BOOL ButtonState = GetButtonStatus( p_Button );

        if( m_CurrentButtonState[ p_Button ] != ButtonState )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedButtons.size( ) < s_ReservedButtonCount )
            {
                m_ChangedButtons.push_back( p_Button );
            }
        }

        return !( m_CurrentButtonState[ p_Button ] = ButtonState )  && m_PreviousButtonState[ p_Button ];
    }

    BIT_BOOL MouseLinux::GetCurrentButtonState( const eButton p_Button )
    {
        return m_CurrentButtonState[ p_Button ];
    }

    BIT_BOOL MouseLinux::GetPreviousButtonState( const eButton p_Button )
    {
         return m_PreviousButtonState[ p_Button ];
    }

    // Set state functions
    void MouseLinux::SetCurrentButtonState( const eButton p_Button, const BIT_BOOL p_State )
    {
         // Push the key to the changed key vector.
        if( m_CurrentButtonState[ p_Button ] != p_State )
        {
            // Push back the reserved key if we have space for the changed key
            if( m_ChangedButtons.size( ) < s_ReservedButtonCount )
            {
                m_ChangedButtons.push_back( p_Button );
            }
        }

        // Set the current state
        m_CurrentButtonState[ p_Button ] = p_State;
    }

    void MouseLinux::SetPreviousButtonState( const eButton p_Button, const BIT_BOOL p_State )
    {
        m_PreviousButtonState[ p_Button ] = p_State;
    }

    // Private functions
    BIT_BOOL MouseLinux::GetButtonStatus( const eButton p_Button )
    {
        // Required variables for the XQueryPointer function
        ::Window RootReturn, ChildReturn;
        int x, y, wx, wy;
        unsigned int Buttons;

        // Get the pointer states
        XQueryPointer( m_pDisplay, DefaultRootWindow( m_pDisplay ), &RootReturn, &ChildReturn, &x, &y, &wx, &wy, &Buttons );

        // Create a button mask
        BIT_UINT32 ButtonMask = Buttons & s_ButtonMask;

        // Make sure that any button is pressed
        if( !ButtonMask )
        {
            return BIT_FALSE;
        }

        // Bit mask once again with the translation map.
        return ( ButtonMask & m_ButtonTranslationsBit[ p_Button ] );
    }

}

#endif
