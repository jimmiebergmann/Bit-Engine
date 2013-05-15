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

#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <list>
#include <string>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

// Opengl "private" extensions
PFNGLGETSTRINGIPROC __glGetStringi = BIT_NULL;

// Vertex buffer object functions
PFNGLBINDVERTEXARRAYPROC __glBindVertexArray = BIT_NULL;
PFNGLDELETEVERTEXARRAYSPROC __glDeleteVertexArrays = BIT_NULL;
PFNGLGENVERTEXARRAYSPROC __glGenVertexArrays = BIT_NULL;
PFNGLISVERTEXARRAYPROC __glIsVertexArray = BIT_NULL;

// Buffers, OpenGL 1.5
PFNGLBINDBUFFERPROC __glBindBuffer = BIT_NULL;
PFNGLBUFFERDATAPROC __glBufferData = BIT_NULL;
PFNGLBUFFERSUBDATAPROC __glBufferSubData = BIT_NULL;
PFNGLDELETEBUFFERSPROC __glDeleteBuffers = BIT_NULL;
PFNGLGENBUFFERSPROC __glGenBuffers = BIT_NULL;

// Shader functions
PFNGLATTACHSHADERPROC __glAttachShader = BIT_NULL;
PFNGLCOMPILESHADERPROC __glCompileShader = BIT_NULL;
PFNGLCREATEPROGRAMPROC __glCreateProgram = BIT_NULL;
PFNGLCREATESHADERPROC __glCreateShader = BIT_NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC __glEnableVertexAttribArray = BIT_NULL;
PFNGLVERTEXATTRIBPOINTERPROC __glVertexAttribPointer = BIT_NULL;

// Static opengl extension variable availability
static BIT_BOOL BIT_ARB_vertex_buffer_object = BIT_FALSE;
static BIT_BOOL BIT_ARB_vertex_array_object = BIT_FALSE;
static BIT_BOOL BIT_ARB_framebuffer_object = BIT_FALSE;

static BIT_BOOL BIT_ARB_shading_language_100 = BIT_FALSE;
static BIT_BOOL BIT_ARB_shader_objects = BIT_FALSE;
static BIT_BOOL BIT_ARB_vertex_shader = BIT_FALSE;
static BIT_BOOL BIT_ARB_fragment_shader = BIT_FALSE;

// My own availability variables
static BIT_BOOL s_AllShaderFunctions = BIT_FALSE;
static BIT_BOOL s_GeneralBuffersFunctions = BIT_FALSE;

namespace Bit
{

	namespace OpenGL
	{

