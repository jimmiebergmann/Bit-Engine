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

#include <Bit/Graphics/OpenGL/PostProcessingBloomOpenGL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
	PostProcessingBloomOpenGL::PostProcessingBloomOpenGL( ShaderProgram * p_pShaderProgram, Shader * p_pVertexShader, Shader * p_pFragmentShader,
			VertexObject * p_pVertexObject, Texture * p_pTexture ) :
		m_Loaded( BIT_FALSE ),
		m_pVertexObject( p_pVertexObject ),
		m_pShaderProgram( p_pShaderProgram ),
		m_pVertexShader( p_pVertexShader ),
		m_pFragmentShader( p_pFragmentShader ),
		m_pTexture( p_pTexture )
	{
	}

	PostProcessingBloomOpenGL::~PostProcessingBloomOpenGL()
	{
		// Clear up everyhing
		if( m_pShaderProgram )
		{
			delete m_pShaderProgram;
			m_pShaderProgram = BIT_NULL;
		}
		if( m_pVertexShader )
		{
			delete m_pVertexShader;
			m_pVertexShader = BIT_NULL;
		}
		if( m_pFragmentShader )
		{
			delete m_pFragmentShader;
			m_pFragmentShader = BIT_NULL;
		}

		m_Loaded = BIT_FALSE;
	}

	// Public general functions
	BIT_UINT32 PostProcessingBloomOpenGL::Load( BIT_FLOAT32 p_Power, BIT_UINT32 p_BlurSize, BIT_FLOAT32 p_PixelSize )
	{
		// Make sure we didn't pass a NULL pointer.
		if( !m_pVertexObject || !m_pTexture || !m_pShaderProgram ||
			!m_pVertexShader || !m_pFragmentShader )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Passed NULL pointer to the constructor." );
			return BIT_ERROR;
		}

		// Load the GLSL shader

		// Shader sources
		static const std::string VertexSource =
			"#version 330 \n"
			"precision highp float; \n"

			"in vec3 Position; \n"
			"in vec2 Texture; \n"
			"out vec2 out_Texture; \n"

			"uniform mat4 ProjectionMatrix; \n"

			"void main(void) \n"
			"{ \n"
			"	out_Texture = Texture; \n"
			"	gl_Position = ProjectionMatrix * vec4( Position, 1.0 ); \n"
			"} \n";

		static const std::string FragmentSource =
			"#version 330 \n"
			"precision highp float; \n"

			"in vec2 out_Texture; \n"
			"out vec4 out_Color; \n"

			"uniform sampler2D ColorTexture; \n"
			"uniform float Power; \n"
			"uniform int BlurSize; \n"
			"uniform int BlurSizeTotal; \n"
			"uniform vec2 PixelSize; \n"

			"void main(void) \n"
			"{ \n"

			"	vec4 color = texture2D( ColorTexture, out_Texture ); \n"
			"	vec4 sum = vec4( 0 ); \n"
			
			"	for( int x = -BlurSize; x <= BlurSize; x++ ) \n"
			"	{ \n"
			"		for( int y = -BlurSize; y <= BlurSize; y++ ) \n"
			"		{ \n"
			"			sum += texture2D( ColorTexture, out_Texture + vec2( x * PixelSize.x, y * PixelSize.y ) ); \n"
			"		} \n"
			"	} \n"

			"	sum /= BlurSizeTotal; \n"

			"	out_Color = mix( color, color + sum, Power ); \n"
			"	out_Color.a = 1.0; \n"
			"} \n";

		// Set the sources
		m_pVertexShader->SetSource( VertexSource );
		m_pFragmentShader->SetSource( FragmentSource );

		// Compile the shaders
		if( m_pVertexShader->Compile( ) != BIT_OK )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Can not compile the vertex shader\n" );
			return BIT_ERROR;
		}
		if( m_pFragmentShader->Compile( ) != BIT_OK )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Can not compile the fragment shader\n" );
			return BIT_ERROR;
		}

		// Attach the shaders
		if( m_pShaderProgram->AttachShaders( m_pVertexShader ) != BIT_OK )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Can not attach the vertex shader\n" );
			return BIT_ERROR;
		}
		if( m_pShaderProgram->AttachShaders( m_pFragmentShader ) != BIT_OK )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Can not attach the fragment shader\n" );
			return BIT_ERROR;
		}

		// Set attribute locations
		m_pShaderProgram->SetAttributeLocation( "Position", 0 );
		m_pShaderProgram->SetAttributeLocation( "Texture", 1 );
		

		// Link the shaders
		if( m_pShaderProgram->Link( ) != BIT_OK )
		{
			bitTrace( "[PostProcessingBloomOpenGL::Load] Can not link the shader program\n" );
			return BIT_ERROR;
		}


		// Create an orthographic matrix
		Matrix4x4 ProjectionMatrix;
		ProjectionMatrix.Orthographic( 0.0f, m_pTexture->GetSize( ).x, 0.0, m_pTexture->GetSize( ).y, -1.0, 1.0 );

		// Set uniforms
		m_pShaderProgram->Bind( );
		m_pShaderProgram->SetUniformMatrix4x4f( "ProjectionMatrix", ProjectionMatrix );
		m_pShaderProgram->SetUniform1i( "ColorTexture", 0 );
		m_pShaderProgram->SetUniform1f( "Power", p_Power );
		m_pShaderProgram->SetUniform1i( "BlurSize", p_BlurSize );
		m_pShaderProgram->SetUniform1i( "BlurSizeTotal", ( p_BlurSize + 1 ) * ( p_BlurSize + 1 ) );
		m_pShaderProgram->SetUniform2f( "PixelSize", 1.0f / (BIT_FLOAT32)( m_pTexture->GetSize( ).x ) * p_PixelSize,
			 1.0f / (BIT_FLOAT32)( m_pTexture->GetSize( ).y ) * p_PixelSize );
		m_pShaderProgram->Unbind( );
		
		m_Loaded = BIT_TRUE;
		return BIT_OK;
	}

	void PostProcessingBloomOpenGL::Process( )
	{
		// Make sure everything is loaded.
		if( !m_Loaded )
		{
			return;
		}

		// Bind the shader program and the diffuse texture
		m_pShaderProgram->Bind( );
		m_pTexture->Bind( 0 );

		// Render the fullscreen quad
		m_pVertexObject->Render( VertexObject::RenderMode_Triangles );

		// Unbind everything
		m_pTexture->Unbind( );
		m_pShaderProgram->Unbind( );
	}

	// Public set functions
	void PostProcessingBloomOpenGL::SetTexture( Texture * p_pTexture )
	{
		if( p_pTexture != BIT_NULL )
		{
			m_pTexture = p_pTexture;
		}
	}

	// Public get functions
	Texture * PostProcessingBloomOpenGL::GetTexture( ) const
	{
		return m_pTexture;
	}

}