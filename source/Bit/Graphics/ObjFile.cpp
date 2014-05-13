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

	const std::string & ObjFile::MaterialGroup::GetMaterialName( ) const
	{
		return m_MaterialName;
	}

	const ObjFile::Face & ObjFile::MaterialGroup::GetFlatFace( const SizeType p_Index ) const
	{
		return m_FlatFaces[ p_Index ];
	}

	const ObjFile::Face & ObjFile::MaterialGroup::GetSmoothFace( const SizeType p_Index ) const
	{
		return m_SmoothFaces[ p_Index ];
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

	const ObjFile::MaterialGroup & ObjFile::ObjectGroup::GetMaterialGroup( const SizeType p_Index ) const
	{
		return m_MaterialGroups[ p_Index ];
	}

	SizeType ObjFile::ObjectGroup::GetMaterialGroupCount( ) const
	{
		return static_cast<SizeType>( m_MaterialGroups.size( ) );
	}


	// Object class
	ObjFile::Object::Object( )
	{
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
		return m_ObjectGroups[ p_Index ];
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
	}

	Bool ObjFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.str( p_Memory );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool ObjFile::LoadFromStream( std::istream & p_Stream )
	{
		const SizeType maxLineLength = 128;
		char line[ maxLineLength ];
		std::string pre;
		pre.reserve( 32 );

		// Add an object to the obj file class.
		//m_Objects.push_back(


		// Keep on looping until we reach the end of the file.
		while( p_Stream.eof( ) == false )
		{
			// Get the first word on the current line.
			p_Stream >> pre;

			// Get the current line
			p_Stream.getline( line, maxLineLength );

			// Parse the line, check the first character
			switch( pre[ 0 ] )
			{
				// mtllib
				case 'm':
				{
				}
				break;
				// usemtl
				case 'u':
				{
				}
				break;
				// Object
				case 'o':
				{
				}
				break;
				// Group
				case 'g':
				{
				}
				break;
				// Smoothing group
				case 's':
				{
				}
				break;
				// Vertex position/texture coord/normal
				case 'v':
				{
				}
				break;
				// face normal
				case 'f':
				{
				}
				break;
				// Unknown
				default:
				{
				}
				break;
			}

		}

		return false;
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

		// Read the stream
		Bool status = LoadFromStream( fin );

		// Close the file
		fin.close( );

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
	}

	const std::string & ObjFile::GetMaterialFilename( ) const
	{
		return m_MaterialFilename;
	}

	ObjFile::Object & ObjFile::GetObject( const SizeType p_Index )
	{
		return m_Objects[ p_Index ];
	}

	SizeType ObjFile::GetObjectCount( ) const
	{
		return static_cast<SizeType>( m_Objects.size( ) );
	}

}