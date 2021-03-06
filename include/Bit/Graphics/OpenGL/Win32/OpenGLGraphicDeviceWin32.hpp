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

#ifndef BIT_GRAPHICS_OPENGL_GRAPHIC_DEVICE_WIN32_HPP
#define BIT_GRAPHICS_OPENGL_GRAPHIC_DEVICE_WIN32_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS

#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/Graphics/OpenGL/OpenGLFramebuffer.hpp>

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
	class BIT_API OpenGLGraphicDeviceWin32 : public GraphicDevice, NonCopyable
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
		OpenGLGraphicDeviceWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_RenderOutput The output window.
		/// \param p_Version The version of the graphic device context
		///		that you would like to open.
		///		Using the newest as possible by default.
		///
		////////////////////////////////////////////////////////////////
		OpenGLGraphicDeviceWin32(	const RenderWindow & p_RenderOutput,
									const Version & p_Version = Version::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. Closing the device.
		///
		////////////////////////////////////////////////////////////////
		~OpenGLGraphicDeviceWin32( );

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
		/// \brief Making this graphic device  NOT to the current one(unbind).
		///
		/// This means that you can have multiple graphic devices
		/// for a single render output, for example a window.
		///
		////////////////////////////////////////////////////////////////
		virtual void MakeNotCurrent();

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
		/// \brief Clear selected buffer. The selected buffer have to be
		/// set via the Framebuffer::SetOutputBuffers function.
		///
		////////////////////////////////////////////////////////////////
		virtual void Clear(	const Framebuffer::eAttachment & p_Attachment,
							const SizeType p_Index,
							const Vector4f32 p_Value);

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
		/// \brief Enable/disable depth test.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetDepthTestStatus(const Bool p_Enabled);

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable texturing.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetTexturingStatus(const Bool p_Enabled);

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable blending.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetBlendingStatus(const Bool p_Enabled);

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable multisampling.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetMultisamplingStatus(const Bool p_Enabled);

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable face culling.
		///
		/// \param p_FaceCulling Used if p_Enabled is true.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetFaceCullingStatus(const Bool p_Enabled, const eCulling p_FaceCulling = eCulling::BackFace);

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable smooth lines.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetSmoothLinesStatus(const Bool p_Enabled);

		////////////////////////////////////////////////////////////////
		/// \brief Check if depth test is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetDepthTestStatus();

		////////////////////////////////////////////////////////////////
		/// \brief Check if texturing is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetTexturingStatus();

		////////////////////////////////////////////////////////////////
		/// \brief Check if blending is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetBlendingStatus();

		////////////////////////////////////////////////////////////////
		/// \brief Check if multisampling is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetMultisamplingStatus();

		////////////////////////////////////////////////////////////////
		/// \brief Check if face culling is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetFaceCullingStatus();

		////////////////////////////////////////////////////////////////
		/// \brief Get face culling mode.
		///
		////////////////////////////////////////////////////////////////
		virtual eCulling GetFaceCullingMode();

		////////////////////////////////////////////////////////////////
		/// \brief Check if smooth lines is enabled/disabled.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetSmoothLinesStatus();

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
		/// \brief Create a bloom post-processing effect.
		///
		/// \return A pointer to the new post-process.
		///
		////////////////////////////////////////////////////////////////
		virtual PostProcessingBloom * CreatePostProcessingBloom( ) const;

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
		/// \brief Get the default framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual ShaderProgram * GetDefaultShaderProgram( const eDefaultShaders p_DefaultShader ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default model properties.
		///
		////////////////////////////////////////////////////////////////
		virtual DefaultModelSettings & GetDefaultModelSettings( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the default texture properties.
		///
		////////////////////////////////////////////////////////////////
		virtual TextureProperties & GetDefaultTextureProperties( );

	private:

		// Private functions
		Bool OpenVersion( const RenderWindow & p_RenderOutput, const Version & p_Version );
		Bool OpenBestVersion( const RenderWindow & p_RenderOutput, Version & p_Version );
		Bool LoadDefaultShaders( );
		Bool UnloadDefaultShaders( );
		void LoadDefaultModelSettings( );

		// Private variables
		Bool m_Open;									///< Is the GD open.
		Version m_Version;								///< The version of the GD.
		HDC m_DeviceContextHandle;						///< Device context handle from the render output.
		HGLRC m_Context;								///< The OpenGL context.
		static OpenGLFramebuffer s_DefaultFramebuffer;	///< Default framebuffer(static).
		ShaderProgram * m_pDefaultShaderPrograms[ 3 ];	///< Default shader programs, for example model shaders.
		Shader * m_pDefaultModelFragmentShader;			///< Default model fragment shader.
		Shader * m_pDefaultModelVertexShaders[ 3 ];		///< Default model vertex shaders.
		DefaultModelSettings m_DefaultModelSettings;	///< Default model settings.
		TextureProperties m_DefaultTextureProperties;	///< Default texture properties.

		// Enable/disable statuses
		Bool m_DepthTestStatus;
		Bool m_TextuingStatus;
		Bool m_BlendingStatus;
		Bool m_MultisamplingStatus;
		Bool m_FaceCullingStatus;
		Bool m_SmoothLinesStatus;
		eCulling m_FaceCullingMode;

	};

}

#endif

#endif