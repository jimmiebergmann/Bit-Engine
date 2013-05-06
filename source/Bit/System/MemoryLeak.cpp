#include <Bit/System/MemoryLeak.hpp>

#ifdef BIT_BUILD_DEBUG
#ifdef BIT_PLATFORM_WIN32

namespace Bit
{
	BIT_API void InitMemoryLeak( void * p_pParam)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
}

#endif
#endif