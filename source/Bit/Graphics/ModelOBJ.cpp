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

#include <Bit/Graphics/ModelOBJ.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/Destructor
	ModelOBJ::ModelOBJ( const GraphicDevice & p_GraphicDevice ) :
		m_TotalTriangleCount( 0 ),
		m_GraphicDevice( p_GraphicDevice )
	{
		m_Loaded = BIT_FALSE;
	}

	ModelOBJ::~ModelOBJ( )
	{
		Unload( );
	}

	// Public functions
	BIT_UINT32 ModelOBJ::ReadFile( const char * p_pFilePath )
	{
		// Make sure that the model is not already loaded
		if( m_Loaded )
		{
			bitTrace( "[ModelOBJ::Load] Already loaded\n" );
			return BIT_ERROR;
		}


		// We are done
		m_Loaded = BIT_OK;
		return BIT_OK;
	}

	void ModelOBJ::Unload( )
	{
		// Clear the pointerless vectors
		m_VertexPositions.clear( );
		m_TexturePositions.clear( );
		m_NormalPositions.clear( );
		m_Textures.clear( );
		
		// Clear the texture groups
		for( BIT_MEMSIZE i = 0; i < m_VertexGroups.size( ); i++ )
		{
			delete m_VertexGroups[ i ];
		}
		m_VertexGroups.clear( );
		
		// Reset the count varaibles
		m_TotalTriangleCount = 0;
		m_TotalTriangleIndexCount = 0;

		// Set the loaded flag to false
		m_Loaded = BIT_FALSE;
	}

	void ModelOBJ::Render( )
	{

	}


	// Get functions
	BIT_UINT32 ModelOBJ::GetTriangleCount( ) const
	{
		return m_TotalTriangleCount;
	}

	BIT_UINT32 ModelOBJ::GetTriangleIndexCount( ) const
	{
		return m_TotalTriangleIndexCount;
	}

	BIT_UINT32 ModelOBJ::GetPositionCoordinateCount( ) const
	{
		return m_VertexPositions.size( );
	}

	BIT_UINT32 ModelOBJ::GetTextureCoordinateCount( ) const
	{
		return m_TexturePositions.size( );
	}

	BIT_UINT32 ModelOBJ::GetNormalCoordinateCount( ) const
	{
		return m_NormalPositions.size( );
	}

	BIT_UINT32 ModelOBJ::GetTextureCount( ) const
	{
		return m_Textures.size( );
	}

	BIT_UINT32 ModelOBJ::GetAnimationCount( ) const
	{
		return 0;
	}

	BIT_BOOL ModelOBJ::ContainsRagdoll( ) const
	{
		return BIT_FALSE;
	}


}