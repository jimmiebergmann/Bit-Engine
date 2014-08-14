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
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/ObjFile.hpp>
#include <Bit/Graphics/ObjMaterialFile.hpp>
#include <iostream>
#include <algorithm>
#include <Bit/System/MemoryLeak.hpp>

#if defined( BIT_PLATFORM_WINDOWS )
#undef min
#endif

namespace Bit
{
	Model::Model( const GraphicDevice & p_GraphicDevice ) :
		m_GraphicDevice( p_GraphicDevice )
	{
	}

	Model::~Model( )
	{
		for( ModelMaterialVector::size_type i = 0; i < m_Materials.size( ); i++ )
		{
			delete m_Materials[ i ];
		}
	}

	Bool  Model::LoadFromFile( const std::string & p_Filename )
	{
		// Get the file's extension
		std::string fileExtension = "";
		for( Int32 i = p_Filename.size( ) - 2; i >= 0; i-- )
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

	Bool Model::LoadFromObjFile(	const std::string & p_Filename,
									const Bool p_LoadTextureCoords,
									const Bool p_LoadNormalCoords )
	{
		// Obj files does not support animations
		// Should we use an index buffer for faces???
		// Currently just loading one object with one group, with one material and so on,
		// not supporting any face shape other than triangles.

		// Load a obj file.
		ObjFile obj;
		if( obj.LoadFromFile( p_Filename ) == false )
		{
			std::cout << "[Model::LoadFromObjFile] Failed to open OBJ file." << std::endl;
			return false;
		}

		// //////////////////////////////////////////////////////////////////////////////////////
		// Create the position buffer from the obj file.
		SizeType bufferSize = 0;

		// To through the object in the obj class
		for( SizeType i = 0; i < obj.GetObjectCount( ); i++ )
		{
			ObjFile::Object & object = obj.GetObject( i );

			// To through the object groups in the obj class
			for( SizeType j = 0; j < object.GetObjectGroupCount( ); j++ )
			{
				ObjFile::ObjectGroup & objectGroup = object.GetObjectGroup( j );

				// To through the material groups in the obj class
				for( SizeType k = 0; k < objectGroup.GetMaterialGroupCount( ); k++ )
				{
					Float32 * pBufferData = obj.CreatePositionBuffer<Float32>( bufferSize, i, j, k );

					// Error check the position buffer data
					if( pBufferData == NULL )
					{
						std::cout << "[Model::LoadFromObjFile] No postiion data were found in the obj file." << std::endl;
						return false;
					}

					// Load the position vertex buffer
					VertexBuffer * pPositionVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
					if( pPositionVertexBuffer->Load( bufferSize * 4, pBufferData ) == false )
					{
						std::cout << "[Model::LoadFromObjFile] Can not load the vertex buffer" << std::endl;
						return false;
					}

					// Delete the allocated data
					delete [ ] pBufferData;

					// Add new model vertex data to the vertex group
					ModelVertexData * pModelVertexData = m_VertexGroup.AddVertexData( );

					// Error check the vertex model vertex data
					if( pModelVertexData == NULL )
					{
						std::cout << "[Model::LoadFromObjFile] Can not add vertex data to model vertex group." << std::endl;
						return false;
					}
		
					// Create the vertex array and add it to the vertex data
					VertexArray * pVertexArray = m_GraphicDevice.CreateVertexArray( );
					pModelVertexData->SetVertexArray( pVertexArray );

					// Add the vertex buffer to the vertex array.
					pVertexArray->AddVertexBuffer( *pPositionVertexBuffer, 3, DataType::Float32, 0 );

					// Add the vertex buffer to the model vertex data class.
					pModelVertexData->AddVertexBuffer( pPositionVertexBuffer, 0x01 );
					
					// //////////////////////////////////////////////////////////////////////////////////////
					// Try to add texture coordinate and normal buffers as well.
					// Create the texture coordinate buffer from the obj file.
					pBufferData = obj.CreateTextureCoordBuffer<Float32>( bufferSize, i, j, k );

					// Error check the position buffer data
					if( pBufferData != NULL )
					{
						// Load the position vertex buffer
						VertexBuffer * pTextureVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
						if( pTextureVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
						{
							// Add the vertex buffer to the vertex array.
							pVertexArray->AddVertexBuffer( *pTextureVertexBuffer, 2, DataType::Float32, 1 );

							// Add the vertex buffer to the vertex data class.
							pModelVertexData->AddVertexBuffer( pTextureVertexBuffer, 0x02 );
						}

						// Delete the allocated data
						delete [ ] pBufferData;
					}

					// Create the normal buffer from the obj file.
					pBufferData = obj.CreateNormalBuffer<Float32>( bufferSize, i, j, k );

					// Error check the position buffer data
					if( pBufferData != NULL )
					{
						// Load the position vertex buffer
						VertexBuffer * pNormalVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
						if( pNormalVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
						{
							// Add the vertex buffer to the vertex array.
							pVertexArray->AddVertexBuffer( *pNormalVertexBuffer, 3, DataType::Float32, 2 );

							// Add the vertex buffer to the vertex data class.
							pModelVertexData->AddVertexBuffer( pNormalVertexBuffer, 0x04 );
						}

						// Delete the allocated data
						delete [ ] pBufferData;
					}


				}
			}
		}

/*		
		// //////////////////////////////////////////////////////////////////////////////////////
		// Try to add texture coordinate and normal buffers as well.
		// Create the texture coordinate buffer from the obj file.
		pBufferData = obj.CreateTextureCoordBuffer<Float32>( bufferSize );

		// Error check the position buffer data
		if( pBufferData != NULL )
		{
			// Load the position vertex buffer
			VertexBuffer * pTextureVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
			if( pTextureVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
			{
				// Add the vertex buffer to the vertex array.
				pVertexArray->AddVertexBuffer( *pTextureVertexBuffer, 2, DataType::Float32, 1 );

				// Add the vertex buffer to the vertex data class.
				m_VertexData.AddVertexBuffer( pTextureVertexBuffer, 0x02 );
			}

			// Delete the allocated data
			delete [ ] pBufferData;
		}

		// Create the normal buffer from the obj file.
		pBufferData = obj.CreateNormalBuffer<Float32>( bufferSize );

		// Error check the position buffer data
		if( pBufferData != NULL )
		{
			// Load the position vertex buffer
			VertexBuffer * pNormalVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
			if( pNormalVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
			{
				// Add the vertex buffer to the vertex array.
				pVertexArray->AddVertexBuffer( *pNormalVertexBuffer, 3, DataType::Float32, 2 );

				// Add the vertex buffer to the vertex data class.
				m_VertexData.AddVertexBuffer( pNormalVertexBuffer, 0x04 );
			}

			// Delete the allocated data
			delete [ ] pBufferData;
		}

*/

		// Load the materials of the obj file
		ObjMaterialFile objMaterial;
		if( objMaterial.LoadFromFile( "input/" + obj.GetMaterialFilename( ) ) == false )
		{
			std::cout << "[Model::LoadFromObjFile] Failed to open OBJ material file." << std::endl;
			return false;
		}

		// Get the materials from the OBJ material file
		for( SizeType i = 0; i < objMaterial.GetMaterialCount( ); i++ )
		{
			// Get the obj material
			const ObjMaterialFile::Material & material = objMaterial.GetMaterial( i );

			// Create a new json material value
			Json::Value * pValue = new Json::Value;

			// Add default properties.
			
			// Add color
			(*pValue)[ "Color" ][ "r" ] = material.GetDiffuseColor( ).x;
			(*pValue)[ "Color" ][ "g" ] = material.GetDiffuseColor( ).y;
			(*pValue)[ "Color" ][ "b" ] = material.GetDiffuseColor( ).z;
			(*pValue)[ "Color" ][ "a" ] = material.GetOpticalDensity( );

			// Add shininess
			if( material.GetShininess( ) > 0.0f )
			{
				(*pValue)[ "Shininess" ] = std::min( material.GetShininess( ), 1.0f );
			}

			// Add color map
			if( material.GetAmbientTexture( ).size( ) )
			{
				(*pValue)[ "ColorMap" ] = material.GetAmbientTexture( );
			}

			// Add normal map
			if( material.GetBumpTexture( ).size( ) )
			{
				(*pValue)[ "NormalMap" ] = material.GetBumpTexture( );
			}

			// Add the material to the vector.
			m_Materials.push_back( pValue );
		}

		// Succeeded
		return true;
	}

	Skeleton & Model::GetSkeleton( )
	{
		return m_Skeleton;
	}

	ModelVertexGroup & Model::GetVertexGroup( )
	{
		return m_VertexGroup;
	}

	SizeType Model::GetMaterialCount( ) const
	{
		return static_cast<SizeType>( m_Materials.size( ) );
	}

	const Json::Value & Model::GetMaterial( const SizeType p_Index ) const
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Materials.size( ) ) )
		{
			return Json::Value::NullValue;
		}

		// Get the json material value
		return *m_Materials[ p_Index ];
	}

}