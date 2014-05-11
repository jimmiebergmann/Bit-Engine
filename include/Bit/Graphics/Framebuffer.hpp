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

#ifndef BIT_GRAPHICS_FRAMEBUFFER_HPP
#define BIT_GRAPHICS_FRAMEBUFFER_HPP

#include <Bit/Build.hpp>

namespace Bit
{
	// Forward declaractions
	class Texture;
	class RenderBuffer;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Frambuffer base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Framebuffer 
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~Framebuffer( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Bind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( ) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Unbind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind( ) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Attach a texture to the framebuffer.
		///
		/// The attachment type depends on the pixel format used.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const Texture & p_pTexture ) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Attach a render buffer to the framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const RenderBuffer & p_pRenderBuffer ) = 0;

	};

}

#endif