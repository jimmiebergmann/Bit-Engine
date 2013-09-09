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

#include <Bit/Graphics/Linux/GraphicDeviceLinux.hpp>

#ifdef BIT_PLATFORM_LINUX

#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <Bit/Graphics/OpenGL/FramebufferOpenGL.hpp>
//#include <Bit/Graphics/OpenGL/RenderbufferOpenGL.hpp>
#include <Bit/Graphics/OpenGL/VertexObjectOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderProgramOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderOpenGL.hpp>
#include <Bit/Graphics/OpenGL/TextureOpenGL.hpp>
#include <Bit/Graphics/OpenGL/PostProcessingBloomOpenGL.hpp>
#include <Bit/Graphics/ModelOBJ.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{


	// Constructors/destructors
	GraphicDeviceLinux::GraphicDeviceLinux( ) :
        m_pWindowLinux( BIT_NULL ),
        m_pDisplay( BIT_NULL ),
        m_Window( BIT_NULL ),
        m_DeviceContext( BIT_NULL )
	{
	    m_Open = BIT_FALSE;

		// Enable / Disable statuses
		m_TextureStatus = BIT_FALSE;
		m_AlphaStatus = BIT_FALSE;
		m_DepthTestStatus = BIT_FALSE;
		m_StencilTestStatus = BIT_FALSE;
		m_FaceCullingStatus = BIT_FALSE;
		m_FaceCullingType = 0;
		m_SmoothLinesStatus = BIT_FALSE;
	}

	GraphicDeviceLinux::~GraphicDeviceLinux( )
	{
	    Close( );
	}


	// Public general functions
	BIT_UINT32 GraphicDeviceLinux::Open( const Window & p_Window, const BIT_UINT32 p_Devices )
	{
	    // Make sure the graphic device is not already open
		if( m_Open )
		{
			bitTrace( "[GraphicDeviceLinux::Open] Already open.\n" );
			return BIT_ERROR;
		}


		/*
                // Private variables from the window class
                ::Display * m_pDisplay;
                ::Window m_Window;

                // Private variables in general
                ::GLXContext m_DeviceContext;
                ::XVisualInfo * m_pVisualInfo;
                ::Colormap m_Colormap;
		*/



        // Make sure the window is open
		if( !p_Window.IsOpen( ) )
		{
			bitTrace( "[GraphicDeviceLinux::Open] The window is not open.\n" );
			return BIT_ERROR;
		}

		// Get the Linux window by casting the Window class
		const WindowLinux * pWindow = reinterpret_cast<const WindowLinux *>( &p_Window );
		m_pWindowLinux = const_cast<WindowLinux *>( pWindow );

        // Get the window and display devices
		if( ( m_Window = pWindow->GetWindowDevice( ) ) == BIT_NULL )
		{
		    bitTrace( "[GraphicDeviceLinux::Open] The window is NULL.\n" );
			return BIT_ERROR;
		}

		if( (m_pDisplay = pWindow->GetDisplayDevice( ) ) == BIT_NULL )
		{
		    bitTrace( "[GraphicDeviceLinux::Open] The display is NULL.\n" );
			return BIT_ERROR;
		}

		int Screen =  pWindow->GetScreenDevice( );




		// Create the visual information
		//GLint Att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		GLint Att[] =
		{
		    GLX_RGBA, GLX_DOUBLEBUFFER,
		    GLX_RED_SIZE, 8,
		    GLX_GREEN_SIZE, 8,
		    GLX_BLUE_SIZE, 8,
            GLX_DEPTH_SIZE, 16,
            GLX_STENCIL_SIZE, 8,
            None
        };

		::XVisualInfo * pVisualInfo = BIT_NULL;
        if( (pVisualInfo = glXChooseVisual( m_pDisplay, 0, Att ) ) == BIT_NULL )
		{
		    bitTrace( "[GraphicDeviceLinux::Open] Can not choose visual information.\n" );
			return BIT_ERROR;
		}

		// Create the color map and set it
		if( !(m_Colormap = XCreateColormap( m_pDisplay, RootWindow( m_pDisplay, Screen), pVisualInfo->visual, AllocNone ) ) )
		{
		    bitTrace( "[GraphicDeviceLinux::Open] Can not create the colormap.\n" );
            XFree( pVisualInfo );
            return BIT_ERROR;
		}

        // Set the new color map
        XSetWindowColormap( m_pDisplay, m_Window, m_Colormap );

        // Create a regual context for now
        if( !(m_DeviceContext = glXCreateContext( m_pDisplay, pVisualInfo, NULL, GL_TRUE ) ) )
        {
            bitTrace( "[GraphicDeviceLinux::Open] Can not create the OpenGL context.\n" );
            XFree( pVisualInfo );
            return BIT_ERROR;

        }
        glXMakeCurrent( m_pDisplay, m_Window, m_DeviceContext );

        // Clear the visual info since we are done with it.
        XFree( pVisualInfo );

        // Bind the OpenGL extensions
        BIT_UINT32 GLVersionMajor = 3;
        BIT_UINT32 GLVersionMinor = 1;
        if( OpenGL::BindOpenGLExtensions( GLVersionMajor, GLVersionMinor ) != BIT_OK )
        {
            bitTrace( "[GraphicDeviceLinux::Open] Can not bind the OpeNGL extensions.\n" );
            return BIT_ERROR;
        }

		m_Open = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 GraphicDeviceLinux::Close( )
	{
	    // Destroy the OpenGL context
        if( m_DeviceContext )
        {
            // Release the context from this thread
            if( !glXMakeCurrent( m_pDisplay, m_Window, m_DeviceContext ) )
            {
                bitTrace( "[GraphicDeviceLinux::Close] Can release the OpenGL context.\n" );
                return BIT_ERROR;
            }

            glXDestroyContext( m_pDisplay, m_DeviceContext );
            m_DeviceContext = BIT_NULL;
        }

        // Free the color map
        if( m_Colormap )
        {
            XFreeColormap( m_pDisplay, m_Colormap );
        }

	    m_Open = BIT_FALSE;
		return BIT_ERROR;
	}

	void GraphicDeviceLinux::Present( )
	{
        if( !m_Open || !m_pWindowLinux->IsOpen( ) )
		{
			return;
		}


		glXSwapBuffers( m_pDisplay, m_Window );
	}

	void GraphicDeviceLinux::BindDefaultFramebuffer( )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void GraphicDeviceLinux::BindDefaultShaderProgram( )
	{
		glUseProgram( 0 );
	}

	// Create functions for different renderer elements
	Framebuffer * GraphicDeviceLinux::CreateFramebuffer( ) const
	{
		return new FramebufferOpenGL( );
	}

	/*Renderbuffer * GraphicDeviceLinux::CreateRenderbuffer( ) const
	{
		return new RenderbufferOpenGL( );
	}*/

    VertexObject * GraphicDeviceLinux::CreateVertexObject( ) const
    {
        // Make sure we support OpenGL vertex objects
		if( !OpenGL::GetGeneralBufferAvailability( ) ||
			!OpenGL::GetVertexObjectAvailability( ) ||
			!OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceLinux::CreateVertexObject] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		// Allocate a ne vertex object
		return new VertexObjectOpenGL( );
    }

    ShaderProgram * GraphicDeviceLinux::CreateShaderProgram( ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceLinux::CreateShaderProgram] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		return new ShaderProgramOpenGL( );
	}

	Shader * GraphicDeviceLinux::CreateShader( const Shader::eShaderType p_ShaderType ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceLinux::CreateShader] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		return new ShaderOpenGL( p_ShaderType );
	}


	Texture * GraphicDeviceLinux::CreateTexture( ) const
	{
	   // Make sure we support OpenGL vertex objects
		if( !OpenGL::GetGeneralTextureAvailability( ) )
		{
			bitTrace( "[GraphicDeviceLinux::CreateTexture] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		BIT_BOOL OpenGL2 = ( m_DeviceType == Device_OpenGL_2_1 );
		return new TextureOpenGL( OpenGL2 );
	}

	Model * GraphicDeviceLinux::CreateModel( Model::eModelType p_Type ) const
	{
		switch( p_Type )
		{
		case Model::Model_OBJ:
			{
				return new ModelOBJ( *reinterpret_cast< const GraphicDevice *>( this ) );
			}
			break;

		default:
			break;
		}

		return BIT_NULL;
	}

	PostProcessingBloom * GraphicDeviceLinux::CreatePostProcessingBloom( VertexObject * p_pVertexObject, Texture * p_pTexture )
	{
		ShaderProgram * pShaderProgram = BIT_NULL;
		Shader * pVertexShader = BIT_NULL;
		Shader * pFragmentShader = BIT_NULL;

		// Create the shader program
		if( ( pShaderProgram = CreateShaderProgram( ) ) == BIT_NULL )
		{
			bitTrace( "[GraphicDeviceLinux::CreatePostProcessingBloom] Can not create the shader program\n" );
			return BIT_NULL;
		}

		if( ( pVertexShader = CreateShader( Bit::Shader::Vertex ) ) == BIT_NULL )
		{
			bitTrace( "[GraphicDeviceLinux::CreatePostProcessingBloom] Can not create the vertex shader\n" );
			return BIT_NULL;
		}
		if( ( pFragmentShader = CreateShader( Bit::Shader::Fragment ) ) == BIT_NULL )
		{
			bitTrace( "[GraphicDeviceLinux::CreatePostProcessingBloom] Can not create the vertex shader\n" );
			return BIT_NULL;
		}

		return new PostProcessingBloomOpenGL( pShaderProgram, pVertexShader,pFragmentShader, p_pVertexObject, p_pTexture );
	}

	// Clear functions
	void GraphicDeviceLinux::ClearBuffers( const BIT_UINT32 p_ClearBits )
	{

	}

	void GraphicDeviceLinux::ClearColor( )
	{
        glClear( GL_COLOR_BUFFER_BIT );
	}

	void GraphicDeviceLinux::ClearDepth( )
	{
        glClear( GL_DEPTH_BUFFER_BIT );
	}

	// Enable functions
    void GraphicDeviceLinux::EnableTexture( )
    {
        glEnable( GL_TEXTURE_2D );
		m_TextureStatus = BIT_TRUE;
    }

    void GraphicDeviceLinux::EnableAlpha( )
    {
        // We have to make this function customizable.
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		glAlphaFunc( GL_GREATER, 0 );
		glEnable( GL_ALPHA_TEST );
		m_AlphaStatus = BIT_TRUE;
    }

    void GraphicDeviceLinux::EnableDepthTest( )
    {
        glEnable( GL_DEPTH_TEST );
		m_DepthTestStatus = BIT_TRUE;
    }

    void GraphicDeviceLinux::EnableStencilTest( )
    {
        glEnable( GL_STENCIL_TEST );
		m_StencilTestStatus = BIT_TRUE;
    }

    void GraphicDeviceLinux::EnableFaceCulling( eCulling p_FaceCulling )
    {
        GLenum Mode = GL_FRONT;
		if( p_FaceCulling == eCulling::Culling_BackFace )
		{
			Mode = GL_BACK;
		}
		// else if the culling isn't front face culling, return ( failed )
		else if( p_FaceCulling != eCulling::Culling_FrontFace )
		{
			return;
		}

		glEnable( GL_CULL_FACE );
		glCullFace( Mode );
		m_FaceCullingStatus = BIT_TRUE;
		m_FaceCullingType = p_FaceCulling;
    }

    void GraphicDeviceLinux::EnableSmoothLines( )
    {
        glEnable( GL_LINE_SMOOTH );
		m_SmoothLinesStatus = BIT_TRUE;
    }


    // Disable functions
    void GraphicDeviceLinux::DisableTexture( )
    {
        glDisable( GL_TEXTURE_2D );
		m_TextureStatus = BIT_FALSE;
    }

    void GraphicDeviceLinux::DisableAlpha( )
    {
        glDisable( GL_BLEND );
		glDisable( GL_ALPHA_TEST );
		m_AlphaStatus = BIT_FALSE;
    }

    void GraphicDeviceLinux::DisableDepthTest( )
    {
        glDisable( GL_DEPTH_TEST );
		m_DepthTestStatus = BIT_FALSE;
    }

    void GraphicDeviceLinux::DisableStencilTest( )
    {
        glDisable( GL_STENCIL_TEST );
		m_StencilTestStatus = BIT_FALSE;
    }

    void GraphicDeviceLinux::DisableFaceCulling( )
    {
        glDisable( GL_CULL_FACE );
		m_FaceCullingStatus = BIT_FALSE;
		m_FaceCullingType = 0;
    }

    void GraphicDeviceLinux::DisableSmoothLines( )
    {
        glDisable( GL_LINE_SMOOTH );
		m_SmoothLinesStatus = BIT_FALSE;
    }


    // Set functions
    void GraphicDeviceLinux::SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
        const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A )
    {
        glClearColor( p_R, p_G, p_B, p_A );
    }

    void GraphicDeviceLinux::SetClearDepth( BIT_FLOAT32 p_Depth )
    {
        glClearDepth( p_Depth );
    }

    void GraphicDeviceLinux::SetClearStencil( BIT_UINT32 p_Stencil )
    {
        glClearStencil( p_Stencil );
    }

    void GraphicDeviceLinux::SetViewport( const BIT_UINT32 p_LX, const BIT_UINT32 p_LY,
        const BIT_UINT32 p_HX, const BIT_UINT32 p_HY ) // Lower and higher coordinates
    {
        glViewport( p_LX, p_LY, p_HX, p_HY );
		m_ViewportLow = Vector2_si32( p_LX, p_LY );
		m_ViewportHigh = Vector2_si32( p_HX, p_HY );
    }

    void GraphicDeviceLinux::SetLineWidth( const BIT_FLOAT32 p_Width )
    {
        glLineWidth( p_Width );
    }


    // Get functions
    // ...


}

#endif
