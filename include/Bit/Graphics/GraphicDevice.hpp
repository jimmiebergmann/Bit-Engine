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
#include <Bit/Graphics/VertexObject.hpp>

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
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }

		// Create functions for different renderer elements
		virtual VertexObject * CreateVertexObject( ) const = 0;

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits ) = 0;
		virtual void ClearColor( ) = 0;
		virtual void ClearDepth( ) = 0;

		// Enable functions
		virtual void EnableTexture( ) = 0;
		virtual void EnableAlpha( ) = 0;
		virtual void EnableDepthTest( ) = 0;
		virtual void EnableStencilTest( ) = 0;
		virtual void EnableFaceCulling( BIT_UINT32 p_FaceCulling ) = 0;
		virtual void EnableSmoothLines( ) = 0;

		// Disable functions
		virtual void DisableTexture( ) = 0;
		virtual void DisableAlpha( ) = 0;
		virtual void DisableDepthTest( ) = 0;
		virtual void DisableStencilTest( ) = 0;
		virtual void DisableFaceCulling( ) = 0;
		virtual void DisableSmoothLines( ) = 0;

		// Set functions
		virtual void SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A ) = 0;
		virtual void SetClearDepth( BIT_FLOAT32 p_Depth ) = 0;
		virtual void SetClearStencil( BIT_UINT32 p_Stencil ) = 0;
		virtual void SetViewport( const BIT_UINT32 p_LX, const BIT_UINT32 p_LY,
			const BIT_UINT32 p_HX, const BIT_UINT32 p_HY ) = 0; // Lower and higher coordinates
		virtual void SetLineWidth( const BIT_FLOAT32 p_Width ) = 0;

		// Get functions(inlines)
		BIT_INLINE BIT_UINT32 GetDeviceType( ) const { return m_DeviceType; }
		BIT_INLINE BIT_BOOL GetTextureStatus( ) const { return m_TextureStatus; }
		BIT_INLINE BIT_BOOL GetAlphaStatus( ) const { return m_AlphaStatus; }
		BIT_INLINE BIT_BOOL GetDepthTestStatus( ) const { return m_DepthTestStatus; }
		BIT_INLINE BIT_BOOL GetStencilTestStatus( ) const { return m_StencilTestStatus; }
		BIT_INLINE BIT_BOOL GetFaceFullingStatus( ) const { return m_FaceCullingStatus; }
		BIT_INLINE BIT_UINT32 GetFaceCullingType( ) const { return m_FaceCullingType; }
		BIT_INLINE BIT_BOOL GetSmoothLinesStatus( ) const { return m_SmoothLinesStatus; }
		BIT_INLINE Vector2_si32 GetViewportSize( ) const { return m_ViewportHigh - m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportLow( ) const { return m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportHigh( ) const { return m_ViewportHigh; }

	protected:

		// Protected functions
		BIT_BOOL m_Open;
		BIT_UINT32 m_DeviceType;
		Vector2_si32 m_ViewportLow;
		Vector2_si32 m_ViewportHigh;

		// Enable / Disable statuses
		BIT_BOOL m_TextureStatus;
		BIT_BOOL m_AlphaStatus;
		BIT_BOOL m_DepthTestStatus;
		BIT_BOOL m_StencilTestStatus;
		BIT_BOOL m_FaceCullingStatus;
		BIT_UINT32 m_FaceCullingType;
		BIT_BOOL m_SmoothLinesStatus;

	};

	// Create a cross platform renderer via this function
	BIT_API GraphicDevice * CreateGraphicDevice( );

}

#endif