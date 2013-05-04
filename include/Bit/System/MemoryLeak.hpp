#ifndef __BIT_MEMORY_LEAK_HPP__
#define __BIT_MEMORY_LEAK_HPP__

#ifdef BUILD_DEBUG
    #ifdef PLATFORM_WINDOWS

        #include <stdlib.h>
        #include <crtdbg.h>

        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

        namespace Bit
        {
            void InitMemoryLeak( void * p_pParam);
        }

        #define bitInitMemoryLeak Bit::InitMemoryLeak

    #else
        #define bitInitMemoryLeak sizeof
    #endif
#else
    #define bitInitMemoryLeak sizeof
#endif


#endif
