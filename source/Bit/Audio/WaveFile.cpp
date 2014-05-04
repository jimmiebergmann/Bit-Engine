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

#include <Bit/Audio/WaveFile.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// RIFF header class
	WaveFile::RiffHeader::RiffHeader( )
	{
	}

	void WaveFile::RiffHeader::SetChunkId(	const Uint8 p_A, const Uint8 p_B,
											const Uint8 p_C, const Uint8 p_D )
	{
		m_ChunkId[ 0 ] = p_A;
		m_ChunkId[ 1 ] = p_B;
		m_ChunkId[ 2 ] = p_C;
		m_ChunkId[ 3 ] = p_D;
	}

	void WaveFile::RiffHeader::SetChunkSize( const Uint32 p_ChunkSize )
	{
		m_ChunkSize = p_ChunkSize;
	}

	void WaveFile::RiffHeader::SetFormat(	const Uint8 p_A, const Uint8 p_B,
											const Uint8 p_C, const Uint8 p_D )
	{
		m_Format[ 0 ] = p_A;
		m_Format[ 1 ] = p_B;
		m_Format[ 2 ] = p_C;
		m_Format[ 3 ] = p_D;
	}

	const Uint8 * WaveFile::RiffHeader::GetChunkId( ) const
	{
		return m_Format;
	}

	Uint32 WaveFile::RiffHeader::GetChunkSize( ) const
	{
		return m_ChunkSize;
	}

	const Uint8 * WaveFile::RiffHeader::GetFormat( ) const
	{
		return m_Format;
	}


	// Fmt class
	WaveFile::FmtChunk::FmtChunk( )
	{
	}


	// Data class
	WaveFile::DataChunk::DataChunk( const Bool p_DeallocateFlag ) :
		m_pData( NULL ),
		m_DeallocateFlag( p_DeallocateFlag )
	{

	}

	WaveFile::DataChunk::~DataChunk( )
	{
		if( m_DeallocateFlag )
		{
			if( m_pData )
			{
				delete [ ] m_pData;
			}
		}
	}

	void WaveFile::DataChunk::Clear( )
	{
		if( m_pData )
		{
			delete [ ] m_pData;
			m_pData = NULL;
		}
	}

	void WaveFile::DataChunk::SetSubChunkId(	const Uint8 p_A, const Uint8 p_B,
												const Uint8 p_C, const Uint8 p_D )
	{
		m_SubChunkId[ 0 ] = p_A;
		m_SubChunkId[ 1 ] = p_B;
		m_SubChunkId[ 2 ] = p_C;
		m_SubChunkId[ 3 ] = p_D;
	}

	void WaveFile::DataChunk::SetSubChunkSize( const Uint32 p_SubChunkSize )
	{
		m_SubChunkSize = p_SubChunkSize;
	}

	void WaveFile::DataChunk::SetData( const Uint8 * p_pData, const SizeType p_DataSize )
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

		// Allocate the new data
		m_pData = new Uint8[ p_DataSize ];

		// Copy the data
		memcpy( m_pData, p_pData, p_DataSize );
	}

	const Uint8 * WaveFile::DataChunk::GetSubChunkId( ) const
	{
		return m_SubChunkId;
	}
		
	Uint32 WaveFile::DataChunk::GetSubChunkSize( ) const
	{
		return m_SubChunkSize;
	}
	
	const Uint8 * WaveFile::DataChunk::GetData( ) const
	{
		return m_pData;
	}

	// Wave file class
	WaveFile::WaveFile( const Bool p_DeallocateFlag ) :
		m_RiffHeader( ),
		m_FmtChunk( ),
		m_DataChunk( p_DeallocateFlag )
	{
	}

	WaveFile::~WaveFile( )
	{

	}

	Bool WaveFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.str( p_Memory );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool WaveFile::LoadFromStream( std::istream & p_Stream )
	{
		// Read the "RIFF" chunk


		// Read the "fmt" chunk

		// Read the "data" chunk

		
		return false;
	}

	Bool WaveFile::LoadFromFile( const std::string & p_Filename )
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

	void WaveFile::Clear( )
	{
		m_DataChunk.Clear( );
	}

	WaveFile::RiffHeader & WaveFile::GeRiffHeader( )
	{
		return m_RiffHeader;
	}

	WaveFile::FmtChunk & WaveFile::GetFmtChunk( )
	{
		return m_FmtChunk;
	}

	WaveFile::DataChunk & WaveFile::GetDataChunk( )
	{
		return m_DataChunk;
	}


}