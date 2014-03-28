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

#include <Bit/Graphics/OpenGL/OpenGLVertexBufferObject.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenGLVertexBufferObject::OpenGLVertexBufferObject(  ) :
		m_pVertexBufferObject( 0 ),
		m_DataSize( 0 )
	{
		glGenBuffers( 1, &m_pVertexBufferObject );
	}

	OpenGLVertexBufferObject::OpenGLVertexBufferObject( const SizeType p_DataSize, void * p_pVertexData ) :
		m_pVertexBufferObject( 0 ),
		m_DataSize( 0 )
	{
		glGenBuffers( 1, &m_pVertexBufferObject );
		Load( p_DataSize, p_pVertexData );
	}

	OpenGLVertexBufferObject::~OpenGLVertexBufferObject( )
	{
		if(m_pVertexBufferObject )
		{
			// Delete the VBO
			glBindBuffer ( GL_ARRAY_BUFFER, 0 );
			glDeleteBuffers( 1, &m_pVertexBufferObject );
		}
	}

	void OpenGLVertexBufferObject::Bind( ) const
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObject );
	}

	Bool OpenGLVertexBufferObject::Load( const SizeType p_DataSize, void * p_pVertexData )
	{
		// Check the buffer size
		if( p_DataSize == 0 )
		{

			return false;
		}

		// Bind and create allocate the VBO
		glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObject );
		glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>( p_DataSize ), reinterpret_cast<GLvoid*>( p_pVertexData ), GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		// Set the data size
		m_DataSize = p_DataSize;

		// Succeeded
		return true;
	}

	SizeType OpenGLVertexBufferObject::GetBufferSize( ) const
	{
		return m_DataSize;
	}

}