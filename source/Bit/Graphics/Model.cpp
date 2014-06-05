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
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/VertexBuffer.hpp>
#include <Bit/Graphics/ObjFile.hpp>
#include <iostream>
#include <algorithm>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	Model::Model( const GraphicDevice & p_GraphicDevice ) :
		m_GraphicDevice( p_GraphicDevice )
	{
	}

	Model::~Model( )
	{
	}

	Bool  Model::LoadFromFile( const std::string & p_Filename )
	{
		// Get the file's extension
		std::string fileExtension = "";
		for( SizeType i = p_Filename.size( ) - 2; i >= 0; i-- )
		{
			// Look for '.'
			if( p_Filename[ i ] == '.' )
			{
				fileExtension = p_Filename.substr( i + 1, p_Filename.size( ) - i - 1 );
				break;
			}
		}

		// Make all the characters in the file extension to upper case letters
		std::transform( fileExtension.begin( ), fileExtension.end( ), fileExtension.begin( ), ::toupper );

		// Load the right format.
		if( fileExtension == "OBJ" )
		{
			return LoadFromObjFile( p_Filename );
		}
		else if( fileExtension == "MD2" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD2 models yet.\n";
			return false;
		}
		else if( fileExtension == "MD3" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD3 models yet.\n";
			return false;
		}
		else if( fileExtension == "MD5" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD5 models yet.\n";
			return false;
		}
		else if( fileExtension == "MDL" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MDL models yet.\n";
			return false;
		}

		// Unknown extension
		std::cout << "[Model::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		return false;
	}

	Bool Model::LoadFromObjFile( const std::string & p_Filename )
	{
		// Obj files does not support animations
		// Should we use an index buffer for faces???
		// Currently just loading one object with one group, with one material and so on,
		// not supporting any face shape other than triangles.

		// Load a obj file.
		ObjFile obj;
		if( obj.LoadFromFile( p_Filename ) == false )
		{
			return false;
		}

		// Get the first object
		if( obj.GetObjectCount( ) == 0 )
		{
			std::cout << "[Model::LoadFromObjFile] No objects were found." << std::endl;
			return false;
		}
		ObjFile::Object & object = obj.GetObject( 0 );

		// Error check the object group
		if( object.GetObjectGroupCount( ) == 0 )
		{
			std::cout << "[Model::LoadFromObjFile] No object groups were found." << std::endl;
			return false;
		}
		ObjFile::ObjectGroup & objectGroup = object.GetObjectGroup( 0 );
		
		// Error check the material group
		if( objectGroup.GetMaterialGroupCount( ) == 0 )
		{
			std::cout << "[Model::LoadFromObjFile] No material groups were found." << std::endl;
			return false;
		}
		ObjFile::MaterialGroup & materialGroup = objectGroup.GetMaterialGroup( 0 );

		// Calculate the buffer size
		SizeType bufferSize =	( materialGroup.GetFlatFaceCount( )		* 3 * 3 )/* +
								( materialGroup.GetSmoothFaceCount( )	* 3 * 3 )*/;

		// Create a vertex buffer
		VertexBuffer * pVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
		Float32 * pBufferData = new Float32[ bufferSize ];

		// Go throguh the flat faces and add the data to the buffer
		for( SizeType i = 0; i < materialGroup.GetSmoothFaceCount( ); i++ )
		{
			// Get the current face
			ObjFile::Face & face = materialGroup.GetFlatFace( i );
			if( face.GetFaceCornerCount( ) != 3 )
			{
				std::cout << "[Model::LoadFromObjFile] Face corner counter error:" << face.GetFaceCornerCount( ) << std::endl;
				
				// Delete the allocated data
				delete [ ] pBufferData;
				return false;
			}

			// Go throguh the face corners
			for( SizeType j = 0; j < 3; j++ )
			{
				// Get the current corner
				ObjFile::FaceCorner & faceCorner = face.GetFaceCorner( j );

				// Get the vertex index.
				Int32 vertexIndex = faceCorner.VertexIndex;

				if( vertexIndex < 0 || vertexIndex >= object.GetVertexCount( ) )
				{
					std::cout << "[Model::LoadFromObjFile] Vertex index error:" << vertexIndex << std::endl;
				
					// Delete the allocated data
					delete [ ] pBufferData;
					return false;
				}

				// Get the vertex coordinate.
				const Vector3f32 position = object.GetVertex( vertexIndex );

				// Set the buffer data
				pBufferData[ ( i * 9 ) + ( j * 3 ) ] = position.x;
				pBufferData[ ( i * 9 ) + ( j * 3 ) + 1 ] = position.y;
				pBufferData[ ( i * 9 ) + ( j * 3 ) + 2 ] = position.z;
			}
		}


		// Load the vertex buffer
		if( pVertexBuffer->Load( bufferSize * 4, pBufferData ) == false )
		{
			std::cout << "[Model::LoadFromObjFile] Can not load the vertex buffer" << std::endl;
			return false;
		}

		// Delete the allocated data
		delete [ ] pBufferData;

		// Add the vertex buffer to the vertex data class.
		m_VertexData.AddVertexBuffer( pVertexBuffer );

		// Succeeded
		return true;
	}

	Skeleton & Model::GetSkeleton( )
	{
		return m_Skeleton;
	}

	ModelVertexData & Model::GetVertexData( )
	{
		return m_VertexData;
	}

}