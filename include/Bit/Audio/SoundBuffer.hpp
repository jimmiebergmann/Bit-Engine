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

#ifndef BIT_AUDIO_SOUND_BUFFER_HPP
#define BIT_AUDIO_SOUND_BUFFER_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{
	
	////////////////////////////////////////////////////////////////
	/// \ingroup Audio
	/// \brief Sound buffer class.
	///
	/// This class loads audio files into a buffer.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API SoundBuffer
	{

	public:

		/////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		/// 
		////////////////////////////////////////////////////////////////
		virtual ~SoundBuffer( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Load buffer from memory
		///
		/// Loads raw audio data from memory.
		/// You are allowed to delete the passed data when the function call is done.
		///
		///	\param p_pData Pointer to the audio data.
		/// \param p_DataSize Audio data size.
		///	\param p_ChannelCount Channel count. 1 - mono, 2 - stereo, etc.
		/// \param p_SampleRate Sample rate, 8000, 44100, etc.
		/// \param m_BitsPerSample Bits per sample, 8, 16, etc.
		/// \param p_ForceMono Converting the sound to mono if true.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromMemory(	const Uint8 * p_pData, const SizeType p_DataSize,
										const SizeType p_ChannelCount, const SizeType p_SampleRate,
										const SizeType p_BitsPerSample, const bool p_ForceMono = false ) = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Load buffer from file.
		///
		/// This function will figure out what file format to load,
		/// depending on the file extension.
		///
		/// \param p_Filename Path to the file.
		/// \param p_ForceMono Converting the sound to mono if true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromFile(	const std::string & p_Filename, 
									const bool p_ForceMono = false ) = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Load buffer from WAV file.
		///
		/// \param p_Filename Path to the file.
		/// \param p_ForceMono Converting the sound to mono if true.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Bool LoadFromWavFile(	const std::string & p_Filename, 
										const bool p_ForceMono = false ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Destroy the sound buffer.
		///
		/// Not necessary to call before the destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual void Destroy( ) = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get the raw data.
		/// 
		////////////////////////////////////////////////////////////////
		virtual Uint8 * GetData( ) const = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get the size of the raw data.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SizeType GetDataSize( ) const = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get the channel count.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SizeType GetChannelCount( ) const = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get the sample rate.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SizeType GetSampleRate( ) const = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get the bits per sample count.
		/// 
		////////////////////////////////////////////////////////////////
		virtual SizeType GetBitsPerSample( ) const = 0;

		/////////////////////////////////////////////////////////////////
		/// \brief Get buffer.
		/// 
		////////////////////////////////////////////////////////////////
		virtual const void * GetBuffer( ) const = 0;

	};

}

#endif
