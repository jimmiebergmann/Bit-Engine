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
#include <sstream>
#include <iostream>
#include <string.h>
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
		m_Dot( '.' ),
		m_End( 0 )
	{
		memcpy( m_Signature, "TRUEVISION-XFILE", 16 );
	}

	void TgaFile::Footer::SetExtensionOffset( const Uint32 p_ExtensionOffset )
	{
		m_ExtensionOffset = p_ExtensionOffset;
	}

	void TgaFile::Footer::SetDeveloperAreaOffset( const Uint32 p_DeveloperAreaOffset )
	{
		m_DeveloperAreaOffset = p_DeveloperAreaOffset;
	}

	Uint32 TgaFile::Footer::GetExtensionOffset( ) const
	{
		return m_ExtensionOffset;
	}

	Uint32 TgaFile::Footer::GetDeveloperAreaOffset( ) const
	{
		return m_DeveloperAreaOffset;
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

	Bool TgaFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.str( p_Memory );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool TgaFile::LoadFromStream( std::istream & p_Stream )
	{
		// Read the stream size.
		p_Stream.seekg( 0, std::fstream::end );
		SizeType fileSize = static_cast<SizeType>( p_Stream.tellg( ) );
		p_Stream.seekg( 0, std::fstream::beg );

		// Error check the stream size
		if( fileSize < 18 )
		{
			std::cout << "[TgaFile::LoadFromStream] Missing header field." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Read the header.
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_IdLength ), 1 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ColorMapType ), 1 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageType ), 1 );
		// Read color map specifications
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Offset ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Length ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_EntrySize ), 1 );
		// Read image specifications
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginX ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginY ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageWidth ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageHeight ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_PixelDepth ), 1 );
		p_Stream.read( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageDescriptor ), 1 );

		// Error check the header field.
		if( m_Header.m_ImageSpec.m_PixelDepth != 8 &&
			m_Header.m_ImageSpec.m_PixelDepth != 16 &&
			m_Header.m_ImageSpec.m_PixelDepth != 24 &&
			m_Header.m_ImageSpec.m_PixelDepth != 32 )
		{
			std::cout	<< "[TgaFile::LoadFromStream] Not supporting "
						<< (int)m_Header.m_ImageSpec.m_PixelDepth << " bit pixel depth." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Error check the image type
		if( m_Header.GetImageType( ) != UncompressedTrueColorImage &&
			m_Header.GetImageType( ) != UncompressedGrayscaleImage )
		{
			std::cout	<< "[TgaFile::LoadFromStream] Not supporting color mapped or compressed images." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		if(	m_Header.GetImageType( ) == UncompressedGrayscaleImage && m_Header.m_ImageSpec.m_PixelDepth != 8 )
		{
			std::cout	<< "[TgaFile::LoadFromStream] Not supporting non 8 bit grayscale iamges." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
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
				std::cout << "[TgaFile::LoadFromStream] The expected data size is too large." << std::endl;
				p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
				return false;
			}

			// Allocate the data
			m_pData = new Uint8[ m_DataSize ];

			// Read the bitmap data
			p_Stream.read( reinterpret_cast<char *>( m_pData ), m_DataSize );
		}

		// Read the footer( optional )
		if( fileSize >= 44 + m_DataSize )
		{
			// Seek to the end where the footer is expected to be
			p_Stream.seekg( 26, std::fstream::end );

			// Read the footer
			p_Stream.read( reinterpret_cast<char *>( &m_Footer.m_ExtensionOffset ), 4 );
			p_Stream.read( reinterpret_cast<char *>( &m_Footer.m_DeveloperAreaOffset ), 4 );
			p_Stream.read( reinterpret_cast<char *>( m_Footer.m_Signature ), 16 );
			p_Stream.read( reinterpret_cast<char *>( &m_Footer.m_Dot ), 1 );
			p_Stream.read( reinterpret_cast<char *>( &m_Footer.m_End ), 1 );
		}

		// Go back to the begining of the stream
		p_Stream.seekg( 0, std::fstream::beg );

		// Succeeded.
		return true;
	}

	Bool TgaFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ), std::fstream::binary );
		if( fin.is_open( ) == false )
		{
			std::cout << "[TgaFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		// Read the stream
		Bool status = LoadFromStream( fin );

		// Close the file
		fin.close( );

		// Return the status
		return status;
	}

	Bool TgaFile::SaveToMemory( std::string & p_Memory, const Bool p_Validate )
	{
		// Load a string stream
		std::stringstream ss;

		// Save the stream
		if( SaveToStream( ss, p_Validate ) == false )
		{
			return false;
		}

		// Get the string
		p_Memory = ss.str( );

		// Succeeded
		return true;
	}

	Bool TgaFile::SaveToStream( std::ostream & p_Stream, const Bool p_Validate )
	{
		// Error check the data
		if( m_pData == NULL || m_DataSize == 0 )
		{
			std::cout << "[TgaFile::SaveToStream] No image data." << std::endl;
			return false;
		}

		// Write header data
		if( p_Validate )
		{
			Uint8 temp8 = 0;
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
			temp8 = 2;
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_IdLength ), 1 );
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ColorMapType ), 1 );
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageType ), 1 );
		}

		// Write color map specifications
		if( p_Validate )
		{
			Uint16 temp16 = 0;
			Uint8 temp8 = 0;
			p_Stream.write( reinterpret_cast<char *>( &temp16 ), 2 );
			p_Stream.write( reinterpret_cast<char *>( &temp16 ), 2 );
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Offset ), 2 );
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_Length ), 2 );
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ColorMapSpec.m_EntrySize ), 1 );
		}
		// Write image specifications
		p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginX ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_OriginY ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageWidth ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageHeight ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_PixelDepth ), 1 );

		if( p_Validate )
		{
			Uint8 descriptor = 0;
			if( m_Header.m_ImageSpec.m_PixelDepth == 32 )
			{
				descriptor = 8;
			}
			p_Stream.write( reinterpret_cast<char *>( &descriptor ), 1 );
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( &m_Header.m_ImageSpec.m_ImageDescriptor ), 1 );
		}

		// Write pixel data
		p_Stream.write( reinterpret_cast<char *>( m_pData ), m_DataSize );

		// Write footer
		if( p_Validate )
		{
			Uint32 temp32 = 0;
			Uint32 temp8 = '.';
			p_Stream.write( reinterpret_cast<char *>( &temp32 ), 4 );
			p_Stream.write( reinterpret_cast<char *>( &temp32 ), 4 );
			static const Uint8 signature[ ] = "TRUEVISION-XFILE";
			p_Stream.write( reinterpret_cast<const char *>( signature ), 16 );
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
			p_Stream.write( reinterpret_cast<char *>( &temp8 ), 1 );
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( &m_Footer.m_ExtensionOffset ), 4 );
			p_Stream.write( reinterpret_cast<char *>( &m_Footer.m_DeveloperAreaOffset ), 4 );
			p_Stream.write( reinterpret_cast<char *>( m_Footer.m_Signature ), 16 );
			p_Stream.write( reinterpret_cast<char *>( &m_Footer.m_Dot ), 1 );
			p_Stream.write( reinterpret_cast<char *>( &m_Footer.m_End ), 1 );
		}

		// Succeeded
		return true;
	}

	Bool TgaFile::SaveToFile( const std::string & p_Filename, const Bool p_Validate )
	{
		// Load a string stream.
		std::stringstream ss;

		// Save the stream.
		if( SaveToStream( ss, p_Validate ) == false )
		{
			return false;
		}

		// Open the file.
		std::ofstream fout( p_Filename.c_str( ), std::fstream::binary );
		if( fout.is_open( ) == false )
		{
			std::cout << "[WaveFile::SaveToFile] Can not open the file. " << std::endl;
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

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
