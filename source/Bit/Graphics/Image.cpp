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
#include <Bit/Graphics/TgaFile.hpp>
#include <Bit/Graphics/BmpFile.hpp>
#include <Bit/Graphics/PngFile.hpp>
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
		m_PixelDepth( 0 )
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
		m_PixelDepth = p_Depth;

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
		for( Int32 i = p_Filename.size( ) - 2; i >= 0; i-- )
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
			return LoadFromPngFile( p_Filename ); 
		}
		else if( fileExtension == "JPG" )
		{
			std::cout << "[Image::LoadFromFile] Not supporting JPG images yet.\n";
			return false;
		}

		// Unknown extension
		std::cout << "[Image::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		return false;
	}

	Bool Image::LoadFromTgaFile( const std::string & p_Filename )
	{
		// Read the TGA file.
		TgaFile tga( false );
		if( tga.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Get the image type
		if( tga.GetHeader( ).GetImageType( ) != TgaFile::UncompressedTrueColorImage )
		{
			std::cout << "[Image::LoadFromTgaFile] Not an uncompressed true color image.\n";
			delete tga.GetData( ); // Delete the data by hand
			return false;
		}

		// Get the pixel depth
		m_PixelDepth = tga.GetHeader( ).GetImageSpec( ).GetPixelDepth( ) / 8;

		if( m_PixelDepth != 3 && m_PixelDepth != 4 )
		{
			std::cout << "[Bit::Image::LoadFromTgaFile] Wrong pixel depth format: " << (Int32)m_PixelDepth << " bytes.\n";
			delete tga.GetData( ); // Delete the data by hand
			return false;
		}

		// Get the image size
		m_Size.x = static_cast<Uint32>( tga.GetHeader( ).GetImageSpec( ).GetImageWidth( ) );
		m_Size.y = static_cast<Uint32>( tga.GetHeader( ).GetImageSpec( ).GetImageHeight( ) );

		// Delete the old data
		if( m_pData != NULL )
		{
			delete [ ] m_pData;
		}

		// Get the image data
		m_pData = const_cast<Uint8 *>( tga.GetData( ) );

		// Convert to the right color component order
		if( m_PixelDepth == 3 )
		{
			BgrToRgb( );
		}
		else if( m_PixelDepth == 4 )
		{
			BgraToRgba( );
		}

		// Succeeded.
		return true;
	}

	Bool Image::LoadFromBmpFile( const std::string & p_Filename )
	{
		// Read the TGA file.
		BmpFile bmp( false );
		if( bmp.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Get the image type
		if( bmp.GetDibHeader( ).GetCompression( ) != BmpFile::NoCompression )
		{
			std::cout << "[Bit::Image::LoadFromBmpFile] Not an uncompressed true color image.\n";
			delete bmp.GetData( ); // Delete the data by hand
			return false;
		}

		// Get the pixel depth
		m_PixelDepth = static_cast<Uint8>( bmp.GetDibHeader( ).GetPixelDepth( ) / 8 );

		if( m_PixelDepth != 3 && m_PixelDepth != 4 )
		{
			std::cout << "[Bit::Image::LoadFromBmpFile] Wrong pixel depth format: " << (Int32)m_PixelDepth << " bytes.\n";
			delete bmp.GetData( ); // Delete the data by hand
			return false;
		}

		// Get the image size
		m_Size.x = static_cast<Uint32>( bmp.GetDibHeader( ).GetBitmapWidth( ) );
		m_Size.y = static_cast<Uint32>( bmp.GetDibHeader( ).GetBitmapHeight( ) );

		// Delete the old data
		if( m_pData != NULL )
		{
			delete [ ] m_pData;
		}

		// Get the image data
		m_pData = const_cast<Uint8 *>( bmp.GetData( ) );

		// Convert to the right color component order
		if( m_PixelDepth == 3 )
		{
			BgrToRgb( );
		}
		else if( m_PixelDepth == 4 )
		{
			BgraToRgba( );
		}

		return true;
	}

	Bool Image::LoadFromPngFile( const std::string & p_Filename )
	{
		// Read the PNG file.
		PngFile png( false );
		if( png.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Get the pixel depth
		m_PixelDepth = png.GetPixelDepth( ) / 8;

		if( m_PixelDepth != 3 && m_PixelDepth != 4 )
		{
			std::cout << "[Bit::Image::LoadFromPngFile] Wrong pixel depth format: " << m_PixelDepth << " bytes.\n";
			delete png.GetData( ); // Delete the data by hand
			return false;
		}

		// Get the image size
		m_Size = png.GetImageSize( );

		// Delete the old data
		if( m_pData != NULL )
		{
			delete [ ] m_pData;
		}

		// Get the image data
		m_pData = const_cast<Uint8 *>( png.GetData( ) );

		// Succeeded.
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
			m_PixelDepth = 0;
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
		SizeType pos = p_Index * m_PixelDepth;
		m_pData[ pos ]		= p_Pixel.Red;
		m_pData[ pos + 1 ]	= p_Pixel.Green;
		m_pData[ pos + 2 ]	= p_Pixel.Blue;

		// Are we using the alpha channel?
		if( m_PixelDepth == 4 )
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
		SizeType pos = p_Index * m_PixelDepth;
		Pixel pixel( 0, 0, 0, 255 );
		pixel.Red	= m_pData[ pos ] ;
		pixel.Green	= m_pData[ pos + 1];
		pixel.Blue	= m_pData[ pos + 2];

		// Are we using the alpha channel?
		if( m_PixelDepth == 4 )
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
		return static_cast<SizeType>( m_Size.x * m_Size.y ) * static_cast<SizeType>( m_PixelDepth );
	}

	Uint8 Image::GetPixelDepth( ) const
	{
		return m_PixelDepth;
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



