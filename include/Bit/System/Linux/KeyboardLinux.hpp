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

#include <Bit/Build.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/keysym.h>
#undef Bool
#undef None
#include <Bit/System/KeyboardBase.hpp>
#include <vector>


namespace Bit
{

    class BIT_API KeyboardLinux : public Private::KeyboardBase
    {

    public:

        // Friend classes
		friend class RenderWindowLinux;

		// Constructor/destructor
		KeyboardLinux( );
		~KeyboardLinux( );

		// Public general functions
		virtual void Update( );

		// Key translation function for platform keys
		virtual eKey TranslateKeyToBitKey( const Uint16 p_Key );
		virtual Uint16 TranslateKeyToSystemKey( const eKey p_Key );

		// Get state functions
		virtual Bool KeyIsDown( const eKey p_Key );
		virtual Bool KeyIsUp( const eKey p_Key );
		virtual Bool KeyIsJustPressed( const eKey p_Key );
		virtual Bool KeyIsJustReleased( const eKey p_Key );

	private:

		// Set state functions
		virtual Bool GetCurrentKeyState( const eKey p_Key );
		virtual Bool GetPreviousKeyState( const eKey p_Key );
		virtual void SetCurrentKeyState( const eKey p_key, const Bool p_State );
        virtual void SetPreviousKeyState( const eKey p_key, const Bool p_State );

		// Private functions
        Bool GetKeyStatus( const eKey p_key );

		// Private variables
		::Display * m_pDisplay;
		Bool m_CurrentKeyState[ KeyCount ];
        Bool m_PreviousKeyState[ KeyCount ];
        std::vector< eKey > m_ChangedKeys;

		// MAKE STATIC
		Uint16 m_KeyTranslationsBitToSystem[ KeyCount ];
        eKey m_KeyTranslationsSystemToBit[ 65536 ];

    };

}

#endif

#endif
