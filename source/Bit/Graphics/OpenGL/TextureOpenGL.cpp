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

#include <Bit/Graphics/OpenGL/TextureOpenGL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// OpenGL constants
	static const GLenum OpenGLTypes[ 17 ] = 
	{
		0, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		GL_UNSIGNED_INT, GL_INT, 0, 0, GL_FLOAT, GL_DOUBLE, GL_BYTE, 0
	};


	// Constructor/destrucotr
	TextureOpenGL::TextureOpenGL( const BIT_BOOL p_OpenGL2 ) :
		m_ID( 0 )
	{
		m_Loaded = BIT_FALSE;
		m_Format = 0;
		m_OpenGL2 = p_OpenGL2;
	}

	TextureOpenGL::~TextureOpenGL ( )
	{
		// Delete the texture
		if( m_ID )
		{
			glDeleteTextures( 1, &m_ID );
		}

		m_Loaded = false;
	}

	// General public functions
	BIT_UINT32 TextureOpenGL::Load( const Image & p_Image, const BIT_BOOL p_Mipmapping )
	{
		// Check if the image already is loaded
		if( m_Loaded )
		{
			bitTrace( "[TextureOpenGL::Load] Already loaded.\n" );
			return BIT_ERROR;
		}

		// Make sure the image class contains any data at all.
		if( p_Image.ContainsData() == BIT_FALSE )
		{
			bitTrace( "[TextureOpenGL::Load] The passed image does not contain any data.\n" );
			return BIT_ERROR;
		}

		// Check the image BPP, we are currently just supporting 24 and 32 bit images.
		BIT_UINT32 Depth = p_Image.GetDepth( );
		if(Depth != 3 && Depth != 4)
		{
			bitTrace( "[TextureOpenGL::Load] Not a 24 or 32 bit image.\n" );
			return BIT_ERROR;
		}

		static const GLint OpenGLTextureFormats[2] = { GL_RGB, GL_RGBA };
		GLint Format = OpenGLTextureFormats[ Depth - 3 ];
		m_Format = Depth - 2;

		// Generate an OpenGL texture id.
		glGenTextures( 1, &m_ID );
		glBindTexture( GL_TEXTURE_2D, m_ID );

		// Generate the mipmap ( Opengl > 1.4 style )
		if( m_OpenGL2 && p_Mipmapping )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE ); 
		}

		// Set the texure data
		m_Size = p_Image.GetSize( );
		glTexImage2D ( GL_TEXTURE_2D, 0, Format, m_Size.x, m_Size.y, 0,
			(GLenum)Format, GL_UNSIGNED_BYTE, (GLvoid *)p_Image.GetData() );

		// Generate the mipmap ( Opengl 3.x style )
		if( !m_OpenGL2 && p_Mipmapping )
		{
			glGenerateMipmap( GL_TEXTURE_2D );
		}

		// Add anisotropy filtering
		if( p_Mipmapping )
		{
			GLfloat Levels;
			glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Levels );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f );
		}

		// Unbind the texture
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 TextureOpenGL::Load( Vector2_ui32 p_Size, const eColorComponent p_Format,
		const eColorComponent p_InternalFormat, const eDataType p_FormatType, void * p_Data )
	{
		// Check if the image already is loaded
		if( m_Loaded )
		{
			bitTrace( "[TextureOpenGL::Load] Already loaded.\n" );
			return BIT_ERROR;
		}

		// Check if the formats are correct
		if( p_Format < 1 || p_Format > 4 )
		{
			bitTrace( "[TextureOpenGL::Load] Wrong format.\n" );
			return BIT_ERROR;
		}
		if( p_InternalFormat < 1 || p_InternalFormat > 4 )
		{
			bitTrace( "[TextureOpenGL::Load] Wrong internal format.\n" );
			return BIT_ERROR;
		}
		if( p_FormatType < 1 || p_FormatType > 16 )
		{
			bitTrace( "[TextureOpenGL::Load] Wrong format type.\n" );
			return BIT_ERROR;
		}

		// Set the size and format
		m_Size = p_Size;
		m_Format = p_Format;

		// OpenGL formats
		static const GLint OpenGLTextureFormats[ 4 ] = { GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL };

		// Get the needed opengl attributes
		GLenum Format = OpenGLTextureFormats[ p_Format - 1];
		GLint InternalFormat = OpenGLTextureFormats[ p_InternalFormat - 1];
		GLint FormatType = OpenGLTypes[ p_FormatType ];

		// Make sure we are using a valid format type
		if( FormatType == 0 )
		{
			bitTrace( "[TextureOpenGL::Load] Parsed invalid format type.\n" );
			return BIT_ERROR;
		}

		// Let's create the texture
		glGenTextures( 1, &m_ID );
		glBindTexture( GL_TEXTURE_2D, m_ID );

		// Set the data
		glTexImage2D ( GL_TEXTURE_2D, 0, InternalFormat, p_Size.x, p_Size.y, 0,
			Format, FormatType, (GLvoid *)p_Data );

		// Unbind the texture
		glBindTexture( GL_TEXTURE_2D, 0 );

		m_Loaded = BIT_OK;
		return BIT_OK;
	}

	void TextureOpenGL::Bind( BIT_UINT32 p_Index )
	{
		glActiveTexture( GL_TEXTURE0 + p_Index );
		glBindTexture( GL_TEXTURE_2D, m_ID );
	}

	void TextureOpenGL::Unbind( )
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	// Set functions
	BIT_UINT32 TextureOpenGL::SetFilters( const eFilter * p_pFilters )
	{
		// This code is working, but I don't know about it's efficiency...
		if( p_pFilters == BIT_NULL ||
			*p_pFilters == Filter_None || *( p_pFilters + 1 ) == Filter_None )
		{
			bitTrace( "[TextureOpenGL::Load] No filters are passed.\n" );
			return BIT_ERROR;
		}

		// Bind the texture
		glBindTexture( GL_TEXTURE_2D, m_ID );

		// Currently max 4 filters supported.
		const BIT_UINT32 MaxFilters = 4;
		GLenum  Filter = 0;
 		GLint  	Param = 0;

		for( BIT_MEMSIZE i = 0; i < MaxFilters; i++ )
		{
			// Make sure the filter or param isn't 0
			if( *p_pFilters == 0 || *( p_pFilters + 1 ) == 0 )
			{
				// Break the for-loop.
				break;
			}

			// Swtich the filter type
			switch( *p_pFilters )
			{
			case Filter_Mag:
				Filter = GL_TEXTURE_MAG_FILTER;
				break;
			case Filter_Min:
				Filter = GL_TEXTURE_MIN_FILTER;
				break;
			case Filter_Wrap_X:
				Filter = GL_TEXTURE_WRAP_S;
				break;
			case Filter_Wrap_Y:
				Filter = GL_TEXTURE_WRAP_T;
				break;
			default:
				glBindTexture( GL_TEXTURE_2D, 0 );
				return BIT_ERROR;
			}

			// Swtich the filters value 
			switch( *(p_pFilters + 1) )
			{
			case Filter_Nearest:
				Param = GL_NEAREST;
				break;
			case Filter_Linear:
				Param = GL_LINEAR;
				break;
			case Filter_Nearest_Mipmap:
				Param = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case Filter_Linear_Mipmap:
				Param = GL_LINEAR_MIPMAP_LINEAR;
				break;
			case Filter_Repeat:
				Param = GL_REPEAT;
				break;
			case Filter_Clamp:
				Param = GL_CLAMP;
				break;
			default:
				glBindTexture( GL_TEXTURE_2D, 0 );
				return BIT_ERROR;
			}

			// Set the opengl texture filter.
			glTexParameteri ( GL_TEXTURE_2D, Filter, Param );

			// Increment the pointer by 2,
			p_pFilters += 2;
		}

		// Unbind the texture
		glBindTexture( GL_TEXTURE_2D, 0 );

		return BIT_OK;
	}

}