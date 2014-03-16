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
#include <Bit/Graphics/OpenGL/OpenGLVertexArrayObject.hpp>
#include <Bit/Graphics/OpenGL/OpenGLVertexBufferObject.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShader.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShaderProgram.hpp>
#include <iostream>

namespace Bit
{
	// Global varaibles
	// Save all the avaible opengl versions.
	static const SizeType g_OpenGLVersionCount = 11;
	static GraphicDevice::Version g_OpenGLVersions[ g_OpenGLVersionCount ] =
	{
		GraphicDevice::Version( 2, 0 ),
		GraphicDevice::Version( 2, 1 ),
		GraphicDevice::Version( 3, 0 ),
		GraphicDevice::Version( 3, 1 ),
		GraphicDevice::Version( 3, 2 ),
		GraphicDevice::Version( 3, 3 ),
		GraphicDevice::Version( 4, 0 ),
		GraphicDevice::Version( 4, 1 ),
		GraphicDevice::Version( 4, 2 ),
		GraphicDevice::Version( 4, 3 ),
		GraphicDevice::Version( 4, 4 )
	};
	
	OpenGLGraphicDeviceWin32::OpenGLGraphicDeviceWin32( ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_DeviceContextHandle( NULL ),
		m_Context( NULL )
	{
	}

	
	OpenGLGraphicDeviceWin32::OpenGLGraphicDeviceWin32( const RenderWindow & p_RenderOutput, const Version & p_Version ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_DeviceContextHandle( NULL ),
		m_Context( NULL )
	{
		Open( p_RenderOutput, p_Version );
	}

	OpenGLGraphicDeviceWin32::~OpenGLGraphicDeviceWin32( )
	{
		Close( );
	}

	bool OpenGLGraphicDeviceWin32::Open( const RenderWindow & p_RenderOutput, const Version & p_Version )
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
		if( contextVersion == GraphicDevice::DefaultVersion )
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
		if( OpenGL::BindOpenGLExtensions( contextVersion.Major, contextVersion.Minor ) != true )
		{
			//bitTrace( "[GraphicDeviceWin32::Open] Can not bind the OpenGL extensions.\n" );
			std::cout << "[OpenGL::BindOpenGLExtensions] Binding opengl extensions failed.\n";
			return false;
		}

		// Set the default viewport to the window's size
		SetViewport( Vector2u32( 0, 0 ), p_RenderOutput.GetVideoMode( ).GetSize( ) );

		// Set the some member varaibles
		m_Open = true;
		m_Version = contextVersion;
		m_DeviceContextHandle = p_RenderOutput.GetDeviceContextHandle( );

		// Return true at success.
		return true;
	}

	void OpenGLGraphicDeviceWin32::Close( )
	{
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
		}

		m_Open = false;
		m_Version.Major = 0;
		m_Version.Minor = 0;
		m_DeviceContextHandle = NULL;
		m_Context = NULL;
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

	VertexArrayObject * OpenGLGraphicDeviceWin32::CreateVertexArrayObject( ) const
	{
		return new OpenGLVertexArrayObject;
	}

	VertexBufferObject * OpenGLGraphicDeviceWin32::CreateVertexBufferObject( ) const
	{
		return new OpenGLVertexBufferObject;
	}

	Shader * OpenGLGraphicDeviceWin32::CreateShader( ShaderType::eType p_Type ) const
	{
		return new OpenGLShader( p_Type );
	}

	ShaderProgram * OpenGLGraphicDeviceWin32::CreateShaderProgram( ) const
	{
		return new OpenGLShaderProgram;
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

	GraphicDevice::Version OpenGLGraphicDeviceWin32::GetVersion( ) const
	{
		return m_Version;
	}

	bool OpenGLGraphicDeviceWin32::OpenVersion( const RenderWindow & p_RenderOutput,
												const GraphicDevice::Version & p_Version )
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
			WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>( p_Version.Major ),
			WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>( p_Version.Minor ),
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

		glDisable( GL_DEPTH_TEST );

		return true;
	}

	bool OpenGLGraphicDeviceWin32::OpenBestVersion( const RenderWindow & p_RenderOutput,
													Version & p_Version  )
	{
		// Loop backwards( the highest version first )
		for( SizeType i = g_OpenGLVersionCount - 1; i >= 0; i-- )
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

}

#endif