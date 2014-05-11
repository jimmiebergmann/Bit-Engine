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

#include <Bit/Graphics/OpenGL/OpenGLTexture.hpp>
#include <Bit/Graphics/Image.hpp>

#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static GLenum g_OpenGLDataTypes[ static_cast<SizeType>( DataType::SizeType ) + 1 ] =
	{
		/*None		*/ 0,
		/*Int8		*/ GL_BYTE,
		/*Uint8		*/ GL_UNSIGNED_BYTE,
		/*Int16		*/ GL_SHORT,
		/*Uint16	*/ GL_UNSIGNED_SHORT,
		/*Int32		*/ GL_INT,
		/*Uint32	*/ GL_UNSIGNED_INT,
		/*Int64		*/ 0, 
		/*Uint64	*/ 0,
		/*Float32	*/ GL_FLOAT,
		/*Float64	*/ GL_DOUBLE,
		/*Bool		*/ GL_UNSIGNED_BYTE,
		/*SizeType	*/ 0
	};

	static GLint g_OpenGLTextureFilters[ 6 ] =
	{
		/*Nearest				*/ GL_NEAREST,
		/*NearestMipmapNearest	*/ GL_NEAREST_MIPMAP_NEAREST,
		/*NearestMipmapLinear	*/ GL_NEAREST_MIPMAP_LINEAR,
		/*Linear				*/ GL_LINEAR,
		/*LinearMipmapNearest	*/ GL_LINEAR_MIPMAP_NEAREST,
		/*LinearMipmapLinear	*/ GL_LINEAR_MIPMAP_LINEAR
	};

	static GLint g_OpenGLTextureWrapping[ 2 ] =
	{
		/*Repeat	*/ GL_REPEAT,
		/*Clamp		*/ GL_CLAMP
	};

	

		


	// Constructor/destrucotr
	OpenGLTexture::OpenGLTexture( ) :
		m_Id( 0 )
	{
		m_Loaded = false;
	}

	OpenGLTexture::~OpenGLTexture ( )
	{
		// Delete the texture
		if( m_Id )
		{
			glDeleteTextures( 1, &m_Id );
		}

		m_Loaded = false;
	}

	// General public functions
	Bool OpenGLTexture::LoadFromMemory(	const void * p_pData, 
										Vector2u32 p_Size,
										const SizeType p_ColorComponentsPerPixel,
										const ePixelFormat p_Format,
										const DataType::eType p_Datatype,
										const Bool p_Mipmapping )
	{
		// Check if the image already is loaded
		if( m_Loaded )
		{
			std::cout << "[TextureOpenGL::LoadFromMemory] Already loaded.\n";
			return false;
		}

		// Check if the formats are correct
		/*if( p_Format < 1 || p_Format > 4 )
		{
			std::cout << "[TextureOpenGL::LoadFromMemory] Wrong format.\n";
			return false;
		}
		if( p_InternalFormat < 1 || p_InternalFormat > 4 )
		{
			std::cout << "[TextureOpenGL::LoadFromMemory] Wrong internal format.\n";
			return false;
		}
		if( p_FormatType < 1 || p_FormatType > 16 )
		{
			std::cout << "[TextureOpenGL::LoadFromMemory] Wrong format type.\n";
			return false;
		}*/

		// Set the size and format
		m_Size = p_Size;
		//m_Format = p_Format;

		// OpenGL formats
		static const GLenum openGLTextureFormats[ 4 ] = { GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL, GL_RGB, GL_RGBA };

		// Get the needed opengl attributes
		GLenum dataType = g_OpenGLDataTypes[ static_cast<SizeType>( p_Datatype ) ];
		GLenum format = openGLTextureFormats[ static_cast<SizeType>( p_Format ) ];

		//GLenum Format = OpenGLTextureFormats[ p_Format - 1];
		//GLint InternalFormat = OpenGLTextureFormats[ p_InternalFormat - 1];
	//	GLint FormatType = OpenGLTypes[ p_FormatType ];

		// Make sure we are using a valid format type
		/*if( FormatType == 0 )
		{
			std::cout << "[TextureOpenGL::LoadFromMemory] Parsed invalid format type.\n";
			return BIT_ERROR;
		}*/

		// Let's create the texture
		glGenTextures( 1, &m_Id );
		glBindTexture( GL_TEXTURE_2D, m_Id );

		// Set the data
		glTexImage2D (	GL_TEXTURE_2D, 0, p_ColorComponentsPerPixel, p_Size.x, p_Size.y, 0,
						format, dataType, reinterpret_cast<const GLvoid *>( p_pData ) );

		// Set default filters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		// Generate the mipmap
		if( p_Mipmapping )
		{
			glGenerateMipmap( GL_TEXTURE_2D );
		}

		// Unbind the texture
		glBindTexture( GL_TEXTURE_2D, 0 );

		// Succeeded.
		return m_Loaded = true;
	}

	Bool OpenGLTexture::LoadFromFile( const std::string & p_Filename, const Bool p_Mipmapping )
	{
		// Load the image
		Image image;
		if( image.LoadFromFile( p_Filename ) == false )
		{
			std::cout << "[TextureOpenGL::LoadFromFile] Can not load the file\n";
			return false;
		}

		// Load from the image.
		return LoadFromImage( image, p_Mipmapping );
	}

	Bool OpenGLTexture::LoadFromImage( const Image & p_Image, const Bool p_Mipmapping )
	{
		// Check if the image already is loaded
		if( m_Loaded )
		{
			std::cout << "[TextureOpenGL::LoadFromImage] Already loaded.\n";
			return false;
		}

		// Check the image BPP, we are currently just supporting 24 and 32 bit images.
		const Uint8 depth = p_Image.GetPixelDepth( );
		if( depth != 3 && depth != 4)
		{
			std::cout << "[TextureOpenGL::LoadFromImage] Not a 24 or 32 bit image.\n";
			return false;
		}

		// Get the opengl format of the image data.
		static const GLenum openGLTextureFormats[ 2 ] = { GL_RGB, GL_RGBA };
		const SizeType openGLTextureIndex = depth - 3;
		GLenum format = openGLTextureFormats[ openGLTextureIndex ];

		// Get the format

		//m_Format = depth - 2;

		// Generate an OpenGL texture id.
		glGenTextures( 1, &m_Id );
		glBindTexture( GL_TEXTURE_2D, m_Id );

		// Generate the mipmap ( Opengl > 1.4 style )
		/*if( p_Mipmapping )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE ); 
		}*/

		// Set the texure data
		m_Size = p_Image.GetSize( );
		glTexImage2D (	GL_TEXTURE_2D, 0, format, m_Size.x, m_Size.y, 0,
						format, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid *>( p_Image.GetData( )) );

		// Set default filers
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		// Generate the mipmap
		if( p_Mipmapping )
		{
			glGenerateMipmap( GL_TEXTURE_2D );
		}

		// Unbind the texture
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		// Succeeded.
		return m_Loaded = true;
	}

	void OpenGLTexture::Bind( const Uint32 p_Index )
	{
		glActiveTexture( GL_TEXTURE0 + p_Index );
		glBindTexture( GL_TEXTURE_2D, m_Id );
	}

	void OpenGLTexture::Unbind( )
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	Bool OpenGLTexture::SetMagnificationFilter( const eFilter p_Filter )
	{
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_OpenGLTextureFilters[ p_Filter ] );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return true;
	}

	Bool OpenGLTexture::SetMinificationFilter( const eFilter p_Filter )
	{
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_OpenGLTextureFilters[ p_Filter ] );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return true;
	}

	Bool OpenGLTexture::SetWrapping( const eWarpping p_WrapX, const eWarpping p_WrapY )
	{
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_OpenGLTextureWrapping[ p_WrapX ] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_OpenGLTextureWrapping[ p_WrapY ] );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return true;
	}

	Bool OpenGLTexture::SetWrappingX( const eWarpping p_WrapX )
	{
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_OpenGLTextureWrapping[ p_WrapX ] );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return true;
	}

	Bool OpenGLTexture::SetWrappingY( const eWarpping p_WrapY )
	{
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_OpenGLTextureWrapping[ p_WrapY ] );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return true;
	}

	Bool OpenGLTexture::SetAnisotropic( const Uint32 p_Level )
	{
		// Error check.
		if( OpenGL::IsAnisotropicFilterAvailable( ) == false ||
			p_Level > OpenGL::GetAnisotropicMaxLevel( ) )
		{
			return false;
		}

		// Set the anisotropy level.
		glBindTexture( GL_TEXTURE_2D, m_Id );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<GLfloat>( p_Level ) );
		glBindTexture( GL_TEXTURE_2D, 0 );

		// Succeeded
		return true;
	}

	Vector2u32 OpenGLTexture::GetSize( ) const
	{
		return m_Size;
	}

	Bool OpenGLTexture::IsLoaded( ) const
	{
		return m_Loaded;
	}

}