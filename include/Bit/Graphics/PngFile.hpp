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

#ifndef BIT_GRAPHICS_PNG_FILE_HPP
#define BIT_GRAPHICS_PNG_FILE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Tga bitmap image file class.
	///
	/// The image data is stored with 8, 16, 24, or 32 bits per pixel.
	/// Currently not supporting RLE compression.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API PngFile
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// \param p_DeallocateFlag Does not deallocate the bitmap data
		/// at destruction if false. This is useful if you want to
		/// take over the bitmap data pointer from the tga file class.
		///
		////////////////////////////////////////////////////////////////
		PngFile( const Bool p_DeallocateFlag = true );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		/// Clears the data.
		///
		////////////////////////////////////////////////////////////////
		~PngFile( );

		////////////////////////////////////////////////////////////////
		/// \brief Load BMP file from memory.
		///
		/// \param p_pMemory Pointer to the memory.
		/// \param p_Size Size of the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMemory( const std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Load BMP file from file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the BMP header and data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the raw bitmap data.
		///
		////////////////////////////////////////////////////////////////
		const Uint8 * GetData( ) const;
		
		////////////////////////////////////////////////////////////////
		/// \brief Get the raw bitmap data size in bytes.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetDataSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the image size
		///
		////////////////////////////////////////////////////////////////
		Vector2u32 GetImageSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the pixel depth, in bits.
		///
		////////////////////////////////////////////////////////////////
		Uint8 GetPixelDepth( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Set the raw bitmap data.
		///
		/// Clears any previous data.
		/// The input data is copied into a new array.
		///
		////////////////////////////////////////////////////////////////
		void SetData( const Uint8 * p_pData, const SizeType p_DataSize  );

		////////////////////////////////////////////////////////////////
		/// \brief Get the deallocator flag.
		///
		////////////////////////////////////////////////////////////////
		Bool GetDeallocatorFlag( ) const;

	private:

		////////////////////////////////////////////////////////////////
		/// \brief Load png image from a byte array in memory.
		///
		////////////////////////////////////////////////////////////////
		bool LoadFromByteArray( const Uint8 * p_pData, const SizeType p_DataSize );

		// Private variables
		Vector2u32 m_ImageSize;	///< The image size.
		Uint8 m_PixelDepth;		///< The pixel depth.
		Uint8 * m_pData;		///< Raw pixel data.
		SizeType m_DataSize;	///< The data size in bytes.
		Bool m_DeallocateFlag;	///< Data deallocation at destruction flag, true by default.

	};

}

#endif