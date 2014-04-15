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

#ifndef BIT_GRAPHICS_IMAGE_HPP
#define BIT_GRAPHICS_IMAGE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Pixel class.
	///
	/// Internally stores the pixel data in RGBA( Red, green, blue and alpha ) format,
	/// where every single component are stored in the range of 0 to 255.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Pixel
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		Pixel( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		Pixel(	const Uint8 p_Red,
				const Uint8 p_Green,
				const Uint8 p_Blue,
				const Uint8 p_Alpha = 255 );

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		Pixel( const Vector3u8 p_RGB, const Uint8 p_Alpha = 255 );

		////////////////////////////////////////////////////////////////
		/// \brief Equals to operator
		///
		////////////////////////////////////////////////////////////////
		Bool operator == ( const Pixel & p_Pixel ) const;

		// Public members
		Uint8 Red;		///< Red color component
		Uint8 Green;	///< Green color component
		Uint8 Blue;		///< Blue color component
		Uint8 Alpha;	///< Alpha component

	};


	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Image class.
	///
	/// This class stores pixel data which can be assembled into an image.
	/// The data can be manipulated directly via software or it can
	/// be used for supplying texture classes with data which can be
	/// stored in the VRAM, for example...
	/// Stores the pixel data in 3 or 4 byte format.
	///
	/// \see Texture
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Image
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		Image( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		/// Cleaning up allocated data.
		///
		////////////////////////////////////////////////////////////////
		~Image( );

		////////////////////////////////////////////////////////////////
		/// \brief Load image data from memory
		///
		/// Loads raw pixel data from memory.
		/// The passed image data to this function can be deleted afterwards.
		///
		///	\param p_pData Pointer to the [R][GB][A] image data.
		/// \param p_Depth Pixel data depth in bytes(1/3/4).
		///	\param p_Size Image size(width and height).
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMemory( const Uint8 * p_pData, const Uint8 p_Depth, const Vector2u32 & p_Size );

		////////////////////////////////////////////////////////////////
		/// \brief Load image data from a file.
		///
		/// The filename must contain a file suffix in order to let the function
		/// know what kind of image format you are trying to load.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Load image data from a TGA file.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromTgaFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Load image data from a BMP file.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromBmpFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the image data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Set a single pixel fragment at the given index.
		///
		/// \param p_Index Index of the pixel to set.
		/// \param p_Pixel The pixel data to set.
		///
		////////////////////////////////////////////////////////////////
		void SetPixel( const Uint32 p_Index, const Pixel & p_Pixel );

		////////////////////////////////////////////////////////////////
		/// \brief Set a single pixel fragment at the given position.
		///
		/// \param p_Position Position of the pixel to set.
		/// \param p_Pixel The pixel data to set.
		///
		////////////////////////////////////////////////////////////////
		void SetPixel( const Vector2u32 & p_Position, const Pixel & p_Pixel );

		////////////////////////////////////////////////////////////////
		/// \brief Get a single pixel fragment from a given index.
		///
		/// \param p_Index Pixel index
		///
		////////////////////////////////////////////////////////////////
		Pixel GetPixel( const Uint32 p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get a single pixel fragment from a given position
		///
		/// \param p_Position Pixel position
		///
		////////////////////////////////////////////////////////////////
		Pixel GetPixel( const Vector2u32 & p_Position );

		////////////////////////////////////////////////////////////////
		/// \brief Get a pointer to the raw image data.
		///
		////////////////////////////////////////////////////////////////
		const Uint8 * GetData( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the pixel depth in bytes.
		///
		////////////////////////////////////////////////////////////////
		Uint8 GetPixelDepth( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the size in bytes of the raw data.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetDataSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the image size(width, height)
		///
		////////////////////////////////////////////////////////////////
		Vector2u32 GetSize( ) const;

	private:

		////////////////////////////////////////////////////////////////
		/// \brief Converting the pixel component order from BGR to RGB order.
		///
		////////////////////////////////////////////////////////////////
		void BgrToRgb( );

		////////////////////////////////////////////////////////////////
		/// \brief Converting the pixel component order from BGRA to RGBA order.
		///
		////////////////////////////////////////////////////////////////
		void BgraToRgba( );

		// Private variables
		Uint8 * m_pData;	///< The pixel data.
		Uint8 m_PixelDepth;	///< pixel depth in bytes, 1/3/4
		Vector2u32 m_Size;	///< The size of the image(width, height).
		

	};

}



#endif
