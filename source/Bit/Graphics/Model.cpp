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

#include <Bit/Graphics/Model.hpp>
#include <memory>
#include <fstream>
#include <string>
#include <stdio.h>
#include <Bit/System.hpp>
#include <Bit/System/SmartArray.hpp>
#include <Bit/System/Randomizer.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
	Model::Model( ) :
		m_Loaded( BIT_FALSE )
	{

	}

	Model::~Model( )
	{
		DeallocateData( );
	}

	// Public functions
	BIT_UINT32 Model::ReadFile( const char * p_pFilePath )
	{
		// Get the file's extension
		std::string FileExtension = GetFileExtension( p_pFilePath );

		if( FileExtension == "OBJ" )
		{
			BIT_UINT32 Status = ReadOBJ( p_pFilePath );
			m_Loaded = ( Status == BIT_OK );
			return Status;
		}

		bitTrace( BIT_NULL, "[Bit::Model::ReadFile] <ERROR> "
			"Unknow extension: %s.\n", FileExtension );

		return BIT_ERROR;
	}

	BIT_UINT32 Model::ReadOBJ( const char * p_pFilePath )
	{
		// Make sure the model is not already loaded.
		if( m_Loaded )
		{
			bitTrace( "[Model::ReadOBJ] Already loaded.\n" );
			return BIT_ERROR;
		}

		// Open the main .obj file
		std::ifstream fin( p_pFilePath );
		
		// Could we open the file?
		if( !fin.is_open( ) )
		{
			return BIT_ERROR_OPEN_FILE;
		}

		// Usefil pre-declared variales for the file reading
		std::string Prefix = "";
		std::string String = "";
		Vector3_f32 Vec3;
		Vector2_f32 Vec2;
		Triangle Tri;
		BIT_UINT32 UnknowPrefixCount = 0;
		BIT_SCHAR8 LineBuffer[ 64 ];

		// Keep on reading until we reach any stream flag
		while( fin.good( ) )
		{
			fin.getline( (char*)LineBuffer, 63 );

			if( LineBuffer[ 0 ] == 'v' )
			{
				if( LineBuffer[ 1 ] == 't' )
				{
					if( sscanf( (const char*)LineBuffer, "vt %f %f", &Vec2.x, &Vec2.y ) == 2 )
					{
						m_VertexTextures.push_back( Vec2 );
					}
				}
				else if( LineBuffer[ 1 ] == 'n' )
				{
					if( sscanf( (const char*)LineBuffer, "vn %f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
					{
						m_VertexNormals.push_back( Vec3.Normalize( ) );
					}
				}
				/*
				// IGNORE THIS
				else if( LineBuffer[ 1 ] == 'p' )
				{
					// ...
				}
				*/
				else
				{
					if( sscanf( (const char*)LineBuffer, "v %f %f %f", &Vec3.x, &Vec3.y, &Vec3.z ) == 3 )
					{
						m_VertexPositions.push_back( Vec3 );
					}
				}
			}
			else if( LineBuffer[ 0 ] == 'f' )
			{
				for( BIT_MEMSIZE i = 0; i < 3; i++ )
				{
					LineBuffer[ 63 ] = 0;
					DecodeOBJFaces( &LineBuffer[ 2 ],  Tri.PositionIndex,
						Tri.TextureIndex, Tri.NormalIndex );
				}
			
				// Add the triangle
				m_Triangles.push_back( Tri );
			}
			else if( LineBuffer[ 0 ] == '#' )
			{
				continue;
			}
			else
			{
				UnknowPrefixCount++;
			}
		}

		// Close the main file
		fin.close( );

		// Validate the triangles by making sure that no indices are out of bound
		for( BIT_MEMSIZE i = 0; i < m_Triangles.size( ); i++ )
		{
			for( BIT_MEMSIZE j = 0; j < 3; j++ )
			{
				if( m_Triangles[ i ].PositionIndex[ j ] > m_VertexPositions.size( ) ||
					m_Triangles[ i ].PositionIndex[ j ] == -1 )
				{
					bitTrace("[Model::ReadOBJ] Invalid position index at: %i %i  index: %i\n",
						i, j, m_Triangles[ i ].PositionIndex[ j ] );
					return BIT_ERROR;
				}

				if( m_Triangles[ i ].TextureIndex[ j ] > m_VertexTextures.size( )  &&
					m_Triangles[ i ].TextureIndex[ j ] != -1  )
				{
					bitTrace("[Model::ReadOBJ] Invalid texture index at: %i %i  index: %i\n",
						i, j, m_Triangles[ i ].TextureIndex[ j ] );
					return BIT_ERROR;
				}

				if( m_Triangles[ i ].NormalIndex[ j ] > m_VertexNormals.size( ) &&
					m_Triangles[ i ].NormalIndex[ j ] != -1  )
				{
					bitTrace("[Model::ReadOBJ] Invalid normal index at: %i %i  index: %i\n",
						i, j, m_Triangles[ i ].NormalIndex[ j ] );
					return BIT_ERROR;
				}
			}

		}

		// Are there any unknow prefixes?
		if( UnknowPrefixCount )
		{
			bitTrace("[Model::ReadOBJ] Unknown prefix count: %i\n", UnknowPrefixCount);
		}


		// Open the .mtl file if any
		// ...

		return BIT_OK;
	}

	void Model::DeallocateData( )
	{

		m_Loaded = BIT_FALSE;
	}


	BIT_UINT32 Model::LoadVertexObject( VertexObject & p_VertexObject )
	{
		// Error check
		if( !m_Loaded )
		{
			bitTrace("[Model::LoadVertexObject] Not loaded yet\n");
			return BIT_ERROR;
		}
		if( p_VertexObject.IsLoaded( ) )
		{
			bitTrace("[Model::LoadVertexObject] The vertex object is already loaded\n");
			return BIT_ERROR;
		}

		if( !m_VertexPositions.size( ) )
		{
			bitTrace("[Model::LoadVertexObject] No vertices\n");
			return BIT_ERROR;
		}

		// Calculate and set the buffers
		SmartArray<BIT_FLOAT32> pVertexPositions( m_Triangles.size( ) * 3 * 3 );
		SmartArray<BIT_FLOAT32> pVertexTextures( m_Triangles.size( ) * 3 * 2 );
		SmartArray<BIT_FLOAT32> pVertexNormals( m_Triangles.size( ) * 3 * 3 );

		// Add vertex positsion buffer if any
		if( m_VertexPositions.size( ) )
		{
			// Lopp through all the triangles
			for( BIT_MEMSIZE i = 0; i < m_Triangles.size( ); i++ )
			{
				// Loop every single vertex in the triangle
				for( BIT_MEMSIZE j = 0; j < 3; j++ )
				{
					BIT_SINT32 Index = m_Triangles[ i ].PositionIndex[ j ];
				
					pVertexPositions[ ( i * 9 ) + ( j * 3 ) + 0 ] = m_VertexPositions[ Index ].x;
					pVertexPositions[ ( i * 9 ) + ( j * 3 ) + 1 ] = m_VertexPositions[ Index ].y;
					pVertexPositions[ ( i * 9 ) + ( j * 3 ) + 2 ] = m_VertexPositions[ Index ].z;
				}
			}

			// Add the vertex buffer
			if( p_VertexObject.AddVertexBuffer( pVertexPositions, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
			{
				bitTrace("[Model::LoadVertexObject] Can not add the vertex buffer\n");
				return BIT_ERROR;
			}
		}

		// Add vertex textures buffer if any
		if( m_VertexTextures.size( ) )
		{
			// Lopp through all the triangles
			for( BIT_MEMSIZE i = 0; i < m_Triangles.size( ); i++ )
			{
				// Loop every single vertex in the triangle
				for( BIT_MEMSIZE j = 0; j < 3; j++ )
				{
					BIT_SINT32 Index = m_Triangles[ i ].TextureIndex[ j ];
				
					pVertexTextures[ ( i * 6 ) + ( j * 2 ) + 0 ] = m_VertexPositions[ Index ].x;
					pVertexTextures[ ( i * 6 ) + ( j * 2 ) + 1 ] = m_VertexPositions[ Index ].y;
				}
			}

			// Add the vertex buffer
			if( p_VertexObject.AddVertexBuffer( pVertexTextures, 2, BIT_TYPE_FLOAT32 ) != BIT_OK )
			{
				bitTrace("[Model::LoadVertexObject] Can not add the vertex buffer\n");
				return BIT_ERROR;
			}
		}

		// Add vertex normal buffer if any
		if( m_VertexNormals.size( ) )
		{
			// Lopp through all the triangles
			for( BIT_MEMSIZE i = 0; i < m_Triangles.size( ); i++ )
			{
				// Loop every single vertex in the triangle
				for( BIT_MEMSIZE j = 0; j < 3; j++ )
				{
					BIT_SINT32 Index = m_Triangles[ i ].NormalIndex[ j ];
				
					pVertexNormals[ ( i * 9 ) + ( j * 3 ) + 0 ] = m_VertexNormals[ Index ].x;
					pVertexNormals[ ( i * 9 ) + ( j * 3 ) + 1 ] = m_VertexNormals[ Index ].y;
					pVertexNormals[ ( i * 9 ) + ( j * 3 ) + 2 ] = m_VertexNormals[ Index ].z;
				}
			}

			// Add the vertex buffer
			if( p_VertexObject.AddVertexBuffer( pVertexNormals, 3, BIT_TYPE_FLOAT32 ) != BIT_OK )
			{
				bitTrace("[Model::LoadVertexObject] Can not add the vertex buffer\n");
				return BIT_ERROR;
			}
		}

		// Load the vertex object
		if( p_VertexObject.Load( m_Triangles.size( ), 3 ) != BIT_OK )
		{
			bitTrace("[Model::LoadVertexObject] Can not load the vertex object\n");
			return BIT_ERROR;
		}

		return BIT_OK;

	}

	// Set functions
	// ...

	// Get functions
	BIT_UINT32 Model::GetVertexPositionCount( ) const
	{
		return m_VertexPositions.size( );
	}

	BIT_UINT32 Model::GetVertexTextureCount( ) const
	{
		return m_VertexTextures.size( );
	}

	BIT_UINT32 Model::GetVertexNormalCount( ) const
	{
		return m_VertexNormals.size( );
	}

	BIT_UINT32 Model::GetTriangleCount( ) const
	{
		return m_Triangles.size( );
	}

	
	BIT_UINT32 Model::GetVertexPosition( BIT_UINT32 p_Index, Vector3_f32 p_Coord ) const
	{
		if( p_Index >= m_VertexPositions.size( ) )
		{
			bitTrace( "[Model::GetVertexPosition] Invalid index.\n" );
			return BIT_ERROR;
		}

		p_Coord = m_VertexPositions[ p_Index ];
		return BIT_OK; 
	}

	BIT_UINT32 Model::GetVertexTexture( BIT_UINT32 p_Index, Vector2_f32 p_Coord ) const
	{
		if( p_Index >= m_VertexTextures.size( ) )
		{
			bitTrace( "[Model::GetVertexTexture] Invalid index.\n" );
			return BIT_ERROR;
		}

		p_Coord = m_VertexTextures[ p_Index ];
		return BIT_OK; 
	}

	BIT_UINT32 Model::GetVertexNormal( BIT_UINT32 p_Index, Vector3_f32 p_Coord ) const
	{
		if( p_Index >= m_VertexNormals.size( ) )
		{
			bitTrace( "[Model::GetVertexNormal] Invalid index.\n" );
			return BIT_ERROR;
		}

		p_Coord = m_VertexNormals[ p_Index ];
		return BIT_OK; 
	}


	BIT_UINT32 Model::GetTriangle( BIT_UINT32 p_Index, Triangle p_Triangle ) const
	{
		if( p_Index >= m_Triangles.size( ) )
		{
			bitTrace( "[Model::GetTriangle] Invalid index.\n" );
			return BIT_ERROR;
		}

		p_Triangle = m_Triangles[ p_Index ];
		return BIT_OK;
	}


	// Private functions
	void Model::DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
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