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

#include <Bit/Graphics/OpenGL/VertexObjectOpenGL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// Static private variables(etg
	GLenum VertexObjectOpenGL::s_RenderModes[ 3 ] = { GL_TRIANGLES, GL_TRIANGLES, GL_TRIANGLES };


	// Constructor / Destructor
	VertexObjectOpenGL::VertexObjectOpenGL( ) :
		m_VertexArrayObject( 0 ),
		m_pVertexBufferObjects( BIT_NULL )
	{
		m_Loaded = BIT_FALSE;
	}

	VertexObjectOpenGL::~VertexObjectOpenGL( )
	{
		Unload( );
	}

	// Virtual public functions
	BIT_UINT32 VertexObjectOpenGL::Load( BIT_UINT32 p_PieceCount, BIT_UINT32 p_PieceSize )
	{
		if( m_Loaded || !m_BufferVector.size( ) )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Already loaded or no buffers available.\n" );
			return BIT_ERROR;
		}

		// Load the vertex buffer object
		glGenVertexArrays( 1, &m_VertexArrayObject );
		glBindVertexArray( m_VertexArrayObject );
 
		// Allocate memory for the VBOs
		m_VertexBufferObjectCount = m_BufferVector.size( );
		m_pVertexBufferObjects = new GLuint [ m_VertexBufferObjectCount ];
		
		// Generate the VBOs
		glGenBuffers( m_VertexBufferObjectCount, m_pVertexBufferObjects );

		// Load every single vertex array object.
		for( BIT_MEMSIZE i = 0; i < m_VertexBufferObjectCount; i++ )
		{
			// Let's load it
			BIT_UINT32 ElementCount = m_BufferVector[ i ].DimensionCount;
			BIT_UINT32 TotalBufferSize = p_PieceCount * p_PieceSize * ElementCount * sizeof( BIT_FLOAT32 );
			
			glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObjects[ i ] );
			glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)TotalBufferSize, m_BufferVector[i].pBuffer, GL_STATIC_DRAW );
			glVertexAttribPointer( (GLuint)i, ElementCount, GL_FLOAT, GL_FALSE, 0, 0 ); 
			glEnableVertexAttribArray( i );
		}

		// Unbind the Arrau buffer
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		// Calculate the total piece size
		m_TotalPieceSize = p_PieceCount * p_PieceSize;

		// Clear the buffer vector, we don't need it anymore
		m_BufferVector.clear( );

		// We are done, everything is fine
		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 VertexObjectOpenGL::Unload( )
	{
		if( m_pVertexBufferObjects != BIT_NULL )
		{
			// Delete the VBOs
			glBindBuffer ( GL_ARRAY_BUFFER, 0 );
			glDeleteBuffers( 3, m_pVertexBufferObjects );
			delete [ ] m_pVertexBufferObjects;
			m_pVertexBufferObjects = BIT_NULL;
		}

		if( m_VertexArrayObject )
		{
			// Delete the VAO
			glBindVertexArray( 0 );
			glDeleteVertexArrays( 1, &m_VertexArrayObject );
			m_VertexArrayObject = 0;
		}

		m_Loaded = BIT_FALSE;
		return BIT_OK;
	}

	BIT_UINT32 VertexObjectOpenGL::AddVertexBuffer( void * p_pBuffer, const BIT_UINT32 p_VertexDimensions )
	{
		if( m_Loaded )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Already loaded.\n" );
			return BIT_ERROR;
		}
		
		// Add the buffer to a temporary struct which is being used in the Load function.
		BufferStruct BufferData;
		BufferData.pBuffer = p_pBuffer;
		BufferData.DimensionCount = p_VertexDimensions;
		m_BufferVector.push_back( BufferData );
		
		return BIT_OK;
	}

	void VertexObjectOpenGL::Render( eRenderMode p_Mode )
	{
		if( !m_Loaded )
		{
			return;
		}

		glBindVertexArray( m_VertexArrayObject );
		glDrawArrays( s_RenderModes[ (int)( p_Mode ) ], 0, m_TotalPieceSize );
		glBindVertexArray( 0 );
	}

}