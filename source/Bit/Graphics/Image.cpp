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
#include <algorithm>
#include <fstream>
#include <cstring>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Pixel class
	Pixel::Pixel( )
	{
	}

	Pixel::Pixel(	const Uint8 p_Red,
					const Uint8 p_Green,
					const Uint8 p_Blue,
					const Uint8 p_Alpha ) :
		Red( p_Red ),
		Green( p_Green ),
		Blue( p_Blue ),
		Alpha( p_Alpha )
	{
	}

	Pixel::Pixel( Vector3u8 p_RGB, const Uint8 p_Alpha ) :
		Red( p_RGB.x ),
		Green( p_RGB.y ),
		Blue( p_RGB.z ),
		Alpha( p_Alpha )
	{
	}

	Bool Pixel::operator == ( const Pixel & p_Pixel ) const
	{
		return	Red == p_Pixel.Red &&
				Green == p_Pixel.Green &&
				Blue == p_Pixel.Blue &&
				Alpha == p_Pixel.Alpha;
	}


	// Image class
	Image::Image( ) :
		m_pData( NULL ),
		m_Size( 0, 0 ),
		m_Depth( 0 )
	{
	}

	Image::~Image( )
	{
		Clear( );
	}

	Bool Image::LoadFromMemory( const Uint8 * p_pData, const Uint8 p_Depth, const Vector2u32 & p_Size )
	{
		// Make params are valid.
		if( p_pData == NULL ||
			p_Size.x == 0 || p_Size.y == 0 ||
			p_Depth == 0 || p_Depth > 4 )
		{
			return false;
		}

		// Clear the image
		Clear( );

		// Set the size and depth
		m_Size = p_Size;
		m_Depth = p_Depth;

		// Allocate an array for the image data to copy
		Uint32 imageSize = GetDataSize( );
		m_pData = new Uint8[ imageSize ];

		// Copy the data
		memcpy( m_pData, p_pData, imageSize );

		return true;
	}

	Bool Image::LoadFromFile( const std::string & p_Filename )
	{
		
		// Get the file's extension
		std::string fileExtension = "";
		for( SizeType i = p_Filename.size( ) - 2; i >= 0; i-- )
		{
			// Look for '.'
			if( p_Filename[ i ] == '.' )
			{
				fileExtension = p_Filename.substr( i + 1, p_Filename.size( ) - i - 1 );
				break;
			}
		}

		// Make all the characters in the file extension to upper case letters
		std::transform( fileExtension.begin( ), fileExtension.end( ), fileExtension.begin( ), ::toupper );

		// Load the right format.
		if( fileExtension == "TGA" )
		{
			return LoadFromTgaFile( p_Filename );
		}
		else if( fileExtension == "BMP" )
		{
			return LoadFromBmpFile( p_Filename );
		}
		else if( fileExtension == "PNG" )
		{
			std::cout << "[Bit::Image::LoadFromFile] Not supporting PNG images yet.\n";
			return false;
		}
		else if( fileExtension == "JPG" )
		{
			std::cout << "[Bit::Image::LoadFromFile] Not supporting JPG images yet.\n";
			return false;
		}

		std::cout << "[Bit::Image::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		
		return true;
	}

	Bool Image::LoadFromTgaFile( const std::string & p_Filename )
	{
		// Open the file
		std::ifstream fin( p_Filename.c_str( ), std::ios::binary );
		if( fin.is_open( ) == false )
		{
			std::cout << "[Image::LoadFromTgaFile] Can not open the file.\n";
			return false;
		}

		// Get the file size
		fin.seekg( 0, std::fstream::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::fstream::beg );

		// Make sure that the file has enought space for the header data.
		if( fileSize < 18 )
		{
			std::cout << "[Image::LoadFromTgaFile] File size error 1.\n";
			return false;
		}

		// Start to read the header data
		unsigned char type[4];
		unsigned char info[6];
		fin.read( reinterpret_cast<char *>( type ), 3 );

		// Seek to the position of the info data
		fin.seekg(12);

		// Read the info data
		fin.read( reinterpret_cast<char *>( info ), 6 );

		// Check if the image is either a color or greyscale image
		// 2 == color, 3 == greyscale
		if( (type[1] != 0 || type[2] != 2) && type[2] != 3 )
		{
			fin.close( );
			std::cout << "[Image::LoadFromTgaFile] Wrong TGA type.\n";
			return false;
		}

		// Clear the old image data
		Clear( );

		// Use the Info to calculate the size of the image
		m_Size.x = info[0] + ( info[1] * 256 );
		m_Size.y = info[2] + ( info[3] * 256 );
		m_Depth = info[4] / 8;

		// Get the full size of the image data
		Uint32 imageSize = GetDataSize( );

		// Make sure that the file has enought space for the pixel data.
		if( fileSize < 18 + imageSize  )
		{
			std::cout << "[Image::LoadFromTgaFile] File size error 1.\n";
			Clear( );
			return false;
		}

		// Make sure the image is a 24 or 32 bit image
		if( m_Depth != 3 && m_Depth != 4 )
		{
			std::cout << "[Image::LoadFromTgaFile] Not a 24 or 32 bit depth image.\n";
			fin.close( );
			Clear( );
			return false;
		}

		// Allocate the pixel data
		m_pData = new Uint8[ imageSize ];

		// Read the image data
		fin.read( reinterpret_cast<char *>( m_pData ), imageSize);

		// TGA is stored as BGR or BGRA, we have to swap the bytes
		if( m_Depth == 3 )
		{
			BgrToRgb( );
		}
		else if( m_Depth == 4 )
		{
			BgraToRgba( );
		}

		// Close the file since we are done.
		fin.close();
		return true;
	}

	Bool Image::LoadFromBmpFile( const std::string & p_Filename )
	{
		// Open the file
		std::ifstream fin( p_Filename.c_str( ), std::ios::binary );
		if( fin.is_open( ) == false )
		{
			std::cout << "[Image::LoadFromBmpFile] Can not open the file.\n";
			return false;
		}

		// Get the file size
		fin.seekg( 0, std::fstream::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::fstream::beg );

		// Make sure that the file has enought space for both headers
		if( fileSize < 54 )
		{
			std::cout << "[Image::LoadFromTgaFile] File size error 1.\n";
			return false;
		}

		// Read the Bitmap file header
		Uint8 magic[ 2 ];
		fin.read( reinterpret_cast<char*>( magic ), 2 );

		// Error check the magic field
		if( magic[ 0 ] != 'B' || magic[ 1 ] != 'M' )
		{
			fin.close( );
			std::cout << "[Image::LoadFromBmpFile] Not a BMP file.\n";
			return false;
		}
		
		// Read the bitmap information header(DIB)
		// Read the pixel data offset
		fin.seekg( 10, std::fstream::beg );
		Uint32 pixelDataOffset;
		fin.read( reinterpret_cast<char*>( &pixelDataOffset ), 4 );

		// Clear the old image data
		Clear( );

		// Read the image size
		Int32 imageWith;
		Int32 imageHeight;
		fin.seekg( 18, std::fstream::beg );
		fin.read( reinterpret_cast<char*>( &imageWith ), 4 );
		fin.read( reinterpret_cast<char*>( &imageHeight ), 4 );
		m_Size.x = static_cast<Uint32>( imageWith );
		m_Size.y = static_cast<Uint32>( imageHeight );

		// Read the depth
		fin.seekg( 28, std::fstream::beg );
		Uint16 depth;
		fin.read( reinterpret_cast<char*>( &depth ), 2 );
		m_Depth = static_cast<Uint8>( depth ) / 8;

		// Error check the depth
		if( depth != 24 && depth != 32 )
		{
			std::cout << "[Image::LoadFromBmpFile] Not a 24 or 32 bit depth image.\n";
			fin.close( );
			Clear( );
			return false;
		}

		// Error check the size
		Uint32 rowSize = ( ( depth * imageWith + 31 ) / 32 ) * 4;
		Uint32 dataSize = rowSize * imageHeight;
		Uint32 imageSize = GetDataSize( );
		if( dataSize != GetDataSize( ) )
		{
			std::cout << "[Image::LoadFromBmpFile] Wring data size.\n";
			fin.close( );
			Clear( );
			return false;
		}
		
		if( fileSize < pixelDataOffset + dataSize )
		{
			std::cout << "[Image::LoadFromBmpFile] File size error 2.\n";
			fin.close( );
			Clear( );
			return false;
		}

		// Read the compression method
		Uint8 compression;
		fin.read( reinterpret_cast<char*>( &compression ), 1 );
		if( compression != 0 )
		{
			fin.close( );
			std::cout << "[Image::LoadFromBmpFile] Not currently supporting compression.\n";
			return false;
		}

		// Go to the pixel data
		fin.seekg( pixelDataOffset, std::fstream::beg );

		// Allocate the pixel data
		m_pData = new Uint8[ imageSize ];

		// Read the pixel data
		fin.read( reinterpret_cast<char *>( m_pData ), imageSize );

		// Set the alpha value
		for( SizeType i = 0; i < m_Size.x * m_Size.y; i++ )
		{
			m_pData[ 3 + ( i * 4 ) ] = 255;
		}

		// Convert the pixel format
		BgraToRgba( );

		// Succeeded
		return true;
	}

	void Image::Clear( )
	{
		if( m_pData != NULL )
		{
			// Delete the allocated data
			delete [ ] m_pData;
			m_pData = NULL;

			// Clear the size and depth
			m_Size = Vector2u32( 0, 0 );
			m_Depth = 0;
		}
	}

	void Image::SetPixel( const Uint32 p_Index, const Pixel & p_Pixel )
	{
		if( m_pData == NULL ||
			p_Index >= ( m_Size.x * m_Size.y ) )
		{
			return;
		}

		// Set the pixel
		SizeType pos = p_Index * m_Depth;
		m_pData[ pos ]		= p_Pixel.Red;
		m_pData[ pos + 1 ]	= p_Pixel.Green;
		m_pData[ pos + 2 ]	= p_Pixel.Blue;

		// Are we using the alpha channel?
		if( m_Depth == 4 )
		{
			m_pData[ pos + 3 ] = p_Pixel.Alpha;
		}
	}

	void Image::SetPixel( const Vector2u32 & p_Position, const Pixel & p_Pixel )
	{
		Uint32 index = ( p_Position.y * m_Size.x ) + p_Position.x;
		SetPixel( index, p_Pixel );
	}

	Pixel Image::GetPixel( const Uint32 p_Index )
	{
		if( m_pData == NULL ||
			p_Index >= ( m_Size.x * m_Size.y ) )
		{
			return Pixel( 0, 0, 0, 255 );
		}

		// Set the pixel that we want to return
		SizeType pos = p_Index * m_Depth;
		Pixel pixel( 0, 0, 0, 255 );
		pixel.Red	= m_pData[ pos ] ;
		pixel.Green	= m_pData[ pos + 1];
		pixel.Blue	= m_pData[ pos + 2];

		// Are we using the alpha channel?
		if( m_Depth == 4 )
		{
			pixel.Alpha = m_pData[ pos + 3 ];
		}

		return pixel;
	}

	Pixel Image::GetPixel( const Vector2u32 & p_Position )
	{
		Uint32 index = ( p_Position.y * m_Size.x ) + p_Position.x;
		return GetPixel( index );
	}

	/*
	Bool Image::ContainsData( ) const
	{
		return ( m_pData != BIT_NULL);
	}*/

	const Uint8 * Image::GetData( ) const
	{
		return m_pData;
	}

	Vector2u32 Image::GetSize( ) const
	{
		return m_Size;
	}

	SizeType Image::GetDataSize( ) const
	{
		return static_cast<SizeType>( m_Size.x * m_Size.y * static_cast<Uint32>( m_Depth ) );
	}

	Uint8 Image::GetDepth( ) const
	{
		return m_Depth;
	}

	// Conversion functions
	void Image::BgrToRgb( )
	{
		// Make sure we have any data to swap
		if( m_pData == NULL )
		{
			std::cout << "[Image::BgrToRgb] <ERROR> "
				"Image not containing any data.\n";
			return;
		}


		SizeType size = GetDataSize( );
		Uint8 temporaryByte = 0;

		// Loop through the data we want to swap
		for( SizeType i = 0; i < size; i += 3 )
		{
			temporaryByte = m_pData[ i ];
			m_pData[ i ] = m_pData[ i + 2 ];
			m_pData[ i + 2 ] = temporaryByte;
		}

	}

	void Image::BgraToRgba( )
	{
		// Make sure we have any data to swap
		if( m_pData == NULL )
		{
			std::cout << "[Image::BgraToRgba] Image not containing any data.\n";
			return;
		}

		SizeType size = GetDataSize( );
		Uint8 temporaryByte = 0;

		// Loop through the data we want to swap
		for ( SizeType i = 0; i < size; i += 4 )
		{
			temporaryByte = m_pData[ i ];
			m_pData[ i ] = m_pData[ i + 2 ];
			m_pData[ i + 2 ] = temporaryByte;
		}
	}

}



