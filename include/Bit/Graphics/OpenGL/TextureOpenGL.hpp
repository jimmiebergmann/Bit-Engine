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

#ifndef __BIT_GRAPHICS_TEXTURE_OPENGL_HPP__
#define __BIT_GRAPHICS_TEXTURE_OPENGL_HPP__

#include <Bit/Graphics/Texture.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{
	
	class BIT_API TextureOpenGL : public Texture
	{

	public:

		// Constructor/destrucotr
		TextureOpenGL( const BIT_BOOL p_OpenGL2 );
		virtual ~TextureOpenGL ( );

		// General public functions
		virtual BIT_UINT32 Load( const Image & p_Image, const BIT_BOOL p_Mipmapping );
		virtual BIT_UINT32 Load( Vector2_ui32 p_Size, const eColorComponent p_Format,
			const eColorComponent p_InternalFormat, const eDataType p_FormatType, void * p_Data );
		virtual void Bind( BIT_UINT32 p_Index );
		virtual void Unbind( );

		// Set functions
		virtual BIT_UINT32 SetFilters( const eFilter * p_pFilters );

		// Get functions
		BIT_INLINE GLuint GetID( ) const { return m_ID; }

	private:

		// Private variables
		GLuint m_ID;
		BIT_BOOL m_OpenGL2; // Indicate if we are using OpenGL 2
	};
}

#endif