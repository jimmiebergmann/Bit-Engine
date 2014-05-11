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

#ifndef BIT_GRAPHICS_OPENGL_FRAMEBUFFER_HPP
#define BIT_GRAPHICS_OPENGL_FRAMEBUFFER_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/Framebuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLTexture.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL frambuffer class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLFramebuffer : public Framebuffer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		OpenGLFramebuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~OpenGLFramebuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Bind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( );

		///////////////////////////////////////////////////////////////
		/// \brief Unbind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind( );

		///////////////////////////////////////////////////////////////
		/// \brief Attach a texture to the framebuffer.
		///
		/// The attachment type depends on the pixel format used.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const Texture & p_pTexture );

		///////////////////////////////////////////////////////////////
		/// \brief Attach a render buffer to the framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const RenderBuffer & p_pRenderBuffer );

		///////////////////////////////////////////////////////////////
		/// \brief Get OpenGL framebuffer ID.
		///
		////////////////////////////////////////////////////////////////
		GLuint GetId( ) const;

	private:

		GLuint m_FramebufferObject;		///< OpenGL framebuffer object.
		Uint32 m_ColorAttachmentCount;	///< Number of attached color textures/render buffers.
		Bool m_DepthAttached;			///< Indicates if a depth texture/render buffer is attached.

	};

}

#endif
