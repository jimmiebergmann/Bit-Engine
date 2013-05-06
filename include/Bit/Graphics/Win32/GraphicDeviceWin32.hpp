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


#ifndef __BIT_GRAPHICS_GRAPHIC_DEVICE_WIN32_HPP__
#define __BIT_GRAPHICS_GRAPHIC_DEVICE_WIN32_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Window/Win32/WindowWin32.hpp>

namespace Bit
{
	class BIT_API GraphicDeviceWin32 : public GraphicDevice
	{

	public:

		// Constructors/destructors
		GraphicDeviceWin32( );
		~GraphicDeviceWin32( );

		// Public general functions
		virtual BIT_UINT32 Open( const Window & p_Window, const BIT_UINT32 p_Devices );
		virtual BIT_UINT32 Close( );
		virtual void Present( );

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits );
		virtual void ClearColor( );
		virtual void ClearDepth( );

		// Create functions for different renderer elements
		// ..

		// Get functions

		// Set functions
		virtual void SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A );
		

	private:

		// Private variables
		::HGLRC m_Context;
		::HDC m_DeviceContext;


	};


}

#endif




