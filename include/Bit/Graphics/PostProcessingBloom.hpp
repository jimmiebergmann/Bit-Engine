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

#ifndef __BIT_GRAPHICS_POST_PROCESSING_BLOOM_HPP__
#define __BIT_GRAPHICS_POST_PROCESSING_BLOOM_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/Texture.hpp>

namespace Bit
{

	class BIT_API PostProcessingBloom
	{

	public:

		// Destructor
		virtual ~PostProcessingBloom( ) { }

		// Public general functions
		virtual BIT_UINT32 Load( BIT_FLOAT32 p_Power, BIT_UINT32 p_BlurSize, BIT_FLOAT32 p_PixelSize ) = 0;
		virtual void Process( ) = 0;

		// Public set functions
		virtual void SetTexture( Texture * p_pTexture ) = 0;

		// Public get functions
		virtual Texture * GetTexture( ) const = 0;

	protected:


	};

}

#endif