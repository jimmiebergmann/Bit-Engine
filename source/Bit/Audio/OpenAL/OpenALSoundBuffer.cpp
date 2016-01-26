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


#include <Bit/Audio/OpenAL/OpenALSoundBuffer.hpp>
#include <Bit/Audio/WaveFile.hpp>
#include <algorithm>
#include <fstream>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenALSoundBuffer::OpenALSoundBuffer( ) :
		m_Buffer( 0 ),
		m_pData( NULL ),
		m_DataSize( 0 ),
		m_ChannelCount( 0 ),
		m_SampleRate( 0 ),
		m_BitsPerSample( 0 )
	{
	}

	OpenALSoundBuffer::~OpenALSoundBuffer( )
	{
		if( m_pData )
		{
			delete [ ] m_pData;
		}

		if( m_Buffer )
		{
			alDeleteBuffers( 1, &m_Buffer );
		}
	}

	Bool OpenALSoundBuffer::LoadFromMemory(	const Uint8 * p_pData, const SizeType p_DataSize,
											const SizeType p_ChannelCount, const SizeType p_SampleRate,
											const SizeType p_BitsPerSample, const bool p_ForceMono )
	{
		// Delete the old buffer, if any
		if( m_Buffer )
		{
			alDeleteBuffers( 1, &m_Buffer );
		}

		// Genereate the buffer
		alGenBuffers( 1, &m_Buffer );

		// Find the right openAL format.
		ALenum openALFormat;
		bool isMono = false;

		if( p_ChannelCount == 1 )
		{
			isMono = true;

			if( p_BitsPerSample == 8 )
			{
				openALFormat = AL_FORMAT_MONO8;
			}
			else if( p_BitsPerSample == 16 )
			{
				openALFormat = AL_FORMAT_MONO16;
			}
			else
			{
				bitLogAudErr( "Mono frequence error" );
				return false;
			}
		}
		else if( p_ChannelCount == 2 )
		{
			if( p_BitsPerSample == 8 )
			{
				openALFormat = AL_FORMAT_STEREO8;
			}
			else if( p_BitsPerSample == 16 )
			{
				openALFormat = AL_FORMAT_STEREO16;
			}
			else
			{
				bitLogAudErr( "Stereo frequence error." );
				return false;
			}
		}
		else
		{
			bitLogAudErr( "Channel count error." );
			return false;
		}

		// Get the data pointer
		m_pData = const_cast<Uint8 *>( p_pData );
		m_DataSize = p_DataSize;
		m_ChannelCount = p_ChannelCount;
		m_SampleRate = p_SampleRate;
		m_BitsPerSample = p_BitsPerSample;

		// Is is a stereo buffer, but we are forced to use mono?
		// Convert to mono sound
		if( !isMono && p_ForceMono )
		{
			Uint8 * pOldData = m_pData;
			SizeType oldDataSize = m_DataSize;
			m_DataSize /= 2;

			// Create a new data buffer
			m_pData = new Uint8[ m_DataSize ];

			// Convert the old data to mono
			ConvertToMono( m_pData, pOldData, oldDataSize, p_BitsPerSample, openALFormat );

			// Delete the old data.
			delete [ ] pOldData;
		}

		// Set the buffe data
		alBufferData( m_Buffer, openALFormat, (ALvoid*)m_pData, m_DataSize, m_SampleRate );

		// Succeeded
		return true;
	}

	Bool OpenALSoundBuffer::LoadFromFile(	const std::string & p_Filename,
											const bool p_ForceMono )
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
		if( fileExtension == "WAV" )
		{
			return LoadFromWavFile( p_Filename, p_ForceMono );
		}
		else if( fileExtension == "OGG" )
		{
			bitLogAudErr( "Not supporting OGG images yet." );
			return false;
		}

		// Unknown extension
		bitLogAudErr( "Unknow extension: " << fileExtension );
		return false;
	}

	Bool OpenALSoundBuffer::LoadFromWavFile(	const std::string & p_Filename,
												const bool p_ForceMono )
	{
		// Load wave file
		WaveFile wave( false );
		if( wave.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Get the required variables.
		const Uint8 * pConstData	= wave.GetDataChunk( ).GetData( );
		const Uint32 dataSize		= wave.GetDataChunk( ).GetSubChunkSize( );
		const Uint16 channelCount	= wave.GetFmtChunk( ).GetChannelCount( );
		const Uint32 sampleRate		= wave.GetFmtChunk( ).GetSampleRate( );
		const Uint16 bitsPerSample	= wave.GetFmtChunk( ).GetBitsPerSample( );
		
		// Create and load the buffer.
		return LoadFromMemory(	pConstData, dataSize, channelCount,
								sampleRate, bitsPerSample, p_ForceMono );
	}

	 void OpenALSoundBuffer::Destroy( )
	 {
		 if( m_pData )
		{
			delete [ ] m_pData;
			m_pData = NULL;
		}

		if( m_Buffer )
		{
			alDeleteBuffers( 1, &m_Buffer );
			m_Buffer = 0;
		}
	 }

	Uint8 * OpenALSoundBuffer::GetData( ) const
	{
		return m_pData;
	}

	SizeType OpenALSoundBuffer::GetDataSize( ) const
	{
		return m_DataSize;
	}

	SizeType OpenALSoundBuffer::GetChannelCount( ) const
	{
		return m_ChannelCount;
	}

	SizeType OpenALSoundBuffer::GetSampleRate( ) const
	{
		return m_SampleRate;
	}

	SizeType OpenALSoundBuffer::GetBitsPerSample( ) const
	{
		return m_BitsPerSample;
	}

	const void * OpenALSoundBuffer::GetBuffer( ) const
	{
		return reinterpret_cast<const void*>( &m_Buffer );
	}

	void OpenALSoundBuffer::ConvertToMono(	Uint8 * p_pDestination,
											const Uint8 * p_pSource,
											const SizeType p_SourceSize,
											const Uint16 p_BitsPerSample,
											ALenum & p_OpenALFormat )
	{
		// Divide the size
		const SizeType dataSize = p_SourceSize / 2;
			
		// Get the new format
		if( p_BitsPerSample == 8 )
		{
			// Set the right openAL format.
			p_OpenALFormat = AL_FORMAT_MONO8;

			// Make the average value out of the left and right sample value.
			// Go through all the 8 bit samples.
			for( SizeType i = 0; i < dataSize; i++ )
			{
				p_pDestination[ i ] = static_cast<Uint8>(
									(	static_cast<Uint16>( p_pSource[ i * 2 ] ) +
										static_cast<Uint16>( p_pSource[ i * 2 + 1 ] ) ) / 2 );
			}
		}
		else if( p_BitsPerSample == 16 )
		{
			// Set the right openAL format.
			p_OpenALFormat = AL_FORMAT_MONO16;

			// Make the average value out of the left and right sample value.
			// Go through all the 16 bit samples.
			for( SizeType i = 0; i < dataSize; i+= 2 )
			{
				// Get the left and right sample
				const Int16 left = *( reinterpret_cast<const Int16 *>( &p_pSource[ i * 2 ] ) );
				const Int16 right = *( reinterpret_cast<const Int16 *>( &p_pSource[ i * 2 + 2 ] ) );

				// Combine the two samples.
				const Int16 monoSample = static_cast<Int16>(
										(	static_cast<Int32>( left ) +
											static_cast<Int32>( right ) ) / 2 );

				// Set the new mono sample.
				p_pDestination[ i ] =  static_cast<Uint8>( monoSample );
				p_pDestination[ i + 1 ] =  static_cast<Uint8>( monoSample >> 8 );
			}
		}

	}

}