		BIT_API BIT_UINT32 BindOpenGLExtensions( const BIT_UINT32 p_Major, const BIT_UINT32 p_Minor )
		{
			// Get all the available extensions
			std::list< std::string > AvailableExtensions;
			GLint AvailableExtensionCount = 0;

			// Get the available extension count
			glGetIntegerv( GL_NUM_EXTENSIONS, &AvailableExtensionCount );

			// Get the proc address for glGetStringi
			__glGetStringi = (PFNGLGETSTRINGIPROC)glGetProcAddress( "glGetStringi" );

			// Get all the available extensions
			for( BIT_MEMSIZE i = 0; i < AvailableExtensionCount; i++ )
			{
				//if( std::string( (char*)__glGetStringi( GL_EXTENSIONS, i ) ) == "GL_ARB_shading_language_100" )
				//{
				//}

				AvailableExtensions.push_back( ( char * )( __glGetStringi( GL_EXTENSIONS, i ) ) );
				//bitTrace( "%s\n", glGetStringi( GL_EXTENSIONS, i ) );
			}

			// Go through some of the extensions to confirm some of them.
			for( std::list< std::string >::iterator it = AvailableExtensions.begin( );
				it != AvailableExtensions.end( ); it++ )
			{
				if( *it == "GL_ARB_vertex_buffer_object" )
				{
					BIT_ARB_vertex_buffer_object = BIT_TRUE;
				}
				if( *it == "GL_ARB_vertex_array_object" )
				{
					BIT_ARB_vertex_array_object = BIT_TRUE;
				}
				if( *it == "GL_ARB_framebuffer_object" )
				{
					BIT_ARB_framebuffer_object = BIT_TRUE;
				}

				// Checking for shader extensions
				if( *it == "GL_ARB_shading_language_100" )
				{
					BIT_ARB_shading_language_100 = BIT_TRUE;
				}
				if( *it == "GL_ARB_shader_objects" )
				{
					BIT_ARB_shader_objects = BIT_TRUE;
				}
				if( *it == "GL_ARB_vertex_shader" )
				{
					BIT_ARB_vertex_shader = BIT_TRUE;
				}
				if( *it == "GL_ARB_fragment_shader" )
				{
					BIT_ARB_fragment_shader = BIT_TRUE;
				}

			}

			// Use a boolean to check if any attempt in getting
			// any OpenGL function failed
			BIT_BOOL Ret = 0;

			// Get the buffer functions
			if( p_Major > 1 || ( p_Major == 1 && p_Minor >= 5 ) )
			{

				Ret |= ( __glBindBuffer = ( PFNGLBINDBUFFERPROC )
					( glGetProcAddress( "glBindBuffer" ) ) ) == BIT_NULL;

				Ret |= ( __glBufferData = ( PFNGLBUFFERDATAPROC )
					( glGetProcAddress( "glBufferData" ) ) ) == BIT_NULL;

				Ret |= ( __glBufferSubData = ( PFNGLBUFFERSUBDATAPROC )
					( glGetProcAddress( "glBufferSubData" ) ) ) == BIT_NULL;

				Ret |= ( __glDeleteBuffers = ( PFNGLDELETEBUFFERSPROC )
					( glGetProcAddress( "glDeleteBuffers" ) ) ) == BIT_NULL;

				Ret |= ( __glGenBuffers = ( PFNGLGENBUFFERSPROC )
					( glGetProcAddress( "glGenBuffers" ) ) ) == BIT_NULL;

				if( !Ret )
				{
					s_GeneralBuffersFunctions = BIT_TRUE;
				}
			}

			// Reset the return flag
			Ret = 0;

			// Get the vertex array object functions if they are available
			if( BIT_ARB_vertex_array_object )
			{
				Ret |= ( __glBindVertexArray = ( PFNGLBINDVERTEXARRAYPROC )
					( glGetProcAddress( "glBindVertexArray" ) ) ) == BIT_NULL;

				Ret |= ( __glDeleteVertexArrays = ( PFNGLDELETEVERTEXARRAYSPROC )
					( glGetProcAddress( "glDeleteVertexArrays" ) ) ) == BIT_NULL;

				Ret |= ( __glGenVertexArrays = ( PFNGLGENVERTEXARRAYSPROC )
					( glGetProcAddress( "glGenVertexArrays" ) ) ) == BIT_NULL;

				Ret |= ( __glIsVertexArray = ( PFNGLISVERTEXARRAYPROC )
					( glGetProcAddress( "glIsVertexArray" ) ) ) == BIT_NULL;

				if( Ret )
				{
					BIT_ARB_vertex_array_object = BIT_FALSE;
				}
			}

			// Reset the return flag
			Ret = 0;

			// Load the shader functions if the shader extenstions are available
			// and we are using the right verison of OpenGL
			// WARNING: comparison of unsigned expression >= 0 is always true [-Wtype-limits]|
			if( p_Major > 1 || ( p_Major == 2 && p_Major >= 0 ) )
			{

				Ret |= ( __glEnableVertexAttribArray = ( PFNGLENABLEVERTEXATTRIBARRAYPROC )
					( glGetProcAddress( "glEnableVertexAttribArray" ) ) ) == BIT_NULL;

				Ret |= ( __glVertexAttribPointer = ( PFNGLVERTEXATTRIBPOINTERPROC )
					( glGetProcAddress( "glVertexAttribPointer" ) ) ) == BIT_NULL;



				//Ret |=  ( __glAttachShader = ( PFNGLATTACHSHADERPROC )glGetProcAddress( "glAttachShader" ) ) == BIT_NULL;
/*
				Ret |= ( ( __bglCompileShader =
					( PFNGLCOMPILESHADERPROC )glGetProcAddress(
						"glCompileShader" ) ) == BIT_NULL );

				Ret |= ( ( __bglCreateProgram =
					( PFNGLCREATEPROGRAMPROC )glGetProcAddress(
						"glCreateProgram" ) } == BIT_NULL );

				Ret |= ( ( __bglCreateShader =
					( PFNGLCREATESHADERPROC )glGetProcAddress(
						"glCreateShader" ) ) == BIT_NULL );
				*/
/*
				Ret |= ( ( __bglDeleteProgram =
					( PFNGLDELETEPROGRAMPROC )glGetProcAddress(
						"glDeleteProgram" ) ) == BIT_NULL );

				Ret |= ( ( __bglDeleteShader =
					( PFNGLDELETESHADERPROC )glGetProcAddress(
						"glDeleteShader" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetProgramInfoLog =
					( PFNGLGETPROGRAMINFOLOGPROC )glGetProcAddress(
						"glGetProgramInfoLog" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetProgramiv =
					( PFNGLGETPROGRAMIVPROC )glGetProcAddress(
						"glGetProgramiv" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetShaderInfoLog =
					( PFNGLGETSHADERINFOLOGPROC )glGetProcAddress(
						"glGetShaderInfoLog" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetShaderiv =
					( PFNGLGETSHADERIVPROC )glGetProcAddress(
						"glGetShaderiv" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetUniformLocation =
					( PFNGLGETUNIFORMLOCATIONPROC )glGetProcAddress(
						"glGetUniformLocation" ) ) == BIT_NULL );

				Ret |= ( ( __bglLinkProgram =
					( PFNGLLINKPROGRAMPROC )glGetProcAddress(
						"glLinkProgram" ) ) == BIT_NULL );

				Ret |= ( ( __bglShaderSource =
					( PFNGLSHADERSOURCEPROC )glGetProcAddress(
						"glShaderSource" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform1f =
					( PFNGLUNIFORM1FPROC )glGetProcAddress(
						"glUniform1f" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform1fv =
					( PFNGLUNIFORM1FVPROC )glGetProcAddress(
						"glUniform1fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform1i =
					( PFNGLUNIFORM1IPROC )glGetProcAddress(
						"glUniform1i" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform1iv =
					( PFNGLUNIFORM1IVPROC )glGetProcAddress(
						"glUniform1iv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform2f =
					( PFNGLUNIFORM2FPROC )glGetProcAddress(
						"glUniform2f" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform2fv =
					( PFNGLUNIFORM2FVPROC )glGetProcAddress(
						"glUniform2fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform2i =
					( PFNGLUNIFORM2IPROC )glGetProcAddress(
						"glUniform2i" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform2iv =
					( PFNGLUNIFORM2IVPROC )glGetProcAddress(
						"glUniform2iv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform3f =
					( PFNGLUNIFORM3FPROC )glGetProcAddress(
						"glUniform3f" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform3fv =
					( PFNGLUNIFORM3FVPROC )glGetProcAddress(
						"glUniform3fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform3i =
					( PFNGLUNIFORM3IPROC )glGetProcAddress(
						"glUniform3i" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform3iv =
					( PFNGLUNIFORM3IVPROC )glGetProcAddress(
						"glUniform3iv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform4f =
					( PFNGLUNIFORM4FPROC )glGetProcAddress(
						"glUniform4f" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform4fv =
					( PFNGLUNIFORM4FVPROC )glGetProcAddress(
						"glUniform4fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform4i =
					( PFNGLUNIFORM4IPROC )glGetProcAddress(
						"glUniform4i" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniform4iv =
					( PFNGLUNIFORM4IVPROC )glGetProcAddress(
						"glUniform4iv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniformMatrix2fv =
					( PFNGLUNIFORMMATRIX2FVPROC )glGetProcAddress(
						"glUniformMatrix2fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniformMatrix3fv =
					( PFNGLUNIFORMMATRIX3FVPROC )glGetProcAddress(
						"glUniformMatrix3fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUniformMatrix4fv =
					( PFNGLUNIFORMMATRIX4FVPROC )glGetProcAddress(
						"glUniformMatrix4fv" ) ) == BIT_NULL );

				Ret |= ( ( __bglUseProgram =
					( PFNGLUSEPROGRAMPROC )glGetProcAddress(
						"glUseProgram" ) ) == BIT_NULL );

				Ret |= ( ( __bglValidateProgram =
					( PFNGLVALIDATEPROGRAMPROC )glGetProcAddress(
						"glValidateProgram" ) ) == BIT_NULL );

				Ret |= ( ( __bglBindAttribLocation =
					( PFNGLBINDATTRIBLOCATIONPROC )glGetProcAddress(
						"glBindAttribLocation" ) ) == BIT_NULL );

				Ret |= ( ( __bglGetAttribLocation =
					( PFNGLGETATTRIBLOCATIONPROC )glGetProcAddress(
						"glGetAttribLocation" ) ) == BIT_NULL );

				Ret |= ( ( __bglEnableVertexAttribArray =
					( PFNGLENABLEVERTEXATTRIBARRAYPROC )glGetProcAddress(
						"glEnableVertexAttribArray" ) ) == BIT_NULL );

				Ret |= ( ( __bglVertexAttribPointer =
					( PFNGLVERTEXATTRIBPOINTERPROC )glGetProcAddress(
						"glVertexAttribPointer" ) ) == BIT_NULL );


*/
				if( Ret == 0 )
				{
					s_AllShaderFunctions = BIT_TRUE;
				}
			}
			/*

*/



			return BIT_OK;
		}

		BIT_API BIT_BOOL GetVertexObjectAvailability( )
		{
			return	BIT_ARB_vertex_array_object;
		}

		BIT_API BIT_BOOL GetGeneralBufferAvailability( )
		{
			return s_GeneralBuffersFunctions;
		}

		BIT_API BIT_BOOL GetShaderAvailability( )
		{
			return	s_AllShaderFunctions &&
					BIT_ARB_shading_language_100 &&
					BIT_ARB_shader_objects &&
					BIT_ARB_vertex_shader &&
					BIT_ARB_fragment_shader;
		}

	}

}
