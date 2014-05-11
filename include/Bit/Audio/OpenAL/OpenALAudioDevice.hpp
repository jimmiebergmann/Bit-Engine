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

#ifndef BIT_AUDIO_OPENAL_AUDIO_DEVICE_HPP
#define BIT_AUDIO_OPENAL_AUDIO_DEVICE_HPP

#include <Bit/Build.hpp>
#include <Bit/Audio/AudioDevice.hpp>
#include <Bit/Audio/OpenAL/OpenAL.hpp>

namespace Bit
{
	
	////////////////////////////////////////////////////////////////
	/// \ingroup Audio
	/// \brief OpenAL audio device class.'
	///
	/// Stereo sounds wont work in 3D.
	/// Use "force mono" if you want to make sure that the sound
	/// will work in any 3D scene.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API OpenALAudioDevice : public AudioDevice
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////////
		OpenALAudioDevice( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. Closing the device.
		/// 
		////////////////////////////////////////////////////////////////
		~OpenALAudioDevice( );

		////////////////////////////////////////////////////////////////
		/// \brief Open the audio device.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool Open( );

		////////////////////////////////////////////////////////////////
		/// \brief Close the audio device.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( );

		////////////////////////////////////////////////////////////////
		/// \brief Making this audio device to the current one.
		///
		////////////////////////////////////////////////////////////////
		virtual void MakeCurrent( );

		////////////////////////////////////////////////////////////////
		/// \brief Create a sound object.
		///
		/// \return A pointer to the Sound
		/// 
		////////////////////////////////////////////////////////////////
		virtual Sound * CreateSound( );

		////////////////////////////////////////////////////////////////
		/// \brief Create a sound buffer object.
		///
		/// \return A pointer to the sound buffer.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SoundBuffer * CreateSoundBuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Set the global volume. Value from 0.0 to 1.0.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetGlobalVolume( Float32 p_Volume );

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position 3D position of the listener.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerPosition( Vector3f32 p_Position );

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position 3D position of the target.
		///		Defines what direction the listener is looking.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerTarget( Vector3f32 p_Target );

		////////////////////////////////////////////////////////////////
		/// \brief Set the listener position.
		///
		/// \param p_Position Set the velocity of the listener.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetListenerVelocity( Vector3f32 p_Velocity );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the device is open.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const;

	private:

		// Private varaibles
		Bool m_Open;				///< Is the device open?
		ALCdevice * m_pDevice;		///< OpenAL device.
		ALCcontext * m_pContext;	///< OpenAl context.

	};

}

#endif