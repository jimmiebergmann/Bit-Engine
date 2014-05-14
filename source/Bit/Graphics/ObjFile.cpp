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

#include <Bit/Graphics/ObjFile.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Face class
	ObjFile::Face::Face( )
	{
	}

	const ObjFile::FaceCorner & ObjFile::Face::GetFaceCorner( const SizeType p_Index ) const
	{
		return m_FaceCorners[ p_Index ];
	}

	SizeType ObjFile::Face::GetFaceCornerCount( ) const
	{
		return static_cast<SizeType>( m_FaceCorners.size( ) );
	}


	// Material group class
	ObjFile::MaterialGroup::MaterialGroup( )
	{
	}

	ObjFile::MaterialGroup::~MaterialGroup( )
	{
		for( FaceVector::size_type i = 0; i < m_FlatFaces.size( ); i++ )
		{
			delete m_FlatFaces[ i ];
		}
		for( FaceVector::size_type i = 0; i < m_SmoothFaces.size( ); i++ )
		{
			delete m_SmoothFaces[ i ];
		}
	}

	const std::string & ObjFile::MaterialGroup::GetMaterialName( ) const
	{
		return m_MaterialName;
	}

	const ObjFile::Face & ObjFile::MaterialGroup::GetFlatFace( const SizeType p_Index ) const
	{
		return *m_FlatFaces[ p_Index ];
	}

	const ObjFile::Face & ObjFile::MaterialGroup::GetSmoothFace( const SizeType p_Index ) const
	{
		return *m_SmoothFaces[ p_Index ];
	}

	SizeType ObjFile::MaterialGroup::GetFlatFaceCount( ) const
	{
		return static_cast<SizeType>( m_FlatFaces.size( ) );
	}

	SizeType ObjFile::MaterialGroup::GetSmoothFaceCount( ) const
	{
		return static_cast<SizeType>( m_SmoothFaces.size( ) );
	}


	// Object group class
	ObjFile::ObjectGroup::ObjectGroup( )
	{
	}

	ObjFile::ObjectGroup::~ObjectGroup( )
	{
		for( MaterialGroupVector::size_type i = 0; i < m_MaterialGroups.size( ); i++ )
		{
			delete m_MaterialGroups[ i ];
		}
	}

	const std::string & ObjFile::ObjectGroup::GetName( ) const
	{
		return m_Name;
	}

	const ObjFile::MaterialGroup & ObjFile::ObjectGroup::GetMaterialGroup( const SizeType p_Index ) const
	{
		return *m_MaterialGroups[ p_Index ];
	}

	SizeType ObjFile::ObjectGroup::GetMaterialGroupCount( ) const
	{
		return static_cast<SizeType>( m_MaterialGroups.size( ) );
	}


	// Object class
	ObjFile::Object::Object( )
	{
	}

	ObjFile::Object::~Object( )
	{
		for( ObjectGroupVector::size_type i = 0; i < m_ObjectGroups.size( ); i++ )
		{
			delete m_ObjectGroups[ i ];
		}
	}

	const std::string & ObjFile::Object::GetName( ) const
	{
		return m_Name;
	}

	const Vector3f32 & ObjFile::Object::GetVertex( const SizeType p_Index ) const
	{
		return m_Vertices[ p_Index ];
	}

	const Vector3f32 & ObjFile::Object::GetTextureCoord( const SizeType p_Index ) const
	{
		return m_TextureCoords[ p_Index ];
	}

	const Vector3f32 & ObjFile::Object::GetNormal( const SizeType p_Index ) const
	{
		return m_Normals[ p_Index ];
	}

	const ObjFile::ObjectGroup & ObjFile::Object::GetObjectGroup( const SizeType p_Index ) const
	{
		return *m_ObjectGroups[ p_Index ];
	}

	SizeType ObjFile::Object::GetVertexCount( ) const
	{
		return static_cast<SizeType>( m_Vertices.size( ) );
	}

	SizeType ObjFile::Object::GetTextureCoordCount( ) const
	{
		return static_cast<SizeType>( m_TextureCoords.size( ) );
	}

	SizeType ObjFile::Object::GetNormalCount( ) const
	{
		return static_cast<SizeType>( m_Normals.size( ) );
	}

	SizeType ObjFile::Object::GetObjectGroupCount( ) const
	{
		return static_cast<SizeType>( m_ObjectGroups.size( ) );
	}


	// OBJ file class
	ObjFile::ObjFile( ) :
		m_MaterialFilename( "" )
	{
	}

	ObjFile::~ObjFile( )
	{
		for( ObjectVector::size_type i = 0; i < m_Objects.size( ); i++ )
		{
			delete m_Objects[ i ];
		}
	}

	Bool ObjFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.rdbuf( )->pubsetbuf( const_cast<char*>( p_Memory.c_str( ) ), p_Memory.size( ) );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool ObjFile::LoadFromStream( std::istream & p_Stream )
	{
		const SizeType maxLineLength = 128;
		char line[ maxLineLength ];
		char character;
		std::string pre;
		pre.reserve( 32 );
		std::string faceGroup;
		faceGroup.reserve( 32 );

		// Variables for checking if we've found different groups
		Bool foundObject = false;
		Bool foundObjectGroup = false;
		Bool foundMaterialGroup = false;

		// Create a new object
		// pCurrentObject is always pointing to the current object.
		Object * pCurrentObject = new Object;
		ObjectGroup * pCurrentObjectGroup = new ObjectGroup;
		MaterialGroup * pCurrentMaterialGroup = new MaterialGroup;
		MaterialGroup::FaceVector * pCurrentSmoothGroup = &pCurrentMaterialGroup->m_SmoothFaces;
		pCurrentObjectGroup->m_MaterialGroups.push_back( pCurrentMaterialGroup );
		pCurrentObject->m_ObjectGroups.push_back( pCurrentObjectGroup );
		m_Objects.push_back( pCurrentObject );

		// Keep on looping until we reach the end of the file.
		while( p_Stream.eof( ) == false )
		{
			// Get the first word on the current line.
			p_Stream >> pre;

			// Error check the stream.
			if( p_Stream.eof( ) )
			{
				break;
			}

			// Get a single character, in order to get rid of the space( NOT necessary? )
			p_Stream.get( character );

			// Get the current line
			p_Stream.getline( line, maxLineLength );

			// Parse the line, check the first character
			switch( pre[ 0 ] )
			{
				// mtllib
				case 'm':
				{
					if( pre == "mtllib" )
					{
						m_MaterialFilename = line;
					}
				}
				break;
				// Object
				case 'o':
				{
					// Make sure that the rest of the line is fine.
					if( line[ 0 ] == 0 )
					{
						// Ignore the line.
						break;
					}

					// Create a new object
					if( foundObject )
					{
						pCurrentObject = new Object;
						pCurrentObjectGroup = new ObjectGroup;
						pCurrentMaterialGroup = new MaterialGroup;
						pCurrentSmoothGroup = &pCurrentMaterialGroup->m_SmoothFaces;
						pCurrentObjectGroup->m_MaterialGroups.push_back( pCurrentMaterialGroup );
						pCurrentObject->m_ObjectGroups.push_back( pCurrentObjectGroup );
						m_Objects.push_back( pCurrentObject );		
					}

					// Set the name of the object
					pCurrentObject->m_Name = line;

					// Set the found object flag to true.
					// Reset some other flags as well.
					foundObject = true;
					foundObjectGroup = false;
					foundMaterialGroup = false;
				}
				break;
				// Group
				case 'g':
				{
					// Create a new object group
					if( foundObjectGroup )
					{
						pCurrentObjectGroup = new ObjectGroup;
						pCurrentMaterialGroup = new MaterialGroup;
						pCurrentSmoothGroup = &pCurrentMaterialGroup->m_SmoothFaces;
						pCurrentObjectGroup->m_MaterialGroups.push_back( pCurrentMaterialGroup );
						pCurrentObject->m_ObjectGroups.push_back( pCurrentObjectGroup );
					}

					// Set the name of the object group
					pCurrentObjectGroup->m_Name = line;

					// Set the found object group flag to true.
					// Reset some other flags as well.
					foundObjectGroup = true;
					foundMaterialGroup = false;
				}
				break;
				// usemtl
				case 'u':
				{
					// Create a new material
					if( foundMaterialGroup )
					{
						pCurrentMaterialGroup = new MaterialGroup;
						pCurrentSmoothGroup = &pCurrentMaterialGroup->m_SmoothFaces;
						pCurrentObjectGroup->m_MaterialGroups.push_back( pCurrentMaterialGroup );
					}

					// Set the name of the object group
					pCurrentMaterialGroup->m_MaterialName = line;

					// Set the found object group flag to true.
					// Reset some other flags as well.
					foundMaterialGroup = true;
				}
				break;
				// Smoothing group
				case 's':
				{
					// Set the smooth group pointer
					if( strcmp( line, "0" ) == 0 || strcmp( line, "off" ) == 0 )
					{
						pCurrentSmoothGroup = &pCurrentMaterialGroup->m_FlatFaces;
					}
					else
					{
						pCurrentSmoothGroup = &pCurrentMaterialGroup->m_SmoothFaces;
					}
				}
				break;
				// Vertex position/texture coord/normal
				case 'v':
				{
					if( pre == "v" )
					{
						std::stringstream parser( line );
						Vector3f32 coord;
						parser >> coord.x;
						parser >> coord.y;
						parser >> coord.z;
						pCurrentObject->m_Vertices.push_back( coord );
					}
					else if( pre == "vt")
					{
						std::stringstream parser( line );
						Vector3f32 coord;
						parser >> coord.x;
						parser >> coord.y;
						parser >> coord.z;
						pCurrentObject->m_TextureCoords.push_back( coord );
					}
					else if( pre == "vn" )
					{
						std::stringstream parser( line );
						Vector3f32 coord;
						parser >> coord.x;
						parser >> coord.y;
						parser >> coord.z;
						pCurrentObject->m_Normals.push_back( coord );
					}
				}
				break;
				// face normal
				case 'f':
				{
					// Prepare the face parser
					std::stringstream parser( line );
					Face * pFace = new Face;
					pCurrentSmoothGroup->push_back( pFace );

					// Read every single index group in the current face.
					while( parser.eof( ) == false )
					{
						parser >> faceGroup;
						FaceCorner faceCorner;

						// Parse the face group
						if( sscanf( faceGroup.c_str( ), "%i/%i/%i",
							&faceCorner.VertexIndex,
							&faceCorner.TextureCoordIndex,
							&faceCorner.NormalIndex ) == 3 )
						{
							pFace->m_FaceCorners.push_back( faceCorner );
						}
						else if( sscanf( faceGroup.c_str( ), "%i//%i",
							&faceCorner.VertexIndex,
							&faceCorner.NormalIndex ) == 2 )
						{
							faceCorner.TextureCoordIndex = -1;
							pFace->m_FaceCorners.push_back( faceCorner );
						}
						else if( sscanf( faceGroup.c_str( ), "%i/%i",
							&faceCorner.VertexIndex,
							&faceCorner.TextureCoordIndex ) == 2 )
						{
							faceCorner.NormalIndex = -1;
							pFace->m_FaceCorners.push_back( faceCorner );
						}
						else if( sscanf( faceGroup.c_str( ), "%i",
							&faceCorner.VertexIndex ) == 1 )
						{
							faceCorner.TextureCoordIndex = -1;
							faceCorner.NormalIndex = -1;
							pFace->m_FaceCorners.push_back( faceCorner );
						}
						
					}
				}
				break;
				// Unknown
				default:
				{
				}
				break;
			}
		}

		// Succeeded.
		return true;
	}

	Bool ObjFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ) );
		if( fin.is_open( ) == false )
		{
			std::cout << "[BmpFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		// Check the filesize
		fin.seekg( 0, std::ifstream::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::ifstream::beg );

		// Create a buffer
		char * pBuffer = new char[ fileSize + 1 ];
		fin.read( pBuffer, fileSize );
		pBuffer[ fileSize ] = 0;

		// Close the file
		fin.close( );

		// Create a stream
		std::stringstream       ss;
		ss.write( pBuffer, fileSize );

		// Delete buffer
		delete [ ] pBuffer;

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool ObjFile::SaveToMemory( std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;

		// Save the stream
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Get the string
		p_Memory = ss.str( );

		// Succeeded
		return true;
	}

	Bool ObjFile::SaveToStream( std::ostream & p_Stream )
	{
		return false;
	}

	Bool ObjFile::SaveToFile( const std::string & p_Filename )
	{
		// Load a string stream.
		std::stringstream ss;

		// Save the stream.
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Open the file.
		std::ofstream fout( p_Filename.c_str( ), std::fstream::binary );
		if( fout.is_open( ) == false )
		{
			std::cout << "[BmpFile::SaveToFile] Can not open the file. " << std::endl;
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

		// Succeeded.
		return true;
	}

	void ObjFile::Clear( )
	{
		m_MaterialFilename = "";

		for( ObjectVector::size_type i = 0; i < m_Objects.size( ); i++ )
		{
			delete m_Objects[ i ];
		}
		m_Objects.clear( );
	}

	const std::string & ObjFile::GetMaterialFilename( ) const
	{
		return m_MaterialFilename;
	}

	ObjFile::Object & ObjFile::GetObject( const SizeType p_Index )
	{
		return *m_Objects[ p_Index ];
	}

	SizeType ObjFile::GetObjectCount( ) const
	{
		return static_cast<SizeType>( m_Objects.size( ) );
	}

}