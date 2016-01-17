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

#include <Bit/Graphics/OpenGL/OpenGLRenderbuffer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenGLRenderbuffer::OpenGLRenderbuffer( ) :
		m_RenderbufferObject( 0 )
	{
	}

	OpenGLRenderbuffer::~OpenGLRenderbuffer( )
	{
		if( m_RenderbufferObject )
		{
			glDeleteRenderbuffers(1, &m_RenderbufferObject);
		}
	}

	Bool OpenGLRenderbuffer::Load(	const Vector2u32 p_Size,
									const Texture::ePixelFormat p_Format,
									const Uint32 p_MultisampleLevel )
	{
		if( m_RenderbufferObject )
		{
			glDeleteRenderbuffers(1, &m_RenderbufferObject);
		}

		// Set the pixel format
		m_PixelFormat = p_Format;

		// OpenGL formats
		static const GLenum openGLTextureFormats[ 4 ] = { GL_DEPTH_COMPONENT24, GL_DEPTH24_STENCIL8, GL_RGB8, GL_RGBA8 };

		// Get the needed opengl attributes
		GLenum format = openGLTextureFormats[ static_cast<SizeType>( p_Format ) ];

		// Create color render buffer
		glGenRenderbuffers( 1, &m_RenderbufferObject );
		glBindRenderbuffer( GL_RENDERBUFFER, m_RenderbufferObject );

		if( p_MultisampleLevel == 0 )
		{
			glRenderbufferStorage( GL_RENDERBUFFER, format, p_Size.x, p_Size.y );
		}
		else
		{
			glRenderbufferStorageMultisample( GL_RENDERBUFFER, p_MultisampleLevel, format, p_Size.x, p_Size.y );
		}
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );

		return true;
	}
	
	Texture::ePixelFormat OpenGLRenderbuffer::GetPixelFormat( ) const
	{
		return m_PixelFormat;
	}


	GLuint OpenGLRenderbuffer::GetId( ) const
	{
		return m_RenderbufferObject;
	}

}