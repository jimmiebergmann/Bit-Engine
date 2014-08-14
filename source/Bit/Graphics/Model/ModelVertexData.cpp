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

#include <Bit/Graphics/Model/ModelVertexData.hpp>
#include <Bit/Graphics/VertexBuffer.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	ModelVertexData::ModelVertexData( ) :
		m_pVertexArray( NULL ),
		m_Bitmask( 0 )
	{
	}

	ModelVertexData::~ModelVertexData( )
	{
		// Delete all the vertex buffers
		for( VertexBufferVector::iterator it = m_VertexBuffers.begin( ); it != m_VertexBuffers.end( ); it++ )
		{
			delete (*it);
		}

		if( m_pVertexArray )
		{
			delete m_pVertexArray;
		}
	}

	Bool ModelVertexData::AddVertexBuffer( VertexBuffer * p_pVertexBuffer, const Uint16 p_Bitmask )
	{
		// Error check the parameter.
		if( p_pVertexBuffer == NULL || p_pVertexBuffer->IsLoaded( ) == false )
		{
			return false;
		}

		// Add the vertex buffer to the vector.
		m_VertexBuffers.push_back( p_pVertexBuffer );

		// Apply the bitmask value
		m_Bitmask |= p_Bitmask;

		// Succeeded
		return true;
	}

	Bool ModelVertexData::SetVertexArray( VertexArray * p_pVertexArray )
	{
		// Error check the parameter.
		if( p_pVertexArray == NULL )
		{
			return false;
		}

		// Delete the old vertex array if any.
		if( m_pVertexArray )
		{
			delete m_pVertexArray;
		}

		// Set the new vertex array.
		m_pVertexArray = p_pVertexArray;

		// Succeeded
		return true;
	}

	void ModelVertexData::SetBitmask( const Uint16 p_Bitmask )
	{
		m_Bitmask = p_Bitmask;
	}

	SizeType ModelVertexData::GetVertexBufferCount( ) const
	{
		return static_cast<SizeType>( m_VertexBuffers.size( ) );
	}	

	const VertexBuffer * ModelVertexData::GetVertexBuffer( const SizeType & p_Index ) const
	{
		return m_VertexBuffers[ p_Index ];
	}

	const VertexArray * ModelVertexData::GetVertexArray(  ) const
	{
		return m_pVertexArray;
	}

	Uint16 ModelVertexData::GetBitmask(  ) const
	{
		return m_Bitmask;
	}



	ModelVertexGroup::ModelVertexGroup( )
	{
	}

	ModelVertexGroup::~ModelVertexGroup( )
	{
		for( VertexDataVector::iterator it = m_VertexData.begin( ) ; it != m_VertexData.end( ); it++ )
		{
			delete (*it);
		}
	}

	ModelVertexData * ModelVertexGroup::AddVertexData( )
	{
		// Create the vertex data
		ModelVertexData * pVertexData = new ModelVertexData;

		// Add the vertex data to the vecor
		m_VertexData.push_back( pVertexData );

		// Return the pointer of the vertex data.
		return pVertexData;
	}

	void ModelVertexGroup::RemoveVertexData( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_VertexData.size( ) ) )
		{
			return;
		}

		// Remove the data at the given index.
		m_VertexData.erase( m_VertexData.begin( ) + p_Index );
	}

	ModelVertexData * ModelVertexGroup::GetVertexData( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_VertexData.size( ) ) )
		{
			return NULL;
		}

		// Return the data at the given index.
		return m_VertexData[ p_Index ];
	}

	SizeType ModelVertexGroup::GetVertexDataCount( ) const
	{
		return static_cast<SizeType>( m_VertexData.size( ) );
	}

}