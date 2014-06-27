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
#include <string.h>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// RIFF header class
	WaveFile::RiffHeader::RiffHeader( ) :
		m_ChunkSize( 0 )
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
	WaveFile::FmtChunk::FmtChunk( ) :
		m_SubChunkSize( 16 ),
		m_AudioFormat( 1 ),
		m_ChannelCount( 0 ),
		m_SampleRate( 0 ),
		m_ByteRate( 0 ),
		m_BlockAlign( 0 ),
		m_BitsPerSample( 0 )
	{
	}

	void WaveFile::FmtChunk::SetSubChunkId( const Uint8 p_A, const Uint8 p_B,
											const Uint8 p_C, const Uint8 p_D )
	{
		m_SubChunkId[ 0 ] = p_A;
		m_SubChunkId[ 1 ] = p_B;
		m_SubChunkId[ 2 ] = p_C;
		m_SubChunkId[ 3 ] = p_D;
	}

	void WaveFile::FmtChunk::SetSubChunkSize( const Uint32 p_SubChunkSize )
	{
		m_SubChunkSize = p_SubChunkSize;
	}

	void WaveFile::FmtChunk::SetAudioFormat( const Uint16 p_AudioFormat )
	{
		m_AudioFormat = p_AudioFormat;
	}

	void WaveFile::FmtChunk::SetChannelCount( const Uint16 p_ChannelCount )
	{
		m_ChannelCount = p_ChannelCount;
	}

	void WaveFile::FmtChunk::SetSampleRate( const Uint32 p_SampleRate )
	{
		m_SampleRate = p_SampleRate;
	}

	void WaveFile::FmtChunk::SetByteRate( const Uint32 p_ByteRate )
	{
		m_ByteRate = p_ByteRate;
	}

	void WaveFile::FmtChunk::SetBlockAlign( const Uint16 p_BlockAlign )
	{
		m_BlockAlign = p_BlockAlign;
	}

	void WaveFile::FmtChunk::SetBitsPerSample( const Uint16 p_BitsPerSample )
	{
		m_BitsPerSample = p_BitsPerSample;
	}

	const Uint8 * WaveFile::FmtChunk::GetSubChunkId( ) const
	{
		return m_SubChunkId;
	}

	Uint32 WaveFile::FmtChunk::GetSubChunkSize( ) const
	{
		return m_SubChunkSize;
	}

	Uint16 WaveFile::FmtChunk::GetAudioFormat( ) const
	{
		return m_AudioFormat;
	}

	Uint16 WaveFile::FmtChunk::GetChannelCount( ) const
	{
		return m_ChannelCount;
	}

	Uint32 WaveFile::FmtChunk::GetSampleRate( ) const
	{
		return m_SampleRate;
	}

	Uint32 WaveFile::FmtChunk::GetByteRate( ) const
	{
		return m_ByteRate;
	}

	Uint16 WaveFile::FmtChunk::GetBlockAlign( ) const
	{
		return m_BlockAlign;
	}

	Uint16 WaveFile::FmtChunk::GetBitsPerSample( ) const
	{
		return m_BitsPerSample;
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

		return LoadFromStream( ss );
	}

	Bool WaveFile::LoadFromStream( std::istream & p_Stream )
	{
		// Read the stream size.
		p_Stream.seekg( 0, std::fstream::end );
		SizeType fileSize = static_cast<SizeType>( p_Stream.tellg( ) );
		p_Stream.seekg( 0, std::fstream::beg );

		// Error check the stream size
		if( fileSize < 44 )
		{
			std::cout << "[WaveFile::LoadFromStream] Wrong file size" << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Read the RIFF header
		p_Stream.read( reinterpret_cast<char *>( m_RiffHeader.m_ChunkId ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_RiffHeader.m_ChunkSize ), 4 );
		p_Stream.read( reinterpret_cast<char *>( m_RiffHeader.m_Format ), 4 );

		// Error check the RIFF header( error check chunk size later ).
		if(	m_RiffHeader.m_ChunkId[ 0 ] != 'R' || m_RiffHeader.m_ChunkId[ 1 ] != 'I' ||
			m_RiffHeader.m_ChunkId[ 2 ] != 'F' || m_RiffHeader.m_ChunkId[ 3 ] != 'F' )
		{
			std::cout << "[WaveFile::LoadFromStream] No RIFF header were found." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		if(	m_RiffHeader.m_Format[ 0 ] != 'W' || m_RiffHeader.m_Format[ 1 ] != 'A' ||
			m_RiffHeader.m_Format[ 2 ] != 'V' || m_RiffHeader.m_Format[ 3 ] != 'E' )
		{
			std::cout << "[WaveFile::LoadFromStream] No WAVE field were found.." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Read the fmt chunk
		p_Stream.read( reinterpret_cast<char *>( m_FmtChunk.m_SubChunkId ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_SubChunkSize ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_AudioFormat ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_ChannelCount ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_SampleRate ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_ByteRate ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_BlockAlign ), 2 );
		p_Stream.read( reinterpret_cast<char *>( &m_FmtChunk.m_BitsPerSample ), 2 );

		// Error check the fmt chunk
		if(	m_FmtChunk.m_SubChunkId[ 0 ] != 'f' || m_FmtChunk.m_SubChunkId[ 1 ] != 'm' ||
			m_FmtChunk.m_SubChunkId[ 2 ] != 't' || m_FmtChunk.m_SubChunkId[ 3 ] != ' ' )
		{
			std::cout << "[WaveFile::LoadFromStream] No FMT chunk were found." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		if( m_FmtChunk.m_AudioFormat != 1 )
		{
			std::cout << "[WaveFile::LoadFromStream] No PCM WAVE file." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Move to where the data is expected.
		p_Stream.seekg( 20 + m_FmtChunk.m_SubChunkSize, std::fstream::beg );

		// Read the data chunk( except the data )
		p_Stream.read( reinterpret_cast<char *>( m_DataChunk.m_SubChunkId ), 4 );
		p_Stream.read( reinterpret_cast<char *>( &m_DataChunk.m_SubChunkSize ), 4 );

		// Error check the fmt chunk( except the data )
		if(	m_DataChunk.m_SubChunkId[ 0 ] != 'd' || m_DataChunk.m_SubChunkId[ 1 ] != 'a' ||
			m_DataChunk.m_SubChunkId[ 2 ] != 't' || m_DataChunk.m_SubChunkId[ 3 ] != 'a' )
		{
			std::cout << "[WaveFile::LoadFromStream] No data chunk were found." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		if( m_DataChunk.m_SubChunkSize == 0 ||
			m_DataChunk.m_SubChunkSize + m_FmtChunk.m_SubChunkSize + 28 > fileSize )
		{
			std::cout << "[WaveFile::LoadFromStream] Error in data size." << std::endl;
			p_Stream.seekg( 0, std::fstream::beg ); // Go back to the begining of the stream
			return false;
		}

		// Clear the old audio data.
		if( m_DataChunk.m_pData )
		{
			delete m_DataChunk.m_pData;
		}

		// Allocate the data
		m_DataChunk.m_pData = new Uint8[ m_DataChunk.m_SubChunkSize ];

		// Read the bitmap data
		p_Stream.read( reinterpret_cast<char *>( m_DataChunk.m_pData ), m_DataChunk.m_SubChunkSize );

		// Go back to the begining of the stream
		p_Stream.seekg( 0, std::fstream::beg );

		// Succeeded
		return true;
	}

	Bool WaveFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ), std::fstream::binary );
		if( fin.is_open( ) == false )
		{
			std::cout << "[WaveFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		// Read the stream
		Bool status = LoadFromStream( fin );

		// Close the file
		fin.close( );

		// Return the status
		return status;
	}

	Bool WaveFile::SaveToMemory( std::string & p_Memory, const Bool p_Validate )
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

	Bool WaveFile::SaveToStream( std::ostream & p_Stream, const Bool p_Validate )
	{
		// Write riff header
		if( p_Validate )
		{
			p_Stream << "RIFF";
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( m_RiffHeader.m_ChunkId ), 4 );
		}

		p_Stream.write( reinterpret_cast<char *>( &m_RiffHeader.m_ChunkSize ), 4 );

		if( p_Validate )
		{
			p_Stream << "WAVE";
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( m_RiffHeader.m_Format ), 4 );
		}

		// Write fmt chunk
		if( p_Validate )
		{
			p_Stream << "fmt ";
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( m_FmtChunk.m_SubChunkId ), 4 );
		}
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_SubChunkSize ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_AudioFormat ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_ChannelCount ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_SampleRate ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_ByteRate ), 4 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_BlockAlign ), 2 );
		p_Stream.write( reinterpret_cast<char *>( &m_FmtChunk.m_BitsPerSample ), 2 );

		// Always make sure to add NULL bytes if the chunk size isn't 16
		if( m_FmtChunk.m_SubChunkSize > 16 )
		{
			for( SizeType i = 0; i < m_FmtChunk.m_SubChunkSize - 16; i++ )
			{
				p_Stream.put( 0 );
			}
		}

		// Write data chunk
		if( p_Validate )
		{
			p_Stream << "data";
		}
		else
		{
			p_Stream.write( reinterpret_cast<char *>( m_DataChunk.m_SubChunkId ), 4 );
		}
		p_Stream.write( reinterpret_cast<char *>( &m_DataChunk.m_SubChunkSize ), 4 );
		p_Stream.write( reinterpret_cast<char *>( m_DataChunk.m_pData ), m_DataChunk.m_SubChunkSize );

		return true;
	}

	Bool WaveFile::SaveToFile( const std::string & p_Filename, const Bool p_Validate )
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

	void WaveFile::Clear( )
	{
		m_DataChunk.Clear( );
	}

	WaveFile::RiffHeader & WaveFile::GetRiffHeader( )
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
