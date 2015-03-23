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

#include <Bit/Window/SimpleRenderWindow.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/ShaderProgram.hpp>
#include <Bit/Graphics/Shader.hpp>
#include <Bit/Graphics/OpenGL/OpenGLGraphicDevice.hpp>
#include <Bit/System/MatrixManager.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	SimpleRenderWindow::SimpleRenderWindow( ) :
		m_RenderWindow( ),
		m_pGraphicDevice( NULL ),
		m_pShaderProgram( NULL ),
		m_pVertexShader( NULL ),
		m_pFragmentShader( NULL )
	{
	}

	SimpleRenderWindow::SimpleRenderWindow(	const VideoMode & p_VideoMode,
											const std::string & p_Title,
											const Uint32 p_Style ) :
		m_RenderWindow( ),
		m_pGraphicDevice( NULL ),
		m_pShaderProgram( NULL ),
		m_pVertexShader( NULL ),
		m_pFragmentShader( NULL )
	{
		Open( p_VideoMode, p_Title, p_Style );
	}

	SimpleRenderWindow::~SimpleRenderWindow( )
	{
		Close( );
	}

	Bool SimpleRenderWindow::Open(	const VideoMode & p_VideoMode,
									const std::string & p_Title,
									const Uint32 p_Style )
	{
		// Close already open windows or graphic devices
		if( m_RenderWindow.IsOpen( ) || m_pGraphicDevice )
		{
			if( m_pGraphicDevice )
			{
				delete m_pGraphicDevice;
				m_pGraphicDevice = NULL;
			}

			m_RenderWindow.Close( );
		}

		// Open the window
		if( m_RenderWindow.Open( p_VideoMode, p_Title, p_Style ) == false )
		{
			return false;
		}

		// Create the graphic device
		m_pGraphicDevice = new OpenGLGraphicDevice;
		if( m_pGraphicDevice->Open( m_RenderWindow ) == false )
		{
			delete m_pGraphicDevice;
			m_pGraphicDevice = NULL;
			return false;
		}

		// Initialize the graphic device
		m_pGraphicDevice->SetViewport( Vector2u32( 0, 0 ), p_VideoMode.GetSize( ) );
		m_pGraphicDevice->SetClearColor( 0, 0, 0, 255 );

		// Create shaders
		m_pShaderProgram = m_pGraphicDevice->CreateShaderProgram( );
		m_pVertexShader = m_pGraphicDevice->CreateShader( ShaderType::Vertex );
		m_pFragmentShader = m_pGraphicDevice->CreateShader( ShaderType::Fragment );

		if( !m_pShaderProgram || !m_pVertexShader || !m_pFragmentShader )
		{
			return false;
		}

		// Add shader sources
		static const std::string vertexSource =
			"#version 330\n"

			// Matrix uniforms
			"uniform mat4 uProjectionMatrix;\n"
			"uniform mat4 uModelViewMatrix;\n"
			"uniform vec3 uPosition;\n"
			"uniform vec3 uSize;\n"

			// In values
			"in vec3 position;\n"

			// Main function
			"void main( )\n"
			"{\n"

				// Set the vertex position
			"	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4( position * uSize + uPosition, 1.0 );\n"

			"}\n";

		if( m_pVertexShader->CompileFromMemory( vertexSource ) == false )
		{
			return false;
		}

		static const std::string fragmentSource =
			"#version 330\n"

			// Uniforms
			"uniform vec4 uColor;\n"

			// Out values
			"out vec4 outColor;\n"

			// Main function
			"void main( )\n"
			"{ \n"

				// Create final color
			"	outColor = uColor;\n"
	
			"}\n";
		
		if( m_pFragmentShader->CompileFromMemory( fragmentSource ) == false )
		{
			return false;
		}

		
		// Create a projection matrix

		MatrixManager::SetCurrentStack( MatrixManager::Projection );
		MatrixManager::LoadOrthographic(	0.0f,
											static_cast<Bit::Float32>( m_RenderWindow.GetVideoMode( ).GetSize( ).x ),
											0.0f,
											static_cast<Bit::Float32>( m_RenderWindow.GetVideoMode( ).GetSize( ).y ),
											-1.0f,
											1.0f );

		MatrixManager::SetCurrentStack( MatrixManager::ModelView );
		MatrixManager::LoadIdentity( );

		// Attach shaders and set up everything
		m_pShaderProgram->AttachShader( *m_pVertexShader );
		m_pShaderProgram->AttachShader( *m_pFragmentShader );
		m_pShaderProgram->SetAttributeLocation( "position", 0 );
		m_pShaderProgram->Link( );
		m_pShaderProgram->Bind( );
		m_pShaderProgram->SetUniformMatrix4x4f( "uProjectionMatrix", MatrixManager::GetProjectionMatrix( ) );
		m_pShaderProgram->SetUniformMatrix4x4f( "uModelViewMatrix",  MatrixManager::GetModelViewMatrix( ) );
		m_pShaderProgram->SetUniform3f( "uPosition", 0.0f, 0.0f, 0.0f );
		m_pShaderProgram->SetUniform3f( "uSize", 1.0f, 1.0f, 1.0f );
		m_pShaderProgram->SetUniform4f( "uColor", 1.0f, 1.0f, 1.0f, 1.0f );
		m_pShaderProgram->Unbind( );


		// Succeeded
		return true;
	}

	void SimpleRenderWindow::Close( )
	{
		if( m_pVertexShader )
		{
			delete m_pVertexShader;
			m_pVertexShader = NULL;
		}

		if( m_pFragmentShader )
		{
			delete m_pFragmentShader;
			m_pFragmentShader = NULL;
		}

		if( m_pShaderProgram )
		{
			delete m_pShaderProgram;
			m_pShaderProgram = NULL;
		}

		if( m_pGraphicDevice )
		{
			delete m_pGraphicDevice;
			m_pGraphicDevice = NULL;
		}

		m_RenderWindow.Close( );
	}

	void SimpleRenderWindow::Update( )
	{
		m_RenderWindow.Update( );
	}

	void SimpleRenderWindow::SetClearColor( Uint8 p_Red, Uint8 p_Green, Uint8 p_Blue, Uint8 p_Alpha )
	{
		if( m_pGraphicDevice )
		{
			m_pGraphicDevice->SetClearColor( p_Red, p_Green, p_Blue, p_Alpha );
		}
	}

	void SimpleRenderWindow::Draw( Shape * p_pShape, const PrimitiveMode::eMode p_PrimitiveMode )
	{
		// Error check the pointer
		if( p_pShape == NULL )
		{
			return;
		}

		// Get and error check the vertex array
		VertexArray * pVertexArray = p_pShape->GetVertexArray( );
		if( pVertexArray == NULL )
		{
			return;
		}

		// Bind shader program
		m_pShaderProgram->Bind( );

		// Set uniforms
		m_pShaderProgram->SetUniformMatrix4x4f( "uModelViewMatrix",  MatrixManager::GetModelViewMatrix( ) );
		m_pShaderProgram->SetUniform3f( "uPosition", p_pShape->GetPosition( ).x, p_pShape->GetPosition( ).y, 0.0f );
		m_pShaderProgram->SetUniform3f( "uSize", p_pShape->GetSize( ).x, p_pShape->GetSize( ).y, 0.0f );
		m_pShaderProgram->SetUniform4f( "uColor", 1.0f, 1.0f, 1.0f, 1.0f );

		// Render the array
		pVertexArray->Render( p_PrimitiveMode );
		
		// Unbind shader program
		m_pShaderProgram->Unbind( );
	}

	Shape * SimpleRenderWindow::CreateQuadShape( )
	{
		Shape * pShape = new Shape;
		if( pShape->LoadQuad(  m_pGraphicDevice ) == false )
		{
			delete pShape;
			return NULL;
		}

		return pShape;
	}

	Shape * SimpleRenderWindow::CreateCircleShape( const Uint32 p_Parts )
	{
		Shape * pShape = new Shape;
		if( pShape->LoadCircle( m_pGraphicDevice, p_Parts ) == false )
		{
			delete pShape;
			return NULL;
		}

		return pShape;
	}

	void SimpleRenderWindow::DestroyShape( Shape * p_pShape )
	{
		if( p_pShape == NULL )
		{
			return;
		}

		delete p_pShape;
	}

	void SimpleRenderWindow::Present( )
	{
		if( m_pGraphicDevice == NULL )
		{
			return;
		}

		m_pGraphicDevice->Present( );
		m_pGraphicDevice->ClearColor( );
		m_pGraphicDevice->ClearDepth( );
	}

	Bool SimpleRenderWindow::PollEvent( Event & p_Event )
	{
		return m_RenderWindow.PollEvent( p_Event );
	}

	void SimpleRenderWindow::SetTitle( const std::string & p_Title )
	{
		m_RenderWindow.SetTitle( p_Title );
	}

	Bool SimpleRenderWindow::IsOpen( ) const
	{
		return m_RenderWindow.IsOpen( ) && m_pGraphicDevice;
	}

	Bool SimpleRenderWindow::IsFocused( ) const
	{
		return m_RenderWindow.IsFocused( );
	}

	const VideoMode & SimpleRenderWindow::GetVideoMode( ) const
	{
		return m_RenderWindow.GetVideoMode( );
	}

	const std::string & SimpleRenderWindow::GetTitle( ) const
	{
		return m_RenderWindow.GetTitle( );
	}

}