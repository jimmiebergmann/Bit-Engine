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
	/*
	GL_BYTE
	GL_UNSIGNED_BYTE
	GL_SHORT
	GL_UNSIGNED_SHORT
	GL_INT
	GL_UNSIGNED_INT
	GL_HALF_FLOAT
	GL_FLOAT
	GL_DOUBLE
	GL_FIXED
	GL_INT_2_10_10_10_REV
	GL_UNSIGNED_INT_2_10_10_10_REV

	const BIT_UINT32 BIT_TYPE_NONE = 0;
	const BIT_UINT32 BIT_TYPE_UCHAR8 = 1;
	const BIT_UINT32 BIT_TYPE_SCHAR8 = 2;
	const BIT_UINT32 BIT_TYPE_UCHAR16 = 3;
	const BIT_UINT32 BIT_TYPE_SCHAR16 = 4;
	const BIT_UINT32 BIT_TYPE_UINT8 = 5;
	const BIT_UINT32 BIT_TYPE_SINT8 = 6;
	const BIT_UINT32 BIT_TYPE_UINT16 = 7;
	const BIT_UINT32 BIT_TYPE_SINT16 = 8;
	const BIT_UINT32 BIT_TYPE_UINT32 = 9;
	const BIT_UINT32 BIT_TYPE_SINT32 = 10;
	const BIT_UINT32 BIT_TYPE_UINT64 = 11;
	const BIT_UINT32 BIT_TYPE_SINT64 = 12;
	const BIT_UINT32 BIT_TYPE_FLOAT32 = 13;
	const BIT_UINT32 BIT_TYPE_FLOAT64 = 14;
	const BIT_UINT32 BIT_TYPE_BYTE = 15;
	const BIT_UINT32 BIT_TYPE_BOOL = 16;
*/

	static const GLenum OpenGLTypes[ 17 ] =
	{
		0, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
		GL_UNSIGNED_INT, GL_INT, 0, 0, GL_FLOAT, GL_DOUBLE, GL_BYTE, 0
	};
	static const BIT_UINT32 OpenGLTypeSize[ 17 ] =
	{
		0, sizeof( BIT_UCHAR8 ), sizeof( BIT_SCHAR8 ), sizeof( BIT_SCHAR8 ),  sizeof( BIT_UCHAR8 ),
		sizeof( BIT_UINT8 ), sizeof( BIT_SINT8 ), sizeof( BIT_UINT16 ), sizeof( BIT_SINT16 ),
		sizeof( BIT_UINT32 ), sizeof( BIT_SINT32 ), 0, 0, sizeof( BIT_FLOAT32 ), sizeof( BIT_FLOAT64 ),
		sizeof( BIT_BYTE ), 0
	};

	// Static private variables
	static GLenum RenderModes[ 3 ] = { GL_TRIANGLES, GL_LINES, GL_LINE_STRIP };


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
	BIT_UINT32 VertexObjectOpenGL::Load( const BIT_UINT32 p_PieceCount, const BIT_UINT32 p_PieceSize )
	{
		if( m_Loaded || !m_Buffers.size( ) )
		{
			bitTrace( "[ VertexObjectOpenGL::Load] Already loaded or no buffers are available\n" );
			return BIT_ERROR;
		}

		// Load the vertex buffer object
		glGenVertexArrays( 1, &m_VertexArrayObject );
		glBindVertexArray( m_VertexArrayObject );

		// Allocate memory for the VBOs
		m_VertexBufferObjectCount = m_Buffers.size( );
		m_pVertexBufferObjects = new GLuint [ m_VertexBufferObjectCount ];

		// Generate the VBOs
		glGenBuffers( m_VertexBufferObjectCount, m_pVertexBufferObjects );

		// Load every single vertex array object.
		for( BIT_MEMSIZE i = 0; i < m_VertexBufferObjectCount; i++ )
		{
			// Let's load it
			BIT_UINT32 ElementCount = m_Buffers[ i ].DimensionCount;
			BIT_UINT32 TotalBufferSize = p_PieceCount * p_PieceSize * ElementCount * m_Buffers[ i ].TypeSize;

			glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObjects[ i ] );
			glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)TotalBufferSize, m_Buffers[i].pBuffer, GL_STATIC_DRAW );
			glVertexAttribPointer( (GLuint)i, ElementCount, m_Buffers[ i ].Type, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray( i );
		}

		// Unbind the Array buffer
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		// Calculate the total piece size
		m_TotalPieceSize = p_PieceCount * p_PieceSize;

		// We are done, everything is fine
		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 VertexObjectOpenGL::LoadFullscreenQuad( const Vector2_ui32 p_Size )
	{
		// Make sure that the vertex object isn't alrady loaded.
		if( m_Loaded )
		{
			bitTrace( "[ VertexObjectOpenGL::LoadFullscreenQuad] Already loaded\n" );
			return BIT_ERROR;
		}

		// Load the vertex buffer object
		glGenVertexArrays( 1, &m_VertexArrayObject );
		glBindVertexArray( m_VertexArrayObject );

		// Allocate memory for the VBOs
		m_VertexBufferObjectCount = 2;
		m_pVertexBufferObjects = new GLuint [ 2 ];

		// Generate the VBOs
		glGenBuffers( m_VertexBufferObjectCount, m_pVertexBufferObjects );


		const BIT_FLOAT32 VertexPositions[ 18 ] =
		{
			0.0f, 0.0f, 0.0f,
			static_cast<const BIT_FLOAT32>(p_Size.x), 0.0f, 0.0f,
			static_cast<const BIT_FLOAT32>(p_Size.x), static_cast<const BIT_FLOAT32>(p_Size.y), 0.0f,
			0.0f, 0.0f, 0.0f,
			static_cast<const BIT_FLOAT32>(p_Size.x), static_cast<const BIT_FLOAT32>(p_Size.y), 0.0f,
			0.0f, static_cast<const BIT_FLOAT32>(p_Size.y), 0.0f
		};

		const BIT_FLOAT32 VertexTextures[ 12 ] =
		{
			0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,
			0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f
		};

		// Add the vertex position data
		glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObjects[ 0 ] );
		glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)72, VertexPositions, GL_STATIC_DRAW );
		glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( 0 );

		// Add the texture position data
		glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObjects[ 1 ] );
		glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)48, VertexTextures, GL_STATIC_DRAW );
		glVertexAttribPointer( (GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( 1 );

		// Unbind the Array buffer
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		// Set the total piece size
		m_TotalPieceSize = 6;

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
			glDeleteBuffers( m_VertexBufferObjectCount, m_pVertexBufferObjects );
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

	BIT_UINT32 VertexObjectOpenGL::AddVertexBuffer( void * p_pBuffer, const BIT_UINT32 p_VertexDimensions, const eDataType p_DataType )
	{
		if( m_Loaded )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Already loaded.\n" );
			return BIT_ERROR;
		}

		// Find the most fitting OpenGL data type
		if( p_DataType == 0 || p_DataType > 16 )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Wrong data type.\n" );
			return BIT_ERROR;
		}
		GLenum Type = OpenGLTypes[ p_DataType ];
		BIT_UINT32 TypeSize = OpenGLTypeSize[ p_DataType ];

		// Make sure the data type is an accepted data type for vertex object( m_Type != 0 )
		if( Type == 0 )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Wrong data type.\n" );
			return BIT_ERROR;
		}

		// Add the buffer to a temporary struct which is being used in the Load function.
		BufferStruct BufferData;
		BufferData.pBuffer = p_pBuffer;
		BufferData.DimensionCount = p_VertexDimensions;
		BufferData.Type = Type;
		BufferData.TypeSize = TypeSize;
		m_Buffers.push_back( BufferData );

		return BIT_OK;
	}

	BIT_UINT32 VertexObjectOpenGL::UpdateVertexBuffer( const BIT_UINT32 p_Index, const void * p_pBuffer,
		const BIT_UINT32 p_Offset, const BIT_UINT32 p_DataSize )
	{
		if( !m_Loaded )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Not loaded yet.\n" );
			return BIT_ERROR;
		}

		// Make sure the index isn't outside the array bound
		if( p_Index >= m_Buffers.size( ) )
		{
			bitTrace( "[ VertexObjectOpenGL::AddVertexBuffer] Index is out of bound.\n" );
			return BIT_ERROR;
		}

		// Bind and update the buffer
		glBindBuffer( GL_ARRAY_BUFFER, m_pVertexBufferObjects[ p_Index ] );

		glBufferSubData( GL_ARRAY_BUFFER, p_Offset * m_Buffers[ p_Index ].TypeSize,
			p_DataSize * m_Buffers[ p_Index ].TypeSize, p_pBuffer );

		return BIT_OK;
	}

	void VertexObjectOpenGL::Render( const eRenderMode p_Mode )
	{
		if( !m_Loaded )
		{
			return;
		}

		glBindVertexArray( m_VertexArrayObject );
		glDrawArrays( RenderModes[ static_cast<int>( p_Mode ) ], 0, m_TotalPieceSize );
		glBindVertexArray( 0 );
	}

	void VertexObjectOpenGL::Render( const eRenderMode p_Mode, BIT_UINT32 p_Start, BIT_UINT32 p_Length )
	{
		if( !m_Loaded )
		{
			return;
		}

		glBindVertexArray( m_VertexArrayObject );
		glDrawArrays( RenderModes[ static_cast<int>( p_Mode ) ], p_Start, p_Length );
		glBindVertexArray( 0 );
	}

}
