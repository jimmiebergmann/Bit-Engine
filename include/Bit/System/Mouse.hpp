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

#ifndef __BIT_SYSTEM_MOUSE_HPP__
#define __BIT_SYSTEM_MOUSE_HPP__

#include <Bit/DataTypes.hpp>

namespace Bit
{

    class BIT_API Mouse
    {

    public:

        // Public enum

        #ifdef BIT_PLATFORM_WIN32

            enum eButton
            {
               Button_1 = 1,
               Button_2 = 2,
               Button_3 = 3
            };

         #elif defined( BIT_PLATFORM_LINUX )

            enum eButton
            {
               Button_1 = 1,
               Button_2 = 2,
               Button_3 = 3
            };

        #endif


        // Constructor
        Mouse( );

        // Get state functions
        BIT_BOOL ButtonIsDown( eButton p_Button );
        BIT_BOOL ButtonIsUp( eButton p_Button );
        BIT_BOOL ButtonIsJustPressed( eButton p_Button );
        BIT_BOOL ButtonIsJustReleased( eButton p_Button );

    private:



    };

}

#endif
