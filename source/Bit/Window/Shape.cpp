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

#include <Bit/Window/Shape.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/VertexBuffer.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Shape::Shape( ) :
		m_Position( 0.0f, 0.0f ),
		m_Size( 1.0f, 1.0f ),
		m_Rotation( Radians( 0.0f ) ),
		m_pVertexArray( NULL ),
		m_pVertexBuffer( NULL )
	{
	}

	Shape::~Shape( )
	{
		if( m_pVertexBuffer )
		{
			delete m_pVertexBuffer;
		}

		if( m_pVertexArray )
		{
			delete m_pVertexArray;
		}
	}

	Bool Shape::LoadQuad( GraphicDevice * p_pGraphicDevice )
	{
		// Allocate the pointers
		if( ( m_pVertexBuffer = p_pGraphicDevice->CreateVertexBuffer( ) ) == NULL )
		{
			return false;
		}

		if( ( m_pVertexArray = p_pGraphicDevice->CreateVertexArray( ) ) == NULL )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			return false;
		}


		// Load the buffer
		Float32 buffer[ 8 ] =
		{
			0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f
		};
		if( m_pVertexBuffer->Load( 32, buffer ) == false )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			delete m_pVertexArray;
			m_pVertexArray = NULL;
		}

		// Add the vertex buffer to the vertex array
		if( m_pVertexArray->AddVertexBuffer( *m_pVertexBuffer, 2, DataType::Float32, 0 ) == false )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			delete m_pVertexArray;
			m_pVertexArray = NULL;
		
			return false;
		}

		// Succeeded
		return true;
	}
		
	Bool Shape::LoadCircle( GraphicDevice * p_pGraphicDevice, const Uint32 p_Parts )
	{
		// You need at least 3 parts
		if( p_Parts < 3 )
		{
			return false;
		}

		// Allocate the pointers
		if( ( m_pVertexBuffer = p_pGraphicDevice->CreateVertexBuffer( ) ) == NULL )
		{
			return false;
		}

		if( ( m_pVertexArray = p_pGraphicDevice->CreateVertexArray( ) ) == NULL )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			return false;
		}


		// Allocate the buffer
		const SizeType bufferSize = (p_Parts * 2 ) + 2;
		Float32 * pBuffer = new Float32[ bufferSize  ];

		// Compute the angle for each part
		Float32 partAngle = 360.0f / p_Parts;

		// Set the first point
		pBuffer[ 0 ] = 0.0f;
		pBuffer[ 1 ] = 1.0f;

		// Add the other points
		for( SizeType i = 1; i <= p_Parts; i++ )
		{
			Vector2f32 point( 0.0f, 1.0f );
			point.Rotate( Degrees( partAngle * i ) );

			pBuffer[ ( i * 2 ) ]		= point.x;
			pBuffer[ ( i * 2 ) + 1 ]	= point.y;
		}

		// Load the vertex buffer
		if( m_pVertexBuffer->Load( bufferSize * sizeof( Float32 ), pBuffer ) == false )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			delete m_pVertexArray;
			m_pVertexArray = NULL;
			
			// Delete the buffer
			delete pBuffer;
		}

		// Delete the buffer
		delete pBuffer;

		// Add the vertex buffer to the vertex array
		if( m_pVertexArray->AddVertexBuffer( *m_pVertexBuffer, 2, DataType::Float32, 0 ) == false )
		{
			delete m_pVertexBuffer;
			m_pVertexBuffer = NULL;
			delete m_pVertexArray;
			m_pVertexArray = NULL;
		
			return false;
		}

		// Succeeded
		return true;
	}

	VertexArray * Shape::GetVertexArray( ) const
	{
		return m_pVertexArray;
	}

	void Shape::SetPosition( const Vector2f32 & p_Position )
	{
		m_Position = p_Position;
	}
		
	void Shape::SetSize( const Vector2f32 & p_Size )
	{
		m_Size = p_Size;
	}

	void Shape::SetRadius( const Float32 p_Radius )
	{
		m_Size.x = p_Radius;
		m_Size.y = p_Radius;
	}

	void Shape::SetRotation( const Angle & p_Angle )
	{
		m_Rotation = p_Angle;
	}
		
	Vector2f32 Shape::GetPosition( ) const
	{
		return m_Position;
	}
		
	Vector2f32 Shape::GetSize( ) const
	{
		return m_Size;
	}

	Angle Shape::GetRotation( ) const
	{
		return m_Rotation;
	}

}