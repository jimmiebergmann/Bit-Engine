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
			Color = 1,			///< Color attachment.
			Depth = 2,			///< Depth attachment.
			DepthStencil = 4	///< Depth and color attachment.
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
		virtual void Bind() const = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Unbind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind() const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Check if the framebuffer contains any errors.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool CheckForError() = 0;
		
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
		virtual Bool Attach(const Renderbuffer & p_pRenderBuffer) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Set output buffers
		///
		/// \param p_Attachments Array of buffer attachments.
		/// \param p_AttachmentIndices Array of attachment indices.
		/// \param p_AttachmentCount Number of attachments.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool SetOutputBuffers(	const eAttachment * p_Attachments,
										const SizeType * p_AttachmentIndices,
										const SizeType p_AttachmentCount) = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Blit the framebuffer to another.
		///
		/// \param p_Destination The destination framebuffer.
		/// \param p_Size The size of the area to blit, from the data position(0, 0).
		/// \param p_Attachment Bitfield mask of the attachments to blit.
		///		The available attachments are defined in Framebuffer::eAttachment.
		///
		////////////////////////////////////////////////////////////////
		virtual void Blit(	const Framebuffer & p_Destination,
							const Vector2u32 p_Size,
							const Uint32 p_Attachment = Color ) const = 0;

		///////////////////////////////////////////////////////////////
		/// \brief Blit the framebuffer to another.
		///
		/// \param p_Destination The destination framebuffer.
		/// \param p_DestinationBoundLow The lower bounds of the destination.
		/// \param p_DestinationBoundHigh The higher bounds of the destination.
		/// \param p_SourceBoundLow The lower bounds of the source.
		/// \param p_SourceBoundHigh The higher bounds of the source.
		/// \param p_Attachment Bitfield mask of the attachments to blit.
		///		The available attachments are defined in Framebuffer::eAttachment.
		///
		////////////////////////////////////////////////////////////////
		virtual void Blit(	const Framebuffer & p_Destination,
							const Vector2u32 p_DestinationBoundLow,
							const Vector2u32 p_DestinationBoundHigh,
							const Vector2u32 p_SourceBoundLow,
							const Vector2u32 p_SourceBoundHigh,
							const Uint32 p_Attachment = Color ) const = 0;

	};

}

#endif