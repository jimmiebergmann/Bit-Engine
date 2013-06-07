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

#include <Bit/Graphics/Win32/GraphicDeviceWin32.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <Bit/Graphics/OpenGL/VertexObjectOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderProgramOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderOpenGL.hpp>
#include <Bit/Graphics/OpenGL/TextureOpenGL.hpp>
#include <Bit/Graphics/Model/ModelOBJ.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{


	// Constructors/destructors
	GraphicDeviceWin32::GraphicDeviceWin32( )
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

	GraphicDeviceWin32::~GraphicDeviceWin32( )
	{
		Close( );
	}


	// Public general functions
	BIT_UINT32 GraphicDeviceWin32::Open( const Window & p_Window, const BIT_UINT32 p_Devices )
	{
		// Make sure the graphic device is not already open
		if( m_Open )
		{
			bitTrace( "[GraphicDeviceWin32::Open] Already open.\n" );
			return BIT_ERROR;
		}

		// Make sure the window is open
		if( !p_Window.IsOpen( ) )
		{
			bitTrace( "[GraphicDeviceWin32::Open] The window is not open.\n" );
			return BIT_ERROR;
		}

		// Get the Win32 window by casting the Window class
		const WindowWin32 * pWindow = reinterpret_cast<const WindowWin32 *>( &p_Window );

		// Get the device context which is the key variable which is being used for swapping buffers
		m_DeviceContext = pWindow->GetDeviceContext( );

		// Let's decide which device type we should use.
		// We are just supporting OpenGL in the Win32 graphic device.
		BIT_BOOL UseAnyVersion = ( p_Devices == Bit::GraphicDevice::Device_Any );
		BIT_BOOL UseDeprecatedVersion = ( p_Devices == Bit::GraphicDevice::Device_OpenGL_2_1 );
		BIT_UINT32 GLVersionMajor = 3;
		BIT_UINT32 GLVersionMinor = 1;

		
		// //////////////////////////////////////////////
		// !NOTE! SKIP THIS FOR NOW!
		/*if( p_Devices & Bit::GraphicDevice::Device_OpenGL_3_1 )
		{
			GLVersionMajor = 3;
			GLVersionMinor = 1;
		}
		else if( p_Devices & Bit::GraphicDevice::Device_OpenGL_2_1 )
		{

		}
		else if( !UseAnyVersion )
		{
			bitTrace( "[GraphicDeviceWin32::Open] No fitting device type specified.\n" );
			return BIT_ERROR;
		}*/
		// !NOTE! SKIT THIS FOR NOW!
		// //////////////////////////////////////////////

		// Create an OpenGL context


		/*

		// Make sure the window is created.
		if(p_Window.IsCreated() == false)
		{
			return BIT_ERROR;
		}

		m_RendererType = BIT_RENDERER_TYPE_NONE;

		// Get the HDC from the window class, also make sure it's not null.
		if(( m_DeviceContext = p_Window.GetData().DeviceContext) == BIT_NULL )
		{
			return BIT_ERROR;
		}

		// Check the buffer bits to find out if we should create
		// a depth/stencil buffer
		BYTE DepthBits = 0;
		BYTE StencilBits = 0;
		if( p_BufferBits & BIT_BUFFER_DEPTH )
		{
			DepthBits = 16;
		}
		if( p_BufferBits & BIT_BUFFER_STENCIL )
		{
			StencilBits = 1;
		}*/

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

		if((PixelFormat = ChoosePixelFormat( m_DeviceContext, &PFD ) ) == 0)
		{
			bitTrace( "[GraphicDeviceWin32::Open] Can not choose pixel format.\n" );
			return BIT_ERROR;
		}
		if( ( SetPixelFormat( m_DeviceContext, PixelFormat, &PFD ) ) == BIT_FALSE )
		{
			bitTrace( "[GraphicDeviceWin32::Open] Can not set pixel format.\n" );
			return BIT_ERROR;
		}

		// Create a temporary regual context.
		// We need this context to create the 3.x context.
		HGLRC TemporaryContext = wglCreateContext(m_DeviceContext);

		if( TemporaryContext == BIT_NULL )
		{
			bitTrace( "[GraphicDeviceWin32::Open] Can not create a regular OpenGL context.\n" );
			return BIT_ERROR;
		}

		// Make the temporary context to the current one
		wglMakeCurrent( BIT_NULL, BIT_NULL );
		wglMakeCurrent( m_DeviceContext, TemporaryContext );

		// Attributes for the OGL 3.3 context
		int Attribs[ ] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, GLVersionMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB, GLVersionMinor,
			0
		};

		// We need the proc address for the function
		// we are going to use for OGL 3.3 context creation.
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
		if((wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress( "wglCreateContextAttribsARB" )) == BIT_NULL )
		{
			return BIT_ERROR;
		}

		// Create the context
		if((m_Context = wglCreateContextAttribsARB( m_DeviceContext, 0, Attribs)) != BIT_NULL )
		{
			// Delete the old temporary context
			wglMakeCurrent( BIT_NULL, BIT_NULL );
			wglDeleteContext( TemporaryContext );

			// Make the new OpenGL context to the current one.
			wglMakeCurrent( m_DeviceContext, m_Context );

			m_DeviceType = Bit::GraphicDevice::Device_OpenGL_3_1;
		}
		else
		{
			// The creation of the OGL 3.3 context failed, use the temporary > 3.3 context instead.
			m_Context = TemporaryContext;
			m_DeviceType = Bit::GraphicDevice::Device_OpenGL_2_1;
		}

		// Load all the opengl extensions
		/*if( BindOpenGLExtensions( 3,3 ) != BIT_OK )
		{
			return BIT_ERROR;
		}
*/

