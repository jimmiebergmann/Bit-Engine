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

#include <Bit/Graphics/OpenGL/OpenGLFramebuffer.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static const GLenum g_OpenGLFrambufferAttachments[ 4 ] =
	{
		/* Depth */			GL_DEPTH_ATTACHMENT,
		/* DepthStencil */	GL_DEPTH_STENCIL_ATTACHMENT,
		/* Rgb */			GL_COLOR_ATTACHMENT0,
		/* Rgba */			GL_COLOR_ATTACHMENT0
	};

	// Constructor/destructor
	OpenGLFramebuffer::OpenGLFramebuffer( ) :
		m_FramebufferObject( 0 ),
		m_ColorAttachmentCount( 0 ),
		m_DepthAttached( false )
	{
	}

	OpenGLFramebuffer::~OpenGLFramebuffer( )
	{
		// Delete the framebuffer object
		if( m_FramebufferObject )
		{
			glDeleteFramebuffers( 1, &m_FramebufferObject );
		}
	}

	void OpenGLFramebuffer::Bind( )
	{
		// Bind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, m_FramebufferObject );
	}

	void OpenGLFramebuffer::Unbind( )
	{
		// Unbind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	Bool OpenGLFramebuffer::Attach( const Texture & p_pTexture )
	{
		// Cast the texture pointer to an opengl texture
		const OpenGLTexture * pOpenGLTexture =
			reinterpret_cast< const OpenGLTexture * >( &p_pTexture );

		// Get the format
		Texture::ePixelFormat format = pOpenGLTexture->GetPixelFormat( );

		// Get the right attachment type.
		GLenum attachment = g_OpenGLFrambufferAttachments[ format ];

		// Incement the attachment depending on the type
		if( attachment == GL_COLOR_ATTACHMENT0 )
		{
			// FIX THIS ERROR CHECK!
			if( m_ColorAttachmentCount > 15 )
			{
				std::cout << "[OpenGLFramebuffer::Attach(Texture)] Attached too many color attachments.\n";
				return false;
			}

			// Increment the attachment and the color attachment counter
			attachment += m_ColorAttachmentCount;
			m_ColorAttachmentCount++;
		}
		else if( attachment == GL_DEPTH_ATTACHMENT || attachment == GL_DEPTH_STENCIL_ATTACHMENT )
		{
			if( m_DepthAttached )
			{
				std::cout << "[OpenGLFramebuffer::Attach(Texture)] Attached too many depth/stencil attachments.\n";
				return false;
			}

			// Increment the depth attachment counter
			m_DepthAttached = true;
		}

		// Generate the framebuffer object if necessary
		if( m_FramebufferObject == 0 )
		{
			glGenFramebuffers( 1, &m_FramebufferObject );
		}

		// Bind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, m_FramebufferObject );

		// Attach the texture
		glFramebufferTexture2D( GL_FRAMEBUFFER, attachment,
								GL_TEXTURE_2D, pOpenGLTexture->GetId( ), 0 );

		// Check the framebuffer status!
		GLenum FramebufferStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if( FramebufferStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );

			std::cout << "[OpenGLFramebuffer::Attach(Texture)] Framebuffer error.\n";
			return false;
		}

		// Unbind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

		return true;
	}

	Bool OpenGLFramebuffer::Attach( const RenderBuffer & p_pRenderBuffer )
	{
		return false;
	}

}
