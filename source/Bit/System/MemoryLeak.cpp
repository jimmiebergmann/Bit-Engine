#include <Bit/System/MemoryLeak.hpp>

#ifdef BUILD_DEBUG
#ifdef PLATFORM_WINDOWS

namespace Bit
{
	void BIT_API InitMemoryLeak( void * p_pParam)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
}

#endif
#endif