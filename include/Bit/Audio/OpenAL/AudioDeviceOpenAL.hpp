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

#ifndef BIT_AUDIO_DEVICE_OPENAL_HPP
#define BIT_AUDIO_DEVICE_OPENAL_HPP

#include <Bit/Audio/AudioDevice.hpp>
#include <Bit/Audio/OpenAL/OpenAL.hpp>


namespace Bit
{
	
	class AudioDeviceOpenAL : public AudioDevice
	{

	public:

		// Constructors/destructors
		AudioDeviceOpenAL( );
		~AudioDeviceOpenAL( );

		// Public general functions
		virtual BIT_UINT32 Open( );

		// Create functions for different audio elements
		virtual Audio * CreateAudio( );

		// Set functions
		virtual void SetGlobalVolume( BIT_FLOAT32 p_Volume );
		virtual void SetListenerPosition( Vector3_f32 p_Position );
		virtual void SetListenerTarget( Vector3_f32 p_Target );
		virtual void SetListenerVelocity( Vector3_f32 p_Velocity );

	private:

		ALCdevice * m_pDevice;
		ALCcontext * m_pContext;

	};

}

#endif