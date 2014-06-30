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

#ifndef BIT_GRAPHICS_OPENGL_GRAPHIC_DEVICE_LINUX_HPP
#define BIT_GRAPHICS_OPENGL_GRAPHIC_DEVICE_LINUX_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_LINUX


#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <Bit/Graphics/OpenGL/OpenGLFramebuffer.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/NonCopyable.hpp>


namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL Win32 Graphic device base class.
	///
	/// Default settings:
	///	 -	Depth test disabled.
	///	 -	Textures enabled.
	///	 -	Face culling disabled.
	///	 -	Smooth lines disabled.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLGraphicDeviceLinux : public GraphicDevice, NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		/// Initialize the graphic device with the Open function.
		///
		/// \see Open
		///
		////////////////////////////////////////////////////////////////
		OpenGLGraphicDeviceLinux( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_RenderOutput The output window.
		/// \param p_Version The version of the graphic device context
		///		that you would like to open.
		///		Using the newest as possible by default.
		///
		////////////////////////////////////////////////////////////////
		OpenGLGraphicDeviceLinux(	const RenderWindow & p_RenderOutput,
									const Version & p_Version = Version::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. Closing the device.
		///
		////////////////////////////////////////////////////////////////
		~OpenGLGraphicDeviceLinux( );

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the graphic device.
		///
		/// \param p_RenderOutput The output window.
		/// \param p_Version The version of the graphic device context
		///		that you would like to open.
		///		Using the newest as possible by default.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Open(	const RenderWindow & p_RenderOutput,
							const Version & p_Version = Version::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Close the graphic device.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( );

		////////////////////////////////////////////////////////////////
		/// \brief Making this graphic device to the current one.
		///
		/// This means that you can have multiple graphic devices
		/// for a single render output, for example a window.
		///
		////////////////////////////////////////////////////////////////
		virtual void MakeCurrent( );

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
		virtual void Present( );

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs color buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearColor( );

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs depth buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearDepth( );

		////////////////////////////////////////////////////////////////
		/// \brief Enable depth test.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableDepthTest( );

		////////////////////////////////////////////////////////////////
		/// \brief Enable textures.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableTexture( );

		////////////////////////////////////////////////////////////////
		/// \brief Enable multisampling
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableMultisampling( );

		////////////////////////////////////////////////////////////////
		/// \brief Enable face culling.
		///
		/// \param p_FaceCulling Front of back face culling.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableFaceCulling( eCulling p_FaceCulling );

		////////////////////////////////////////////////////////////////
		/// \brief Enable smooth lines.
		///
		/// For primitive mode Lines and LineStrip.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableSmoothLines( );

		////////////////////////////////////////////////////////////////
		/// \brief Disable depth test.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableDepthTest( );

		////////////////////////////////////////////////////////////////
		/// \brief Disable textures.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableTexture( );

		////////////////////////////////////////////////////////////////
		/// \brief Disable face culling.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableFaceCulling( );

		////////////////////////////////////////////////////////////////
		/// \brief Disable smooth lines.
		///
		/// For primitive mode Lines and LineStrip.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableSmoothLines( );

		////////////////////////////////////////////////////////////////
		/// \brief Create a framebuffer.
		///
		/// \return A pointer to the new framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Framebuffer * CreateFramebuffer( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a renderbuffer.
		///
		/// \return A pointer to the new renderbuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Renderbuffer * CreateRenderbuffer( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a vertex array.
		///
		/// \return A pointer to the VAO
		///
		////////////////////////////////////////////////////////////////
		virtual VertexArray * CreateVertexArray( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a vertex buffer.
		///
		/// \return A pointer to the VBO
		///
		////////////////////////////////////////////////////////////////
		virtual VertexBuffer * CreateVertexBuffer( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a shader
		///
		/// \param p_Type Enumerator of the shader type.
		///
		/// \return A pointer to the shader
		///
		////////////////////////////////////////////////////////////////
		virtual Shader * CreateShader( ShaderType::eType p_Type ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a shader program
		///
		/// \return A pointer to the shader program
		///
		////////////////////////////////////////////////////////////////
		virtual ShaderProgram * CreateShaderProgram( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a texture
		///
		/// \return A pointer to the new texture
		///
		////////////////////////////////////////////////////////////////
		virtual Texture * CreateTexture( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a model
		///
		/// \return A pointer to the new model.
		///
		/// \see CreateModelRenderer
		///
		////////////////////////////////////////////////////////////////
		virtual Model * CreateModel( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Create a model
		///
		/// \return A pointer to the new model.
		///
		/// \see CreateModel
		///
		////////////////////////////////////////////////////////////////
		virtual ModelRenderer * CreateModelRenderer( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Setting the viewport area.
		///
		/// \param p_Position The position of the viewport.
		/// \param p_Size The size of the viewport.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetViewport( const Vector2u32 & p_Position, const Vector2u32 & p_Size );

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
									const Uint8 p_Blue, const Uint8 p_Alpha );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the graphic device is open.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the context version
		///
		////////////////////////////////////////////////////////////////
		virtual Version GetVersion( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual const Framebuffer & GetDefaultFramebuffer( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default model renderer.
		///
		////////////////////////////////////////////////////////////////
		//virtual const ModelRenderer & GetDefaultModelRenderer( );

	private:

		// Private functions
		Bool OpenVersion( const RenderWindow & p_RenderOutput, const Version & p_Version );
		Bool OpenBestVersion( const RenderWindow & p_RenderOutput, Version & p_Version );

		// Private variables
		Bool m_Open;									///< Is the GD open.
		Version m_Version;								///< The version of the GD.
		RenderWindow * m_pRenderWindow;                 ///< Pointer to the render window
	    ::Display * m_pDisplay;                         ///< X11 display from the render window.
		::Window m_Window;                              ///< X11 window from the render window.
		::GLXContext m_DeviceContext;                   ///< The OpenGL context.
		::Colormap m_Colormap;                          ///< X11 colormap.
		static OpenGLFramebuffer s_DefaultFramebuffer;	///< Default framebuffer(static).
		//ModelRenderer * m_pDefaultModelRenderer;		///< Default model renderer.

	};

}

#endif

#endif
