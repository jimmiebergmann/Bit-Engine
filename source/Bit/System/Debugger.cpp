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

// Note:
// The original header was written by Stuart ( teh_programerer@interactivelaboratories.com )

#include <Bit/System/Debugger.hpp>
#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_WIN32
	#include <windows.h>
#elif defined( BIT_PLATFORM_LINUX )
	#define  _vsnprintf_s vsnprintf
	#include <iostream>
#endif

#include <cstdio>
#include <stdarg.h>
#include <Bit/System/MemoryLeak.hpp>


namespace Bit
{

	void BIT_API Trace( const char * p_pMessage, ... )
	{
		// No message
		if(p_pMessage == BIT_NULL)
		{
			return;
		}

		// Assemble the complete message
		char CompleteMessage[ 1024 ];
		BIT_UINT32 ReturnVal;
		va_list ArgPtr;

		va_start( ArgPtr, p_pMessage );
		ReturnVal = _vsnprintf_s( CompleteMessage, 1024, p_pMessage, ArgPtr );
		va_end( ArgPtr );

		// Output the message
		#ifdef BIT_PLATFORM_WIN32

			OutputDebugStringA( CompleteMessage );

		#elif defined( BIT_PLATFORM_LINUX )

			std::cout << CompleteMessage;

		#endif

	}

}
