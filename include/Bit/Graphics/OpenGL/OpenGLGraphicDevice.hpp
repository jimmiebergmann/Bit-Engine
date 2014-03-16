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

#include <Bit/Build.hpp>

#ifdef BIT_PLATFORM_WINDOWS
	#include <Bit/Graphics/OpenGL/Win32/OpenGLGraphicDeviceWin32.hpp>
#elif BIT_PLATFORM_LINUX
	#include <Bit/Graphics/OpenGL/Linux/OpenGLGraphicDeviceLinux.hpp>
#endif

namespace Bit
{
	#ifdef BIT_PLATFORM_WINDOWS
		typedef OpenGLGraphicDeviceWin32 OpenGLGraphicDevice;
	#elif BIT_PLATFORM_LINUX
		#error No render window is available for this platform.
	#endif

};
