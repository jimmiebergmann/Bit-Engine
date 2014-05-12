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
#include <Bit/System/Vector2.hpp>

namespace Bit
{
	// Forward declaractions
	class Texture;
	class Renderbuffer;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Frambuffer base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Framebuffer 
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Attachment enumerator.
		///
		////////////////////////////////////////////////////////////////
		enum eAttachment
		{
			Color,			///< Color attachment.
			Depth,			///< Depth attachment.
			DepthStencil	///< Depth and color attachment.
		};

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
		virtual Bool Attach( const Renderbuffer & p_pRenderBuffer ) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Blit the framebuffer to another.
		///
		////////////////////////////////////////////////////////////////
		virtual void Blit(	const Framebuffer & p_Destination,
							const eAttachment p_Attachment,
							const Vector2u32 p_DestinationPosition,
							const Vector2u32 p_DestinationSize,
							const Vector2u32 p_SourcePosition,
							const Vector2u32 p_SourceSize ) const = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Blit the framebuffer to the standard framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Blit(	const eAttachment p_Attachment,
							const Vector2u32 p_DestinationPosition,
							const Vector2u32 p_DestinationSize,
							const Vector2u32 p_SourcePosition,
							const Vector2u32 p_SourceSize ) const = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Get default framebuffer object.
		///
		////////////////////////////////////////////////////////////////
		virtual const Framebuffer & GetDefault( ) const = 0;

	};

}

#endif