/*
		// Get some opengl variables
		GLint MaxTextureSize = 0;
		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &MaxTextureSize );
		m_MaxTextureSize = static_cast< BIT_UINT32 >( MaxTextureSize );

		m_Created = BIT_TRUE;
		return BIT_OK;

		*/



		if( OpenGL::BindOpenGLExtensions( GLVersionMajor, GLVersionMinor ) != BIT_OK )
		{
			bitTrace( "[GraphicDeviceWin32::Open] Can not bind the OpenGL extensions.\n" );
			return BIT_ERROR;
		}




		m_Open = BIT_TRUE;
		return BIT_OK;
	}

	BIT_UINT32 GraphicDeviceWin32::Close( )
	{
		// Release the context from the current thread
		if( !wglMakeCurrent( NULL, NULL ) )
		{
			bitTrace( "[GraphicDeviceWin32::Close] Can not release the context.\n" );
		}

		// Delete the context
		if( !wglDeleteContext( m_Context ))
		{
			bitTrace( "[GraphicDeviceWin32::Close] Can not delete the context.\n" );
		}
		m_Context = BIT_NULL;

		m_Open = BIT_FALSE;
		return BIT_ERROR;
	}

	void GraphicDeviceWin32::Present( )
	{
		if( !m_Open )
		{
			return;
		}

		SwapBuffers( m_DeviceContext );
	}
	
	// Create functions for different renderer elements
	VertexObject * GraphicDeviceWin32::CreateVertexObject( ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetGeneralBufferAvailability( ) ||
			!OpenGL::GetVertexObjectAvailability( ) ||
			!OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceWin32::CreateVertexObject] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		// Allocate a ne vertex object
		return new VertexObjectOpenGL( );
	}

	ShaderProgram * GraphicDeviceWin32::CreateShaderProgram( ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceWin32::CreateShaderProgram] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		return new ShaderProgramOpenGL( );
	}

	Shader * GraphicDeviceWin32::CreateShader( const Shader::eShaderType p_ShaderType ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetShaderAvailability( ) )
		{
			bitTrace( "[GraphicDeviceWin32::CreateShader] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		return new ShaderOpenGL( p_ShaderType );
	}

	Texture * GraphicDeviceWin32::CreateTexture( ) const
	{
		// Make sure we support OpenGL vertex objects
		if( !OpenGL::GetGeneralTextureAvailability( ) )
		{
			bitTrace( "[GraphicDeviceWin32::CreateTexture] Not supporting the required functions.\n" );
			return BIT_NULL;
		}

		return new TextureOpenGL( );
	}

	Model * GraphicDeviceWin32::CreateModel( Model::eModelType p_Type ) const
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

	// Clear functions
	void GraphicDeviceWin32::ClearBuffers( const BIT_UINT32 p_ClearBits )
	{

	}

	void GraphicDeviceWin32::ClearColor( )
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void GraphicDeviceWin32::ClearDepth( )
	{
		glClear( GL_DEPTH_BUFFER_BIT );
	}

	// Enable functions
	void GraphicDeviceWin32::EnableTexture( )
	{
		glEnable( GL_TEXTURE_2D );
		m_TextureStatus = BIT_TRUE;
	}

	void GraphicDeviceWin32::EnableAlpha( )
	{
		// We have to make this function customizable.
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		glAlphaFunc( GL_GREATER, 0 );
		glEnable( GL_ALPHA_TEST );
		m_AlphaStatus = BIT_TRUE;
	}

	void GraphicDeviceWin32::EnableDepthTest( )
	{
		glEnable( GL_DEPTH_TEST );
		m_DepthTestStatus = BIT_TRUE;
	}

	void GraphicDeviceWin32::EnableStencilTest( )
	{
		glEnable( GL_STENCIL_TEST );
		m_StencilTestStatus = BIT_TRUE;
	}

	void GraphicDeviceWin32::EnableFaceCulling( BIT_UINT32 p_FaceCulling )
	{
		/*GLenum Mode = GL_FRONT;
		if( p_FaceCulling == BIT_RENDERER_BACKFACE_CULLING )
		{
			Mode = GL_BACK;
		}
		// else if the culling isn't front face culling, return ( failed )
		else if( p_FaceCulling != BIT_RENDERER_FRONTFACE_CULLING )
		{
			return;
		}

		glEnable( GL_CULL_FACE );
		glCullFace( Mode );
		m_FaceCullingStatus = BIT_TRUE;
		m_FaceCullingType = p_FaceCulling;*/
	}

	void GraphicDeviceWin32::EnableSmoothLines( )
	{
		glEnable( GL_LINE_SMOOTH );
		m_SmoothLinesStatus = BIT_TRUE;
	}

	// Disable functions
	void GraphicDeviceWin32::DisableTexture( )
	{
		glDisable( GL_TEXTURE_2D );
		m_TextureStatus = BIT_FALSE;
	}

	void GraphicDeviceWin32::DisableAlpha( )
	{
		glDisable( GL_BLEND );
		glDisable( GL_ALPHA_TEST );
		m_AlphaStatus = BIT_FALSE;
	}

	void GraphicDeviceWin32::DisableDepthTest( )
	{
		glDisable( GL_DEPTH_TEST );
		m_DepthTestStatus = BIT_FALSE;
	}

	void GraphicDeviceWin32::DisableStencilTest( )
	{
		glDisable( GL_STENCIL_TEST );
		m_StencilTestStatus = BIT_FALSE;
	}

	void GraphicDeviceWin32::DisableFaceCulling( )
	{
		glDisable( GL_CULL_FACE );
		m_FaceCullingStatus = BIT_FALSE;
		m_FaceCullingType = 0;
	}

	void GraphicDeviceWin32::DisableSmoothLines( )
	{
		glDisable( GL_LINE_SMOOTH );
		m_SmoothLinesStatus = BIT_FALSE;
	}

	// Set functions
	void GraphicDeviceWin32::SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A )
	{
		glClearColor( p_R, p_G, p_B, p_A );
	}

	void GraphicDeviceWin32::SetClearDepth( BIT_FLOAT32 p_Depth )
	{
		glClearDepth( p_Depth );
	}

	void GraphicDeviceWin32::SetClearStencil( BIT_UINT32 p_Stencil )
	{
		glClearStencil( p_Stencil );
	}

	void GraphicDeviceWin32::SetViewport( const BIT_UINT32 p_LX, const BIT_UINT32 p_LY,
		const BIT_UINT32 p_HX, const BIT_UINT32 p_HY )
	{
		glViewport( p_LX, p_LY, p_HX, p_HY );
		m_ViewportLow = Vector2_si32( p_LX, p_LY );
		m_ViewportHigh = Vector2_si32( p_HX, p_HY );
	}

	void GraphicDeviceWin32::SetLineWidth( const BIT_FLOAT32 p_Width )
	{
		glLineWidth( p_Width );
	}

	// Get functions
	// ...

}