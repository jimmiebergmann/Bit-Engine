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

#ifndef __BIT_GRAPHICS_FRAMEBUFFER_OPENGL_HPP__
#define __BIT_GRAPHICS_FRAMEBUFFER_OPENGL_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/Framebuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{
	class BIT_API FramebufferOpenGL : public Framebuffer
	{

	public:

		// Constructor/destructor
		FramebufferOpenGL( );
		virtual ~FramebufferOpenGL( );

		// Virtual public functions
		virtual void Bind( );
		virtual void Unbind( );
		virtual BIT_UINT32 Attach( const Texture * p_pTexture );

		// Get functions
		BIT_INLINE GLuint GetID( ) const { return m_FramebufferObject; }

	private:

		GLuint m_FramebufferObject;
		BIT_UINT32 m_ColorAttachmentCount;
		BIT_UINT32 m_DepthAttachmentCount;

	};

}

#endif
