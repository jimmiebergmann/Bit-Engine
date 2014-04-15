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

	// Color map specification class
	TgaFile::ColorMapSpecification::ColorMapSpecification( ) :
		m_Offset( 0 ),
		m_Length( 0 ),
		m_EntrySize( 0 )
	{
	}

	void TgaFile::ColorMapSpecification::SetOffset( const Uint16 p_Offset )
	{
		m_Offset = p_Offset;
	}

	void TgaFile::ColorMapSpecification::SetLength( const Uint16 p_Length )
	{
		m_Length = p_Length;
	}

	void TgaFile::ColorMapSpecification::SetEntrySize( const Uint8 p_EntrySize )
	{
		m_EntrySize = p_EntrySize;
	}

	Uint16 TgaFile::ColorMapSpecification::GetOffset( ) const
	{
		return m_Offset;
	}

	Uint16 TgaFile::ColorMapSpecification::GetLength( ) const
	{
		return m_Length;
	}

	Uint8 TgaFile::ColorMapSpecification::GetEntrySize( ) const
	{
		return m_EntrySize;
	}

	// Image specification class
	TgaFile::ImageSpecification::ImageSpecification( ) :
		m_OriginX( 0 ),
		m_OriginY( 0 ),
		m_ImageWidth( 0 ),
		m_ImageHeight( 0 ),
		m_PixelDepth( 0 ),
		m_ImageDescriptor( 0 )
	{
	}


	Uint16 TgaFile::ImageSpecification::GetOriginX( ) const
	{
		return m_OriginX;
	}

	Uint16 TgaFile::ImageSpecification::GetOriginY( ) const
	{
		return m_OriginY;
	}

	Uint16 TgaFile::ImageSpecification::GetImageWidth( ) const
	{
		return m_ImageWidth;
	}

	Uint16 TgaFile::ImageSpecification::GetImageHeight( ) const
	{
		return m_ImageHeight;
	}

	Uint8 TgaFile::ImageSpecification::GetPixelDepth( ) const
	{
		return m_PixelDepth;
	}

	Uint8 TgaFile::ImageSpecification::GetImageDescriptor( ) const
	{
		return m_ImageDescriptor;
	}

	void TgaFile::ImageSpecification::SetOriginX( const Uint16 p_OriginX )
	{
		m_OriginX = p_OriginX;
	}

	void TgaFile::ImageSpecification::SetOriginY( const Uint16 p_OriginY )
	{
		m_OriginY = p_OriginY;
	}

	void TgaFile::ImageSpecification::SetImageWidth( const Uint16 p_ImageWidth )
	{
		m_ImageWidth = p_ImageWidth;
	}

	void TgaFile::ImageSpecification::SetImageHeight( const Uint16 p_ImageHeight )
	{
		m_ImageHeight = p_ImageHeight;
	}

	void TgaFile::ImageSpecification::SetPixelDepth( const Uint8 p_PixelDepth )
	{
		m_PixelDepth = p_PixelDepth;
	}

	void TgaFile::ImageSpecification::SetImageDescriptor( const Uint8 p_ImageDescriptor )
	{
		m_ImageDescriptor = p_ImageDescriptor;
	}

	// Header class
	TgaFile::Header::Header( ) :
		m_IdLength( 0 ),
		m_ColorMapType( 0 ),
		m_ImageType( 0 ),
		m_ColorMapSpec( ),
		m_ImageSpec( )
	{
	}

	void TgaFile::Header::SetIdLength( const Uint8 p_IdLength )
	{
		m_IdLength = p_IdLength;
	}

	void TgaFile::Header::SetColorMapType( const Uint8 p_ColorMapType )
	{
		m_ColorMapType = p_ColorMapType;
	}

	void TgaFile::Header::SetImageType( const Uint8 p_ImageType )
	{
		m_ImageType = p_ImageType;
	}

	Uint8 TgaFile::Header::GetIdLength( ) const
	{
		return m_IdLength;
	}

	Uint8 TgaFile::Header::GetColorMapType( ) const
	{
		return m_ColorMapType;
	}

	Uint8 TgaFile::Header::GetImageType(  ) const
	{
		return m_ImageType;
	}

	TgaFile::ColorMapSpecification & TgaFile::Header::GetColorMapSpec( )
	{
		return m_ColorMapSpec;
	}

	TgaFile::ImageSpecification & TgaFile::Header::GetImageSpec( )
	{
		return m_ImageSpec;
	}

	// Footer class
	TgaFile::Footer::Footer( ) :
		m_ExtensionOffset( 0 ),
		m_DeveloperAreaOffset( 0 ),
		m_Dot( 0 ),
		m_End( 0 )
	{
	}

	// Tga file class
	TgaFile::TgaFile( const Bool p_DeallocateFlag ) :
		m_Header( ),
		m_Footer( ),
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_DeallocateFlag( p_DeallocateFlag )
	{
	}

	TgaFile::~TgaFile( )
	{
		// Delete the allocated data
		if( m_DeallocateFlag && m_pData )
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
		fin.read( reinterpret_cast<char *>( &m_Header.m_IdLength ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ColorMapType ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageType ), 1 );
		// Read color map specifications
		fin.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Offset ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Length ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_EntrySize ), 1 );
		// Read image specifications
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginX ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginY ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageWidth ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageHeight ), 2 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_PixelDepth ), 1 );
		fin.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageDescriptor ), 1 );

		// Error check the header field.
		if( m_Header.m_ImageSpec.m_PixelDepth != 8 &&
			m_Header.m_ImageSpec.m_PixelDepth != 16 &&
			m_Header.m_ImageSpec.m_PixelDepth != 24 &&
			m_Header.m_ImageSpec.m_PixelDepth != 32 )
		{
			std::cout	<< "[TgaFile::LoadFromFile] Not supporting "
						<< (int)m_Header.m_ImageSpec.m_PixelDepth << " bit pixel depth." << std::endl;
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

		if(	m_Header.GetImageType( ) == UncompressedGrayscaleImage && m_Header.m_ImageSpec.m_PixelDepth != 8 )
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
		m_DataSize =	static_cast<SizeType>( m_Header.m_ImageSpec.m_ImageWidth ) *
						static_cast<SizeType>( m_Header.m_ImageSpec.m_ImageHeight ) *
						static_cast<SizeType>( m_Header.m_ImageSpec.m_PixelDepth / 8 );

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

	const Uint8 * TgaFile::GetData( ) const
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

	Bool TgaFile::GetDeallocatorFlag( ) const
	{
		return m_DeallocateFlag;
	}

}