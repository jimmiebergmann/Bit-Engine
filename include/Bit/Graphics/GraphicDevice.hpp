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

#ifndef BIT_GRAPHICS_GRAPHIC_DEVICE_HPP
#define BIT_GRAPHICS_GRAPHIC_DEVICE_HPP

#include <Bit/Build.hpp>
#include <Bit/Window/RenderWindow.hpp>
/*#include <Bit/Window/Window.hpp>
#include <Bit/Graphics/Shader.hpp>
#include <Bit/Graphics/Model.hpp>
*/
namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Graphic device base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API GraphicDevice
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Version struct
		///
		////////////////////////////////////////////////////////////////
		struct Version
		{
			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			/// \param p_Major The major version number.
			/// \param p_Minor The minor version number.
			///
			////////////////////////////////////////////////////////////////
			Version( const Uint32 p_Major, Uint32 p_Minor );

			////////////////////////////////////////////////////////////////
			/// \brief Equal to operator
			///
			////////////////////////////////////////////////////////////////
			bool operator == ( const Version & p_Version ) const;

			// Public variables
			Uint32 Major;	///< Major version
			Uint32 Minor;	///< Minor version
		};

		////////////////////////////////////////////////////////////////
		/// \brief Static instance of the default version.
		///
		////////////////////////////////////////////////////////////////
		static Version DefaultVersion;

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~GraphicDevice( );

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the graphic device.
		///
		/// \param p_RenderOutput The output window.
		/// \param p_Version The version of the graphic device context
		///		that you would like to open.
		///
		////////////////////////////////////////////////////////////////
		virtual bool Open( const RenderWindow & p_RenderOutput, const Version & p_Version ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Close the graphic device.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Making this graphic device to the current one.
		///
		/// This means that you can have multiple graphic devices
		/// for a single render output, for example a window.
		///
		////////////////////////////////////////////////////////////////
		virtual void MakeCurrent( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Present the graphics, swap buffers.
		///
		/// Call this function when you want to swap the color buffers.
		/// This function wont clear the buffers.
		///
		/// \see ClearColor
		/// \see ClearDepth
		///
		////////////////////////////////////////////////////////////////
		virtual void Present( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs color buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearColor( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs depth buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearDepth( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Setting the viewport area.
		///
		/// \param p_Position The position of the viewport.
		/// \param p_Size The size of the viewport.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetViewport( const Vector2u32 & p_Position, const Vector2u32 & p_Size ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Setting the clear color.
		///
		/// All params are in the range of 0 to 255.
		///
		/// \param p_Red Red color component.
		/// \param p_Green Green color component.
		/// \param p_Blue Blue color component.
		/// \param p_Alpha Alpha component.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetClearColor( const Uint8 p_Red, const Uint8 p_Green,
									const Uint8 p_Blue, const Uint8 p_Alpha ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the graphic device is open.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the context version
		///
		////////////////////////////////////////////////////////////////
		virtual Version GetVersion( ) const = 0;

	};

	/*
	// Forward declarations
	class Framebuffer;
	class VertexObject;
	class ShaderProgram;
	class Texture;
	class PostProcessingBloom;

	
	// Graphic Device class
	class BIT_API GraphicDevice
	{

	public:

		// Public enums
		enum eDevice
		{
			Device_Any = 0,			// Picking the most fitting and newest one.
			Device_OpenGL_2_1 = 1,	// OpenGL 2.1 context
			Device_OpenGL_3_1 = 2,	// OpenGL 3.1 context
		};

		enum eCulling
		{
			Culling_FrontFace = 0,
			Culling_BackFace = 1
		};


		// Constructors/destructors
		virtual ~GraphicDevice( ) { }

		// Public general functions
		virtual BIT_UINT32 Open( const Window & p_Window, const BIT_UINT32 p_Devices ) = 0;
		virtual BIT_UINT32 Close( ) = 0;
		virtual void Present( ) = 0;
		virtual void BindDefaultFramebuffer( ) = 0;
		virtual void BindDefaultShaderProgram( ) = 0;

		// Public functions
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }

		// Create functions for different renderer elements
		virtual Framebuffer * CreateFramebuffer( ) const = 0;
		//virtual Renderbuffer * CreateRenderbuffer( ) const = 0;
		virtual VertexObject * CreateVertexObject( ) const = 0;
		virtual ShaderProgram * CreateShaderProgram( ) const = 0;
		virtual Shader * CreateShader( const Shader::eShaderType p_ShaderType ) const = 0;
		virtual Texture * CreateTexture( ) const = 0;
		virtual Model * CreateModel( Model::eModelType p_Type ) const = 0;
		virtual PostProcessingBloom * CreatePostProcessingBloom( VertexObject * p_pVertexObject, Texture * p_pTexture ) = 0;

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits ) = 0;
		virtual void ClearColor( ) = 0;
		virtual void ClearDepth( ) = 0;

		// Enable functions
		virtual void EnableTexture( ) = 0;
		virtual void EnableAlpha( ) = 0;
		virtual void EnableDepthTest( ) = 0;
		virtual void EnableStencilTest( ) = 0;
		virtual void EnableFaceCulling( eCulling p_FaceCulling ) = 0;
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
		BIT_INLINE BIT_BOOL GetFaceCullingStatus( ) const { return m_FaceCullingStatus; }
		BIT_INLINE BIT_UINT32 GetFaceCullingType( ) const { return m_FaceCullingType; }
		BIT_INLINE BIT_BOOL GetSmoothLinesStatus( ) const { return m_SmoothLinesStatus; }
		BIT_INLINE Vector2_si32 GetViewportSize( ) const { return m_ViewportHigh - m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportLow( ) const { return m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportHigh( ) const { return m_ViewportHigh; }

	protected:

		// Protected functions
		BIT_BOOL m_Open;
		eDevice m_DeviceType;
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
*/
}

#endif
