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

#ifndef BIT_AUDIO_AUDIO_DEVICE_HPP
#define BIT_AUDIO_AUDIO_DEVICE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	// Forward declarations
	class Sound;
	class SoundBuffer;
	
	////////////////////////////////////////////////////////////////
	/// \ingroup Audio
	/// \brief Audio device base class.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API AudioDevice
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		/// 
		////////////////////////////////////////////////////////////////
		virtual ~AudioDevice( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Open the audio device.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool Open( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a sound object.
		///
		/// \return A pointer to the sound.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Sound * CreateSound( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a sound buffer object.
		///
		/// \return A pointer to the sound buffer.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SoundBuffer * CreateSoundBuffer( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the global volume. Value from 0.0 to 1.0.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetGlobalVolume( Float32 p_Volume ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position 3D position of the listener.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerPosition( Vector3f32 p_Position ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position 3D position of the target.
		///		Defines what direction the listener is looking.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerTarget( Vector3f32 p_Target ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position Set the velocity of the listener.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerVelocity( Vector3f32 p_Velocity ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the device is open.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const = 0;

	};

}

#endif