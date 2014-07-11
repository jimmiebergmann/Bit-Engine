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

#include <Bit/Graphics/OpenGL/OpenGLVertexArray.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static GLenum g_OpenGLDataTypes[ static_cast<SizeType>( DataType::SizeType ) + 1 ] =
	{
		/*None		*/ 0,
		/*Int8		*/ GL_BYTE,
		/*Uint8		*/ GL_UNSIGNED_BYTE,
		/*Int16		*/ GL_SHORT,
		/*Uint16	*/ GL_UNSIGNED_SHORT,
		/*Int32		*/ GL_INT,
		/*Uint32	*/ GL_UNSIGNED_INT,
		/*Int64		*/ 0, 
		/*Uint64	*/ 0,
		/*Float32	*/ GL_FLOAT,
		/*Float64	*/ GL_DOUBLE,
		/*Bool		*/ GL_UNSIGNED_BYTE,
		/*SizeType	*/ 0
	};

	static SizeType g_OpenGLDataSize[ static_cast<SizeType>( DataType::SizeType ) + 1 ] =
	{
		/*None		*/ 0,
		/*Int8		*/ sizeof( Int8 ),
		/*Uint8		*/ sizeof( Int8 ),
		/*Int16		*/ sizeof( Int16 ),
		/*Uint16	*/ sizeof( Int16 ),
		/*Int32		*/ sizeof( Int32 ),
		/*Uint32	*/ sizeof( Int32 ),
		/*Int64		*/ 0,
		/*Uint64	*/ 0,
		/*Float32	*/ sizeof( Float32 ),
		/*Float64	*/ sizeof( Float64 ),
		/*Bool		*/ sizeof( Bool ),
		/*SizeType	*/ 0
	};

	static GLenum g_OpenGLPrimitiveModes[ static_cast<SizeType>( PrimitiveMode::TriangleFan ) + 1 ] =
	{
		/*Points		*/ GL_POINTS,
		/*Lines			*/ GL_LINES,
		/*LineStrip		*/ GL_LINE_STRIP,
		/*Triangles		*/ GL_TRIANGLES,
		/*TriangleStrip	*/ GL_TRIANGLE_STRIP,
		/*TriangleFan	*/ GL_TRIANGLE_FAN
	};

	OpenGLVertexArray::OpenGLVertexArray( ) :
		m_VertexArrayObject( 0 ),
		m_VertexSize( 0 ),
		m_BufferCount( 0 )
	{
	}

	OpenGLVertexArray::~OpenGLVertexArray( )
	{
		if( m_VertexArrayObject )
		{
			// Delete the VAO
			glBindVertexArray( 0 );
			glDeleteVertexArrays( 1, &m_VertexArrayObject );
		}
	}

	Bool OpenGLVertexArray::AddVertexBuffer(	VertexBuffer & p_VertexBufferObject,
												const SizeType p_ComponentCount, 
												const DataType::eType p_DataType,
												const Int32 p_VertexIndex )
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

		// Generate a VBO if needed.
		if( m_VertexArrayObject == 0 )
		{
			glGenVertexArrays( 1, &m_VertexArrayObject );
		}

		// Bind the VAO
		glBindVertexArray( m_VertexArrayObject );

		// Add the VBO(bind it)
		p_VertexBufferObject.Bind( );

		// Compute the vertex index.
		GLuint vertexIndex = m_BufferCount;
		if( p_VertexIndex >= 0 )
		{
			vertexIndex = static_cast<GLuint>( p_VertexIndex );
		}
		
		// Set the vertex attribute pointer at the current buffer index.
		glVertexAttribPointer( vertexIndex, p_ComponentCount, type, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( vertexIndex );

		// Unbind the VAO
		glBindVertexArray( 0 );

		// Increment the buffer count
		m_BufferCount++;

		// Calculate the vertex count
		m_VertexSize = p_VertexBufferObject.GetBufferSize( ) / g_OpenGLDataSize[ static_cast<SizeType>( p_DataType ) ];

		return true;
	}

	void OpenGLVertexArray::Render( PrimitiveMode::eMode p_PrimitiveMode ) const
	{
		glBindVertexArray( m_VertexArrayObject );
		glDrawArrays( g_OpenGLPrimitiveModes[ static_cast<SizeType>( p_PrimitiveMode ) ], 0, m_VertexSize );
		glBindVertexArray( 0 );
	}

}