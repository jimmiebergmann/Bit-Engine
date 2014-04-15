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

#ifndef BIT_GRAPHICS_TGA_FILE_HPP
#define BIT_GRAPHICS_TGA_FILE_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Tga bitmap image file class.
	///
	/// The image data is stored with 8, 16, 24, or 32 bits per pixel.
	/// Currently not supporting color maps.
	/// Currently not supporting RLE compression.
	/// Currently not supporting 15 bit pixel data.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API TgaFile
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Image type enumerator
		///
		////////////////////////////////////////////////////////////////
		enum eImageType
		{
			NoImageData = 0,					///< No image data.
			UncompressedColorMappedImage = 1,	///< Uncompressed color mapped image.
			UncompressedTrueColorImage = 2,		///< Uncompressed true color image.
			UncompressedGrayscaleImage = 3,		///< Uncompressed grayscale image.
			RleColorMappedImage = 9,			///< RLE compressed color mapped image.
			RleTrueColorImage = 10,				///< RLE compressed true color image.
			RleGrayscaleImage = 11,				///< RLE compressed grayscale image.
		};


		////////////////////////////////////////////////////////////////
		/// \brief Color map specification structure.
		///
		////////////////////////////////////////////////////////////////
		struct ColorMapSpecification
		{
			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			ColorMapSpecification( );

			Uint16 Offset;		///< Index of the first entry.
			Uint16 Length;		///< Number of entries.
			Uint8 EntrySize;	///< Number of bits per pixel.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Image specification structure.
		///
		////////////////////////////////////////////////////////////////
		struct ImageSpecification
		{

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			ImageSpecification( );
			
			Uint16 OriginX;			///< Coordinate of lower-left corner for displays where origin is at the lower left.
			Uint16 OriginY;			///< As OriginX.
			Uint16 ImageWidth;		///< Width in pixels.
			Uint16 ImageHeight;		///< Height in pixels.
			Uint8 PixelDepth;		///< Bits per pixel.
			Uint8 ImageDescriptor;	///< Bits 3-0 give the alpha channel depth, bits 5-4 give direction.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Header structure.
		///
		////////////////////////////////////////////////////////////////
		class Header
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Header( );

			////////////////////////////////////////////////////////////////
			/// \brief Set image type from a image type enumerator.
			///
			////////////////////////////////////////////////////////////////
			void SetImageType( const eImageType & p_ImageType );

			////////////////////////////////////////////////////////////////
			/// \brief Get image type as a image type enumerator.
			///
			////////////////////////////////////////////////////////////////
			eImageType GetImageType(  ) const;

			// Public variables, in order to make data manipulation easier.
			Uint8 IdLength;							///< Length of the image ID field.
			Uint8 ColorMapType;						///< Whether a color map is included.
			Uint8 ImageType;						///< Compression and color types.
			ColorMapSpecification ColorMapSpec;		///< Describes the color map.
			ImageSpecification ImageSpec;			///< Image dimensions and format.
		};


		////////////////////////////////////////////////////////////////
		/// \brief Footer structure.
		///
		////////////////////////////////////////////////////////////////
		struct Footer
		{

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Footer( );
			
			// Public variables, in order to make data manipulation easier.
			Uint32 ExtensionOffset;		///< Offset in bytes from beginning of the file.
			Uint32 DeveloperAreaOffset;	///< Offset in bytes from beginning of the file.
			Uint8 Signature[ 16 ];		///< Contains "TRUEVISION-XFILE"
			Uint8 Dot;					///< Contains "."
			Uint8 End;					///< Contains NULL
		};

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// \param p_DeallocateData Does not deallocate the bitmap data
		/// at destruction if false. This is useful if you want to
		/// take over the bitmap data pointer from the tga file class.
		///
		////////////////////////////////////////////////////////////////
		TgaFile( const Bool p_DeallocateData = true );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		/// Clears the data.
		///
		////////////////////////////////////////////////////////////////
		~TgaFile( );

		////////////////////////////////////////////////////////////////
		/// \brief Load TGA file from file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the tga header, footer and data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the header field.
		///
		////////////////////////////////////////////////////////////////
		Header & GetHeader( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the footer field.
		///
		////////////////////////////////////////////////////////////////
		Footer & GetFooter( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the raw bitmap data.
		///
		////////////////////////////////////////////////////////////////
		Uint8 * GetData( ) const;
		
		////////////////////////////////////////////////////////////////
		/// \brief Get the raw bitmap data size in bytes.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetDataSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Set the raw bitmap data.
		///
		/// Clears any previous data.
		/// The input data is copied into a new array.
		///
		////////////////////////////////////////////////////////////////
		void SetData( const Uint8 * p_pData, const SizeType p_DataSize  );

	private:

		Header m_Header;		///< Header field.
		Footer m_Footer;		///< Footer field.
		Uint8 * m_pData;		///< Raw pixel data.
		SizeType m_DataSize;	///< The data size in bytes.
		Bool m_DeallocateData;	///< Data deallocation at destruction flag, true by default.

	};

}

#endif