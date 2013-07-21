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


#include <Bit/Graphics/Image.hpp>
#include <Bit/System.hpp>
#include <fstream>
#include <cstring>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Pixel class
	Pixel::Pixel( )
	{
	}

	Pixel::Pixel( BIT_BYTE p_R, BIT_BYTE p_G, BIT_BYTE p_B ) :
		m_R( p_R ),
		m_G( p_G ),
		m_B( p_B ),
		m_A( 0 )
	{
	}

	Pixel::Pixel( BIT_BYTE p_R, BIT_BYTE p_G, BIT_BYTE p_B, BIT_BYTE p_A ) :
		m_R( p_R ),
		m_G( p_G ),
		m_B( p_B ),
		m_A( p_A )
	{
	}

	Pixel::Pixel( Vector3_byte p_RGB, BIT_BYTE p_A ) :
		m_R( p_RGB.x ),
		m_G( p_RGB.y ),
		m_B( p_RGB.z ),
		m_A( p_A )
	{
	}


	// Image class
	Image::Image( ) :
		m_pData(BIT_NULL),
		m_Size( 0, 0 ),
		m_Depth(0)
	{
	}

	Image::~Image( )
	{
		DeallocateData( );
	}

	BIT_UINT32 Image::ReadFile( const char * p_pFilePath )
	{
		// Get the file's extension
		std::string FileExtension = GetFileExtension( p_pFilePath );

		if( FileExtension == "TGA" )
		{
			return ReadTGA( p_pFilePath );
		}
		else if( FileExtension == "PNG" )
		{
			bitTrace( "[Bit::Image::ReadFile] Not supporting PNG images yet.\n" );
			return BIT_ERROR;
		}

		bitTrace( "[Bit::Image::ReadFile] Unknow extension: %s.\n", FileExtension.c_str( ) );

		return BIT_ERROR;
	}

	BIT_UINT32 Image::ReadTGA( const char * p_pFilePath )
	{
		// Open the file
		std::ifstream File( p_pFilePath, std::ios::binary );
		if(File.is_open() == false)
		{
			return BIT_ERROR_OPEN_FILE;
		}

		// Start to read the header data
		unsigned char Type[4];
		unsigned char Info[6];
		File.read((char*)Type, sizeof(unsigned char) * 3);

		// Seek to the position of the info data
		File.seekg(12);

		// Read the info data
		File.read((char*)Info, sizeof(unsigned char) * 6);

		// Check if the image is either a color or greyscale image
		// 2 == color, 3 == greyscale
		if( (Type[1] != 0 || Type[2] != 2) && Type[2] != 3 )
		{
			File.close();
			bitTrace( "[Image::ReadTGA] Wrong TGA type.\n" );
			return BIT_ERROR;
		}

		// Use the Info to calculate the size of the image
		m_Size.x = Info[0] + ( Info[1] * 256 );
		m_Size.y = Info[2] + ( Info[3] * 256 );
		m_Depth = Info[4] / 8;

		// Make sure the image is a 24 or 32 bit image
		if(m_Depth != 3 && m_Depth != 4)
		{
			File.close();
			bitTrace( "[Image::ReadTGA] Not a 24 or 32 bit depth image.\n" );
			return BIT_ERROR;
		}

		// Deallocate the old image data
		DeallocateData();

		// Get the full size of the image data and allocate memory for the data
		BIT_UINT32 ImageSize = GetDataSize( );
		m_pData = new BIT_BYTE[ImageSize];

		// Read the image data
		File.read((char*)m_pData, sizeof(BIT_BYTE) * ImageSize);

		// TGA is stored as BGR or BGRA, we have to swap the bytes
		if(m_Depth == 3)
		{
			BGR_To_RGB();
		}
		else if(m_Depth == 4)
		{
			BGRA_To_RGBA();
		}

		// Close the file since we are done.
		File.close();
		return BIT_OK;
	}


	// Set functions
	BIT_UINT32 Image::SetData( BIT_BYTE * p_pData, const Vector2_ui32 p_Size, const BIT_UINT32 p_Depth )
	{
		// Make sure the data pointer isn't NULL
		if(p_pData == BIT_NULL)
		{
			bitTrace( "[Image::AddData] Passed NULL pointer.\n" );
			return BIT_ERROR;
		}

		// Also, we don't accept the width/height/depth to be equal to zero.
		if( p_Size.x == 0 || p_Size.y == 0 || p_Depth == 0)
		{
			bitTrace( "[Image::AddData] With/Height/Depth is 0.\n" );
			return BIT_ERROR;
		}

		// Set the image data
		m_pData = p_pData;
		m_Size = p_Size;
		m_Depth = p_Depth;

		return BIT_OK;
	}

	void Image::SetPixel( const BIT_UINT32 p_Index, Pixel p_Pixel )
	{
		if( m_pData == BIT_NULL ||
			p_Index >= (m_Size.x * m_Size.y) )
		{
			return;
		}

		// Set the pixel
		m_pData[ ( p_Index * m_Depth ) ] = p_Pixel.m_R;
		m_pData[ ( p_Index * m_Depth ) + 1 ] = p_Pixel.m_G;
		m_pData[ ( p_Index * m_Depth ) + 2 ] = p_Pixel.m_B;

		// Are we using the alpha channel?
		if( m_Depth == 4 )
		{
			m_pData[ ( p_Index * m_Depth ) + 3 ] = p_Pixel.m_A;
		}
	}

	Pixel Image::GetPixel( const BIT_UINT32 p_Index )
	{
		if( m_pData == BIT_NULL ||
			p_Index >= (m_Size.x * m_Size.y) )
		{
			return Pixel( 0, 0, 0, 0 );
		}

		// Set the pixel that we want to return
		Pixel P;
		P.m_R = m_pData[ ( p_Index * m_Depth ) ] ;
		P.m_G = m_pData[ ( p_Index * m_Depth ) + 1];
		P.m_B = m_pData[ ( p_Index * m_Depth ) + 2];

		// Are we using the alpha channel?
		if( m_Depth == 4 )
		{
			P.m_A = m_pData[ ( p_Index * m_Depth ) + 3 ];
		}

		return P;
	}

	void Image::DeallocateData( )
	{
		if(m_pData != BIT_NULL)
		{
			delete [ ] m_pData;
			m_pData = BIT_NULL;
			m_Size = Vector2_ui32( 0, 0 );
			m_Depth = 0;
		}
	}

	BIT_BOOL Image::ContainsData( ) const
	{
		return (m_pData != BIT_NULL);
	}

	// Get functions
	BIT_BYTE * Image::GetData( ) const
	{
		return m_pData;
	}

	Vector2_ui32 Image::GetSize( ) const
	{
		return m_Size;
	}

	BIT_UINT32 Image::GetDataSize( ) const
	{
		return m_Size.x * m_Size.y * m_Depth;
	}

	BIT_UINT32 Image::GetDepth( ) const
	{
		return m_Depth;
	}

	// Conversion functions
	void Image::BGR_To_RGB( )
	{
		// Make sure we have any data to swap
		if( ContainsData( ) == BIT_FALSE )
		{
			bitTrace( "[Image::BGR_To_RGB] <ERROR> "
				"Image not containing any data.\n" );
			return;
		}


		BIT_UINT32 Size = GetDataSize( );
		BIT_BYTE TemporaryByte = 0;

		// Loop through the data we want to swap
		for( BIT_UINT32 i = 0; i < Size; i += 3 )
		{
			TemporaryByte = m_pData[ i ];
			m_pData[ i ] = m_pData[ i + 2 ];
			m_pData[ i + 2 ] = TemporaryByte;
		}

	}

	void Image::BGRA_To_RGBA( )
	{
		// Make sure we have any data to swap
		if( ContainsData( ) == BIT_FALSE )
		{
			bitTrace( "[Image::BGRA_To_RGBA] Image not containing any data.\n" );
			return;
		}

		BIT_UINT32 Size = GetDataSize( );
		BIT_BYTE TemporaryByte = 0;

		// Loop through the data we want to swap
		for ( BIT_UINT32 i = 0; i < Size; i += 4 )
		{
			TemporaryByte = m_pData[ i ];
			m_pData[ i ] = m_pData[ i + 2 ];
			m_pData[ i + 2 ] = TemporaryByte;
		}
	}

}



