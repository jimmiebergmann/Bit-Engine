#include <Bit/Audio/OpenAL/AudioDeviceOAL.hpp>
#include <Bit/Audio/OpenAL/SoundOAL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	AudioDeviceOAL::AudioDeviceOAL( ) :
		m_pDevice( BIT_NULL ),
		m_pContext( BIT_NULL)
	{
		m_Open = BIT_FALSE;
	}

	AudioDeviceOAL::~AudioDeviceOAL( )
	{
		if( m_pContext )
		{
			alcMakeContextCurrent( BIT_NULL );
			alcDestroyContext( m_pContext );
			m_pContext = BIT_NULL;
		}

		if( m_pDevice )
		{
			alcCloseDevice( m_pDevice );
			m_pDevice = BIT_NULL;
		}

		m_Open = BIT_FALSE;
	}

	BIT_UINT32 AudioDeviceOAL::Open( )
	{
		// Open the device
		if( ( m_pDevice = alcOpenDevice( BIT_NULL )) == BIT_NULL )
		{
			bitTrace( BIT_NULL, "[Bit::AudioDeviceOAL::Create] <ERROR> "
				"Can not open the device. \n" );
			return BIT_ERROR;
		}

		// Create the context
		if( ( m_pContext = alcCreateContext( m_pDevice, BIT_NULL )) == BIT_NULL )
		{
			bitTrace( BIT_NULL, "[Bit::AudioDeviceOAL::Create] <ERROR> "
				"Can not create the context \n" );
			return BIT_ERROR;
		}

		// Make the context to the current one
		alcMakeContextCurrent( m_pContext );

		//Setting some standard settings
		ALfloat ListenerPos[] = { 0.0f, 0.0f, 0.0f };
		ALfloat ListenerVel[] = { 0.0f, 0.0f, 0.0f };
		ALfloat ListenerOri[] = { 0.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f };
		alListenerfv( AL_POSITION, ListenerPos );
		alListenerfv( AL_VELOCITY, ListenerVel );
		alListenerfv( AL_ORIENTATION, ListenerOri );
		alListenerf( AL_GAIN, 1.0f );

		m_Open = BIT_TRUE;
		return BIT_OK;
	}

	Sound * AudioDeviceOAL::CreateSound( )
	{
		return new SoundOAL( );
	}

	void AudioDeviceOAL::SetGlobalVolume( BIT_FLOAT32 p_Volume )
	{
		alListenerf( AL_GAIN, p_Volume );
	}

	void AudioDeviceOAL::SetListenerPosition( Vector3_f32 p_Position )
	{
		ALfloat Position[3] = { p_Position.x, p_Position.y, p_Position.z };
		alListenerfv( AL_POSITION, Position );
	}

	void AudioDeviceOAL::SetListenerTarget( Vector3_f32 p_Target )
	{
		// Fix this hardcoded mess...
		ALfloat Target[6] = { p_Target.x, p_Target.y, p_Target.z, 0.0f, 1.0f, 0.0f };
		alListenerfv( AL_ORIENTATION, Target );
	}

	void AudioDeviceOAL::SetListenerVelocity( Vector3_f32 p_Velocity )
	{
		ALfloat Velocity[3] = { p_Velocity.x, p_Velocity.y, p_Velocity.z };
		alListenerfv( AL_VELOCITY, Velocity );
	}

	

}