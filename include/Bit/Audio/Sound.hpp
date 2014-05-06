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

#ifndef BIT_AUDIO_SOUND_HPP
#define BIT_AUDIO_SOUND_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/Audio/AudioBuffer.hpp>
#include <string>

namespace Bit
{

	// Forward declarations
	class AudioBuffer;
	
	////////////////////////////////////////////////////////////////
	/// \ingroup Audio
	/// \brief Audio device base class.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API Sound
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		/// 
		////////////////////////////////////////////////////////////////
		virtual ~Sound( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Load sound from buffer.
		///
		/// \param p_pAudioBuffer The audio buffer.
		/// \param p_ForceMono Makes the sound mono if true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromBuffer(	const AudioBuffer & p_pAudioBuffer, 
										const bool p_ForceMono = false ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Load sound from file.
		///
		/// \param p_Filename Path to the file.
		/// \param p_ForceMono Makes the sound mono if true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromFile(	const std::string & p_Filename,
									const bool p_ForceMono = false ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Load sound from WAVE file.
		///
		/// \param p_Filename Path to the file.
		/// \param p_ForceMono Makes the sound mono if true.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromWaveFile(	const std::string & p_Filename,
										const bool p_ForceMono = false ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Play the sound.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void Play( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Pause the sound.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void Pause( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Stop the sound.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void Stop( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the position of the sound in 3D space.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetPosition( const Vector3f32 p_Position ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the volume. 0.1 to 1.0.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetVolume( const Float32 p_Volume ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the pith.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetPitch( const Float32 p_Pitch ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set loop flag.
		///
		/// The sound will keep on looping until you stop it
		/// if the flag is set to true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetLoop( const Bool p_Loop ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set relative flag.
		///
		/// The sound will play in different speaks depending on the
		/// sound position and the listener position if flag is set to true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual void SetRelative( const Bool p_Relative ) = 0;

	};

}

#endif