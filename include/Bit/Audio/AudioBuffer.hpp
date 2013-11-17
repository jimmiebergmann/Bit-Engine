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

#ifndef BIT_AUDIO_BUFFER_HPP
#define BIT_AUDIO_BUFFER_HPP

#include <Bit/DataTypes.hpp>

namespace Bit
{
	
	class BIT_API AudioBuffer
	{

	public:

		// Constructor/destructor
		AudioBuffer( );
		~AudioBuffer( );

		// Public functions
		BIT_UINT32 Read( char * p_pFileName );

		// Get functions
		BIT_BYTE * GetData( ) const;
		BIT_UINT32 GetDataSize( ) const;
		BIT_UINT16 GetChannelCount( ) const;
		BIT_UINT32 GetSampleRate( ) const;
		BIT_UINT16 GetBitsPerSample( ) const;
		BIT_BOOL ContainsData( ) const;

	private:

		// Private functions
		BIT_UINT32 ReadWAVE( char *p_pFileName );

		// Private variables
		BIT_BOOL m_Loaded;
		BIT_UINT16 m_ChannelCount;
		BIT_UINT32 m_SampleRate;
		BIT_UINT16 m_BitsPerSample;
		BIT_UINT32 m_BufferSize;
		BIT_BYTE * m_pBuffer;


	};

}

#endif
