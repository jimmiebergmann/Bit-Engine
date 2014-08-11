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

#include <Bit/Graphics/ModelRenderer.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/ShaderProgram.hpp>
#include <Bit/System/MatrixManager.hpp>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	ModelRenderer::ModelRenderer( const GraphicDevice & p_GraphicDevice ) :
		m_GraphicDevice( p_GraphicDevice )
	{
	}

	ModelRenderer::~ModelRenderer( )
	{
		Unload( );
	}

	Bool ModelRenderer::Load( )
	{
		return true;
	}
		
	void ModelRenderer::Unload( )
	{
	}

	void ModelRenderer::Render( Model & p_Model )
	{
		// For now, render the initial pose only.
		RenderInitialPose( p_Model );


		// Check the animation state.

		// Error check the animation state, to make sure there's any animations.

	}

	void ModelRenderer::RenderVertexAnimation( Model & p_Model )
	{
	}

	void ModelRenderer::RenderSkeletalAnimation( Model & p_Model )
	{
	}

	void ModelRenderer::RenderInitialPose( Model & p_Model )
	{
		// Get const free graphic device
		GraphicDevice & graphicDevice = const_cast<GraphicDevice &>( m_GraphicDevice );

		// Get the default shader program.
		ShaderProgram * pShaderProgram = m_GraphicDevice.GetDefaultShaderProgram( GraphicDevice::InitialPoseShader );

		// Error check the shader program.
		if( pShaderProgram == NULL )
		{
			return;
		}

		// Error check the vertex data.
		if( p_Model.GetVertexData( ).GetVertexArray( ) == NULL )
		{
			return;
		}

		// Bind the shader program.
		pShaderProgram->Bind( );
		
		// Get material data



		// Set uniform data.
		pShaderProgram->SetUniformMatrix4x4f( "uProjectionMatrix", MatrixManager::GetProjectionMatrix( ) );
		pShaderProgram->SetUniformMatrix4x4f( "uModelViewMatrix", MatrixManager::GetModelViewMatrix( ) );

		// Set light uniforms
		GraphicDevice::DefaultModelSettings & modelSettings =  graphicDevice.GetDefaultModelSettings( );

		// Set light count
		pShaderProgram->SetUniform1i( "uLightCount", modelSettings.GetActiveLightCount( ) );

		// Set ambient color
		const Vector3f32 & ambColor = modelSettings.GetAmbientLight( );
		pShaderProgram->SetUniform3f( "uAmbientColor", ambColor.x, ambColor.y, ambColor.z );

		// Go throguh all the lights
		for( SizeType i = 0; i < modelSettings.GetActiveLightCount( ); i++ )
		{
			std::stringstream positionStream;
			positionStream << "uLightPositions[" << i << "]";
			std::stringstream colorStream;
			colorStream << "uLightColors[" << i << "]";

			const Vector4f32 & pos = modelSettings.GetLight( i ).GetPosition( );
			const Vector3f32 & color = modelSettings.GetLight( i ).GetColor( );
			pShaderProgram->SetUniform4f( positionStream.str( ).c_str( ), pos.x, pos.y, pos.z, pos.w );
			pShaderProgram->SetUniform3f( colorStream.str( ).c_str( ), color.x, color.y, color.z );
		}


		//pShaderProgram->SetUniform4f( "uColor", 1.0f, 1.0f, 1.0f, 1.0f );
		if( p_Model.GetVertexData( ).GetBitmask( ) & 0x02 )
		{
			pShaderProgram->SetUniform1i( "uUseTexture", 1 );
		}
		else
		{
			pShaderProgram->SetUniform1i( "uUseTexture", 0 );
		}

		if( p_Model.GetVertexData( ).GetBitmask( ) & 0x04 )
		{
			pShaderProgram->SetUniform1i( "uUseNormals", 1 );
		}
		else
		{
			pShaderProgram->SetUniform1i( "uUseNormals", 0 );
		}

		// Render the model.
		p_Model.GetVertexData( ).GetVertexArray( )->Render( PrimitiveMode::Triangles );

		// Unbind the shader program.
		pShaderProgram->Unbind( );
	}


}