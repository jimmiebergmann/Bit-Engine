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

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/Audio/AudioBuffer.hpp>

namespace Bit
{
	
	class BIT_API Sound
	{

	public:

		// Destructor
		virtual ~Sound( ) { }

		// Public general functions
		virtual BIT_UINT32 Load(	const AudioBuffer & p_pAudioBuffer, const BIT_BOOL p_Loop = BIT_FALSE,
									const BIT_FLOAT32 p_Pitch = 1.0f, const BIT_FLOAT32 p_Volume = 1.0f,
									const Vector3_f32 p_Position = Vector3_f32( 0.0f, 0.0f, 0.0f ) ) = 0;
		virtual void Play( ) = 0;
		virtual void Pause( ) = 0;
		virtual void Stop( ) = 0;

		// Set functions
		virtual void SetPosition( const Vector3_f32 p_Position ) = 0;
		virtual void SetVolume( const BIT_FLOAT32 p_Volume ) = 0;
		virtual void SetPitch( const BIT_FLOAT32 p_Pitch ) = 0;
		virtual void SetLoop( const BIT_BOOL p_Loop ) = 0;
		virtual void SetRelative( const BIT_BOOL p_Relative ) = 0;

		// Get functions
		bool IsLoaded( ) const { return m_Loaded; }

	protected:

		BIT_BOOL m_Loaded;

	};

}

#endif