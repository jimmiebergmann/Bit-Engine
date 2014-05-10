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

#include <Bit/Graphics/BmpFile.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Bmp header class
	BmpFile::BitmapHeader::BitmapHeader( ) :
		m_Identifier( 0 ),
		m_FileSize( 0 ),	
		m_Reserved( 0 ),		
		m_PixelArrayOffset( 0 )
	{
	}

	void BmpFile::BitmapHeader::SetIdentifier( const Uint16 p_Identifier )
	{
		m_Identifier = p_Identifier;
	}

	void BmpFile::BitmapHeader::SetFileSize( const Uint32 p_FileSize )
	{
		m_FileSize = p_FileSize;
	}

	void BmpFile::BitmapHeader::SetReservedSpace( Uint32 p_Reserved )
	{
		m_Reserved = p_Reserved;
	}

	void BmpFile::BitmapHeader::SetPixelArrayOffset( const Uint32 p_Offset )
	{
		m_PixelArrayOffset = p_Offset;
	}

	Uint16 BmpFile::BitmapHeader::GetIdentifier( ) const
	{
		return m_Identifier;
	}

	Uint32 BmpFile::BitmapHeader::GetFileSize( ) const
	{
		return m_FileSize;
	}

	Uint32 BmpFile::BitmapHeader::GetReservedSpace( ) const
	{
		return m_Reserved;
	}

	Uint32 BmpFile::BitmapHeader::GetPixelArrayOffset( ) const
	{
		return m_PixelArrayOffset;
	}

	// Dib header class
	BmpFile::DibHeader::DibHeader( ) :
		m_HeaderSize( 40 ),
		m_BitmapWidth( 0 ),
		m_BitmapHeight( 0 ),
		m_ColorPlanes( 1 ),
		m_PixelDepth( 0 ),
		m_Compression( 0 ),
		m_DataSize( 0 ),
		m_HorizontalResolution( 0 ),
		m_VerticalResolution( 0 ),
		m_ColorPalette( 0 ),
		m_ImportantColors( 0 )
	{
	}

	void BmpFile::DibHeader::SetHeaderSize( const Uint32 p_HeaderSize )
	{
		m_HeaderSize = p_HeaderSize;
	}

	void BmpFile::DibHeader::SetBitmapWidth( const Int32 p_Width )
	{
		m_BitmapWidth = p_Width;
	}

	void BmpFile::DibHeader::SetBitmapHeight( const Int32 p_Height )
	{
		m_BitmapHeight = p_Height;
	}

	void BmpFile::DibHeader::SetColorPlaneCount( const Uint16 p_Count )
	{
		m_ColorPlanes = p_Count;
	}

	void BmpFile::DibHeader::SetPixelDepth( const Uint16 p_PixelDepth )
	{
		m_PixelDepth = p_PixelDepth;
	}

	void BmpFile::DibHeader::SetCompression( const Uint32 p_CompressionMethod )
	{
		m_Compression = p_CompressionMethod;
	}

	void BmpFile::DibHeader::SetCompression( const BmpFile::eCompressionMethod p_CompressionMethod )
	{
		m_Compression = static_cast<Uint32>( p_CompressionMethod );
	}

	void BmpFile::DibHeader::SetBitmapDataSize( const Uint32 p_Size )
	{
		m_DataSize = p_Size;
	}

	void BmpFile::DibHeader::SetHorizontalResolution( const Int32 p_HorizontalResolution )
	{
		m_HorizontalResolution = p_HorizontalResolution;
	}

	void BmpFile::DibHeader::SetVerticalResolution( const Int32 p_VerticalResolution )
	{
		m_VerticalResolution = p_VerticalResolution;
	}

	void BmpFile::DibHeader::SetColorPaletteCount( const Uint32 p_Count )
	{
		m_ColorPalette = p_Count;
	}

	void BmpFile::DibHeader::SetImportantColorCount( const Uint32 p_Count )
	{
		m_ImportantColors = p_Count;
	}

	Uint32 BmpFile::DibHeader::GetHeaderSize( ) const
	{
		return m_HeaderSize;
	}

	Int32 BmpFile::DibHeader::GetBitmapWidth( ) const
	{
		return m_BitmapWidth;
	}

	Int32 BmpFile::DibHeader::GetBitmapHeight( ) const
	{
		return m_BitmapHeight;
	}

	Uint16 BmpFile::DibHeader::GetColorPlaneCount( ) const
		
	{
		return m_ColorPlanes;
	}

	Uint16 BmpFile::DibHeader::GetPixelDepth( ) const
	{
		return m_PixelDepth;
	}

	Uint32 BmpFile::DibHeader::GetCompression( ) const
	{
		return m_Compression;
	}

	Uint32 BmpFile::DibHeader::GetBitmapDataSize( ) const
	{
		return m_DataSize;
	}

	Int32 BmpFile::DibHeader::GetHorizontalResolution( ) const
	{
		return m_HorizontalResolution;
	}

	Int32 BmpFile::DibHeader::GetVerticalResolution( ) const
	{
		return m_VerticalResolution;
	}

	Uint32 BmpFile::DibHeader::GetColorPaletteCount( ) const
	{
		return m_ColorPalette;
	}

	Uint32 BmpFile::DibHeader::GetImportantColorCount( ) const
	{
		return m_ImportantColors;
	}

	// Bmp file class
	BmpFile::BmpFile( const Bool p_DeallocateFlag ) :
		m_BitmapHeader( ),
		m_DibHeader( ),
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_DeallocateFlag( p_DeallocateFlag )
	{
	}

	BmpFile::~BmpFile( )
	{
		// Delete the allocated data
		if( m_DeallocateFlag && m_pData )
		{
			delete m_pData;
		}
	}

	Bool BmpFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.str( p_Memory );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool BmpFile::LoadFromStream( std::istream & p_Stream )
	{
		// Get the stream size
		p_Stream.seekg( 0, std::fstream::end );
		SizeType streamSize = static_cast<SizeType>( p_Stream.tellg( ) );
		p_Stream.seekg( 0, std::fstream::beg );

		// Error check the stream size
		if( streamSize < 54 )
		{
			std::cout << "[BmpFile::LoadFromStream] File size error 1.\n";
			return false;
		}

		// Read the bitmap header
		p_Stream.read( reinterpret_cast<char *>( &m_BitmapHeader.m_Identifier ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_BitmapHeader.m_FileSize ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_BitmapHeader.m_Reserved ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_BitmapHeader.m_PixelArrayOffset ), 4 );

		// Read the dib header
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_HeaderSize ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_BitmapWidth ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_BitmapHeight ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_ColorPlanes ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_PixelDepth ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_Compression ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_DataSize ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_HorizontalResolution ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_VerticalResolution ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_ColorPalette ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DibHeader.m_ImportantColors ), 4 );

		// Error check the pixel depth.
		if( m_DibHeader.m_PixelDepth != 8 && m_DibHeader.m_PixelDepth != 16 &&
			m_DibHeader.m_PixelDepth != 24 && m_DibHeader.m_PixelDepth != 32 )
		{
			std::cout << "[BmpFile::LoadFromStream] Not a 8/16/24 or 32 bit image.\n";
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Calculate of the padded row and data size.
		SizeType paddedRowSize = static_cast<SizeType>( ( m_DibHeader.m_PixelDepth * m_DibHeader.m_BitmapWidth + 31 ) / 32 ) * 4;
		SizeType paddedDataSize = paddedRowSize * m_DibHeader.m_BitmapHeight;

		// Error check the padded data size
		if( streamSize < paddedDataSize + 54 )
		{
			std::cout << "[BmpFile::LoadFromStream] The expected data size is too large." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Caulcate the data size
		SizeType rowSize = m_DibHeader.m_BitmapWidth * ( m_DibHeader.m_PixelDepth / 8 );
		m_DataSize = rowSize * m_DibHeader.m_BitmapHeight;

		// Allocate memory for the pixel data
		m_pData = new Uint8[ m_DataSize ];

		// Read the data from the stream
		// We have to read the data through a loop, because of padding.
		SizeType position = 0;
		SizeType streamPosition = 0;
		SizeType streamStart = static_cast<SizeType>( p_Stream.tellg( ) );
		for( SizeType y = 0; y < m_DibHeader.m_BitmapHeight; y++ )
		{
			// Calculate the offsets
			position = y * rowSize;
			streamPosition = streamStart + ( y * paddedRowSize );
			
			// Go to the current row
			p_Stream.seekg( streamPosition, std::ios::beg );
			
			// Read the row
			p_Stream.read( reinterpret_cast<char *>( m_pData + position ), rowSize );
		}

		// Go back to the begining of the stream
		p_Stream.seekg( 0, std::fstream::beg ); 

		// Succeeded
		return true;
	}

	Bool BmpFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ), std::fstream::binary );
		if( fin.is_open( ) == false )
		{
			std::cout << "[BmpFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		// Read the stream
		Bool status = LoadFromStream( fin );

		// Close the file
		fin.close( );

		// Return the status
		return status;
	}

	Bool BmpFile::SaveToMemory( std::string & p_Memory, const Bool p_Validate )
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

	Bool BmpFile::SaveToStream( std::ostream & p_Stream, const Bool p_Validate )
	{
		if( m_pData == NULL || m_DataSize == 0 )
		{
			std::cout << "[BmpFile::SaveToStream] No image data." << std::endl;
			return false;
		}

		// Write the bitmap header
		p_Stream.write( reinterpret_cast<char *>( &m_BitmapHeader.m_Identifier ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_BitmapHeader.m_FileSize ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_BitmapHeader.m_Reserved ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_BitmapHeader.m_PixelArrayOffset ), 4 );

		// Write the DIB header
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_HeaderSize ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_BitmapWidth ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_BitmapHeight ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_ColorPlanes ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_PixelDepth ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_Compression ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_DataSize ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_HorizontalResolution ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_VerticalResolution ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_ColorPalette ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_DibHeader.m_ImportantColors ), 4 );

		// Write the data
		p_Stream.write( reinterpret_cast<char *>( m_pData ), m_DataSize );

		return true;
	}

	Bool BmpFile::SaveToFile( const std::string & p_Filename, const Bool p_Validate )
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
			std::cout << "[BmpFile::SaveToFile] Can not open the file. " << std::endl;
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

		// Succeeded.
		return true;
	}

	void BmpFile::Clear( )
	{
		// Delete the allocated data
		if( m_pData )
		{
			delete m_pData;
		}
	}

	BmpFile::BitmapHeader & BmpFile::GetBitmapHeader( )
	{
		return m_BitmapHeader;
	}

	BmpFile::DibHeader & BmpFile::GetDibHeader( )
	{
		return m_DibHeader;
	}

	const Uint8 * BmpFile::GetData( ) const
	{
		return m_pData;
	}

	SizeType BmpFile::GetDataSize( ) const
	{
		return m_DataSize;
	}

	void BmpFile::SetData( const Uint8 * p_pData, const SizeType p_DataSize )
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

	Bool BmpFile::GetDeallocatorFlag( ) const
	{
		return m_DeallocateFlag;
	}

}