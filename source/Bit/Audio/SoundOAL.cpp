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

#include <Bit/Audio/OpenAL/SoundOAL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	SoundOAL::SoundOAL( ) :
		m_Source( 0 ),
		m_Buffer( 0 )
	{
		m_Loaded = BIT_FALSE;
	}

	SoundOAL::~SoundOAL( )
	{
		if( m_Source )
		{
			alDeleteSources( 1, &m_Source );
		}
		if( m_Source )
		{
			alDeleteBuffers( 1, &m_Buffer );
		}
		m_Loaded = BIT_FALSE;
	}

	BIT_UINT32 SoundOAL::Load( const AudioBuffer & p_pAudioBuffer, const BIT_BOOL p_Loop,
			const BIT_FLOAT32 p_Pitch, const BIT_FLOAT32 p_Volume, const Vector3_f32 p_Position )
	{
		// Generate the openal buffers
		alGenBuffers( 1, &m_Buffer );
		alGenSources( 1, &m_Source );

		// Set the audio format
		BIT_UINT16 BitsPerSample = p_pAudioBuffer.GetBitsPerSample( );
		BIT_UINT16 ChannelCount = p_pAudioBuffer.GetChannelCount( );

		if( BitsPerSample == 8 )
		{
			if( ChannelCount == 1 )
			{
				m_Format = AL_FORMAT_MONO8;
			}
			else if( ChannelCount == 2 )
			{
				m_Format = AL_FORMAT_STEREO8;
			}
		}
		else if(BitsPerSample == 16 )
		{
			if( ChannelCount == 1 )
			{
				m_Format = AL_FORMAT_MONO16;
			}
			else if( ChannelCount == 2 )
			{
				m_Format = AL_FORMAT_STEREO16;
			}
		}
		else
		{
			bitTrace( BIT_NULL, "[Bit::SoundOAL::Load] <ERROR> "
				"No fitting format.\n" );
			return BIT_ERROR;
		}

		// Create the buffer
		BIT_BYTE * Data = p_pAudioBuffer.GetData( );
		BIT_UINT32 DataSize = p_pAudioBuffer.GetDataSize( );
		BIT_UINT32 Frequence = p_pAudioBuffer.GetSampleRate( );
		
		alBufferData( m_Buffer, m_Format, (ALvoid*)Data, DataSize, Frequence );
		
		// Set the buffer
		alSourcei( m_Source, AL_BUFFER, m_Buffer );

		// Set the rest of the settings
		ALfloat Position[3] = { p_Position.x, p_Position.y, p_Position.z };

		alSourcef( m_Source, AL_PITCH, p_Pitch ); 
		alSourcef( m_Source, AL_GAIN, p_Volume ); 
		alSourcefv( m_Source, AL_POSITION, Position );
		alSourcei( m_Source, AL_LOOPING, p_Loop ); 

		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	void SoundOAL::SetPosition( const Vector3_f32 p_Position )
	{
		ALfloat Position[3] = { p_Position.x, p_Position.y, p_Position.z };
		alSourcefv( m_Source, AL_POSITION, Position );
	}

	void SoundOAL::SetVolume( const BIT_FLOAT32 p_Volume )
	{
		alSourcef( m_Source, AL_GAIN, p_Volume );
	}

	void SoundOAL::SetPitch( const BIT_FLOAT32 p_Pitch )
	{
		alSourcef( m_Source, AL_PITCH, p_Pitch ); 
	}

	void SoundOAL::SetLoop( const BIT_BOOL p_Loop )
	{
		alSourcei( m_Source, AL_LOOPING, p_Loop ); 
	}

	void SoundOAL::Play( )
	{
		alSourcePlay( m_Source );
	}

	void SoundOAL::SetRelative( const BIT_BOOL p_Relative )
	{
		alSourcei( m_Source, AL_SOURCE_RELATIVE, p_Relative ); 
	}

	void SoundOAL::Pause( )
	{
		alSourcePause( m_Source );
	}

	void SoundOAL::Stop( )
	{
		alSourceStop( m_Source );
	}

}

