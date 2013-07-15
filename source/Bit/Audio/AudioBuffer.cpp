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
#include <Bit/System.hpp>
#include <fstream>
#include <cstring>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	AudioBuffer::AudioBuffer( ) :
		m_Loaded( BIT_FALSE ),
		m_ChannelCount( 0 ),
		m_SampleRate( 0 ),
		m_BitsPerSample( 0 ),
		m_BufferSize( 0 ),
		m_pBuffer( BIT_NULL )
	{

	}

	AudioBuffer::~AudioBuffer( )
	{
		if( m_pBuffer )
		{
			delete [] m_pBuffer;
			m_pBuffer = BIT_NULL;
		}
	}

	BIT_UINT32 AudioBuffer::Read( char * p_pFileName )
	{
		/*char FileExtension[4];
		GetFileExtension( p_pFileName, FileExtension, 4 );

		if( strcmp( FileExtension, "WAV" ) == 0 )
		{
			return ReadWAVE( p_pFileName );
		}

		bitTrace( BIT_NULL, "[Bit::AudioBuffer:Load] <ERROR> "
			"Unknow extension: %s.\n", FileExtension );

		return BIT_ERROR;*/
		return ReadWAVE( p_pFileName );
	}

	BIT_BYTE * AudioBuffer::GetData( ) const
	{
		return m_pBuffer;
	}

	BIT_UINT32 AudioBuffer::GetDataSize( ) const
	{
		return m_BufferSize;
	}

	BIT_UINT16 AudioBuffer::GetChannelCount( ) const
	{
		return m_ChannelCount;
	}

	BIT_UINT32 AudioBuffer::GetSampleRate( ) const
	{
		return m_SampleRate;
	}

	BIT_UINT16 AudioBuffer::GetBitsPerSample( ) const
	{
		return m_BitsPerSample;
	}

	BIT_BOOL AudioBuffer::ContainsData( ) const
	{
		return m_Loaded;
	}

	BIT_UINT32 AudioBuffer::ReadWAVE( char *p_pFileName )
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
			bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Can not open the file: %s.\n", p_pFileName );
			return BIT_ERROR_OPEN_FILE;
		}

		// Read the types
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'R' || Type[1] != 'I' || Type[2] != 'F' || Type[3] != 'F' )
		{
			bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Not of RIFF type.\n" );
			return BIT_ERROR;
		}

		fin.read( (char*)&Size, sizeof(unsigned int) );
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'W' || Type[1] != 'A' || Type[2] != 'V' || Type[3] != 'E' )
		{
			bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Not of WAVE type.\n" );
			return BIT_ERROR;
		}

		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'f' || Type[1] != 'm' || Type[2] != 't' || Type[3] != ' ' )
		{
			bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
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
		/*bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] "
			"WAVE data: \n"
			"Chunk Size: %i\n"
			"Format type: %hi\n"
			"Channel count: %hi\n"
			"Sample Rate: %i\n"
			"Average Bytes Per Sec: %i\n"
			"Bytes Per Sample: %hi\n"
			"Bits Per Sample: %hi\n",
			ChunkSize, FormatType,
			ChannelCount, SampleRate,
			AverageBytesPerSec, BytesPerSample,
			BitsPerSample);*/


		m_ChannelCount = static_cast< BIT_UINT16 >( ChannelCount );
		m_SampleRate = static_cast< BIT_UINT32 >( SampleRate );
		m_BitsPerSample = static_cast< BIT_UINT16 >( BitsPerSample );


		// Making sure we have reached the sound data
		fin.read( Type, sizeof(char) * 4 );
		if( Type[0] != 'd' || Type[1] != 'a' || Type[2] != 't' || Type[3] != 'a' )
		{
			bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] <ERROR> "
				"Missing sound data.\n" );
			return BIT_ERROR;
		}

		// Read the data size
		fin.read( (char*)&m_BufferSize, sizeof(unsigned int) );
		/*bitTrace( BIT_NULL, "[Bit::AudioBuffer::ReadWAVE] "
			"Data Size: %i\n" , m_BufferSize );*/

		// Read the buffer data
		m_pBuffer = new BIT_BYTE[ m_BufferSize ];
		fin.read( (char*)m_pBuffer, sizeof(BIT_BYTE) * m_BufferSize );

		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

}