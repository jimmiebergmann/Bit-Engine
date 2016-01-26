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
#include <Bit/Graphics/Renderer.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/Texture.hpp>
#include <Bit/Graphics/VertexBuffer.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/ShaderProgram.hpp>
#include <Bit/Graphics/ObjFile.hpp>
#include <Bit/Graphics/ObjMaterialFile.hpp>
#include <Bit/Graphics/Md2File.hpp>
#include <Bit/Graphics/Model/VertexAnimation.hpp>
#include <Bit/Graphics/Model/VertexAnimationTrack.hpp>
#include <Bit/Graphics/Model/VertexKeyFrame.hpp>
#include <sstream>
#include <Bit/System/Log.hpp>
#include <algorithm>
#include <Bit/System/Path.hpp>
#include <Bit/System/ResourceManager.hpp>
#include <Bit/System/MatrixManager.hpp>
#include <Bit/System/MemoryLeak.hpp>

#if defined( BIT_PLATFORM_WINDOWS )
#undef min
#endif

namespace Bit
{
	Model::Model( const GraphicDevice & p_GraphicDevice ) :
		m_GraphicDevice( p_GraphicDevice ),
		m_Skeleton( this ),
		m_AnimationState( this )
	{
	}

	Model::~Model( )
	{
		for( ModelMaterialVector::size_type i = 0; i < m_Materials.size( ); i++ )
		{
			delete m_Materials[ i ];
		}
	}

	void Model::Draw(Renderer & p_Renderer)
	{
		// Check the animation state.
		if (m_AnimationState.GetState() == AnimationState::Stopped)
		{
			RenderInitialPose(p_Renderer);
		}
		else
		{
			// Error check the animation pointer.
			if (m_AnimationState.m_pAnimation == NULL)
			{
				return;
			}

			if (m_AnimationState.m_pAnimation->GetType() == Animation::Skeletal)
			{
				RenderSkeletalAnimation(p_Renderer);
			}
			else if (m_AnimationState.m_pAnimation->GetType() == Animation::PerVertex)
			{
				RenderVertexAnimation(p_Renderer);
			}
		}
	}

