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

#include <Bit/Graphics/OpenGL/OpenGLVertexArrayObject.hpp>

namespace Bit
{

	static GLenum g_OpenGLDataTypes[ static_cast<SizeType>( DataType::SizeType ) + 1 ] =
	{
		/*None		*/ 0,
		/*Int8,		*/ GL_BYTE,
		/*Uint8,	*/ GL_UNSIGNED_BYTE,
		/*Int16,	*/ GL_SHORT,
		/*Uint16,	*/ GL_UNSIGNED_SHORT,
		/*Int32,	*/ GL_INT,
		/*Uint32,	*/ GL_UNSIGNED_INT,
		/*Int64,	*/ 0, 
		/*Uint64,	*/ 0,
		/*Float32,	*/ GL_FLOAT,
		/*Float64,	*/ GL_DOUBLE,
		/*Bool,		*/ GL_UNSIGNED_BYTE,
		/*SizeType	*/ 0
	};

	OpenGLVertexArrayObject::OpenGLVertexArrayObject( ) :
		m_VertexArrayObject( 0 ),
		m_VertexCount( 0 ),
		m_BufferCount( 0 )
	{
		// Generate a VBO
		glGenVertexArrays( 1, &m_VertexArrayObject );
	}

	OpenGLVertexArrayObject::~OpenGLVertexArrayObject( )
	{
		if( m_VertexArrayObject )
		{
			// Delete the VAO
			glBindVertexArray( 0 );
			glDeleteVertexArrays( 1, &m_VertexArrayObject );
		}
	}

	bool OpenGLVertexArrayObject::AddVertexBuffer(	VertexBufferObject & p_VertexBufferObject,
													const SizeType p_ComponentCount, 
													const DataType::eType p_DataType )
	{
		// Make sure the component count is a value between 1 and 4
		if( p_ComponentCount < 1 || p_ComponentCount > 4 )
		{
			return false;
		}

		// Get the OpenGL data type and error check the type
		GLenum type = g_OpenGLDataTypes[ static_cast<SizeType>( p_DataType ) ];
		if( type == 0 )
		{
			return false;
		}

		// Bind the VAO
		glBindVertexArray( m_VertexArrayObject );

		// Add the VBO(bind it)
		p_VertexBufferObject.Bind( );
		glEnableVertexAttribArray( 0 );
		
		// Set the vertex attribute pointer at the current buffer index.
		glVertexAttribPointer( m_BufferCount, p_ComponentCount, type, GL_FALSE, 0, 0 );

		// Unbind the VAO
		glBindVertexArray( 0 );

		// Increment the buffer count
		m_BufferCount++;
	}

	void OpenGLVertexArrayObject::Render( )
	{
		glBindVertexArray( m_VertexArrayObject );
		glDrawArrays( GL_TRIANGLES, 0, m_VertexCount );
		glBindVertexArray( 0 );
	}

}