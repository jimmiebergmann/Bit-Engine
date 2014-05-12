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
#include <Bit/Graphics/OpenGL/OpenGLRenderbuffer.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	OpenGLFramebuffer OpenGLFramebuffer::s_DefaultFramebuffer;

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
								pOpenGLTexture->GetTarget( ), pOpenGLTexture->GetId( ), 0 );

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

	Bool OpenGLFramebuffer::Attach( const Renderbuffer & p_pRenderBuffer )
	{
		// Cast the texture pointer to an opengl texture
		const OpenGLRenderbuffer * pOpenGLRenderbuffer =
			reinterpret_cast< const OpenGLRenderbuffer * >( &p_pRenderBuffer );

		// Get the format
		Texture::ePixelFormat format = pOpenGLRenderbuffer->GetPixelFormat( );

		// Get the right attachment type.
		GLenum attachment = g_OpenGLFrambufferAttachments[ format ];

		// Incement the attachment depending on the type
		if( attachment == GL_COLOR_ATTACHMENT0 )
		{
			// FIX THIS ERROR CHECK!
			if( m_ColorAttachmentCount > 15 )
			{
				std::cout << "[OpenGLFramebuffer::Attach(Renderbuffer)] Attached too many color attachments.\n";
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
				std::cout << "[OpenGLFramebuffer::Attach(Renderbuffer)] Attached too many depth/stencil attachments.\n";
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
		
		// Attach the render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, pOpenGLRenderbuffer->GetId( ) );
		
		// Check the framebuffer status!
		GLenum FramebufferStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if( FramebufferStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );

			std::cout << "[OpenGLFramebuffer::Attach(Renderbuffer)] Framebuffer error.\n";
			return false;
		}

		// Unbind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

		// Succeeded
		return true;
	}

	void OpenGLFramebuffer::Blit(	const Framebuffer & p_Destination,
									const eAttachment p_Attachment,
									const Vector2u32 p_DestinationPosition,
									const Vector2u32 p_DestinationSize,
									const Vector2u32 p_SourcePosition,
									const Vector2u32 p_SourceSize ) const
	{
		static const GLbitfield openGLAttachments[ 3 ] =
		{
			/* Color		*/ GL_COLOR_BUFFER_BIT,
			/* Depth		*/ GL_DEPTH_BUFFER_BIT,
			/* DepthStencil	*/ GL_STENCIL_BUFFER_BIT
		};

		const GLbitfield attachment = openGLAttachments[ p_Attachment ];

		// Cast the texture pointer to an opengl texture
		const OpenGLFramebuffer * pOpenGLFramebuffer =
			reinterpret_cast< const OpenGLFramebuffer * >( &p_Destination );

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferObject);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pOpenGLFramebuffer->GetId( ) ); 
		glBlitFramebuffer(	p_SourcePosition.x, p_SourcePosition.y,
							p_SourceSize.x, p_SourceSize.y,
							p_DestinationPosition.x, p_DestinationPosition.y,
							p_DestinationSize.x, p_DestinationSize.y,
							attachment, GL_NEAREST );
	}

	void OpenGLFramebuffer::Blit(	const eAttachment p_Attachment,
									const Vector2u32 p_DestinationPosition,
									const Vector2u32 p_DestinationSize,
									const Vector2u32 p_SourcePosition,
									const Vector2u32 p_SourceSize ) const
	{
		static const GLbitfield openGLAttachments[ 3 ] =
		{
			/* Color		*/ GL_COLOR_BUFFER_BIT,
			/* Depth		*/ GL_DEPTH_BUFFER_BIT,
			/* DepthStencil	*/ GL_STENCIL_BUFFER_BIT
		};

		const GLbitfield attachment = openGLAttachments[ p_Attachment ];

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferObject);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0 ); 
		glBlitFramebuffer(	p_SourcePosition.x, p_SourcePosition.y,
							p_SourceSize.x, p_SourceSize.y,
							p_DestinationPosition.x, p_DestinationPosition.y,
							p_DestinationSize.x, p_DestinationSize.y,
							attachment, GL_NEAREST );
	}

	const Framebuffer & OpenGLFramebuffer::GetDefault( ) const
	{
		return s_DefaultFramebuffer;
	}

	GLuint OpenGLFramebuffer::GetId( ) const
	{
		return m_FramebufferObject;
	}
/*
	void OpenGLFramebuffer::Foo( )
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferObject);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0 ); 
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}*/

}
