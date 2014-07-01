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

#include <Bit/Graphics/OpenGL/Linux/OpenGLGraphicDeviceLinux.hpp>
#ifdef BIT_PLATFORM_LINUX

#include <Bit/Graphics/OpenGL/OpenGL.hpp>

#include <Bit/Graphics/OpenGL/OpenGLRenderbuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLVertexArray.hpp>
#include <Bit/Graphics/OpenGL/OpenGLVertexBuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShader.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShaderProgram.hpp>
#include <Bit/Graphics/OpenGL/OpenGLTexture.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/OpenGL/OpenGLModelRenderer.hpp>

#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

    // Global variables and functions
    static Bit::Bool g_XLibErrorCatched = false;

    static int XLibErrorCatcher( ::Display * d, ::XErrorEvent * e)
    {
       /* std::cout << "My X errors:" << std::endl;
        std::cout << "  Type: " << (int)e->type << std::endl;
        std::cout << "  Error code: " << (int)e->error_code << std::endl;
        std::cout << "  Request code: " << (int)e->request_code << std::endl;
        std::cout << "  Minor code: " << (int)e->minor_code << std::endl;
        std::cout << "  Serial: " << (int)e->serial << std::endl;*/
        g_XLibErrorCatched = true;
        return 0;
    }


	// Static member variables
	OpenGLFramebuffer OpenGLGraphicDeviceLinux::s_DefaultFramebuffer;

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

	OpenGLGraphicDeviceLinux::OpenGLGraphicDeviceLinux( ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_pRenderOutput( NULL ),
		m_DeviceContext( NULL )
	{
	}


	OpenGLGraphicDeviceLinux::OpenGLGraphicDeviceLinux( const RenderWindow & p_RenderOutput,
														const Version & p_Version ) :
		m_Open( false ),
		m_Version( 0, 0 ),
		m_pRenderOutput( NULL ),
		m_DeviceContext( NULL )
	{
		Open( p_RenderOutput, p_Version );
	}

	OpenGLGraphicDeviceLinux::~OpenGLGraphicDeviceLinux( )
	{
		Close( );
	}

	Bool OpenGLGraphicDeviceLinux::Open(	const RenderWindow & p_RenderOutput,
											const Version & p_Version )
	{
		// Make sure that the GD is not already open.
		if( m_Open == true )
		{
			std::cout << "[OpenGLGraphicDeviceLinux::BindOpenGLExtensions] The graphic device is already loaded.\n";
			return false;
		}

		// Make sure that the render output is loaded
		if( p_RenderOutput.IsOpen( ) == false )
		{
			std::cout << "[OpenGLGraphicDeviceLinux::BindOpenGLExtensions] The render output is not loaded.\n";
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
			//bitTrace( "[GraphicDeviceWin32::Open] Can not bind the OpenGL extensions.\n" );
			std::cout << "[OpenGL::BindOpenGLExtensions] Binding opengl extensions failed.\n";
			return false;
		}

		// Set the default viewport to the window's size
		SetViewport( Vector2u32( 0, 0 ), p_RenderOutput.GetVideoMode( ).GetSize( ) );

		// Set the some member varaibles
		m_Open = true;
		m_Version = contextVersion;
        m_pRenderOutput = const_cast<RenderWindow*>( &p_RenderOutput );

		// Set default settings.
		DisableDepthTest( );
		EnableTexture( );
		DisableFaceCulling( );
		DisableSmoothLines( );

		// Return true at success.
		return true;
	}

	void OpenGLGraphicDeviceLinux::Close( )
	{
        // Destroy the OpenGL context
        if( m_pRenderOutput && m_DeviceContext )
        {
            // Release the context from this thread
            if( !glXMakeCurrent( XOpenDisplay( NULL ), 0, 0 ) )
            {
                std::cout << "[OpenGLGraphicDeviceLinux::Close] Can release the OpenGL context.\n";
                return;
            }

            glXDestroyContext( XOpenDisplay( NULL ), m_DeviceContext );
            m_DeviceContext = NULL;
        }

        // Free the color map
        if( m_Colormap )
        {
            XFreeColormap( XOpenDisplay( NULL ), m_Colormap );
        }

		m_Open = false;
		m_Version = Version( 0, 0, 0 );
		m_DeviceContext = NULL;
	}

	void OpenGLGraphicDeviceLinux::MakeCurrent( )
	{
	    // Get the nesseccary varaibles from the render output.
	    ::Display * pDisplay = m_pRenderOutput->GetDisplayDevice( );
	    ::Window window = m_pRenderOutput->GetWindowDevice( );

	    // Return if any of the values are invalid.
	    if( pDisplay == NULL || window == 0 )
	    {
            return;
	    }

	    // Turn the context into the current one.
		glXMakeCurrent( pDisplay, window, m_DeviceContext );
	}

	void OpenGLGraphicDeviceLinux::Present( )
	{
		if( m_Open == false )
		{
			return;
		}

		// Get the nesseccary varaibles from the render output.
	    ::Display * pDisplay = m_pRenderOutput->GetDisplayDevice( );
	    ::Window window = m_pRenderOutput->GetWindowDevice( );

	    // Return if any of the values are invalid.
	    if( pDisplay == NULL || window == 0 )
	    {
            return;
	    }

        // Swap the buffer
		glXSwapBuffers( pDisplay, window );
	}

	void OpenGLGraphicDeviceLinux::ClearColor( )
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void OpenGLGraphicDeviceLinux::ClearDepth( )
	{
		glClear( GL_DEPTH_BUFFER_BIT );
	}

	void OpenGLGraphicDeviceLinux::EnableDepthTest( )
	{
		glEnable( GL_DEPTH_TEST );
	}

	void OpenGLGraphicDeviceLinux::EnableTexture( )
	{
		glEnable( GL_TEXTURE_2D );
	}

	void OpenGLGraphicDeviceLinux::EnableMultisampling( )
	{
		glEnable( GL_MULTISAMPLE );
	}

	void OpenGLGraphicDeviceLinux::EnableFaceCulling( eCulling p_FaceCulling )
	{
		glEnable( GL_CULL_FACE );
		glCullFace( g_OpenGLCulling[ p_FaceCulling ] );
	}

	void OpenGLGraphicDeviceLinux::EnableSmoothLines( )
	{
		glEnable( GL_LINE_SMOOTH );
	}

	void OpenGLGraphicDeviceLinux::DisableDepthTest( )
	{
		glDisable( GL_DEPTH_TEST );
	}

	void OpenGLGraphicDeviceLinux::DisableTexture( )
	{
		glDisable( GL_TEXTURE_2D );
	}

	void OpenGLGraphicDeviceLinux::DisableFaceCulling( )
	{
		glDisable( GL_CULL_FACE );
	}

	void OpenGLGraphicDeviceLinux::DisableSmoothLines( )
	{
		glDisable( GL_LINE_SMOOTH );
	}

	Framebuffer * OpenGLGraphicDeviceLinux::CreateFramebuffer( ) const
	{
		return new OpenGLFramebuffer;
	}


	Renderbuffer * OpenGLGraphicDeviceLinux::CreateRenderbuffer( ) const
	{
		return new OpenGLRenderbuffer;
	}

	VertexArray * OpenGLGraphicDeviceLinux::CreateVertexArray( ) const
	{
		return new OpenGLVertexArray;
	}

	VertexBuffer * OpenGLGraphicDeviceLinux::CreateVertexBuffer( ) const
	{
		return new OpenGLVertexBuffer;
	}

	Shader * OpenGLGraphicDeviceLinux::CreateShader( ShaderType::eType p_Type ) const
	{
		return new OpenGLShader( p_Type );
	}

	ShaderProgram * OpenGLGraphicDeviceLinux::CreateShaderProgram( ) const
	{
		return new OpenGLShaderProgram;
	}

	Texture * OpenGLGraphicDeviceLinux::CreateTexture( ) const
	{
		return new OpenGLTexture;
	}

	Model * OpenGLGraphicDeviceLinux::CreateModel( ) const
	{
		return new Model( *this );
	}

	ModelRenderer * OpenGLGraphicDeviceLinux::CreateModelRenderer( ) const
	{
		return NULL;
	}

	void OpenGLGraphicDeviceLinux::SetViewport( const Vector2u32 & p_Position, const Vector2u32 & p_Size )
	{
		glViewport( p_Position.x, p_Position.y, p_Size.x, p_Size.y );
	}

	void OpenGLGraphicDeviceLinux::SetClearColor(	const Uint8 p_Red, const Uint8 p_Green,
													const Uint8 p_Blue, const Uint8 p_Alpha )
	{
		glClearColor(	static_cast<GLclampf>( p_Red ) / 255.0f,
						static_cast<GLclampf>( p_Green ) / 255.0f,
						static_cast<GLclampf>( p_Blue ) / 255.0f,
						static_cast<GLclampf>( p_Alpha ) / 255.0f );
	}

	Bool OpenGLGraphicDeviceLinux::IsOpen( ) const
	{
		return m_Open;
	}

	Version OpenGLGraphicDeviceLinux::GetVersion( ) const
	{
		return m_Version;
	}

	const Framebuffer & OpenGLGraphicDeviceLinux::GetDefaultFramebuffer( ) const
	{
		return s_DefaultFramebuffer;
	}

	Bool OpenGLGraphicDeviceLinux::OpenVersion( const RenderWindow & p_RenderOutput,
												const Version & p_Version )
	{

	    // Make sure the graphic device is not already open
		if( m_Open )
		{
			std::cout << "[OpenGLGraphicDeviceLinux::Open] Already open.\n";
			return false;
		}

        // Make sure the window is open
		if( !p_RenderOutput.IsOpen( ) )
		{
			std::cout << "[OpenGLGraphicDeviceLinux::Open] The window is not open.\n";
			return false;
		}



		// Get the Linux window by casting the Window class
	/*	const WindowLinux * pWindow = reinterpret_cast<const WindowLinux *>( &p_Window );
		m_pWindowLinux = const_cast<WindowLinux *>( pWindow );
*/
        // Get the window and display devices
        ::Display * pDisplay    = p_RenderOutput.GetDisplayDevice( );
		::Window window         = p_RenderOutput.GetWindowDevice( );
		int screen              = p_RenderOutput.GetScreenDevice( );

        // Error check the window and display devices.
		if( pDisplay == NULL )
		{
            std::cout << "[OpenGLGraphicDeviceLinux::Open] The display from the renderOutput is NULL.\n";
			return false;
		}
		if( window == 0 )
		{
            std::cout << "[OpenGLGraphicDeviceLinux::Open] The window from the renderOutput is 0.\n";
			return false;
		}

		// Create the visual information
		GLint Att[] =
		{
		    GLX_RGBA, GLX_DOUBLEBUFFER,
		    GLX_RED_SIZE, 8,
		    GLX_GREEN_SIZE, 8,
		    GLX_BLUE_SIZE, 8,
            GLX_DEPTH_SIZE, 16,
            GLX_STENCIL_SIZE, 8,
            0L, 0L
        };

        int fbElements = 0;
        ::GLXFBConfig *fbc = glXChooseFBConfig( pDisplay, screen, 0, &fbElements);

		::XVisualInfo * pVisualInfo = NULL;
        if( (pVisualInfo = glXChooseVisual( pDisplay, 0, Att ) ) == NULL )
		{
		    std::cout << "[OpenGLGraphicDeviceLinux::Open] Can not choose visual information.\n";
			return false;
		}

		// Create the color map and set it
		if( !(m_Colormap = XCreateColormap( pDisplay, RootWindow( pDisplay, screen), pVisualInfo->visual, AllocNone ) ) )
		{
		    std::cout << "[OpenGLGraphicDeviceLinux::Open] Can not create the colormap.\n";
            XFree( pVisualInfo );
            return false;
		}

        // Set the new color map
        XSetWindowColormap( pDisplay, window, m_Colormap );

        // Create a temporary context.
        ::GLXContext temporaryContext = glXCreateContext( pDisplay, pVisualInfo, NULL, GL_TRUE );
        if( !temporaryContext )
        {
            std::cout << "[OpenGLGraphicDeviceLinux::Open] Can not create a regual OpenGL context.\n";
            XFree( pVisualInfo );
            return false;
        }

        // Make the temporary context to the current one.
        glXMakeCurrent( pDisplay, window, temporaryContext );

        // Clear the visual info since we are done with it.
        XFree( pVisualInfo );


        // Attributes for the OGL 3.3 context
		int attribs[ ] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>( p_Version.GetMajor( ) ),
			GLX_CONTEXT_MINOR_VERSION_ARB, static_cast<int>( p_Version.GetMinor( ) ),
			0
		};


		// We need the proc address for the function
		// we are going to use for OGL 3.3 context creation.
		GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;

		if( ( glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" )) == NULL )
		{
			std::cout << "[OpenGLGraphicDeviceLinux::Open] Can not get the function for creating the context.\n";
			return false;
		}

		// Get the old error X11 error handler and set a new one in order to track if the opengl context is valid.
		g_XLibErrorCatched = false;
        XErrorHandler oldErrorHandler = XSetErrorHandler( XLibErrorCatcher );

        // Create the context
		if( ( m_DeviceContext = glXCreateContextAttribsARB( pDisplay, *fbc, 0, true, attribs ) ) != NULL )
		{
			// Delete the old temporary context
            glXDestroyContext( pDisplay, temporaryContext );

			// Make the new OpenGL context to the current one.
			glXMakeCurrent( pDisplay, window, m_DeviceContext );
		}

		// Reset the old X11 error handler
        XSetErrorHandler( oldErrorHandler );

        // Return false if we catched any error, or else the context is fine.
		if( g_XLibErrorCatched )
		{
            return false;
		}




	/*	// Filling the pixel fromat structure.
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
*/
		return true;
	}

	Bool OpenGLGraphicDeviceLinux::OpenBestVersion( const RenderWindow & p_RenderOutput,
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

}

#endif
