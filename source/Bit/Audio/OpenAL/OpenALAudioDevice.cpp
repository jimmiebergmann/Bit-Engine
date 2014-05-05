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

#include <Bit/Audio/OpenAL/OpenALAudioDevice.hpp>
#include <Bit/Audio/OpenAL/OpenALSound.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenALAudioDevice::OpenALAudioDevice( ) :
		m_Open( false ),
		m_pDevice( NULL ),
		m_pContext( NULL )
	{
	}

	OpenALAudioDevice::~OpenALAudioDevice( )
	{
		if( m_pContext )
		{
			alcMakeContextCurrent( NULL );
			alcDestroyContext( m_pContext );
		}

		if( m_pDevice )
		{
			alcCloseDevice( m_pDevice );
		}
	}

	Bool OpenALAudioDevice::Open( )
	{
		// Open the device
		if( ( m_pDevice = alcOpenDevice( NULL )) == NULL )
		{
			std::cout << "[OpenALAudioDevice::Create] Can not open the device.\n";
			return false;
		}

		// Create the context
		if( ( m_pContext = alcCreateContext( m_pDevice, NULL )) == NULL )
		{
			std::cout << "[OpenALAudioDevice::Create] Can not create the context.\n";
			return false;
		}

		// Make the context to the current one
		alcMakeContextCurrent( m_pContext );

		//Setting some standard settings
		ALfloat ListenerPos[ ] = { 0.0f, 0.0f, 0.0f };
		ALfloat ListenerVel[ ] = { 0.0f, 0.0f, 0.0f };
		ALfloat ListenerOri[ ] = { 0.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f };
		alListenerfv( AL_POSITION, ListenerPos );
		alListenerfv( AL_VELOCITY, ListenerVel );
		alListenerfv( AL_ORIENTATION, ListenerOri );
		alListenerf( AL_GAIN, 1.0f );

		// Succeeded
		return m_Open = true;
	}

	Sound * OpenALAudioDevice::CreateSound( )
	{
		return new OpenALSound;
	}

	void OpenALAudioDevice::SetGlobalVolume( Float32 p_Volume )
	{
		alListenerf( AL_GAIN, p_Volume );
	}

	void OpenALAudioDevice::SetListenerPosition( Vector3f32 p_Position )
	{
		ALfloat position[3] = { p_Position.x, p_Position.y, p_Position.z };
		alListenerfv( AL_POSITION, position );
	}

	void OpenALAudioDevice::SetListenerTarget( Vector3f32 p_Target )
	{
		// Fix this hardcoded mess...
		ALfloat target[6] = { p_Target.x, p_Target.y, p_Target.z, 0.0f, 1.0f, 0.0f };
		alListenerfv( AL_ORIENTATION, target );
	}

	void OpenALAudioDevice::SetListenerVelocity( Vector3f32 p_Velocity )
	{
		ALfloat velocity[3] = { p_Velocity.x, p_Velocity.y, p_Velocity.z };
		alListenerfv( AL_VELOCITY, velocity );
	}

	Bool OpenALAudioDevice::IsOpen( ) const
	{
		return m_Open;
	}	

}