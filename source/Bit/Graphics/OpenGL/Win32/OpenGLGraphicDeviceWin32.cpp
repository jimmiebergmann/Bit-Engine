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

#include <Bit/Graphics/OpenGL/Win32/OpenGLGraphicDeviceWin32.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <Bit/Graphics/OpenGL/OpenGLRenderbuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLVertexArray.hpp>
#include <Bit/Graphics/OpenGL/OpenGLVertexBuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShader.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShaderProgram.hpp>
#include <Bit/Graphics/OpenGL/OpenGLTexture.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/ModelRenderer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLModelRenderer.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Static member variables
	OpenGLFramebuffer OpenGLGraphicDeviceWin32::s_DefaultFramebuffer;

	// Global varaibles
	// Save all the avaible opengl versions.
	static const SizeType g_OpenGLVersionCount = 11;
	static Version g_OpenGLVersions[ g_OpenGLVersionCount ] =
	{
		Version( 2, 0 ),
		Version( 2, 1 ),
		Version( 3, 0 ),
		Version( 3, 1 ),
		Version( 3, 2 ),
		Version( 3, 3 ),
		Version( 4, 0 ),
		Version( 4, 1 ),
		Version( 4, 2 ),
		Version( 4, 3 ),
		Version( 4, 4 )
	};

	static GLenum g_OpenGLCulling[ 2 ] =
	{
		/*FrontFace*/	GL_FRONT,
		/*BackFace*/	GL_BACK
	};
	
	OpenGLGraphicDeviceWin32::OpenGLGraphicDeviceWin32( ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_DeviceContextHandle( NULL ),
		m_Context( NULL )
	{
		for( SizeType i = 0; i < 3; i++ )
		{
			m_pDefaultShaderPrograms[ i ] = NULL;
			m_pDefaultModelVertexShaders[ i ] = NULL;
		}
		m_pDefaultModelFragmentShader = NULL;
	}

	OpenGLGraphicDeviceWin32::OpenGLGraphicDeviceWin32( const RenderWindow & p_RenderOutput,
														const Version & p_Version ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_DeviceContextHandle( NULL ),
		m_Context( NULL )
	{
		for( SizeType i = 0; i < 3; i++ )
		{
			m_pDefaultShaderPrograms[ i ] = NULL;
			m_pDefaultModelVertexShaders[ i ] = NULL;
		}
		m_pDefaultModelFragmentShader = NULL;

		Open( p_RenderOutput, p_Version );
	}

	OpenGLGraphicDeviceWin32::~OpenGLGraphicDeviceWin32( )
	{
		Close( );
	}

	Bool OpenGLGraphicDeviceWin32::Open(	const RenderWindow & p_RenderOutput,
											const Version & p_Version )
	{
		// Make sure that the GD is not already open.
		if( m_Open == true )
		{
			std::cout << "[OpenGL::BindOpenGLExtensions] The graphic device is already loaded.\n";
			return false;
		}

		// Make sure that the render output is loaded
		if( p_RenderOutput.IsOpen( ) == false )
		{
			std::cout << "[OpenGL::BindOpenGLExtensions] The render output is not loaded.\n";
			return false;
		}

		// Store the version of the opengl context that we want to create.
		// This function will create the best as possible if the version is set to 0.0
		Version contextVersion( p_Version );

		// Try to load the best context as possible if the version is set to the default version
		if( contextVersion == Version::Default )
		{
			if( OpenBestVersion( p_RenderOutput, contextVersion ) != true )
			{
				return false;
			}
		}
		// Try to load the requested context, if it fails, create the best context as possible.
		else
		{
			if( OpenVersion( p_RenderOutput, contextVersion ) == false )
			{
				if( OpenBestVersion( p_RenderOutput, contextVersion ) != true )
				{
					return false;
				}
			}
		}

		// Bind the OpenGL extensions
		if( OpenGL::BindOpenGLExtensions( contextVersion.GetMajor( ), contextVersion.GetMinor( ) ) != true )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::Open] Binding opengl extensions failed.\n";
			return false;
		}

		// Load default shaders
		if( LoadDefaultShaders( ) == false )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::Open] Warning: Failed to load default shaders.\n";
		}

		// Set the default viewport to the window's size
		SetViewport( Vector2u32( 0, 0 ), p_RenderOutput.GetVideoMode( ).GetSize( ) );

		// Set the some member varaibles
		m_Open = true;
		m_Version = contextVersion;
		m_DeviceContextHandle = p_RenderOutput.GetDeviceContextHandle( );

		// Set default settings.
		DisableDepthTest( );
		EnableTexture( );
		DisableFaceCulling( );
		DisableSmoothLines( );

		// Return true at success.
		return true;
	}

	void OpenGLGraphicDeviceWin32::Close( )
	{
		// Unload default shaders.
		UnloadDefaultShaders( );

		// Delete the context
		if( m_Context )
		{
			// Release the context from the current thread
			if( !wglMakeCurrent( NULL, NULL ) )
			{
				std::cout << "[OpenGLGraphicDeviceWin32::Close] Can not release the context.\n";
			}

			// Delete the context
			if( !wglDeleteContext( m_Context ))
			{
				std::cout << "[OpenGLGraphicDeviceWin32::Close] Can not delete the context.\n";
			}

			m_Context = NULL;
		}

		m_Open = false;
		m_Version = Version( 0, 0, 0 );
		m_DeviceContextHandle = NULL;
	}

	void OpenGLGraphicDeviceWin32::MakeCurrent( )
	{
		wglMakeCurrent( m_DeviceContextHandle, m_Context );
	}

	void OpenGLGraphicDeviceWin32::Present( )
	{
		if( m_Open == false )
		{
			return;
		}

		SwapBuffers( m_DeviceContextHandle );
	}

	void OpenGLGraphicDeviceWin32::ClearColor( )
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void OpenGLGraphicDeviceWin32::ClearDepth( )
	{
		glClear( GL_DEPTH_BUFFER_BIT );
	}

	void OpenGLGraphicDeviceWin32::EnableDepthTest( )
	{
		glEnable( GL_DEPTH_TEST );
	}

	void OpenGLGraphicDeviceWin32::EnableTexture( )
	{
		glEnable( GL_TEXTURE_2D );
	}

	void OpenGLGraphicDeviceWin32::EnableMultisampling( )
	{
		glEnable( GL_MULTISAMPLE );
	}

	void OpenGLGraphicDeviceWin32::EnableFaceCulling( eCulling p_FaceCulling )
	{
		glEnable( GL_CULL_FACE );
		glCullFace( g_OpenGLCulling[ p_FaceCulling ] );
	}

	void OpenGLGraphicDeviceWin32::EnableSmoothLines( )
	{
		glEnable( GL_LINE_SMOOTH );
	}

	void OpenGLGraphicDeviceWin32::DisableDepthTest( )
	{
		glDisable( GL_DEPTH_TEST );
	}

	void OpenGLGraphicDeviceWin32::DisableTexture( )
	{
		glDisable( GL_TEXTURE_2D );
	}

	void OpenGLGraphicDeviceWin32::DisableFaceCulling( )
	{
		glDisable( GL_CULL_FACE );
	}

	void OpenGLGraphicDeviceWin32::DisableSmoothLines( )
	{
		glDisable( GL_LINE_SMOOTH );
	}

	Framebuffer * OpenGLGraphicDeviceWin32::CreateFramebuffer( ) const
	{
		return new OpenGLFramebuffer;
	}

	
	Renderbuffer * OpenGLGraphicDeviceWin32::CreateRenderbuffer( ) const
	{
		return new OpenGLRenderbuffer;
	}

	VertexArray * OpenGLGraphicDeviceWin32::CreateVertexArray( ) const
	{
		return new OpenGLVertexArray;
	}

	VertexBuffer * OpenGLGraphicDeviceWin32::CreateVertexBuffer( ) const
	{
		return new OpenGLVertexBuffer;
	}

	Shader * OpenGLGraphicDeviceWin32::CreateShader( ShaderType::eType p_Type ) const
	{
		return new OpenGLShader( p_Type );
	}

	ShaderProgram * OpenGLGraphicDeviceWin32::CreateShaderProgram( ) const
	{
		return new OpenGLShaderProgram;
	}

	Texture * OpenGLGraphicDeviceWin32::CreateTexture( ) const
	{
		return new OpenGLTexture;
	}

	Model * OpenGLGraphicDeviceWin32::CreateModel( ) const
	{
		return new Model( *this );
	}

	ModelRenderer * OpenGLGraphicDeviceWin32::CreateModelRenderer( ) const
	{
		return new ModelRenderer( *this );
	}
	
	void OpenGLGraphicDeviceWin32::SetViewport( const Vector2u32 & p_Position, const Vector2u32 & p_Size )
	{
		glViewport( p_Position.x, p_Position.y, p_Size.x, p_Size.y );
	}

	void OpenGLGraphicDeviceWin32::SetClearColor(	const Uint8 p_Red, const Uint8 p_Green,
													const Uint8 p_Blue, const Uint8 p_Alpha )
	{
		glClearColor(	static_cast<GLclampf>( p_Red ) / 255.0f,
						static_cast<GLclampf>( p_Green ) / 255.0f,
						static_cast<GLclampf>( p_Blue ) / 255.0f,
						static_cast<GLclampf>( p_Alpha ) / 255.0f );
	}

	Bool OpenGLGraphicDeviceWin32::IsOpen( ) const
	{
		return m_Open;
	}

	Version OpenGLGraphicDeviceWin32::GetVersion( ) const
	{
		return m_Version;
	}

	const Framebuffer & OpenGLGraphicDeviceWin32::GetDefaultFramebuffer( ) const
	{
		return s_DefaultFramebuffer;
	}

	ShaderProgram * OpenGLGraphicDeviceWin32::GetDefaultShaderProgram( const eDefaultShaders p_DefaultShader ) const
	{
		return m_pDefaultShaderPrograms[ static_cast<SizeType>( p_DefaultShader ) ];
	}

	Bool OpenGLGraphicDeviceWin32::OpenVersion( const RenderWindow & p_RenderOutput,
												const Version & p_Version )
	{
		// Filling the pixel fromat structure.
		static PIXELFORMATDESCRIPTOR PFD = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16, //DepthBits,
			8, //StencilBits,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		// Choose and set the pixel format
		GLuint PixelFormat;

		if((PixelFormat = ChoosePixelFormat( p_RenderOutput.GetDeviceContextHandle( ), &PFD ) ) == 0)
		{
			std::cout<< "[OpenGLGraphicDeviceWin32::Open] Can not choose pixel format.\n";
			return false;
		}
		if( ( SetPixelFormat( p_RenderOutput.GetDeviceContextHandle( ), PixelFormat, &PFD ) ) == false )
		{
			std::cout<<  "[OpenGLGraphicDeviceWin32::Open] Can not set pixel format.\n";
			return false;
		}

		// Create a temporary regual context.
		// We need this context to create any other context.
		HGLRC temporaryContext = wglCreateContext( p_RenderOutput.GetDeviceContextHandle( ) );

		if( temporaryContext == NULL )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::Open] Can not create a regular OpenGL context.\n";
			return false;
		}

		// Make the temporary context to the current one
		wglMakeCurrent( NULL, NULL );
		wglMakeCurrent( p_RenderOutput.GetDeviceContextHandle( ), temporaryContext );

		// Attributes for the OGL 3.3 context
		int attribs[ ] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>( p_Version.GetMajor( ) ),
			WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>( p_Version.GetMinor( ) ),
			0
		};

		// We need the proc address for the function
		// we are going to use for OGL 3.3 context creation.
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
		if((wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress( "wglCreateContextAttribsARB" )) == NULL )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::Open] Can not get the function for creating the context.\n";
			return false;
		}

		// Create the context
		if( ( m_Context = wglCreateContextAttribsARB( p_RenderOutput.GetDeviceContextHandle( ), 0, attribs) ) != NULL )
		{
			// Delete the old temporary context
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( temporaryContext );

			// Make the new OpenGL context to the current one.
			wglMakeCurrent( p_RenderOutput.GetDeviceContextHandle( ), m_Context );
		}

		return true;
	}

	Bool OpenGLGraphicDeviceWin32::OpenBestVersion( const RenderWindow & p_RenderOutput,
													Version & p_Version  )
	{
		// Loop backwards( the highest version first )
		for( Int32 i = g_OpenGLVersionCount - 1; i >= 0; i-- )
		{
			//  Try to open the current version, return true if it succeed.
			if( OpenVersion( p_RenderOutput, g_OpenGLVersions[ i ] ) == true )
			{
				p_Version = g_OpenGLVersions[ i ];
				return true;
			}
		}

		// Failed to load any of the versions.
		return false;
	}

	Bool OpenGLGraphicDeviceWin32::LoadDefaultShaders( )
	{
		m_pDefaultModelVertexShaders[ InitialPoseShader ] = CreateShader( Bit::ShaderType::Vertex );
		m_pDefaultModelFragmentShader = CreateShader( Bit::ShaderType::Fragment );
		m_pDefaultShaderPrograms[ InitialPoseShader ] = CreateShaderProgram( );

		static const std::string vertexSource =
			"#version 330\n"

			"uniform mat4 uProjectionMatrix;\n"
			"uniform mat4 uModelViewMatrix;\n"
			"uniform int uUseTexture;\n"
			"uniform int uUseNormals;\n"

			"in vec3 position;\n"
			"in vec3 normal;\n"
			"in vec2 textureCoord;\n"

			"out vec2 vTextureCoord;\n"
			"out vec3 vNormal;\n"

			"void main( )\n"
			"{\n"
			"	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4( position, 1.0 );\n"
			"	vTextureCoord = textureCoord;\n"
			"	vNormal = normal;\n"
			"}\n";

		if( m_pDefaultModelVertexShaders[ InitialPoseShader ]->CompileFromMemory( vertexSource ) == false )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::LoadDefaultShaders] Failed to compile InitialPoseShader vertex shader.\n"; 
			return false;
		}

		static const std::string fragmentSource =
			"#version 330\n"

			"uniform vec4 uColor;\n"
			"uniform int uUseTexture;\n"
			"uniform int uUseNormals;\n"
			"uniform sampler2D colorTexture;\n"

			"in vec2 vTextureCoord;\n"
			"in vec3 vNormal;\n"

			"out vec4 outColor;\n"

			"void main( )\n"
			"{ \n"
			"	vec4 finalColor = uColor;\n"
			"	if( uUseTexture == 1 ) {\n"
			"		finalColor *= texture2D( colorTexture, vTextureCoord );\n"
			"	}\n"
			"if( uUseNormals == 1 ) {\n"
			"		finalColor *= max( min( dot( vNormal, vec3( 1.0, 0.0, 0.0 ) ) , 1.0 ), 0.0 );\n"
			"	}\n"
			"	outColor = finalColor;\n"
			"}\n";
		
		if( m_pDefaultModelFragmentShader->CompileFromMemory( fragmentSource ) == false )
		{
			std::cout << "[OpenGLGraphicDeviceWin32::LoadDefaultShaders] Failed to compile model fragnebt shader.\n"; 
			return false;
		}

		
		m_pDefaultShaderPrograms[ InitialPoseShader ]->AttachShader( *m_pDefaultModelVertexShaders[ InitialPoseShader ] );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->AttachShader( *m_pDefaultModelFragmentShader );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetAttributeLocation( "position", 0 );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetAttributeLocation( "textureCoord", 1 );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetAttributeLocation( "normal", 2 );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->Link( );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->Bind( );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetUniform4f( "uColor", 0.0f, 1.0f, 0.0f, 1.0f );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetUniform1i( "uUseTexture", 0 );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->SetUniform1i( "uUseNormals", 0 );
		m_pDefaultShaderPrograms[ InitialPoseShader ]->Unbind( );

		return true;
	}

	Bool OpenGLGraphicDeviceWin32::UnloadDefaultShaders( )
	{
		for( SizeType i = 0; i < 3; i++ )
		{
			if( m_pDefaultShaderPrograms[ i ] )
			{
				delete m_pDefaultShaderPrograms[ i ];
				m_pDefaultShaderPrograms[ i ] = NULL;
			}

			if( m_pDefaultModelVertexShaders[ i ] )
			{
				delete m_pDefaultModelVertexShaders[ i ];
				m_pDefaultModelVertexShaders[ i ] = NULL;
			}
		}

		if( m_pDefaultModelFragmentShader )
		{
			delete m_pDefaultModelFragmentShader;
			m_pDefaultModelFragmentShader = NULL;
		}


		return true;
	}

}

#endif
