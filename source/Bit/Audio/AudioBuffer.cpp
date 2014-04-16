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


#include <Bit/Audio/AudioBuffer.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	AudioBuffer::AudioBuffer( ) :
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_ChannelCount( 0 ),
		m_SampleRate( 0 ),
		m_BitsPerSample( 0 )
	{
	}

	AudioBuffer::~AudioBuffer( )
	{
		if( m_pData )
		{
			delete [ ] m_pData;
		}
	}

	Bool AudioBuffer::LoadFromFile( const std::string & p_Filename )
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
		if( fileExtension == "WAV" )
		{
			return LoadFromWavFile( p_Filename );
		}
		else if( fileExtension == "OGG" )
		{
			std::cout << "[AudioBuffer::LoadFromFile] Not supporting OGG images yet.\n";
			return false;
		}

		// Unknown extension
		std::cout << "[AudioBuffer::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		return false;
	}

	Bool AudioBuffer::LoadFromWavFile( const std::string & p_Filename )
	{
		return false;
	}

	Uint8 * AudioBuffer::GetData( ) const
	{
		return m_pData;
	}

	SizeType AudioBuffer::GetDataSize( ) const
	{
		return m_DataSize;
	}

	SizeType AudioBuffer::GetChannelCount( ) const
	{
		return m_ChannelCount;
	}

	SizeType AudioBuffer::GetSampleRate( ) const
	{
		return m_SampleRate;
	}

	SizeType AudioBuffer::GetBitsPerSample( ) const
	{
		return m_BitsPerSample;
	}


/*
	BIT_UINT32 AudioBuffer::ReadWAVE( const char *p_pFileName )
	{
		char Type[4];
		unsigned int Size, ChunkSize;
		unsigned short FormatType, ChannelCount;
		unsigned int SampleRate, AverageBytesPerSec;
		unsigned short BytesPerSample, BitsPerSample;

		// Open the file
		std::ifstream fin( p_pFileName, std::ios_base::binary );
		if( fin.is_open() == BIT_FALSE )
		{
			bitTrace( "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Can not open the file: %s.\n", p_pFileName );
			return BIT_ERROR_OPEN_FILE;
		}

		// Read the types
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'R' || Type[1] != 'I' || Type[2] != 'F' || Type[3] != 'F' )
		{
			bitTrace( "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Not of RIFF type.\n" );
			return BIT_ERROR;
		}

		fin.read( (char*)&Size, sizeof(unsigned int) );
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'W' || Type[1] != 'A' || Type[2] != 'V' || Type[3] != 'E' )
		{
			bitTrace( "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Not of WAVE type.\n" );
			return BIT_ERROR;
		}

		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'f' || Type[1] != 'm' || Type[2] != 't' || Type[3] != ' ' )
		{
			bitTrace( "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Not of fmt type.\n" );
			return BIT_ERROR;
		}

		// Read the rest of the header data
		fin.read( (char*)&ChunkSize, sizeof(unsigned int) );
		fin.read( (char*)&FormatType, sizeof(unsigned short) );
		fin.read( (char*)&ChannelCount, sizeof(unsigned short) );
		fin.read( (char*)&SampleRate, sizeof(unsigned int) );
		fin.read( (char*)&AverageBytesPerSec, sizeof(unsigned int) );
		fin.read( (char*)&BytesPerSample, sizeof(unsigned short) );
		fin.read( (char*)&BitsPerSample, sizeof(unsigned short) );

		m_ChannelCount = static_cast< BIT_UINT16 >( ChannelCount );
		m_SampleRate = static_cast< BIT_UINT32 >( SampleRate );
		m_BitsPerSample = static_cast< BIT_UINT16 >( BitsPerSample );

		// Making sure we have reached the sound data
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'd' || Type[1] != 'a' || Type[2] != 't' || Type[3] != 'a' )
		{
			bitTrace( "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Missing sound data.\n" );
			return BIT_ERROR;
		}

		// Read the data size
		fin.read( (char*)&m_BufferSize, sizeof(unsigned int) );

		// Read the buffer data
		m_pBuffer = new BIT_BYTE[ m_BufferSize ];
		fin.read( (char*)m_pBuffer, sizeof(BIT_BYTE) * m_BufferSize );

		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}
	*/
}
