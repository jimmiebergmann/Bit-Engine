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

#ifndef __BIT_GRAPHICS_GRAPHIC_DEVICE_LINUX_HPP__
#define __BIT_GRAPHICS_GRAPHIC_DEVICE_LINUX_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Window/Linux/WindowLinux.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>


namespace Bit
{
	class BIT_API GraphicDeviceLinux : public GraphicDevice
	{

	public:

		// Constructors/destructors
		GraphicDeviceLinux( );
		~GraphicDeviceLinux( );

		// Public general functions
		virtual BIT_UINT32 Open( const Window & p_Window, const BIT_UINT32 p_Devices );
		virtual BIT_UINT32 Close( );
		virtual void Present( );

		// Create functions for different renderer elements
		virtual VertexObject * CreateVertexObject( ) const;
		virtual ShaderProgram * CreateShaderProgram( ) const;
		virtual Shader * CreateShader( const Shader::eShaderType p_ShaderType ) const;
		virtual Texture * CreateTexture( ) const;

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits );
		virtual void ClearColor( );
		virtual void ClearDepth( );

		// Enable functions
		virtual void EnableTexture( );
		virtual void EnableAlpha( );
		virtual void EnableDepthTest( );
		virtual void EnableStencilTest( );
		virtual void EnableFaceCulling( BIT_UINT32 p_FaceCulling );
		virtual void EnableSmoothLines( );

		// Disable functions
		virtual void DisableTexture( );
		virtual void DisableAlpha( );
		virtual void DisableDepthTest( );
		virtual void DisableStencilTest( );
		virtual void DisableFaceCulling( );
		virtual void DisableSmoothLines( );

		// Set functions
		virtual void SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A );
		virtual void SetClearDepth( BIT_FLOAT32 p_Depth );
		virtual void SetClearStencil( BIT_UINT32 p_Stencil );
		virtual void SetViewport( const BIT_UINT32 p_LX, const BIT_UINT32 p_LY,
			const BIT_UINT32 p_HX, const BIT_UINT32 p_HY ); // Lower and higher coordinates
		virtual void SetLineWidth( const BIT_FLOAT32 p_Width );

		// Get functions
		// ...


	private:

        // Private variables from the window class
        WindowLinux * m_pWindowLinux;
	    ::Display * m_pDisplay;
		::Window m_Window;

		// Private variables in general
		::GLXContext m_DeviceContext;
		::Colormap m_Colormap;

	};


}

#endif




