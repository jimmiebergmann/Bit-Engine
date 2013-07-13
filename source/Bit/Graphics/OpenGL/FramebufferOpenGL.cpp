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

#include <Bit/Graphics/OpenGL/FramebufferOpenGL.hpp>
#include <Bit/Graphics/OpenGL/TextureOpenGL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
	FramebufferOpenGL::FramebufferOpenGL( ) :
		m_ColorAttachmentCount( 0 ),
		m_DepthAttachmentCount( 0 )
	{
		// Generate the framebuffer object
		glGenFramebuffers( 1, &m_FramebufferObject );
	}

	FramebufferOpenGL::~FramebufferOpenGL( )
	{
		// Delete the framebuffer object
		glDeleteFramebuffers( 1, &m_FramebufferObject );
		m_FramebufferObject = 0;
		m_ColorAttachmentCount = 0;
		m_DepthAttachmentCount = 0;
	}

	// Virtual public functions
	void FramebufferOpenGL::Bind( )
	{
		// Bind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, m_FramebufferObject ); 
	}

	void FramebufferOpenGL::Unbind( )
	{
		// Unbind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 ); 
	}

	BIT_UINT32 FramebufferOpenGL::Attach( const Texture * p_pTexture )
	{
		if( p_pTexture == BIT_NULL )
		{
			return BIT_ERROR;
		}

		// Cast the texture pointer to an opengl texture
		const TextureOpenGL * pTextureOpenGL =
			reinterpret_cast< const TextureOpenGL * >( p_pTexture );

		// Get the format
		BIT_UINT32 Format = pTextureOpenGL->GetFormat( );
		if( Format < 1 || Format > 4 )
		{
			bitTrace( "[FramebufferOpenGL::Attach(Texture)] Wrong format type.\n" );
			return BIT_ERROR;
		}

		// Calculate the right attachment
		static const GLenum OpenGLAttachments[ 4 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0,
			GL_DEPTH_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT };

		GLenum Attachment = OpenGLAttachments[ Format - 1];

		// Incement the attachment depending on the type
		if( Attachment == GL_COLOR_ATTACHMENT0 )
		{
			if( m_ColorAttachmentCount > 15 )
			{
				bitTrace( "[FramebufferOpenGL::Attach(Texture)] Attached too many color attachments.\n" );
				return BIT_ERROR;
			}

			// Increment the attachment and the color attachment counter
			Attachment += m_ColorAttachmentCount;
			m_ColorAttachmentCount++;
		}
		else if( Attachment == GL_DEPTH_ATTACHMENT || Attachment == GL_DEPTH_STENCIL_ATTACHMENT )
		{
			if( m_DepthAttachmentCount )
			{
				bitTrace( "[FramebufferOpenGL::Attach(Texture)] Attached too many depth/stencil attachments.\n" );
				return BIT_ERROR;
			}

			// Increment the depth attachment counter
			m_DepthAttachmentCount++;
		}

		// Bind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, m_FramebufferObject ); 

		// Attach the texture
		glFramebufferTexture2D( GL_FRAMEBUFFER, Attachment,
			GL_TEXTURE_2D, pTextureOpenGL->GetID( ), 0 );
	

		// Check the framebuffer status!
		GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if( Status != GL_FRAMEBUFFER_COMPLETE )
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );

			bitTrace( "[FramebufferOpenGL::Attach(Texture)] Framebuffer error.\n" );
			return BIT_ERROR;
		} 

		// unbind the framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 ); 

		// Increment the color attachment counter.
		m_ColorAttachmentCount++;

		return BIT_OK;
	}

}