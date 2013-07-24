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
            Key_None = 0,
            Key_A,
            Key_B,
            Key_C,
            Key_Count
        };

        // Constructors/destructors
		virtual ~Keyboard( ) {}

        // Public general functions
        virtual void Update( ) = 0;

        // Key translation function for platform keys
        virtual eKey TranslateKey( const BIT_UINT32 p_Key ) = 0;

        // Get state functions
        virtual BIT_BOOL KeyIsDown( eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsUp( eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsJustPressed( eKey p_Key ) = 0;
        virtual BIT_BOOL KeyIsJustReleased( eKey p_Key ) = 0;

    };

    // Use this function for keyboard creation!
	// Function for Keyboard allocation
	BIT_API Keyboard * CreateKeyboard( );

}

#endif
