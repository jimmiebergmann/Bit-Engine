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

#include <Bit/Graphics/PngFile.hpp>
#include <Bit/Graphics/LodePng/lodepng.h>
#include <fstream>
#include <sstream>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Png file class
	PngFile::PngFile( const Bool p_DeallocateFlag ) :
		m_ImageSize( 0, 0 ),
		m_PixelDepth( 0 ),
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_DeallocateFlag( p_DeallocateFlag )
	{
	}

	PngFile::~PngFile( )
	{
		// Delete the allocated data
		if( m_DeallocateFlag && m_pData )
		{
			delete m_pData;
		}
	}

	Bool PngFile::LoadFromMemory( const std::string & p_Memory )
	{
		return LoadFromByteArray( reinterpret_cast<const Uint8 *>( p_Memory.c_str( ) ), p_Memory.size( ) );
	}

	Bool PngFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ), std::fstream::binary );
		if( fin.is_open( ) == false )
		{
			bitLogGraErr(  "Can not open the file." );
			return false;
		}

		// Get the file size
		fin.seekg( 0, std::ios::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::ios::beg );

		// Allocate the buffer
		Uint8 * pBuffer = new Uint8[ fileSize ];

		// Read the file
		fin.read( reinterpret_cast<char*>( pBuffer ), fileSize );

		// Close the file
		fin.close( );

		// Load the png from byte array
		Bool status = LoadFromByteArray( pBuffer, fileSize );

		// Delete the buffer
		delete pBuffer;

		// Return the status code
		return status;
	}

	Bool PngFile::SaveToMemory( std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;

		// Save the stream
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Get the string
		p_Memory = ss.str( );

		// Succeeded
		return true;
	}

	Bool PngFile::SaveToStream( std::ostream & p_Stream )
	{
		if( m_pData == NULL || m_DataSize == 0 )
		{
			bitLogGraErr(  "No image data." );
			return false;
		}

		// We need to create a new, and row flipped array
		// Allocate the data memory
		Uint8 * pNewData = new Uint8[ m_DataSize ];

		// Go through each row and copy the data to the data memory
		SizeType ry = 0;
		SizeType rowSize = m_PixelDepth / 8 * m_ImageSize.x;
		for( Int32 y = m_ImageSize.y - 1; y >= 0; y-- )
		{
			// Copy the memory from the vector to the data array
			memcpy( pNewData + (ry * rowSize ),
					m_pData + (y * rowSize ),
					rowSize );

			// Increase the "real" y values.
			ry++;
		}


		// Store the decoded data in a vector.
		std::vector<unsigned char> dataVector;

		// Decode the data
		lodepng::State state;
		unsigned int width = m_ImageSize.x;
		unsigned int height = m_ImageSize.y;
		unsigned int status = lodepng::encode( dataVector, pNewData, width, height, state );

		// Delete the new data
		delete [ ] pNewData;

		// Error check the loading
		if( status || dataVector.size( ) == 0 )
		{
			bitLogGraErr( "Failed to decode image: " << lodepng_error_text(status));
			return false;
		}

		// Write the data to the stream
		p_Stream.write( reinterpret_cast<char *>( dataVector.data( ) ), dataVector.size( ) );

		// Succeeded
		return true;
	}

	Bool PngFile::SaveToFile( const std::string & p_Filename )
	{
		// Load a string stream.
		std::stringstream ss;

		// Save the stream.
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Open the file.
		std::ofstream fout( p_Filename.c_str( ), std::fstream::binary );
		if( fout.is_open( ) == false )
		{
			bitLogGraErr(  "Can not open the file." );
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

		// Succeeded.
		return true;
	}

	void PngFile::Clear( )
	{
		// Delete the allocated data
		if( m_pData )
		{
			delete m_pData;
		}
	}

	const Uint8 * PngFile::GetData( ) const
	{
		return m_pData;
	}

	SizeType PngFile::GetDataSize( ) const
	{
		return m_DataSize;
	}

	Vector2u32 PngFile::GetImageSize( ) const
	{
		return m_ImageSize;
	}

	Uint8 PngFile::GetPixelDepth( ) const
	{
		return m_PixelDepth;
	}

	void PngFile::SetData( const Uint8 * p_pData, const SizeType p_DataSize )
	{
		// Error check the parameters.
		if( p_pData == NULL || p_DataSize == 0 )
		{
			return;
		}

		// Delete the old data
		if( m_pData )
		{
			delete m_pData;
		}

		// Set the data size
		m_DataSize = p_DataSize;

		// Allocate the new data
		m_pData = new Uint8[ m_DataSize ];

		// Copy the data
		memcpy( m_pData, p_pData, m_DataSize );
	}

	Bool PngFile::GetDeallocatorFlag( ) const
	{
		return m_DeallocateFlag;
	}

	bool PngFile::LoadFromByteArray( const Uint8 * p_pData, const SizeType p_DataSize )
	{
		// Store the decoded data in a vector.
		std::vector<unsigned char> dataVector;

		// Decode the data
		lodepng::State state;
		unsigned int width;
		unsigned int height;
		unsigned int status = lodepng::decode(	dataVector, width, height, state,
												reinterpret_cast<const unsigned char *>( p_pData ), p_DataSize );

		// Error check the loading
		if( status )
		{
			bitLogGraErr( "Failed to decode image: " << lodepng_error_text(status));
			return false;
		}

		// Error check the image size
		if( width == 0 || height == 0 )
		{
			bitLogGraErr(  "Image size is 0." );
			return false;
		}

		// Get the number of components per pixel
		SizeType componentCount = dataVector.size( ) / width / height;

		if( (	state.info_raw.colortype != LCT_RGB &&
				state.info_raw.colortype != LCT_RGBA ) ||
				state.info_raw.bitdepth != 8 ||
			(	componentCount != 3 &&
				componentCount != 4 ) )
		{
			bitLogGraErr(  "Not a 24 or 32 bit image." );
			return false;
		}

		// Error check the data vector size
		if(	dataVector.size( ) != width * height * componentCount )
		{
			bitLogGraErr(  "The data size is not right." );
			return false;
		}

		// Set the image size and pixel depth
		m_ImageSize.x = width;
		m_ImageSize.y = height;
		m_PixelDepth = componentCount * 8;

		// Allocate the data memory
		m_DataSize = dataVector.size( );
		m_pData = new Uint8[ m_DataSize ];

		// Go through each row and copy the data to the data memory
		SizeType ry = 0;
		SizeType rowSize = componentCount * width;
		for( Int32 y = height - 1; y >= 0; y-- )
		{
			// Copy the memory from the vector to the data array
			memcpy( m_pData + (ry * rowSize ),
					dataVector.data( ) + (y * rowSize ),
					rowSize );

			// Increase the "real" y values.
			ry++;
		}

		// Succeeded
		return true;
	}

}
