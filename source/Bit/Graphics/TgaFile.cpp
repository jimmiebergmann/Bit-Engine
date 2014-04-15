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

#include <Bit/Graphics/TgaFile.hpp>
#include <fstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Color map specification struct
	TgaFile::ColorMapSpecification::ColorMapSpecification( ) :
		Offset( 0 ),
		Length( 0 ),
		EntrySize( 0 )
	{
	}

	// Image specification struct
	TgaFile::ImageSpecification::ImageSpecification( ) :
		OriginX( 0 ),
		OriginY( 0 ),
		ImageWidth( 0 ),
		ImageHeight( 0 ),
		PixelDepth( 0 ),
		ImageDescriptor( 0 )
	{
	}

	// Header class
	TgaFile::Header::Header( ) :
		IdLength( 0 ),
		ColorMapType( 0 ),
		ImageType( 0 ),
		ColorMapSpec( ),
		ImageSpec( )
	{
	}

	void TgaFile::Header::SetImageType( const eImageType & p_ImageType )
	{
		ImageType =  static_cast<Uint8>( p_ImageType );
	}

	TgaFile::eImageType TgaFile::Header::GetImageType(  ) const
	{
		return static_cast<eImageType>( ImageType );
	}

	// Footer struct
	TgaFile::Footer::Footer( ) :
		ExtensionOffset( 0 ),
		DeveloperAreaOffset( 0 ),
		Dot( 0 ),
		End( 0 )
	{
	}

	// Tga file class
	TgaFile::TgaFile( const Bool p_DeallocateData ) :
		m_Header( ),
		m_Footer( ),
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_DeallocateData( p_DeallocateData )
	{
	}

	TgaFile::~TgaFile( )
	{
		// Delete the allocated data
		if( m_DeallocateData && m_pData )
		{
			delete m_pData;
		}
	}

	Bool TgaFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ) );
		if( fin.is_open( ) == false )
		{
			std::cout << "[TgaFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		// Read the file size.
		fin.seekg( 0, std::fstream::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::fstream::beg );

		// Error check the file size
		if( fileSize < 18 )
		{
			std::cout << "[TgaFile::LoadFromFile] Missing header field." << std::endl;
			fin.close( );
			return false;
		}

		// Read the header.
		fin.read( reinterpret_cast<char *>( &m_Header.IdLength ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.ColorMapType ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageType ), 1 );
		// Read color map specifications
		fin.read( reinterpret_cast<char *>( &m_Header.ColorMapSpec.Offset ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ColorMapSpec.Length ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ColorMapSpec.EntrySize ), 1 );
		// Read image specifications
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.OriginX ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.OriginY ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.ImageWidth ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.ImageHeight ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.PixelDepth ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.ImageSpec.ImageDescriptor ), 1 );

		// Error check the header field.
		if( m_Header.ImageSpec.PixelDepth != 8 &&
			m_Header.ImageSpec.PixelDepth != 16 &&
			m_Header.ImageSpec.PixelDepth != 24 &&
			m_Header.ImageSpec.PixelDepth != 32 )
		{
			std::cout	<< "[TgaFile::LoadFromFile] Not supporting "
						<< (int)m_Header.ImageSpec.PixelDepth << " bit pixel depth." << std::endl;
			fin.close( );
			return false;
		}

		// Error check the image type
		if( m_Header.GetImageType( ) != UncompressedTrueColorImage &&
			m_Header.GetImageType( ) != UncompressedGrayscaleImage )
		{
			std::cout	<< "[TgaFile::LoadFromFile] Not supporting color mapped or compressed images." << std::endl;
			fin.close( );
			return false;
		}

		if(	m_Header.GetImageType( ) == UncompressedGrayscaleImage && m_Header.ImageSpec.PixelDepth != 8 )
		{
			std::cout	<< "[TgaFile::LoadFromFile] Not supporting non 8 bit grayscale iamges." << std::endl;
			fin.close( );
			return false;
		}

		// Clear the old pixel data.
		if( m_pData )
		{
			delete m_pData;
		}

		// Calcualte the data size.
		m_DataSize =	static_cast<SizeType>( m_Header.ImageSpec.ImageWidth ) *
						static_cast<SizeType>( m_Header.ImageSpec.ImageHeight ) *
						static_cast<SizeType>( m_Header.ImageSpec.PixelDepth / 8 );

		// Read the data if we are expecting any data.
		if( m_DataSize != 0 )
		{
			// Check if the data fits in the file
			if( fileSize < m_DataSize + 18 )
			{
				std::cout << "[TgaFile::LoadFromFile] The expected data size is too large for the opened file." << std::endl;
				return false;
			}

			// Allocate the data
			m_pData = new Uint8[ m_DataSize ];

			// Read the bitmap data
			fin.read( reinterpret_cast<char *>( m_pData ), m_DataSize );
		}

		// Read the footer( optional )
		if( fileSize >= 44 + m_DataSize )
		{
			// Seek to the end where the footer is expected to be
			fin.seekg( 26, std::fstream::end );

			// Read the footer
			fin.read( reinterpret_cast<char *>( &m_Footer ), 26 );
		}

		// Succeeded.
		return true;
	}

	void TgaFile::Clear( )
	{
		// Delete the allocated data
		if( m_pData )
		{
			delete m_pData;
		}
	}

	TgaFile::Header & TgaFile::GetHeader( )
	{
		return m_Header;
	}

	TgaFile::Footer & TgaFile::GetFooter( )
	{
		return m_Footer;
	}

	Uint8 * TgaFile::GetData( ) const
	{
		return m_pData;
	}

	SizeType TgaFile::GetDataSize( ) const
	{
		return m_DataSize;
	}

	void TgaFile::SetData( const Uint8 * p_pData, const SizeType p_DataSize )
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

}