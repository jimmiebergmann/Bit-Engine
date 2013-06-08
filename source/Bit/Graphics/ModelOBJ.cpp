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
#include <Bit/System.hpp>
#include <Bit/System/SmartArray.hpp>
#include <fstream>
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

		// Read the model data
		if( ReadData( p_pFilePath ) != BIT_OK )
		{
			bitTrace( "[ModelOBJ::Load] Can not read the data\n" );
			return BIT_ERROR;
		}

		// Load the graphics: Textures, shaders and vertex objects
		if( LoadGraphics( ) != BIT_OK )
		{
			bitTrace( "[ModelOBJ::Load] Can not load the graphics\n" );
			return BIT_ERROR;
		}

		// We are done
		m_Loaded = BIT_OK;
		return BIT_OK;
	}

	void ModelOBJ::Unload( )
	{
		// Clear the pointerless vectors
		m_MaterialLibraries.clear( );
		m_VertexPositions.clear( );
		m_TexturePositions.clear( );
		m_NormalPositions.clear( );
		m_Textures.clear( );
		
		// Clear the vertex groups
		for( BIT_MEMSIZE i = 0; i < m_VertexGroups.size( ); i++ )
		{
			for( BIT_MEMSIZE j = 0; j < m_VertexGroups[ i ]->Materials.size( ); j++ )
			{
				delete m_VertexGroups[ i ]->Materials[ j ];
			}

			delete m_VertexGroups[ i ];
		}
		m_VertexGroups.clear( );
		
		// Clear the render objects
		for( BIT_MEMSIZE i = 0; i < m_RenderObjects.size( ); i++ )
		{
			// Remove the textures via the resource manager?
			// ...

			// Null the textures
			m_RenderObjects[ i ]->pTextureDiffuse = BIT_NULL;
			m_RenderObjects[ i ]->pTextureBump = BIT_NULL;
			
			// Unload the vertex object
			delete m_RenderObjects[ i ]->pVertexObject;

			// Delete the render object
			delete m_RenderObjects[ i ];
		}
		m_RenderObjects.clear( );
		


		// Reset the count varaibles
		m_TotalTriangleCount = 0;

		// Set the loaded flag to false
		m_Loaded = BIT_FALSE;
	}

	void ModelOBJ::Render( VertexObject::eRenderMode p_Mode )
	{
		// Render all the reder objects
		for( BIT_MEMSIZE i = 0; i < m_RenderObjects.size( ); i++ )
		{
			// Bind the diffuse texture
			if( m_RenderObjects[ i ]->pTextureDiffuse )
			{
				m_RenderObjects[ i ]->pTextureDiffuse->Bind( 0 );
			}

			// Bind the bump texture
			if( m_RenderObjects[ i ]->pTextureBump )
			{
				m_RenderObjects[ i ]->pTextureBump->Bind( 1 );
			}

			// Render the vertex object
			m_RenderObjects[ i ]->pVertexObject->Render( p_Mode );
		}
	}

	// Get functions
	std::string ModelOBJ::GetName( ) const
	{
		return m_Name;
	}

	BIT_UINT32 ModelOBJ::GetVertexGroupCount( ) const
	{
		return m_VertexGroups.size( );
	}

	BIT_UINT32 ModelOBJ::GetTriangleCount( ) const
	{
		return m_TotalTriangleCount;
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

	// Private functions
	BIT_UINT32 ModelOBJ::ReadData( const char * p_pFilePath )
	{
		// Open the main .obj file
		std::ifstream fin( p_pFilePath, std::fstream::binary );

		// Could we open the file?
		if( !fin.is_open( ) )
		{
			return BIT_ERROR_OPEN_FILE;
		}

		// Get the file size
		fin.seekg( 0, std::fstream::end );
		BIT_UINT32 FileSize = fin.tellg( );
		fin.seekg( 0, std::fstream::beg );

		// Allocate and read the data
		SmartArray< BIT_SCHAR8 > Data( FileSize );
		fin.read( (char*)Data.Get( ), FileSize );
		fin.close( );

		// Useful variables
		BIT_UINT32 CurrentPosition = 0;
		BIT_SCHAR8 LineBuffer[ 128 ];
		BIT_UINT32 UnknowDataCount = 0;
		std::vector< std::string > m_UnknownData;
		BIT_SCHAR8 TextBuffer64[ 64 ];
		Vector3_f32 Vec3;
		Vector2_f32 Vec2;
		Triangle Tri;

		// Smoothing group varaibles
		BIT_BOOL UseSmoothing = BIT_FALSE;
		std::vector< Triangle > * pTriangleVectorPtr;

		// Create the first vertex and material groups
		MaterialGroup * pMaterialGroup = new MaterialGroup;
		VertexGroup * pVertexGroup = new VertexGroup;
		pMaterialGroup->pTexture = BIT_NULL;
		pTriangleVectorPtr = &pMaterialGroup->TrianglesFlat;
		pVertexGroup->Materials.push_back( pMaterialGroup );
		m_VertexGroups.push_back( pVertexGroup );

		// Keep on reading until we reach any stream flag
		while( CurrentPosition < FileSize )
		{
			// Get the current line 
			CurrentPosition += GetLine( LineBuffer, 128, &Data[ CurrentPosition] );
			
			// Manage the current line
			// Ignore comments
			switch( LineBuffer[ 0 ] )
			{
				// Vertex data
				case 'v':
				{
					// Switch the second character
					switch( LineBuffer[ 1 ] )
					{
						// Positions coordinates
						case ' ':
						{
							if( sscanf( (const char*)&LineBuffer[ 1 ], "%f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
							{
								m_VertexPositions.push_back( Vec3 );
							}
						}
						break;
						// Texture coordinates
						case 't':
						{
							if( sscanf( (const char*)&LineBuffer[ 2 ], "%f %f", &Vec2.x, &Vec2.y ) == 2 )
							{
								m_TexturePositions.push_back( Vec2 );
							}
						}
						break;
						// Normal coordinates
						case 'n':
						{
							if( sscanf( (const char*)&LineBuffer[ 2 ], "%f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
							{
								m_NormalPositions.push_back( Vec3 );
							}
						}
						break;
						default:
						{
						}
						break;
					}
				}
				break;
				// Face data
				case 'f':
				{
					DecodeOBJFaces( &LineBuffer[ 1 ],  Tri.PositionIndices,
						Tri.TextureIndices, Tri.NormalIndices );

					// Add the triangle
					pTriangleVectorPtr->push_back( Tri );
				}
				break;
				// Group prefix
				case 'g':
				{
					// Is this the first vertex group?
					// Then ignore the line since we initially added the first vertex group.
					if( m_VertexGroups.size( ) == 1 )
					{
						continue;
					}

					// Create a new vertex group
					pVertexGroup = new VertexGroup;
					m_VertexGroups.push_back( pVertexGroup );

					// Add a new material
					pMaterialGroup = new MaterialGroup;
					pMaterialGroup->pTexture = BIT_NULL;
					pVertexGroup->Materials.push_back( pMaterialGroup );

					// Set the first triangle group, depending on the smoothing
					if( UseSmoothing )
					{
						pTriangleVectorPtr = &pMaterialGroup->TrianglesSmooth;
					}
					else
					{
						pTriangleVectorPtr = &pMaterialGroup->TrianglesFlat;
					}
				}
				break;
				// u prefix( usemtl - material name )
				case 'u':
				{
					// Scan for "usemtl"
					if( sscanf( (const char *)LineBuffer, "usemtl %s", TextBuffer64 ) == 1 )
					{
						// Is this the first material group?
						if( pVertexGroup->Materials.size( ) == 1 )
						{
							pVertexGroup->Materials[ 0 ]->MaterialName = (char*)TextBuffer64;
							continue;
						}

						// Create a new material
						pMaterialGroup = new MaterialGroup;
						pMaterialGroup->pTexture = BIT_NULL;
						pMaterialGroup->MaterialName = (char*)TextBuffer64;
						pVertexGroup->Materials.push_back( pMaterialGroup );

						// Set the first triangle group, depending on the smoothing
						if( UseSmoothing )
						{
							pTriangleVectorPtr = &pMaterialGroup->TrianglesSmooth;
						}
						else
						{
							pTriangleVectorPtr = &pMaterialGroup->TrianglesFlat;
						}
					}
				}
				break;
				// Smoothing group prefix
				case 's':
				{
					// Are we turning off smoothing groups?
					// Check for the word [o]ff.
					if( LineBuffer[ 2 ] == 'o' )
					{
						UseSmoothing = BIT_FALSE;
						pTriangleVectorPtr = &pMaterialGroup->TrianglesFlat;
						continue;
					}

					// The following data is probably a number
					BIT_SINT32 SmoothingNumber = atoi( (const char *)&LineBuffer[ 2 ] );

					// Turn smoothing groups off if the smoothing number is 0
					if( SmoothingNumber == 0 )
					{
						UseSmoothing = BIT_FALSE;
						pTriangleVectorPtr = &pMaterialGroup->TrianglesFlat;
						continue;
					}

					// Turn on smoothing groups
					UseSmoothing = BIT_TRUE;
					pTriangleVectorPtr = &pMaterialGroup->TrianglesSmooth;
				}
				break;
				// m prefix( mtllib - material library )
				case 'm':
				{

					continue;
				}
				break;
				// object name prefix
				case 'o':
				{
					if( sscanf( (const char *)&LineBuffer[ 1 ], "%s", TextBuffer64 ) == 1 )
					{
						m_Name = std::string( (char*)TextBuffer64 );
					}
				}
				break;
				// Empty line
				case '\r':
				{
					continue;
				}
				break;
				case '\n':
				{
					continue;
				}
				break;
				// Comment
				case '#':
				{
					continue;
				}
				break;
				// Unknow data
				default:
				{
					m_UnknownData.push_back( (char*)LineBuffer );
					UnknowDataCount++;
				}
				break;

			}
		}

		// Validate the triangle indices to make sure that no indices are out of bound.
		// Also, calculate the total triangle count
		m_TotalTriangleCount = 0;

		for( VertexGroupIterator it_vg = m_VertexGroups.begin( ); it_vg != m_VertexGroups.end( ); it_vg++ )
		{
			for( MaterialGroupIterator it_mg = (*it_vg)->Materials.begin( ); it_mg != (*it_vg)->Materials.end( ); it_mg++ )
			{
				m_TotalTriangleCount += (*it_mg)->TrianglesFlat.size( );
				m_TotalTriangleCount += (*it_mg)->TrianglesSmooth.size( );

				// Go through flat triangles
				for( TriangleIterator it_tr = (*it_mg)->TrianglesFlat.begin( ); it_tr != (*it_mg)->TrianglesFlat.end( ); it_tr++ )
				{
					for( BIT_MEMSIZE i = 0; i < 3; i++ )
					{
						// Position index erorr check
						if( (*it_tr).PositionIndices[ i ] >= m_VertexPositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle position index error %i / %i\n",
								(*it_tr).PositionIndices[ i ], m_VertexPositions.size( ) );
							return BIT_ERROR;
						}

						// Normal index erorr check
						if( (*it_tr).NormalIndices[ i ] >= m_NormalPositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle normal index error %i / %i\n",
								(*it_tr).NormalIndices[ i ], m_NormalPositions.size( ) );
							return BIT_ERROR;
						}
					}

					for( BIT_MEMSIZE i = 0; i < 2; i++ )
					{
						// Texture index erorr check
						if( (*it_tr).TextureIndices[ i ] >= m_TexturePositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle texture index error %i / %i\n",
								(*it_tr).TextureIndices[ i ], m_TexturePositions.size( ) );
							return BIT_ERROR;
						}
					}
					
				}

			}
		}

		// Were any unknown data lines discovered?
		if( m_UnknownData.size( ) )
		{
			bitTrace( "[ModelOBJ::ReadData] Note: Unknow OBJ data:\n" );
		}
		for( BIT_UINT32 i = 0; i < m_UnknownData.size( ); i++ )
		{
			bitTrace( "%i: %s\n", ( i + 1 ), m_UnknownData[ i ].c_str( )  );
		}

		// Everything went ok.
		return BIT_OK;
	}

	BIT_UINT32 ModelOBJ::LoadGraphics( )
	{
		return BIT_OK;
	}

	void ModelOBJ::DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
			BIT_SINT32 * p_pTexture, BIT_SINT32 * p_pNormal )
	{
		// Let's decode by scanning the string
		if( sscanf( (const char*)p_String, "%i/%i/%i %i/%i/%i %i/%i/%i",
			&p_pPosition[ 0 ], &p_pTexture[ 0 ], &p_pNormal[ 0 ],
			&p_pPosition[ 1 ], &p_pTexture[ 1 ], &p_pNormal[ 1 ],
			&p_pPosition[ 2 ], &p_pTexture[ 2 ], &p_pNormal[ 2 ] ) == 9 )
		{
			for( BIT_MEMSIZE i = 0; i < 3; i++ )
			{
				p_pPosition[ i ]--;
				p_pTexture[ i ]--;
				p_pNormal[ i ]--;
			}

			return;
		}

		if( sscanf( (const char*)p_String, "%i//%i %i//%i %i//%i",
			&p_pPosition[ 0 ], &p_pNormal[ 0 ],
			&p_pPosition[ 1 ], &p_pNormal[ 1 ],
			&p_pPosition[ 2 ], &p_pNormal[ 2 ] ) == 6 )
		{
			for( BIT_MEMSIZE i = 0; i < 3; i++ )
			{
				p_pPosition[ i ]--;
				p_pTexture[ i ] = -1;
				p_pNormal[ i ]--;
			}

			return;
		}

		if( sscanf( (const char*)p_String, "%i/%i %i/%i %i/%i",
			&p_pPosition[ 0 ], &p_pTexture[ 0 ],
			&p_pPosition[ 1 ], &p_pTexture[ 1 ],
			&p_pPosition[ 2 ], &p_pTexture[ 2 ] ) == 4 )
		{
			for( BIT_MEMSIZE i = 0; i < 3; i++ )
			{
				p_pPosition[ i ]--;
				p_pTexture[ i ]--;
				p_pNormal[ i ] = -1;
			}

			return;
		}

		if( sscanf( (const char*)p_String, "%i %i %i",
			&p_pPosition[ 0 ], &p_pPosition[ 1 ], &p_pPosition[ 2 ] ) == 3 )
		{
			for( BIT_MEMSIZE i = 0; i < 3; i++ )
			{
				p_pPosition[ i ]--;
				p_pTexture[ i ] = -1;
				p_pNormal[ i ] = -1;
			}
			return;
		}
	}

}