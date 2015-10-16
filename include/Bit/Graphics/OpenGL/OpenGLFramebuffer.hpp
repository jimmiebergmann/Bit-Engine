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

#ifndef BIT_GRAPHICS_OPENGL_FRAMEBUFFER_HPP
#define BIT_GRAPHICS_OPENGL_FRAMEBUFFER_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <Bit/Graphics/Framebuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLTexture.hpp>


namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL frambuffer class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLFramebuffer : public Framebuffer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		OpenGLFramebuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~OpenGLFramebuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Bind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( ) const;

		///////////////////////////////////////////////////////////////
		/// \brief Unbind framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Check if the framebuffer contains any errors.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool CheckForError();

		///////////////////////////////////////////////////////////////
		/// \brief Attach a texture to the framebuffer.
		///
		/// The attachment type depends on the pixel format used.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const Texture & p_pTexture );

		///////////////////////////////////////////////////////////////
		/// \brief Attach a render buffer to the framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Attach( const Renderbuffer & p_pRenderBuffer );

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
										const SizeType p_AttachmentCount);

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
							const Uint32 p_Attachment = Color ) const;

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
							const Uint32 p_Attachment = Color ) const;

		///////////////////////////////////////////////////////////////
		/// \brief Get OpenGL framebuffer ID.
		///
		////////////////////////////////////////////////////////////////
		GLuint GetId( ) const;

	private:

		GLuint m_FramebufferObject;						///< OpenGL framebuffer object.
		Uint32 m_ColorAttachmentCount;					///< Number of attached color textures/render buffers.
		Bool m_DepthAttached;							///< Indicates if a depth texture/render buffer is attached.
		static OpenGLFramebuffer s_DefaultFramebuffer;	///< Default framebuffer

	};

}

#endif