	Bool Model::LoadFromFile(	const std::string & p_Filename,
								const Bool p_LoadTextureCoords,
								const Bool p_LoadNormals,
								const Bool p_LoadTangents,
								const Bool p_LoadBinormals )
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
			return LoadFromObjFile( p_Filename, p_LoadTextureCoords, p_LoadNormals, p_LoadTangents, p_LoadBinormals );
		}
		else if( fileExtension == "MD2" )
		{
			return LoadFromMd2File( p_Filename, p_LoadTextureCoords, p_LoadNormals, p_LoadTangents, p_LoadBinormals );
		}
		else if( fileExtension == "MD3" )
		{
			bitLogGraErr(  "Not supporting MD3 models yet." );
			return false;
		}
		else if( fileExtension == "MD5" )
		{
			bitLogGraErr(  "Not supporting MD5 models yet." );
			return false;
		}
		else if( fileExtension == "MDL" )
		{
			bitLogGraErr(  "Not supporting MDL models yet." );
			return false;
		}

		// Unknown extension
		bitLogGraErr(  "Unknow model extension: " << fileExtension );
		return false;
	}

	Bool Model::LoadFromObjFile(	const std::string & p_Filename,
									const Bool p_LoadTextureCoords,
									const Bool p_LoadNormals,
									const Bool p_LoadTangents,
									const Bool p_LoadBinormals )
	{
		// Obj files does not support animations
		// Should we use an index buffer for faces???
		// not supporting any face shape other than triangles.

		// Load the obj file.
		ObjFile obj;
		if( obj.LoadFromFile( p_Filename ) == false )
		{
			bitLogGraErr(  "Failed to load OBJ file." );
			return false;
		}

		// Load the materials of the obj file if possible
		ObjMaterialFile objMaterial;
		
		if( obj.GetMaterialFilename( ).size( ) &&
			objMaterial.LoadFromFile(Path::GetDirectory(p_Filename) + "/" + obj.GetMaterialFilename()) == true)
		{
			// Get the materials from the OBJ material file
			for( SizeType i = 0; i < objMaterial.GetMaterialCount( ); i++ )
			{
				// Get the obj material
				const ObjMaterialFile::Material & material = objMaterial.GetMaterial( i );

				// Create a new json material value
				ModelMaterial * pMaterial = new ModelMaterial;

				// Add default properties.
				(*pMaterial)[ "MaterialName" ] = material.GetName( );
			
				// Add diffuse color
				Vector4f32 diffuseColor(material.GetDiffuseColor().x,
										material.GetDiffuseColor().y,
										material.GetDiffuseColor().z,
										material.GetOpticalDensity());

				pMaterial->SetDiffuseColor(diffuseColor);

				(*pMaterial)["Color"]["r"] = diffuseColor.x;
				(*pMaterial)["Color"]["g"] = diffuseColor.y;
				(*pMaterial)["Color"]["b"] = diffuseColor.z;
				(*pMaterial)["Color"]["a"] = diffuseColor.w;

				// Add shininess
				if( material.GetShininess( ) > 0.0f )
				{
					(*pMaterial)[ "Shininess" ] = std::min( material.GetShininess( ), 1.0f );
				}

				// Add color map
				if (material.GetDiffuseTexture().size())
				{
					(*pMaterial)["ColorMap"] = material.GetDiffuseTexture();
					
					// add the texture to the material
					Texture * pTexture = ResourceManager::GetDefault()->GetTexture(material.GetDiffuseTexture());
					if( pTexture )
					{
						pMaterial->SetColorTexture( pTexture );
					}
				}

				// Add normal map
				if( material.GetBumpTexture( ).size( ) )
				{
					(*pMaterial)[ "NormalMap" ] = material.GetBumpTexture( );

					// add the texture to the material
					Texture * pTexture = ResourceManager::GetDefault( )->GetTexture( material.GetBumpTexture( ) );
					if( pTexture )
					{
						pMaterial->SetNormalTexture( pTexture );
					}
				}

				// Add the material to the vector.
				m_Materials.push_back( pMaterial );
			}
		}
		else
		{
			bitLogGraErr(  "Failed to open OBJ material file." );
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
					ObjFile::MaterialGroup & materialGroup = objectGroup.GetMaterialGroup( j );

					// Create the vertex buffer.
					Float32 * pBufferData = obj.CreatePositionBuffer<Float32>( bufferSize, i, j, k );

					// Error check the position buffer data
					if( pBufferData == NULL )
					{
						bitLogGraErr(  "No postiion data were found in the obj file." );
						return false;
					}

					// Load the position vertex buffer
					VertexBuffer * pPositionVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
					if( pPositionVertexBuffer->Load( bufferSize * 4, pBufferData ) == false )
					{
						bitLogGraErr(  "Can not load the vertex buffer" );
						return false;
					}

					// Delete the allocated data
					delete [ ] pBufferData;

					// Add new model vertex data to the vertex group
					ModelVertexData * pModelVertexData = m_VertexGroup.AddVertexData( );

					// Error check the vertex model vertex data
					if( pModelVertexData == NULL )
					{
						bitLogGraErr(  "Can not add vertex data to model vertex group." );
						return false;
					}
		
					// Create the vertex array and add it to the vertex data
					VertexArray * pVertexArray = m_GraphicDevice.CreateVertexArray( );
					pModelVertexData->SetVertexArray( pVertexArray );


					// Find the material for the current vertex data
					for( SizeType l = 0; l < m_Materials.size( ); l++ )
					{
						if( m_Materials[ l ]->Get( "MaterialName", "" ).AsString( ) == materialGroup.GetMaterialName( ) )
						{
							pModelVertexData->SetMaterial( m_Materials[ l ] );
						}
					}

					// Add the vertex buffer to the vertex array.
					pVertexArray->AddVertexBuffer(*pPositionVertexBuffer, 3, DataType::Float32, ModelVertexData::PositionIndex);

					// Add the vertex buffer to the model vertex data class.
					pModelVertexData->AddVertexBuffer(pPositionVertexBuffer, ModelVertexData::PositionMask);
					
					// //////////////////////////////////////////////////////////////////////////////////////
					// Try to add texture coordinate and normal buffers as well.
					// Create the texture coordinate buffer from the obj file.
					if( p_LoadTextureCoords )
					{
						pBufferData = obj.CreateTextureCoordBuffer<Float32>( bufferSize, i, j, k );

						// Error check the position buffer data
						if( pBufferData != NULL )
						{
							// Load the position vertex buffer
							VertexBuffer * pTextureVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
							if( pTextureVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
							{
								// Add the vertex buffer to the vertex array.
								pVertexArray->AddVertexBuffer(*pTextureVertexBuffer, 2, DataType::Float32, ModelVertexData::TextureCoordIndex);

								// Add the vertex buffer to the vertex data class.
								pModelVertexData->AddVertexBuffer(pTextureVertexBuffer, ModelVertexData::TextureCoordMask);
							}

							// Delete the allocated data
							delete [ ] pBufferData;
						}
					}

					// Create the normal buffer from the obj file.
					if( p_LoadNormals )
					{
						pBufferData = obj.CreateNormalBuffer<Float32>( bufferSize, i, j, k );

						// Error check the position buffer data
						if( pBufferData != NULL )
						{
							// Load the position vertex buffer
							VertexBuffer * pNormalVertexBuffer = m_GraphicDevice.CreateVertexBuffer( );
							if( pNormalVertexBuffer->Load( bufferSize * 4, pBufferData ) != false )
							{
								// Add the vertex buffer to the vertex array.
								pVertexArray->AddVertexBuffer(*pNormalVertexBuffer, 3, DataType::Float32, ModelVertexData::NormalIndex);

								// Add the vertex buffer to the vertex data class.
								pModelVertexData->AddVertexBuffer(pNormalVertexBuffer, ModelVertexData::NormalMask);
							}

							// Delete the allocated data
							delete [ ] pBufferData;
						}
					}

				}
			}
		}

		// Succeeded
		return true;
	}


	Bool Model::LoadFromMd2File(	const std::string & p_Filename,
									const Bool p_LoadTextureCoords,
									const Bool p_LoadNormals,
									const Bool p_LoadTangents,
									const Bool p_LoadBinormals )
	{
		// Load the obj file.
		Md2File md2;
		if( md2.LoadFromFile( p_Filename ) == false )
		{
			bitLogGraErr(  "Failed to load MD2 file." );
			return false;
		}

		// Load material if there's any skin
		// Create and add a new material value
		ModelMaterial * pMaterial = new ModelMaterial;
		m_Materials.push_back( pMaterial );

		// Add default properties.
		(*pMaterial)[ "MaterialName" ] = "Default";
			
		// Add color map
		if( md2.GetSkinCount( ) > 0 && strlen( md2.GetSkin( 0 )->Name ) )
		{
			(*pMaterial)[ "ColorMap" ] = md2.GetSkin( 0 )->Name;
					
			// add the texture to the material
			Texture * pTexture = ResourceManager::GetDefault( )->GetTexture( md2.GetSkin( 0 )->Name );
			if( pTexture )
			{
				pMaterial->SetColorTexture( pTexture );
			}
		}

		// Get the first frame
		if( md2.GetFrameCount( ) == 0 )
		{
			bitLogGraErr(  "No frames were found." );
			return false;
		}

		// Get the first frame(initial pose)
		if (LoadMd2Frame(	md2, &m_VertexGroup, 0,
							p_LoadTextureCoords,
							p_LoadNormals,
							p_LoadTangents,
							p_LoadBinormals) == false)
		{
			bitLogGraErr(  "Could not load frame 0." );
			return false;
		}

		// Now, let's add all the other keyframes, let's animate!

		// Create an animation. All animations are actually just one.
		VertexAnimation * pAnimation = m_Skeleton.CreateVertexAnimation();

		// Create an animation track.
		VertexAnimationTrack * pAnimationTrack = reinterpret_cast<VertexAnimationTrack*>(pAnimation->CreateTrack());

		// Go through the rest of the frames.
		for (SizeType i = 1; i < md2.GetFrameCount(); i++)
		{

			// Create key frame.
			VertexKeyFrame * pKeyFrame = reinterpret_cast<VertexKeyFrame*>(pAnimationTrack->CreateKeyFrame(Seconds(static_cast<Float64>(i - 1) / 25.0f)));
		
			// Load the frame.
			if (LoadMd2Frame(md2, &(pKeyFrame->GetVertexGroup()), i,
				p_LoadTextureCoords,
				p_LoadNormals,
				p_LoadTangents,
				p_LoadBinormals) == false)
			{
				bitLogGraErr(  "Could not load frame " << i << "." );
				return false;
			}
		}

		// Fix in terpolations
		FixVertexInterpolation(	pAnimationTrack,
								p_LoadTextureCoords,
								p_LoadNormals,
								p_LoadTangents,
								p_LoadBinormals);

		// Succeeded
		return true;
	}

	Skeleton & Model::GetSkeleton( )
	{
		return m_Skeleton;
	}

	AnimationState & Model::GetAnimationState()
	{
		return m_AnimationState;
	}

	ModelVertexGroup & Model::GetVertexGroup( )
	{
		return m_VertexGroup;
	}

	SizeType Model::GetMaterialCount( ) const
	{
		return static_cast<SizeType>( m_Materials.size( ) );
	}

	ModelMaterial & Model::GetMaterial( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Materials.size( ) ) )
		{
			return const_cast<ModelMaterial &>( ModelMaterial::ErrorMaterial );
		}

		// Get the json material value
		return *m_Materials[ p_Index ];
	}

	void Model::RenderInitialPose(Renderer & p_Renderer)
	{
		// Get const free graphic device
		GraphicDevice * pGraphicDevice = p_Renderer.GetGraphicDevice();

		// Get the default shader program.
		ShaderProgram * pShaderProgram = m_GraphicDevice.GetDefaultShaderProgram(GraphicDevice::InitialPoseShader);

		// Error check the shader program.
		if (pShaderProgram == NULL)
		{
			return;
		}

		// Get the vertex group
		ModelVertexGroup & vertexGroup = GetVertexGroup();

		// Error check the vertex data.
		if (vertexGroup.GetVertexDataCount() == 0)
		{
			return;
		}

		// Bind the shader program.
		pShaderProgram->Bind();

		// Set uniform data.
		pShaderProgram->SetUniformMatrix4x4f("uProjectionMatrix", MatrixManager::GetProjectionMatrix());
		pShaderProgram->SetUniformMatrix4x4f("uModelViewMatrix", MatrixManager::GetModelViewMatrix());

		// Set light uniforms
		GraphicDevice::DefaultModelSettings & modelSettings = pGraphicDevice->GetDefaultModelSettings();

		// Set light count
		pShaderProgram->SetUniform1i("uLightCount", modelSettings.GetActiveLightCount());

		// Set ambient color
		const Vector3f32 & ambColor = modelSettings.GetAmbientLight();
		pShaderProgram->SetUniform3f("uAmbientColor", ambColor.x, ambColor.y, ambColor.z);

		// Go throguh all the lights
		for (SizeType i = 0; i < modelSettings.GetActiveLightCount(); i++)
		{
			std::stringstream positionStream;
			positionStream << "uLightPositions[" << i << "]";
			std::stringstream colorStream;
			colorStream << "uLightColors[" << i << "]";

			const Vector4f32 & pos = modelSettings.GetLight(i).GetPosition();
			const Vector3f32 & color = modelSettings.GetLight(i).GetColor();
			pShaderProgram->SetUniform4f(positionStream.str().c_str(), pos.x, pos.y, pos.z, pos.w);
			pShaderProgram->SetUniform3f(colorStream.str().c_str(), color.x, color.y, color.z);
		}

		// Go throguh all the vertex data elements.
		for (SizeType i = 0; i < vertexGroup.GetVertexDataCount(); i++)
		{
			// Get the current vertex data.
			ModelVertexData * vertexData = vertexGroup.GetVertexData(i);

			// Error check the vertex data pointer.
			if (vertexData == NULL || vertexData->GetVertexArray() == NULL)
			{
				continue;
			}

			//pShaderProgram->SetUniform4f( "uColor", 1.0f, 1.0f, 1.0f, 1.0f );
			if (vertexData->GetBitmask() & ModelVertexData::TextureCoordMask)
			{
				pShaderProgram->SetUniform1i("uUseTexture", 1);
			}
			else
			{
				pShaderProgram->SetUniform1i("uUseTexture", 0);
			}

			// Set use normals flag.
			if (vertexData->GetBitmask() & ModelVertexData::NormalMask)
			{
				pShaderProgram->SetUniform1i("uUseNormals", 1);
			}
			else
			{
				pShaderProgram->SetUniform1i("uUseNormals", 0);
			}

			// Get the model material
			const ModelMaterial & material = vertexData->GetMaterial();

			// Bind the color texture if any
			Texture * pTexture = material.GetColorTexture();

			// Bind the texture.
			if (pTexture)
			{
				pTexture->Bind(0);
			}

			// Render the model.
			vertexData->GetVertexArray()->Render(PrimitiveMode::Triangles);
		}

		// Unbind the shader program.
		pShaderProgram->Unbind();
	}

	void Model::RenderVertexAnimation(Renderer & p_Renderer)
	{
		// Get and set the animation time
		if (m_AnimationState.m_State == AnimationState::Playing)
		{
			m_AnimationState.m_Timer.Stop();
			m_AnimationState.m_Time += m_AnimationState.m_Timer.GetTime();
			m_AnimationState.m_Timer.Start();
		}

		// Get the animation track
		VertexAnimationTrack * pAnimationTrack = reinterpret_cast<VertexAnimationTrack *>(m_AnimationState.m_pAnimation->GetTrack( 0 ));
		if (pAnimationTrack == NULL)
		{
			return;
		}

		// Temporary calculate the frame index;
		SizeType frameIndex = static_cast<SizeType>(m_AnimationState.m_Time.AsSeconds() * m_AnimationState.GetAnimationSpeed( ));
		frameIndex %= pAnimationTrack->GetKeyFrameCount();


		// Get const free graphic device
		GraphicDevice * pGraphicDevice = p_Renderer.GetGraphicDevice();

		// Get the default shader program.
		ShaderProgram * pShaderProgram = m_GraphicDevice.GetDefaultShaderProgram(GraphicDevice::VertexAnimationShader);

		// Error check the shader program.
		if (pShaderProgram == NULL)
		{
			return;
		}

		// Get the frame
		VertexKeyFrame * pKeyFrame = reinterpret_cast<VertexKeyFrame *>(pAnimationTrack->GetKeyFrame(frameIndex));

		// Get the vertex group
		ModelVertexGroup & vertexGroup = pKeyFrame->GetVertexGroup();

		// Error check the vertex data.
		if (vertexGroup.GetVertexDataCount() == 0)
		{
			return;
		}

		// Bind the shader program.
		pShaderProgram->Bind();

		// Set animation uniform
		pShaderProgram->SetUniform1f("u_Interpolation", ((m_AnimationState.m_Time * m_AnimationState.GetAnimationSpeed()) % Seconds(1.0f)).AsSeconds());

		

		// Set uniform data.
		pShaderProgram->SetUniformMatrix4x4f("uProjectionMatrix", MatrixManager::GetProjectionMatrix());
		pShaderProgram->SetUniformMatrix4x4f("uModelViewMatrix", MatrixManager::GetModelViewMatrix());

		// Set light uniforms
		GraphicDevice::DefaultModelSettings & modelSettings = pGraphicDevice->GetDefaultModelSettings();

		// Set light count
		pShaderProgram->SetUniform1i("uLightCount", modelSettings.GetActiveLightCount());

		// Set ambient color
		const Vector3f32 & ambColor = modelSettings.GetAmbientLight();
		pShaderProgram->SetUniform3f("uAmbientColor", ambColor.x, ambColor.y, ambColor.z);

		// Go throguh all the lights
		for (SizeType i = 0; i < modelSettings.GetActiveLightCount(); i++)
		{
			std::stringstream positionStream;
			positionStream << "uLightPositions[" << i << "]";
			std::stringstream colorStream;
			colorStream << "uLightColors[" << i << "]";

			const Vector4f32 & pos = modelSettings.GetLight(i).GetPosition();
			const Vector3f32 & color = modelSettings.GetLight(i).GetColor();
			pShaderProgram->SetUniform4f(positionStream.str().c_str(), pos.x, pos.y, pos.z, pos.w);
			pShaderProgram->SetUniform3f(colorStream.str().c_str(), color.x, color.y, color.z);
		}

		// Go throguh all the vertex data elements.
		for (SizeType i = 0; i < vertexGroup.GetVertexDataCount(); i++)
		{
			// Get the current vertex data.
			ModelVertexData * vertexData = vertexGroup.GetVertexData(i);

			// Error check the vertex data pointer.
			if (vertexData == NULL || vertexData->GetVertexArray() == NULL)
			{
				continue;
			}

			//pShaderProgram->SetUniform4f( "uColor", 1.0f, 1.0f, 1.0f, 1.0f );
			if (vertexData->GetBitmask() & ModelVertexData::TextureCoordMask)
			{
				pShaderProgram->SetUniform1i("uUseTexture", 1);
			}
			else
			{
				pShaderProgram->SetUniform1i("uUseTexture", 0);
			}

			// Set use normals flag.
			if (vertexData->GetBitmask() & ModelVertexData::NormalMask)
			{
				pShaderProgram->SetUniform1i("uUseNormals", 1);
			}
			else
			{
				pShaderProgram->SetUniform1i("uUseNormals", 0);
			}

			// Get the model material
			const ModelMaterial & material = vertexData->GetMaterial();

			// Bind the color texture if any
			Texture * pTexture = material.GetColorTexture();

			// Bind the texture.
			if (pTexture)
			{
				pTexture->Bind(0);
			}

			// Render the model.
			vertexData->GetVertexArray()->Render(PrimitiveMode::Triangles);
		}

		// Unbind the shader program.
		pShaderProgram->Unbind();



	}

	void Model::RenderSkeletalAnimation(Renderer & p_Renderer)
	{
	}

	Bool Model::LoadMd2Frame(	Md2File & p_Md2File,
								ModelVertexGroup * p_pModelVertexGroup,
								const SizeType p_FrameIndex,
								const Bool p_LoadTextureCoords,
								const Bool p_LoadNormals,
								const Bool p_LoadTangents,
								const Bool p_LoadBinormals)
	{
		if (p_pModelVertexGroup == NULL)
		{
			return false;
		}

		// Create the vertex buffer.
		SizeType bufferSize = 0;

		Float32 * pBufferData = p_Md2File.CreatePositionBuffer<Float32>(bufferSize, p_FrameIndex);

		// Error check the position buffer data
		if (pBufferData == NULL)
		{
			bitLogGraErr(  "No postiion data were found in the obj file." );
			return false;
		}

		// Load the position vertex buffer
		VertexBuffer * pPositionVertexBuffer = m_GraphicDevice.CreateVertexBuffer();
		if (pPositionVertexBuffer->Load(bufferSize * 4, pBufferData) == false)
		{
			bitLogGraErr(  "Can not load the vertex buffer" );
			return false;
		}

		// Delete the allocated data
		delete[] pBufferData;

		// Add new model vertex data to the vertex group
		ModelVertexData * pModelVertexData = p_pModelVertexGroup->AddVertexData();

		// Error check the vertex model vertex data
		if (pModelVertexData == NULL)
		{
			bitLogGraErr(  "Can not add vertex data to model vertex group." );
			return false;
		}

		// Create the vertex array and add it to the vertex data
		VertexArray * pVertexArray = m_GraphicDevice.CreateVertexArray();
		pModelVertexData->SetVertexArray(pVertexArray);

		// Set the default(the only material for the model vertex data
		pModelVertexData->SetMaterial(m_Materials[0]);

		// Add the vertex buffer to the vertex array.
		pVertexArray->AddVertexBuffer(*pPositionVertexBuffer, 3, DataType::Float32, ModelVertexData::PositionIndex);

		// Add the vertex buffer to the model vertex data class.
		pModelVertexData->AddVertexBuffer(pPositionVertexBuffer, ModelVertexData::PositionMask);

		// //////////////////////////////////////////////////////////////////////////////////////
		// Try to add texture coordinate and normal buffers as well.
		// Create the texture coordinate buffer from the obj file.
		if (p_LoadTextureCoords)
		{
			pBufferData = p_Md2File.CreateTextureCoordBuffer<Float32>(bufferSize);

			// Error check the position buffer data
			if (pBufferData != NULL)
			{
				// Load the position vertex buffer
				VertexBuffer * pTextureVertexBuffer = m_GraphicDevice.CreateVertexBuffer();
				if (pTextureVertexBuffer->Load(bufferSize * 4, pBufferData) != false)
				{
					// Add the vertex buffer to the vertex array.
					pVertexArray->AddVertexBuffer(*pTextureVertexBuffer, 2, DataType::Float32, ModelVertexData::TextureCoordIndex);

					// Add the vertex buffer to the vertex data class.
					pModelVertexData->AddVertexBuffer(pTextureVertexBuffer, ModelVertexData::TextureCoordMask);
				}

				// Delete the allocated data
				delete[] pBufferData;
			}
		}

		// Create the normal buffer from the obj file.
		if (p_LoadNormals)
		{
			pBufferData = p_Md2File.CreateNormalBuffer<Float32>(bufferSize, p_FrameIndex);

			// Error check the position buffer data
			if (pBufferData != NULL)
			{
				// Load the position vertex buffer
				VertexBuffer * pNormalVertexBuffer = m_GraphicDevice.CreateVertexBuffer();
				if (pNormalVertexBuffer->Load(bufferSize * 4, pBufferData) != false)
				{
					// Add the vertex buffer to the vertex array.
					pVertexArray->AddVertexBuffer(*pNormalVertexBuffer, 3, DataType::Float32, ModelVertexData::NormalIndex);

					// Add the vertex buffer to the vertex data class.
					pModelVertexData->AddVertexBuffer(pNormalVertexBuffer, ModelVertexData::NormalMask);
				}

				// Delete the allocated data
				delete[] pBufferData;
			}
		}

		return true;
	}

	void Model::FixVertexInterpolation(	VertexAnimationTrack * p_pAnimationTrack,
										const Bool p_LoadTextureCoords,
										const Bool p_LoadNormals,
										const Bool p_LoadTangents,
										const Bool p_LoadBinormals)
	{

		// Go through all the key frames.
		for (SizeType i = 0; i < p_pAnimationTrack->GetKeyFrameCount() - 1; i++)
		{
			// Get current and next frame
			VertexKeyFrame * pCurrentFrame = reinterpret_cast<VertexKeyFrame *>(p_pAnimationTrack->GetKeyFrame(i));
			VertexKeyFrame * pNextFrame = reinterpret_cast<VertexKeyFrame *>(p_pAnimationTrack->GetKeyFrame(i + 1));

			// Get needed variables.
			ModelVertexData * pCurrentVertexData	= pCurrentFrame->GetVertexGroup().GetVertexData(0);
			VertexArray * pCurrentVertexArray		= pCurrentVertexData->GetVertexArray();
			ModelVertexData * pNextVertexData		= pNextFrame->GetVertexGroup().GetVertexData(0);

			//pCurrentFrame->GetVertexGroup().GetVertexData(0)->GetVertexBufferCount( )

			// Add all the next frame's VBOs to the current frame,
			// Only go through the position and normal masks.
			ModelVertexData::eBufferIndex currentIndex = ModelVertexData::NextPositionIndex;
			for (SizeType j = 0; j < pNextVertexData->GetVertexBufferCount() && j < 3; j++)
			{
				//pCurrentVertexData->AddVertexBuffer(pNextVertexData->GetVertexBuffer(j), currentIndex);
				pCurrentVertexArray->AddVertexBuffer(*pNextVertexData->GetVertexBuffer(j), 3, DataType::Float32, currentIndex);

				if (j == 0)
				{
					currentIndex = ModelVertexData::NextTextureCoordIndex;
				}
				else if (j == 1)
				{
					currentIndex = ModelVertexData::NextNormalIndex;
				}
			}
		
		}

		// Add the first frame to the last one.
		// Get current and next frame
		VertexKeyFrame * pCurrentFrame = reinterpret_cast<VertexKeyFrame *>(p_pAnimationTrack->GetKeyFrame(p_pAnimationTrack->GetKeyFrameCount() - 1));
		VertexKeyFrame * pNextFrame = reinterpret_cast<VertexKeyFrame *>(p_pAnimationTrack->GetKeyFrame(0));

		// Get needed variables.
		ModelVertexData * pCurrentVertexData = pCurrentFrame->GetVertexGroup().GetVertexData(0);
		VertexArray * pCurrentVertexArray = pCurrentVertexData->GetVertexArray();
		ModelVertexData * pNextVertexData = pNextFrame->GetVertexGroup().GetVertexData(0);

		//pCurrentFrame->GetVertexGroup().GetVertexData(0)->GetVertexBufferCount( )

		// Add all the next frame's VBOs to the current frame,
		// Only go through the position and normal masks.
		ModelVertexData::eBufferIndex currentIndex = ModelVertexData::NextPositionIndex;
		for (SizeType j = 0; j < pNextVertexData->GetVertexBufferCount() && j < 3; j++)
		{
			//pCurrentVertexData->AddVertexBuffer(pNextVertexData->GetVertexBuffer(j), currentIndex);
			pCurrentVertexArray->AddVertexBuffer(*pNextVertexData->GetVertexBuffer(j), 3, DataType::Float32, currentIndex);

			if (j == 0)
			{
				currentIndex = ModelVertexData::NextTextureCoordIndex;
			}
			else if (j == 1)
			{
				currentIndex = ModelVertexData::NextNormalIndex;
			}
		}

	}

}