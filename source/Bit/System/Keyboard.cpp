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

#include <Bit/System/Keyboard.hpp>

// Platform independent
#ifdef BIT_PLATFORM_WIN32
	#include <Bit/System/Win32/keyboardWin32.hpp>
	typedef Bit::KeyboardWin32 KeyboardPlatformType;
	#undef CreateWindow
#elif defined( BIT_PLATFORM_LINUX )
	#include <Bit/System/Linux/KeyboardLinux.hpp>
	typedef Bit::KeyboardLinux KeyboardPlatformType;
#endif

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Function for Keyboard allocation
	BIT_API Keyboard * CreateKeyboard( )
	{
	    return new KeyboardPlatformType( );
	}

}
