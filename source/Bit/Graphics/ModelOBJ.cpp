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
#include <Bit/System/ResourceManager.hpp>
#include <Bit/System/MatrixManager.hpp>
#include <Bit/System/SmartArray.hpp>
#include <fstream>

#ifdef BIT_PLATFORM_LINUX
    #include <stdio.h>
    #define sscanf_s sscanf
#endif

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

		// We are done
		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 ModelOBJ::Load( const BIT_UINT32 m_VertexElementBits, const Texture::eFilter * p_pTextureFilters,
			const BIT_BOOL p_Mipmapping )
	{
		// Go through every single vertex group and material
		for( VertexGroupIterator it_vg = m_VertexGroups.begin( ); it_vg != m_VertexGroups.end( ); it_vg++ )
		{
			for( MaterialGroupIterator it_mg = (*it_vg)->Materials.begin( ); it_mg != (*it_vg)->Materials.end( ); it_mg++ )
			{
				// Get the material textures
				Texture * pDiffuseTexture = BIT_NULL;
				Texture * pNormalTexture = BIT_NULL;

				// Is there any material applied to this material group?
				if( (*it_mg)->MaterialName.size( ) )
				{
					// Find the material we are looking for
					for( MaterialIterator it_mat = m_Materials.begin( ); it_mat != m_Materials.end( ); it_mat++ )
					{
						if( (*it_mat)->Name == (*it_mg)->MaterialName )
						{
							// We found the material we are looking for.
							// Get the textures

							pDiffuseTexture = ResourceManager::GetTexture( (*it_mat)->DiffuseTexture, p_pTextureFilters, p_Mipmapping );
							pNormalTexture = ResourceManager::GetTexture( (*it_mat)->NormalTexture, p_pTextureFilters, p_Mipmapping );
							break;
						}
					}
				}

				BIT_UINT32 TriangleCount = (*it_mg)->Triangles.size( );

				// Continue if the current triangle vector is empty
				if( TriangleCount == 0 )
				{
					continue;
				}

				// Create and push a new render object to the render object vector
				RenderObject * pRenderObject = new RenderObject;
				pRenderObject->pTextureDiffuse = pDiffuseTexture;
				pRenderObject->pTextureNormal = pNormalTexture;
				m_RenderObjects.push_back( pRenderObject );

				// Create the vertex object
				if( (pRenderObject->pVertexObject = m_GraphicDevice.CreateVertexObject( ) ) == BIT_NULL )
				{
					bitTrace( "[ModelOBJ::LoadVertexObjects] Can not create the vertex object\n" );
					return BIT_ERROR;
				}

				// Use these buffers for the vertex object
				BIT_FLOAT32 * pPositionBuffer = BIT_NULL;
				BIT_FLOAT32 * pTextureBuffer = BIT_NULL;
				BIT_FLOAT32 * pNormalBuffer = BIT_NULL;
				BIT_FLOAT32 * pTangentBuffer = BIT_NULL;
				BIT_FLOAT32 * pBinormalBuffer = BIT_NULL;

				// Calculation flags
				BIT_BOOL CalculatePositions = (BIT_BOOL)( m_VertexElementBits & VertexObject::Vertex_Position );
				BIT_BOOL CalculateTextures = (BIT_BOOL)( m_VertexElementBits & VertexObject::Vertex_Texture );
				BIT_BOOL CalculateNormals = (BIT_BOOL)( m_VertexElementBits & VertexObject::Vertex_Normal );
				BIT_BOOL CalculateTangents = (BIT_BOOL)( m_VertexElementBits & VertexObject::Vertex_Tangent );
				BIT_BOOL CalculateBinormals = (BIT_BOOL)( m_VertexElementBits & VertexObject::Vertex_Binormal );

				// Vertex positions
				if( CalculatePositions && (*it_mg)->ContainsVertexPositions )
				{
					pPositionBuffer = new BIT_FLOAT32[ TriangleCount * 9 ];
					CreateVertexPositions( it_mg, pPositionBuffer, TriangleCount );

					// Add the position buffer
					if( pRenderObject->pVertexObject->AddVertexBuffer( pPositionBuffer, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
					{
						bitTrace( "[ModelOBJ::LoadVertexObjects] Can not add the vertex position buffer\n" );
						return BIT_ERROR;
					}
				}
				// Texture positions
				if( CalculateTextures && (*it_mg)->ContainsTexturePositions )
				{
					pTextureBuffer = new BIT_FLOAT32[ TriangleCount * 6 ];
					CreateVertexTextures( it_mg, pTextureBuffer, TriangleCount );

					// Add the position buffer
					if( pRenderObject->pVertexObject->AddVertexBuffer( pTextureBuffer, 2, BIT_TYPE_FLOAT32 ) != BIT_OK )
					{
						bitTrace( "[ModelOBJ::LoadVertexObjects] Can not add the texture position buffer\n" );
						return BIT_ERROR;
					}
				}
				// Normal positions
				if( CalculateNormals && (*it_mg)->ContainsNormalPositions )
				{
					pNormalBuffer = new BIT_FLOAT32[ TriangleCount * 9 ];
					CreateVertexNormals( it_mg, pNormalBuffer, TriangleCount );

					// Add the position buffer
					if( pRenderObject->pVertexObject->AddVertexBuffer( pNormalBuffer, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
					{
						bitTrace( "[ModelOBJ::LoadVertexObjects] Can not add the normal position buffer\n" );
						return BIT_ERROR;
					}
				}
				// Tangent / binormal positions
				if( ( CalculateTangents || CalculateBinormals ) &&
					CalculatePositions && CalculateTextures )
				{
					// Tangent memory allocation
					if( CalculateTangents )
					{
						pTangentBuffer = new BIT_FLOAT32[ TriangleCount * 9 ];
					}
					// Binormal memory allocation
					if( CalculateBinormals )
					{
						pBinormalBuffer = new BIT_FLOAT32[ TriangleCount * 9 ];
					}

					// Create the tangent/binormal buffers
					VertexObject::GenerateTangents( pPositionBuffer, pTextureBuffer,
						pTangentBuffer, pBinormalBuffer, TriangleCount );
					//CreateVertexTangentsBinormals( it_mg, pTangentBuffer, pBinormalBuffer, TriangleCount );

					// Add the position buffer
					if( CalculateTangents )
					{
						if( pRenderObject->pVertexObject->AddVertexBuffer( pTangentBuffer, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
						{
							bitTrace( "[ModelOBJ::LoadVertexObjects] Can not add the tangent position buffer\n" );
							return BIT_ERROR;
						}
					}
					if( CalculateBinormals )
					{
						if( pRenderObject->pVertexObject->AddVertexBuffer( pBinormalBuffer, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
						{
							bitTrace( "[ModelOBJ::LoadVertexObjects] Can not add the binormals position buffer\n" );
							return BIT_ERROR;
						}
					}


				}

				// Load the vertex object
				if( pRenderObject->pVertexObject->Load( TriangleCount, 3 ) != BIT_OK )
				{
					bitTrace( "[ModelOBJ::LoadVertexObjects] Can not load the vertex buffer\n" );
					return BIT_ERROR;
				}

				// Delete the buffers
				if( pPositionBuffer )
				{
					delete [ ] pPositionBuffer;
				}
				if( pTextureBuffer )
				{
					delete [ ] pTextureBuffer;
				}
				if( pNormalBuffer )
				{
					delete [ ] pNormalBuffer;
				}
				if( pTangentBuffer )
				{
					delete [ ] pTangentBuffer;
				}
				if( pBinormalBuffer )
				{
					delete [ ] pBinormalBuffer;
				}


			}
		}


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

		// Clear the materials
		for( MaterialIterator it = m_Materials.begin( ); it != m_Materials.end( ); it++ )
		{
			delete (*it);
		}
		m_Materials.clear( );

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
			m_RenderObjects[ i ]->pTextureNormal = BIT_NULL;

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

	void ModelOBJ::Render( const VertexObject::eRenderMode p_Mode )
	{
		// Make sure that everything is loaded
		if( !m_Loaded )
		{
			return;
		}

		// Render all the reder objects
		for( BIT_MEMSIZE i = 0; i < m_RenderObjects.size( ); i++ )
		{
			// Bind the diffuse texture
			if( m_RenderObjects[ i ]->pTextureDiffuse )
			{
				m_RenderObjects[ i ]->pTextureDiffuse->Bind( 0 );
			}

			// Bind the bump texture
			if( m_RenderObjects[ i ]->pTextureNormal )
			{
				m_RenderObjects[ i ]->pTextureNormal->Bind( 1 );
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

		// Create the first vertex and material groups
		MaterialGroup * pMaterialGroup = new MaterialGroup;
		VertexGroup * pVertexGroup = new VertexGroup;
		pVertexGroup->Materials.push_back( pMaterialGroup );
		m_VertexGroups.push_back( pVertexGroup );

		// Keep on reading until we reach any stream flag
		while( CurrentPosition < FileSize )
		{
			// Get the current line
			CurrentPosition += GetLine( LineBuffer, 128, &Data[ CurrentPosition ] );

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
							if( sscanf_s( (const char*)&LineBuffer[ 1 ], "%f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
							{
								m_VertexPositions.push_back( Vec3 );
							}
						}
						break;
						// Texture coordinates
						case 't':
						{
							if( sscanf_s( (const char*)&LineBuffer[ 2 ], "%f %f", &Vec2.x, &Vec2.y ) == 2 )
							{
								m_TexturePositions.push_back( Vec2 );
							}
						}
						break;
						// Normal coordinates
						case 'n':
						{
							if( sscanf_s( (const char*)&LineBuffer[ 2 ], "%f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
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
					pMaterialGroup->Triangles.push_back( Tri );
				}
				break;
				// Group prefix
				case 'g':
				{
					// Is this the first vertex group?
					// Then ignore the line since we initially added the first vertex group.
					if( m_VertexGroups.size( ) == 0 )
					{
						m_VertexGroups.push_back( pVertexGroup );
						continue;
					}

					// Create a new vertex group
					pVertexGroup = new VertexGroup;
					m_VertexGroups.push_back( pVertexGroup );

					// Add a new material
					pMaterialGroup = new MaterialGroup;
					pVertexGroup->Materials.push_back( pMaterialGroup );
				}
				break;
				// u prefix( usemtl - material name )
				case 'u':
				{
					// Scan for "usemtl"
					if( sscanf_s( (const char *)LineBuffer, "usemtl %s", TextBuffer64, sizeof( TextBuffer64 ) ) == 1 )
					{
						// Is this the first material group?
						if( pVertexGroup->Materials.size( ) == 1 )
						{
							pVertexGroup->Materials[ 0 ]->MaterialName = (char*)TextBuffer64;
							continue;
						}

						// Create a new material
						pMaterialGroup = new MaterialGroup;
						pMaterialGroup->MaterialName = (char*)TextBuffer64;
						pVertexGroup->Materials.push_back( pMaterialGroup );
					}
				}
				break;
				// Smoothing group prefix
				case 's':
				{
					continue;
				}
				break;
				// m prefix( mtllib - material library )
				case 'm':
				{
					if( sscanf_s( (const char *)LineBuffer, "mtllib %s", TextBuffer64, sizeof( TextBuffer64 ) ) == 1 )
					{
						if( ReadMaterialFile( (const char *)TextBuffer64, p_pFilePath ) != BIT_OK )
						{
							bitTrace( "[ModelOBJ::ReadData] Error occurred while reading the material library file" );
						}
					}
				}
				break;
				// object name prefix
				case 'o':
				{
					if( sscanf_s( (const char *)&LineBuffer[ 1 ], "%s", TextBuffer64, sizeof( TextBuffer64 ) ) == 1 )
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
		if( ValidateTriangles( ) != BIT_OK )
		{
			return BIT_OK;
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

	BIT_UINT32 ModelOBJ::ReadMaterialFile( const char * p_pFilePath, const char * p_pMainFilePath )
	{
		// Open the main .obj file
		std::string DirectoryPath = GetDirectoryPath( p_pMainFilePath ) + "/";
		std::string FullPath = DirectoryPath + std::string( p_pFilePath );
		std::ifstream fin( FullPath.c_str( ), std::fstream::binary );

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
		BIT_UINT32 LinePosition = 0;
		BIT_SCHAR8 LineBuffer[ 128 ];
		BIT_UINT32 UnknowDataCount = 0;
		std::vector< std::string > m_UnknownData;
		BIT_SCHAR8 TextBuffer64[ 64 ];
		BIT_SCHAR8 TextBuffer128[ 128 ];
		Material * pMaterial = BIT_NULL;

		// Keep on reading until we reach any stream flag
		while( CurrentPosition < FileSize )
		{
			// Get the current line
			CurrentPosition += GetLine( LineBuffer, 128, &Data[ CurrentPosition ] );

			// Ignore spaces and tabs, Have been seen in material files
			for( BIT_MEMSIZE i = 0; i < 128; i++ )
			{
				if( LineBuffer[ i ] != ' ' && LineBuffer[ i ] != '\t' )
				{
					LinePosition = i;
					break;
				}
			}

			// Scan for the data
			if( sscanf_s( (const char *)&LineBuffer[ LinePosition ], "newmtl %s", TextBuffer64, sizeof( TextBuffer64 ) ) == 1 )
			{
				// Create a new material and add it to the vector
				pMaterial = new Material;
				pMaterial->Name = std::string( (char *)TextBuffer64 );
				m_Materials.push_back( pMaterial );
				continue;
			}
			// Scan for diffuse texture
			else if( sscanf_s( (const char *)&LineBuffer[ LinePosition ], "map_Kd %s", TextBuffer128, sizeof( TextBuffer128 ) ) == 1 )
			{
				// Continue if we don't have any material yet.
				if( !pMaterial )
				{
					continue;
				}

				// Set the diffuse texture path
				pMaterial->DiffuseTexture = DirectoryPath + std::string( (char *)TextBuffer128 );
			}
			// Scan for bump texture
			else if( sscanf_s( (const char *)&LineBuffer[ LinePosition ], "map_Disp %s", TextBuffer128, sizeof( TextBuffer128 ) ) == 1 )
			{
				// Continue if we don't have any material yet.
				if( !pMaterial )
				{
					continue;
				}

				// Set the diffuse texture path
				pMaterial->NormalTexture = DirectoryPath + std::string( (char *)TextBuffer128 );
			}
		}

		return BIT_OK;
	}

	void ModelOBJ::DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
			BIT_SINT32 * p_pTexture, BIT_SINT32 * p_pNormal )
	{
		// Let's decode the face string by testing multiple scanning combination
		// in order to find the right one.
		// Consider a faster way.
		if( sscanf_s( (const char*)p_String, "%i/%i/%i %i/%i/%i %i/%i/%i",
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

		if( sscanf_s( (const char*)p_String, "%i//%i %i//%i %i//%i",
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

		if( sscanf_s( (const char*)p_String, "%i/%i %i/%i %i/%i",
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

		if( sscanf_s( (const char*)p_String, "%i %i %i",
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

	BIT_UINT32 ModelOBJ::ValidateTriangles(  )
	{
		m_TotalTriangleCount = 0;

		for( VertexGroupIterator it_vg = m_VertexGroups.begin( ); it_vg != m_VertexGroups.end( ); it_vg++ )
		{
			for( MaterialGroupIterator it_mg = (*it_vg)->Materials.begin( ); it_mg != (*it_vg)->Materials.end( ); it_mg++ )
			{
				// Set the vertex flags to true, set them to false if any index < 0
				(*it_mg)->ContainsVertexPositions = BIT_TRUE;
				(*it_mg)->ContainsTexturePositions = BIT_TRUE;
				(*it_mg)->ContainsNormalPositions = BIT_TRUE;

				// Increate the triangle count
				m_TotalTriangleCount += (*it_mg)->Triangles.size( );

				// Go through the triangles
				for( TriangleIterator it_tr = (*it_mg)->Triangles.begin( ); it_tr != (*it_mg)->Triangles.end( ); it_tr++ )
				{
					for( BIT_MEMSIZE i = 0; i < 3; i++ )
					{
						// Position index error check
						if( (*it_tr).PositionIndices[ i ] >= (BIT_SINT32)m_VertexPositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle position index error %i / %i\n",
								(*it_tr).PositionIndices[ i ], m_VertexPositions.size( ) );
							return BIT_ERROR;
						}
						if( (*it_tr).PositionIndices[ i ] < 0 )
						{
							(*it_mg)->ContainsVertexPositions = BIT_FALSE;
						}

						// Normal index error check
						if( (*it_tr).NormalIndices[ i ] >= (BIT_SINT32)m_NormalPositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle normal index error %i / %i\n",
								(*it_tr).NormalIndices[ i ], m_NormalPositions.size( ) );
							return BIT_ERROR;
						}
						if( (*it_tr).NormalIndices[ i ] < 0 )
						{
							(*it_mg)->ContainsNormalPositions = BIT_FALSE;
						}
					}

					for( BIT_MEMSIZE i = 0; i < 2; i++ )
					{
						// Texture index error check
						if( (*it_tr).TextureIndices[ i ] >= (BIT_SINT32)m_TexturePositions.size( ) )
						{
							bitTrace( "[ModelOBJ::ReadData] Triangle texture index error %i / %i\n",
								(*it_tr).TextureIndices[ i ], m_TexturePositions.size( ) );
							return BIT_ERROR;
						}
						if( (*it_tr).TextureIndices[ i ] < 0 )
						{
							(*it_mg)->ContainsTexturePositions = BIT_FALSE;
						}
					}
				}
			}
		}
		// End of loops

		return BIT_OK;
	}

	void ModelOBJ::CreateVertexPositions( const MaterialGroupIterator & p_MatGroupIt, BIT_FLOAT32 * p_pPositions, const BIT_UINT32 p_TriangleCount )
	{
		// Fill the position buffer
		BIT_UINT32 j = 0;
		for( TriangleIterator it_tr = (*p_MatGroupIt)->Triangles.begin( ); it_tr != (*p_MatGroupIt)->Triangles.end( ); it_tr++ )
		{
			// Loop every single vertex in the triangle
			for( BIT_MEMSIZE k = 0; k < 3; k++ )
			{
				BIT_SINT32 Index = (*it_tr).PositionIndices[ k ];

				p_pPositions[ ( j * 9 ) + ( k * 3 ) + 0 ] = m_VertexPositions[ Index ].x;
				p_pPositions[ ( j * 9 ) + ( k * 3 ) + 1 ] = m_VertexPositions[ Index ].y;
				p_pPositions[ ( j * 9 ) + ( k * 3 ) + 2 ] = m_VertexPositions[ Index ].z;
			}
			j++;
		}
	}

	void ModelOBJ::CreateVertexTextures (const MaterialGroupIterator & p_MatGroupIt, BIT_FLOAT32 * p_pTextures, const BIT_UINT32 p_TriangleCount )
	{
		// Fill the texture buffer
		BIT_UINT32 j = 0;
		for( TriangleIterator it_tr = (*p_MatGroupIt)->Triangles.begin( ); it_tr != (*p_MatGroupIt)->Triangles.end( ); it_tr++ )
		{
			// Loop every single vertex in the triangle
			for( BIT_MEMSIZE k = 0; k < 3; k++ )
			{
				BIT_SINT32 Index = (*it_tr).TextureIndices[ k ];

				p_pTextures[ ( j * 6 ) + ( k * 2 ) + 0 ] = m_TexturePositions[ Index ].x;
				p_pTextures[ ( j * 6 ) + ( k * 2 ) + 1 ] = m_TexturePositions[ Index ].y;
			}
			j++;
		}
	}

	void ModelOBJ::CreateVertexNormals( const MaterialGroupIterator & p_MatGroupIt, BIT_FLOAT32 * p_pNormals, const BIT_UINT32 p_TriangleCount )
	{
		// Fill the position buffer
		BIT_UINT32 j = 0;
		for( TriangleIterator it_tr = (*p_MatGroupIt)->Triangles.begin( ); it_tr != (*p_MatGroupIt)->Triangles.end( ); it_tr++ )
		{
			// Loop every single vertex in the triangle
			for( BIT_MEMSIZE k = 0; k < 3; k++ )
			{
				BIT_SINT32 Index = (*it_tr).NormalIndices[ k ];

				p_pNormals[ ( j * 9 ) + ( k * 3 ) + 0 ] = m_NormalPositions[ Index ].x;
				p_pNormals[ ( j * 9 ) + ( k * 3 ) + 1 ] = m_NormalPositions[ Index ].y;
				p_pNormals[ ( j * 9 ) + ( k * 3 ) + 2 ] = m_NormalPositions[ Index ].z;
			}
			j++;
		}

	}



	void ModelOBJ::CreateVertexTangentsBinormals( const MaterialGroupIterator & p_MatGroupIt, BIT_FLOAT32 * p_pTangents,
		BIT_FLOAT32 * p_pBinormals, const BIT_UINT32 p_TriangleCount )
	{
		// Go through all the triangles
		BIT_UINT32 ti = 0;
		for( TriangleIterator it_tr = (*p_MatGroupIt)->Triangles.begin( ); it_tr != (*p_MatGroupIt)->Triangles.end( ); it_tr++ )
		{

			for( BIT_UINT32 vt = 0; vt < 3; vt++ )
			{
				Vector3_ui32 Indices( vt, ( vt + 1 ) % 3, ( vt + 2 ) % 3 );

				const Vector3_f32 Vertex1 = m_VertexPositions[ (*it_tr).PositionIndices[ Indices.x ] ];
				const Vector3_f32 Vertex2 = m_VertexPositions[ (*it_tr).PositionIndices[ Indices.y ] ];
				const Vector3_f32 Vertex3 = m_VertexPositions[ (*it_tr).PositionIndices[ Indices.z ] ];

				const Vector2_f32 Texture1 = m_TexturePositions[ (*it_tr).TextureIndices[ Indices.x ] ];
				const Vector2_f32 Texture2 = m_TexturePositions[ (*it_tr).TextureIndices[ Indices.y ] ];
				const Vector2_f32 Texture3 = m_TexturePositions[ (*it_tr).TextureIndices[ Indices.z ] ];

				float x1 = Vertex2.x - Vertex1.x;
				float x2 = Vertex3.x - Vertex1.x;
				float y1 = Vertex2.y - Vertex1.y;
				float y2 = Vertex3.y - Vertex1.y;
				float z1 = Vertex2.z - Vertex1.z;
				float z2 = Vertex3.z - Vertex1.z;

				float s1 = Texture2.x - Texture1.x;
				float s2 = Texture3.x - Texture1.x;
				float t1 = Texture2.y - Texture1.y;
				float t2 = Texture3.y - Texture1.y;

				float r = 1.0f / ( s1 * t2 - s2 * t1 );

				// Tangent
				if( p_pTangents )
				{
					Vector3_f32 Tangent( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
						(t2 * z1 - t1 * z2) * r );
					Tangent.Normalize( );

					p_pTangents[ ( ti * 9 ) + ( vt * 3 ) + 0 ] = Tangent.x;
					p_pTangents[ ( ti * 9 ) + ( vt * 3 ) + 1 ] = Tangent.y;
					p_pTangents[ ( ti * 9 ) + ( vt * 3 ) + 2 ] = Tangent.z;
				}

				// Binormal
				if( p_pTangents )
				{
					Vector3_f32 Binormal( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
						(s1 * z2 - s2 * z1) * r );
					Binormal.Normalize( );

					p_pBinormals[ ( ti * 9 ) + ( vt * 3 ) + 0 ] = Binormal.x;
					p_pBinormals[ ( ti * 9 ) + ( vt * 3 ) + 1 ] = Binormal.y;
					p_pBinormals[ ( ti * 9 ) + ( vt * 3 ) + 2 ] = Binormal.z;
				}
			}

			// Increment the triangle index
			ti++;
		}


	}




}
