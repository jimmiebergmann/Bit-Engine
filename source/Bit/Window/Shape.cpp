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

	Shape::Shape( GraphicDevice * p_pGraphicDevice, const Bool p_OrigoInCenter ) :
		m_Position( 0.0f, 0.0f ),
		m_Size( 1.0f, 1.0f ),
		m_pVertexArray( NULL ),
		m_pVertexBuffer( NULL )
	{
		// Allocate the pointers
		m_pVertexBuffer = p_pGraphicDevice->CreateVertexBuffer( );
		m_pVertexArray = p_pGraphicDevice->CreateVertexArray( );

		if( p_OrigoInCenter )
		{
			// Load the buffer
			Float32 buffer[ 12 ] =
			{
				-0.5f, -0.5f,		0.5f, -0.5f,	0.5f, 0.5f,
				-0.5f, -0.5f,		0.5f, 0.5f,		-0.5f, 0.5f
			};
			m_pVertexBuffer->Load( 48, buffer );
		}
		else
		{
			// Load the buffer
			Float32 buffer[ 12 ] =
			{
				0.0f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f,
				0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f
			};
			m_pVertexBuffer->Load( 48, buffer );
		}

		// Add the vertex buffer to the vertex array
		m_pVertexArray->AddVertexBuffer( *m_pVertexBuffer, 2, DataType::Float32, 0 );
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
		
	Vector2f32 Shape::GetPosition( ) const
	{
		return m_Position;
	}
		
	Vector2f32 Shape::GetSize( ) const
	{
		return m_Size;
	}

}