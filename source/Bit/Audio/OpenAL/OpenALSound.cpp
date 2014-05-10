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
#include <Bit/Audio/OpenAL/OpenALSoundBuffer.hpp>
#include <Bit/Audio/WaveFile.hpp>
#include <algorithm>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenALSound::OpenALSound( ) :
		m_Source( 0 )
	{
	}

	OpenALSound::OpenALSound( const SoundBuffer & p_SoundBuffer )
	{
		// Generate the source
		alGenSources( 1, &m_Source );

		// Set up the source.
		static const ALfloat position[ 3 ] = { 0.0f, 0.0f, 0.0f };
		alSourcef( m_Source, AL_PITCH, 1.0f ); 
		alSourcef( m_Source, AL_GAIN, 1.0f ); 
		alSourcefv( m_Source, AL_POSITION, position );
		alSourcei( m_Source, AL_LOOPING, false ); 
		alSourcei( m_Source, AL_SOURCE_RELATIVE, true );

		// Attach sound buffer.
		AttachBuffer( p_SoundBuffer );
	}

	OpenALSound::~OpenALSound( )
	{
		// Delete source
		if( m_Source )
		{
			alDeleteSources( 1, &m_Source );
		}
	}

	Bool OpenALSound::LoadFromBuffer(	const SoundBuffer & p_SoundBuffer )
	{
		// Generate the source if necessary.
		if( m_Source == 0 )
		{
			alGenSources( 1, &m_Source );
		}

		// Set up the source.
		static const ALfloat position[ 3 ] = { 0.0f, 0.0f, 0.0f };
		alSourcef( m_Source, AL_PITCH, 1.0f ); 
		alSourcef( m_Source, AL_GAIN, 1.0f ); 
		alSourcefv( m_Source, AL_POSITION, position );
		alSourcei( m_Source, AL_LOOPING, false ); 
		alSourcei( m_Source, AL_SOURCE_RELATIVE, true );

		// Attach sound buffer.
		AttachBuffer( p_SoundBuffer );

		return true;
	}


	void OpenALSound::AttachBuffer(	const SoundBuffer & p_SoundBuffer )
	{
		const ALuint * pBuffer = reinterpret_cast<const ALuint *>( p_SoundBuffer.GetBuffer( ) );
		alSourcei( m_Source, AL_BUFFER, *pBuffer );
	}

	void OpenALSound::Destroy( )
	{
		if( m_Source )
		{
			alDeleteSources( 1, &m_Source );
			m_Source = 0;
		}
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