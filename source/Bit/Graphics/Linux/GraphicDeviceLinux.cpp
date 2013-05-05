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

#include <Bit/Graphics/Linux/GraphicDeviceLinux.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{


	// Constructors/destructors
	GraphicDeviceLinux::GraphicDeviceLinux( )
	{
	}

	GraphicDeviceLinux::~GraphicDeviceLinux( )
	{
	}


	// Public general functions
	BIT_UINT32 GraphicDeviceLinux::Open( const Window & p_Window, const BIT_UINT32 p_Devices )
	{
		return BIT_ERROR;
	}

	BIT_UINT32 GraphicDeviceLinux::Close( )
	{
		return BIT_ERROR;
	}

	void GraphicDeviceLinux::Present( )
	{

	}

	// Clear functions
	void GraphicDeviceLinux::ClearBuffers( const BIT_UINT32 p_ClearBits )
	{

	}

	void GraphicDeviceLinux::ClearColor( )
	{

	}

	void GraphicDeviceLinux::ClearDepth( )
	{

	}

	// Create functions for different renderer elements
	// ..


	// Get functions


	// Set functions



}
