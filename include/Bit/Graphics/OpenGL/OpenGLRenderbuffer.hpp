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

#ifndef BIT_GRAPHICS_OPENGL_RENDERBUFFER_HPP
#define BIT_GRAPHICS_OPENGL_RENDERBUFFER_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/Renderbuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL Renderbuffer base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLRenderbuffer : public Renderbuffer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		OpenGLRenderbuffer( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~OpenGLRenderbuffer( );

		///////////////////////////////////////////////////////////////
		/// \brief Load renderbuffer
		///
		/// \param p_Size The renderbuffer size
		/// \param p_Format The format for representing the color components.
		/// \param p_MultisampleLevel The number of samples in the multisample buffer.
		///		Multisampling is not being used if p_MultisampleLevel == 0.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Load(	const Vector2u32 p_Size,
							const Texture::ePixelFormat p_Format,
							const Uint32 p_MultisampleLevel = 0 );

		////////////////////////////////////////////////////////////////
		/// \brief Get the buffer's pixel format.
		///
		////////////////////////////////////////////////////////////////
		virtual Texture::ePixelFormat GetPixelFormat( ) const;

		
		////////////////////////////////////////////////////////////////
		/// \brief Get the buffer ID.
		///
		////////////////////////////////////////////////////////////////
		GLuint GetId( ) const;

	private:

		GLuint m_RenderbufferObject;			///< OpenGL framebuffer object.
		Texture::ePixelFormat m_PixelFormat;

	};

}

#endif