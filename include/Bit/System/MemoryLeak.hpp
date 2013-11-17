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

// This is for Win32 only

#ifndef BIT_MEMORY_LEAK_HPP
#define BIT_MEMORY_LEAK_HPP

#include <Bit/DataTypes.hpp>

#ifdef BIT_BUILD_DEBUG
    #ifdef BIT_PLATFORM_WIN32

        #include <stdlib.h>
        #include <crtdbg.h>

        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

        namespace Bit
        {
            BIT_API void InitMemoryLeak( void * p_pParam);
        }

        #define bitInitMemoryLeak Bit::InitMemoryLeak

    #else
        #define bitInitMemoryLeak sizeof
    #endif
#else
    #define bitInitMemoryLeak sizeof
#endif


#endif
