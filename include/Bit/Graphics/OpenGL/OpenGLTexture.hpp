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

#ifndef BIT_GRAPHICS_OPENGL_TEXTURE_HPP
#define BIT_GRAPHICS_OPENGL_TEXTURE_HPP

#include <Bit/Graphics/Texture.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL texture class
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLTexture : public Texture
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		OpenGLTexture( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~OpenGLTexture( );

		////////////////////////////////////////////////////////////////
		/// \brief Load texture from memory.
		///
		/// \param p_pData Pointer to the pixel data.
		/// \param p_Size The size of the texture(width and height).
		/// \param p_BytesPerPixel Number of bytes per pixel.
		///		Must be one of the following: 1/2/3/4.
		/// \param p_Format The format for representing the color components.
		/// \param p_Datatype The data type as the components are stored as.
		/// \param p_Mipmapping Generating mipmaps for the texture if true.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromMemory(	const void * p_pData,
										const Vector2u32 p_Size,
										const SizeType p_BytesPerPixel = 4,
										const ePixelFormat p_Format = Rgba,
										const DataType::eType p_Datatype = DataType::Uint8,
										const Bool p_Mipmapping = false );

		////////////////////////////////////////////////////////////////
		/// \brief Load texture data from a file.
		///
		///	This function is a combination of Bit::Image
		///	and the "LoadFromImage" function.
		///
		/// \param p_Filename File path.
		/// \param p_Mipmapping Generating mipmaps for the texture if true.
		///
		/// \return true if succeeded, else false.
		///
		/// \see LoadFromImage
		///
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromFile( const std::string & p_Filename, const Bool p_Mipmapping = false );

		////////////////////////////////////////////////////////////////
		/// \brief Load texture from an image.
		///
		/// \param p_Image The image to load pixel data from.
		/// \param p_Mipmapping Generating mipmaps for the texture if true.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromImage( const Image & p_Image, const Bool p_Mipmapping = false );

		////////////////////////////////////////////////////////////////
		/// \brief Bind the texture to the given index(0 by default).
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( const Uint32 p_Index = 0 );

		////////////////////////////////////////////////////////////////
		/// \brief Unbind the texture.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind( );

		////////////////////////////////////////////////////////////////
		/// \brief Apply the current properties to the texture.
		///
		/// \return False if the anisotropic lever is invalid, else true.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool ApplyProperties( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the texture properties.
		///
		////////////////////////////////////////////////////////////////
		virtual TextureProperties & GetProperties( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the texture size(width and height)
		///
		////////////////////////////////////////////////////////////////
		virtual Vector2u32 GetSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the texture's pixel format.
		///
		////////////////////////////////////////////////////////////////
		virtual ePixelFormat GetPixelFormat( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Check if the texture is successfully loaded.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsLoaded( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the ID of the OpenGL Texture.
		///
		////////////////////////////////////////////////////////////////
		GLuint GetId( ) const;

	private:

		// Private variables
		GLuint m_Id;					///< OpenGL texture id.
		Bool m_Loaded;					///< Indicates if the texture is loaded.
		Vector2u32 m_Size;				///< Texture size.
		ePixelFormat m_PixelFormat;		///< Pixel format.
		TextureProperties m_Properties;	///< Texture properties, such as filters.

	};
}

#endif
