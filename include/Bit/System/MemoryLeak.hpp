#ifndef __BIT_MEMORY_LEAK_HPP__
#define __BIT_MEMORY_LEAK_HPP__

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
