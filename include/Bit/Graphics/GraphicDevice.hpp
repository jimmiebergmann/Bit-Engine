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

#ifndef __BIT_GRAPHICS_GRAPHIC_DEVICE_HPP__
#define __BIT_GRAPHICS_GRAPHIC_DEVICE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Window/Window.hpp>

namespace Bit
{
	class BIT_API GraphicDevice
	{

	public:

		// Public enums
		enum eDevices
		{
			Device_Any = 0,			// Picking the most fitting and newest one.
			Device_OpenGL_2_1 = 1,	// OpenGL 2.1 context
			Device_OpenGL_3_1 = 2,	// OpenGL 3.1 context
		};


		// Constructors/destructors
		virtual ~GraphicDevice( ) { }

		// Public general functions
		virtual BIT_UINT32 Open( const Window & p_Window, const BIT_UINT32 p_Devices ) = 0;
		virtual BIT_UINT32 Close( ) = 0;
		virtual void Present( ) = 0;

		// Public functions
		BIT_BOOL IsOpen( ) const;

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits ) = 0;
		virtual void ClearColor( ) = 0;
		virtual void ClearDepth( ) = 0;

		// Create functions for different renderer elements
		// ..

		// Get functions
		BIT_UINT32 GetDeviceType( ) const;

		// Set functions
		virtual void SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A ) = 0;


	protected:

		// Protected functions
		BIT_BOOL m_Open;
		BIT_UINT32 m_DeviceType;
		Vector2_si32 m_ViewPortLow;
		Vector2_si32 m_ViewPortHigh;

	};

	// Create a cross platform renderer via this function
	BIT_API GraphicDevice * CreateGraphicDevice( );

}

#endif