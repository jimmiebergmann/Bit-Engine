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

#include <Bit/Audio/OpenAL/OpenALSound.hpp>
#include <Bit/Audio/WaveFile.hpp>
#include <algorithm>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenALSound::OpenALSound( ) :
		m_Source( 0 ),
		m_Buffer( 0 )
	{
	}

	OpenALSound::~OpenALSound( )
	{
		// Delete source and buffer
		if( m_Source )
		{
			alDeleteSources( 1, &m_Source );
		}
		if( m_Buffer )
		{
			alDeleteBuffers( 1, &m_Buffer );
		}
	}

	Bool OpenALSound::LoadFromBuffer( const AudioBuffer & p_pAudioBuffer )
	{
		return false;
	}

	Bool OpenALSound::LoadFromFile( const std::string & p_Filename )
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
		if( fileExtension == "WAV" || fileExtension == "WAVE" )
		{
			return LoadFromWaveFile( p_Filename );
		}
		else if( fileExtension == "OGG" )
		{
			std::cout << "[OpenALSound::LoadFromFile] Not supporting OGG sounds yet.\n";
			return false;
		}

		// Unknown extension
		std::cout << "[OpenALSound::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		return false;
	}

	Bool OpenALSound::LoadFromWaveFile( const std::string & p_Filename )
	{
		// Open wave file
		WaveFile wave;
		if( wave.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Generate the openal buffers
		alGenBuffers( 1, &m_Buffer );
		alGenSources( 1, &m_Source );

		// Set the audio format
		const Uint16 bitsPerSample = wave.GetFmtChunk( ).GetBitsPerSample( );
		const Uint16 channelCount = wave.GetFmtChunk( ).GetChannelCount( );
		ALenum openALFormat;
		if( bitsPerSample == 8 )
		{
			if( channelCount == 1 )
			{
				openALFormat = AL_FORMAT_MONO8;
			}
			else if( channelCount == 2 )
			{
				openALFormat = AL_FORMAT_STEREO8;
			}
		}
		else if(bitsPerSample == 16 )
		{
			if( channelCount == 1 )
			{
				openALFormat = AL_FORMAT_MONO16;
			}
			else if( channelCount == 2 )
			{
				openALFormat = AL_FORMAT_STEREO16;
			}
		}
		else
		{
			std::cout << "[OpenALSound::LoadFromWaveFile] No fitting OAL format.\n";
			return false;
		}

		// Create the buffer
		const Uint8 * pData = wave.GetDataChunk( ).GetData( );
		const SizeType dataSize =  wave.GetDataChunk( ).GetSubChunkSize( );
		const Uint32 frequence =  wave.GetFmtChunk( ).GetSampleRate( );
		
		alBufferData( m_Buffer, openALFormat, (ALvoid*)pData, dataSize, frequence );
		
		// Set the buffer
		alSourcei( m_Source, AL_BUFFER, m_Buffer );

		// Set the rest of the settings
		static const ALfloat position[ 3 ] = { 0.0f, 0.0f, 0.0f };

		alSourcef( m_Source, AL_PITCH, 1.0f ); 
		alSourcef( m_Source, AL_GAIN, 1.0f ); 
		alSourcefv( m_Source, AL_POSITION, position );
		alSourcei( m_Source, AL_LOOPING, false ); 
		alSourcei( m_Source, AL_SOURCE_RELATIVE, true );

		// Succeeded.
		return true;
	}

	void OpenALSound::Play( )
	{
		alSourcePlay( m_Source );
	}

	void OpenALSound::Pause( )
	{
		alSourcePause( m_Source );
	}

	void OpenALSound::Stop( )
	{
		alSourceStop( m_Source );
	}

	void OpenALSound::SetPosition( const Vector3f32 p_Position )
	{
		ALfloat position[3] = { p_Position.x, p_Position.y, p_Position.z };
		alSourcefv( m_Source, AL_POSITION, position );
	}

	void OpenALSound::SetVolume( const Float32 p_Volume )
	{
		alSourcef( m_Source, AL_GAIN, p_Volume );
	}

	void OpenALSound::SetPitch( const Float32 p_Pitch )
	{
		alSourcef( m_Source, AL_PITCH, p_Pitch ); 
	}

	void OpenALSound::SetLoop( const Bool p_Loop )
	{
		alSourcei( m_Source, AL_LOOPING, p_Loop ); 
	}

	void OpenALSound::SetRelative( const Bool p_Relative )
	{
		alSourcei( m_Source, AL_SOURCE_RELATIVE, p_Relative );
	}

}