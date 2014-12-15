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

#ifndef BIT_GRAPHICS_POST_PROCESSING_BLOOM_HPP
#define BIT_GRAPHICS_POST_PROCESSING_BLOOM_HPP

#include <Bit/Build.hpp>
#include <Bit/graphics/PostProcessing.hpp>

namespace Bit
{

	// Forward declaractions
	class Framebuffer;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Bloom post-processing base class.
	///
	/// \see PostProcessing
	///
	////////////////////////////////////////////////////////////////
	class BIT_API PostProcessingBloom : public PostProcessing
	{

	public:

		///////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~PostProcessingBloom( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Set the source framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetSource( Framebuffer * p_pSource ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set the target framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetTarget( Framebuffer * p_pTarget ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the source framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Framebuffer * GetSource( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the target framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Framebuffer * GetTarget( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Render the post processed effect to the target frame buffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Render(  ) const = 0;

	};

}

#endif