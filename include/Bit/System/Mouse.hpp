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

#ifndef BIT_SYSTEM_MOUSE_HPP
#define BIT_SYSTEM_MOUSE_HPP

#include <Bit/DataTypes.hpp>

namespace Bit
{

    class BIT_API Mouse
    {

    public:

        // Public enum
        enum eButton
        {
            Button_1,
            Button_2,
            Button_3,

            Button_Count,
            Button_None
        };


        // Destructor
        virtual ~Mouse( ) { }

        // Public general functions
        virtual void Update( ) = 0;

        // Button translation function for platform keys
        virtual eButton TranslateButtonToBitKey( const BIT_UINT16 p_Button ) = 0;
        virtual BIT_UINT16 TranslateButtonToSystemKey( const eButton p_Button ) = 0;

        // Get state functions
        virtual BIT_BOOL ButtonIsDown( const eButton p_Button ) = 0;
        virtual BIT_BOOL ButtonIsUp( const eButton p_Button ) = 0;
        virtual BIT_BOOL ButtonIsJustPressed( const eButton p_Button ) = 0;
        virtual BIT_BOOL ButtonIsJustReleased( const eButton p_Button ) = 0;

    };

    // Use this function for mouse creation!
	// Function for mouse allocation
	BIT_API Mouse * CreateMouse( );

}

#endif
