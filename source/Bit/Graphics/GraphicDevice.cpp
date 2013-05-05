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

#include <Bit/Graphics/GraphicDevice.hpp>

// Platform independent
#ifdef PLATFORM_WINDOWS
	#include <Bit/Graphics/Win32/GraphicDeviceWin32.hpp>
	typedef Bit::GraphicDeviceWin32 GraphicDevicePlatformType;
	#undef CreateWindow
#elif PLATFORM_LINUX
	//#include <Bit/Window/Linux/WindowLinux.hpp>
	typedef Bit::GraphicDeviceLinux GraphicDevicePlatformType;
#endif

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Public functions
	BIT_BOOL GraphicDevice::IsOpen( ) const
	{
		return m_Open;
	}

	// Get functions
	BIT_UINT32 GraphicDevice::GetDeviceType( ) const
	{
		return m_DeviceType;
	}


	// Create a cross platform renderer via this function
	GraphicDevice * CreateGraphicDevice( )
	{
		return new GraphicDevicePlatformType( );
	}

}