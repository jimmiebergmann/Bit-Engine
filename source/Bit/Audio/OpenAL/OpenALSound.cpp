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

	Bool OpenALSound::LoadFromBuffer(	const AudioBuffer & p_pAudioBuffer, 
										const bool p_ForceMono )
	{
		return false;
	}

	Bool OpenALSound::LoadFromFile(		const std::string & p_Filename, 
										const bool p_ForceMono )
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
			return LoadFromWaveFile( p_Filename, p_ForceMono );
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

	Bool OpenALSound::LoadFromWaveFile( const std::string & p_Filename, 
										const bool p_ForceMono )
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
		bool isMono = true;
		if( bitsPerSample == 8 )
		{
			if( channelCount == 1 )
			{
				openALFormat = AL_FORMAT_MONO8;
			}
			else if( channelCount == 2 )
			{
				openALFormat = AL_FORMAT_STEREO8;
				isMono = false;
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
				isMono = false;
			}
		}
		else
		{
			std::cout << "[OpenALSound::LoadFromWaveFile] No fitting OAL format.\n";
			return false;
		}

		// Create the buffer
		Uint8 * pData = const_cast<Uint8 *>( wave.GetDataChunk( ).GetData( ) );
		SizeType dataSize =  wave.GetDataChunk( ).GetSubChunkSize( );
		const Uint32 frequence =  wave.GetFmtChunk( ).GetSampleRate( );
		
		// Are we forced to use mono sound, and the current audio data is stereo? Convert.
		if( p_ForceMono && isMono == false )
		{
			// Divide the size
			dataSize /= 2;
			
			// Get the new format
			if( bitsPerSample == 8 )
			{
				// Set the right openAL format.
				openALFormat = AL_FORMAT_MONO8;

				// Make the average value out of the left and right sample value.
				// Go through all the 8 bit samples.
				for( SizeType i = 0; i < dataSize; i++ )
				{
					pData[ i ] = static_cast<Uint8>(
									 (	static_cast<Uint16>( pData[ i * 2 ] ) +
										static_cast<Uint16>( pData[ i * 2 + 1 ] ) ) / 2
						);
				}
			}
			else if( bitsPerSample == 16 )
			{
				// Set the right openAL format.
				openALFormat = AL_FORMAT_MONO16;

				// Make the average value out of the left and right sample value.
				// Go through all the 16 bit samples.
				for( SizeType i = 0; i < dataSize / 2; i+= 2 )
				{
					Int16 left = *( reinterpret_cast<Int16 *>( &pData[ i * 2 ] ) );
					Int16 right = *( reinterpret_cast<Int16 *>( &pData[ i * 2 + 2 ] ) );

					//Uint16 monoSample = left;
					Int16 monoSample = static_cast<Int16>(
										 (	static_cast<Int32>( left ) +
											static_cast<Int32>( right ) ) / 2
						);

					// Set the new mono sample.
					pData[ i ] =  static_cast<Uint8>( monoSample );
					pData[ i + 1 ] =  static_cast<Uint8>( monoSample >> 8 );
				}
			}
		}

		// Set the data in the buffer
		alBufferData( m_Buffer, openALFormat, (ALvoid*)pData, dataSize, frequence );
		
		// Set the buffer to the source
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