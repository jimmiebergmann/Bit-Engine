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

// Opengl "private" extensions
PFNGLGETSTRINGIPROC __glGetStringi = NULL;

// Vertex buffer object functions
PFNGLBINDVERTEXARRAYPROC __glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC __glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSPROC __glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYPROC __glIsVertexArray = NULL;

// Texture, OpenGL 1.3
PFNGLACTIVETEXTUREPROC __glActiveTexture = NULL;

// Buffers, OpenGL 1.5
PFNGLBINDBUFFERPROC __glBindBuffer = NULL;
PFNGLBUFFERDATAPROC __glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC __glBufferSubData = NULL;
PFNGLDELETEBUFFERSPROC __glDeleteBuffers = NULL;
PFNGLGENBUFFERSPROC __glGenBuffers = NULL;

// Framebuffers/renderbuffers, OpenGL 3.0
PFNGLBINDFRAMEBUFFERPROC __glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC __glBindRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC __glCheckFramebufferStatus = NULL;
PFNGLDELETEFRAMEBUFFERSPROC __glDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC __glDeleteRenderbuffers = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC __glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC __glFramebufferTexture2D = NULL;
PFNGLGENFRAMEBUFFERSPROC __glGenFramebuffers = NULL;
PFNGLGENRENDERBUFFERSPROC __glGenrRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEPROC __glRenderbufferStorage = NULL;

// Texture functions
PFNGLGENERATEMIPMAPPROC __glGenerateMipmap = NULL;

// Shader functions
PFNGLENABLEVERTEXATTRIBARRAYPROC __glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC __glVertexAttribPointer = NULL;
PFNGLATTACHSHADERPROC __glAttachShader = NULL;
PFNGLCOMPILESHADERPROC __glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC __glCreateProgram = NULL;
PFNGLCREATESHADERPROC __glCreateShader = NULL;
PFNGLDELETEPROGRAMPROC __glDeleteProgram = NULL;
PFNGLDELETESHADERPROC __glDeleteShader = NULL;
PFNGLGETPROGRAMINFOLOGPROC __glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC __glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC __glGetShaderInfoLog = NULL;
PFNGLGETSHADERIVPROC __glGetShaderiv = NULL;
PFNGLGETUNIFORMLOCATIONPROC __glGetUniformLocation = NULL;
PFNGLLINKPROGRAMPROC __glLinkProgram = NULL;
PFNGLSHADERSOURCEPROC __glShaderSource = NULL;
PFNGLVALIDATEPROGRAMPROC __glValidateProgram = NULL;
PFNGLBINDATTRIBLOCATIONPROC __glBindAttribLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC __glGetAttribLocation = NULL;
PFNGLUSEPROGRAMPROC __glUseProgram = NULL;

// Shader uniforms
PFNGLUNIFORM1FPROC __glUniform1f = NULL;
PFNGLUNIFORM1FVPROC __glUniform1fv = NULL;
PFNGLUNIFORM1IPROC __glUniform1i = NULL;
PFNGLUNIFORM1IVPROC __glUniform1iv = NULL;
PFNGLUNIFORM2FPROC __glUniform2f = NULL;
PFNGLUNIFORM2FVPROC __glUniform2fv = NULL;
PFNGLUNIFORM2IPROC __glUniform2i = NULL;
PFNGLUNIFORM2IVPROC __glUniform2iv = NULL;
PFNGLUNIFORM3FPROC __glUniform3f = NULL;
PFNGLUNIFORM3FVPROC __glUniform3fv = NULL;
PFNGLUNIFORM3IPROC __glUniform3i = NULL;
PFNGLUNIFORM3IVPROC __glUniform3iv = NULL;
PFNGLUNIFORM4FPROC __glUniform4f = NULL;
PFNGLUNIFORM4FVPROC __glUniform4fv = NULL;
PFNGLUNIFORM4IPROC __glUniform4i = NULL;
PFNGLUNIFORM4IVPROC __glUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVPROC __glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC __glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC __glUniformMatrix4fv = NULL;

namespace Bit
{

	// Static opengl extension variable availability
	static Bool BIT_ARB_vertex_buffer_object = false;
	static Bool BIT_ARB_vertex_array_object = false;
	static Bool BIT_ARB_framebuffer_object = false;

	static Bool BIT_ARB_shading_language_100 = false;
	static Bool BIT_ARB_shader_objects = false;
	static Bool BIT_ARB_vertex_shader = false;
	static Bool BIT_ARB_fragment_shader = false;

	// My own availability variables
	static Bool s_AllShaderFunctions = false;
	static Bool s_GeneralBuffersFunctions = false;
	static Bool s_GeneralTextureFunctions = false;

	namespace OpenGL
	{

		BIT_API bool BindOpenGLExtensions( const Uint32 p_Major, const Uint32 p_Minor )
		{
			// Get all the available extensions
			std::list< std::string > AvailableExtensions;
			GLint AvailableExtensionCount = 0;

			// Get the available extension count
			glGetIntegerv( GL_NUM_EXTENSIONS, &AvailableExtensionCount );

			// Get the proc address for glGetStringi
			__glGetStringi = (PFNGLGETSTRINGIPROC)glGetProcAddress( "glGetStringi" );

			// Get all the available extensions
			for( SizeType i = 0; i < (SizeType)AvailableExtensionCount; i++ )
			{
				//if( std::string( (char*)__glGetStringi( GL_EXTENSIONS, i ) ) == "GL_ARB_shading_language_100" )
				//{
				//}

				AvailableExtensions.push_back( ( char * )( __glGetStringi( GL_EXTENSIONS, i ) ) );
			}

			// Go through some of the extensions to confirm some of them.
			for( std::list< std::string >::iterator it = AvailableExtensions.begin( );
				it != AvailableExtensions.end( ); it++ )
			{
				if( *it == "GL_ARB_vertex_buffer_object" )
				{
					BIT_ARB_vertex_buffer_object = true;
				}
				if( *it == "GL_ARB_vertex_array_object" )
				{
					BIT_ARB_vertex_array_object = true;
				}
				if( *it == "GL_ARB_framebuffer_object" )
				{
					BIT_ARB_framebuffer_object = true;
				}

				// Checking for shader extensions
				if( *it == "GL_ARB_shading_language_100" )
				{
					BIT_ARB_shading_language_100 = true;
				}
				if( *it == "GL_ARB_shader_objects" )
				{
					BIT_ARB_shader_objects = true;
				}
				if( *it == "GL_ARB_vertex_shader" )
				{
					BIT_ARB_vertex_shader = true;
				}
				if( *it == "GL_ARB_fragment_shader" )
				{
					BIT_ARB_fragment_shader = true;
				}

			}

			// Use a boolean to check if any attempt in getting
			// any OpenGL function failed
			Bool Ret = 0;

			// Get the texture functions
			if( BIT_ARB_framebuffer_object )
			{
				Ret |= ( __glGenerateMipmap = ( PFNGLGENERATEMIPMAPPROC )
					glGetProcAddress( "glGenerateMipmap" ) ) == NULL;

				Ret |= ( __glBindFramebuffer = ( PFNGLBINDFRAMEBUFFERPROC )
					glGetProcAddress( "glBindFramebuffer" ) ) == NULL;

				Ret |= ( __glBindRenderbuffer = ( PFNGLBINDRENDERBUFFERPROC )
					glGetProcAddress( "glBindRenderbuffer" ) ) == NULL;

				Ret |= ( __glCheckFramebufferStatus = ( PFNGLCHECKFRAMEBUFFERSTATUSPROC )
					glGetProcAddress( "glCheckFramebufferStatus" ) ) == NULL;

				Ret |= ( __glDeleteFramebuffers = ( PFNGLDELETEFRAMEBUFFERSPROC )
					glGetProcAddress( "glDeleteFramebuffers" ) ) == NULL;

				Ret |= ( __glDeleteRenderbuffers = ( PFNGLDELETERENDERBUFFERSPROC )
					glGetProcAddress( "glDeleteRenderbuffers" ) ) == NULL;

				Ret |= ( __glFramebufferRenderbuffer = ( PFNGLFRAMEBUFFERRENDERBUFFERPROC )
					glGetProcAddress( "glFramebufferRenderbuffer" ) ) == NULL;

				Ret |= ( __glFramebufferTexture2D = ( PFNGLFRAMEBUFFERTEXTURE2DPROC )
					glGetProcAddress( "glFramebufferTexture2D" ) ) == NULL;

				Ret |= ( __glGenFramebuffers = ( PFNGLGENFRAMEBUFFERSPROC )
					glGetProcAddress( "glGenFramebuffers" ) ) == NULL;

				Ret |= ( __glGenrRenderbuffers = ( PFNGLGENRENDERBUFFERSPROC )
					glGetProcAddress( "glGenrRenderbuffers" ) ) == NULL;

				Ret |= ( __glRenderbufferStorage = ( PFNGLRENDERBUFFERSTORAGEPROC )
					glGetProcAddress( "glRenderbufferStorage" ) ) == NULL;

				if( !Ret )
				{
					BIT_ARB_framebuffer_object = false;
				}
			}

			// Reset the return flag
			Ret = 0;

			
			if( p_Major > 1 || ( p_Major == 1 && p_Minor >= 3 ) )
			{
				Ret |= ( __glActiveTexture = ( PFNGLACTIVETEXTUREPROC )
					glGetProcAddress( "glActiveTexture" ) ) == NULL;

				if( !Ret )
				{
					s_GeneralTextureFunctions = true;
				}
			}

			// Reset the return flag
			Ret = 0;

			// Get the buffer functions
			if( p_Major > 1 || ( p_Major == 1 && p_Minor >= 5 ) )
			{

				Ret |= ( __glBindBuffer = ( PFNGLBINDBUFFERPROC )
					glGetProcAddress( "glBindBuffer" ) ) == NULL;

				Ret |= ( __glBufferData = ( PFNGLBUFFERDATAPROC )
					glGetProcAddress( "glBufferData" ) ) == NULL;

				Ret |= ( __glBufferSubData = ( PFNGLBUFFERSUBDATAPROC )
					glGetProcAddress( "glBufferSubData" ) ) == NULL;

				Ret |= ( __glDeleteBuffers = ( PFNGLDELETEBUFFERSPROC )
					glGetProcAddress( "glDeleteBuffers" ) ) == NULL;

				Ret |= ( __glGenBuffers = ( PFNGLGENBUFFERSPROC )
					glGetProcAddress( "glGenBuffers" ) ) == NULL;

				if( !Ret )
				{
					s_GeneralBuffersFunctions = true;
				}
			}

			// Reset the return flag
			Ret = 0;

			// Get the vertex array object functions if they are available
			if( BIT_ARB_vertex_array_object )
			{
				Ret |= ( __glBindVertexArray = ( PFNGLBINDVERTEXARRAYPROC )
					glGetProcAddress( "glBindVertexArray" ) ) == NULL;

				Ret |= ( __glDeleteVertexArrays = ( PFNGLDELETEVERTEXARRAYSPROC )
					glGetProcAddress( "glDeleteVertexArrays" ) ) == NULL;

				Ret |= ( __glGenVertexArrays = ( PFNGLGENVERTEXARRAYSPROC )
					glGetProcAddress( "glGenVertexArrays" ) ) == NULL;

				Ret |= ( __glIsVertexArray = ( PFNGLISVERTEXARRAYPROC )
					glGetProcAddress( "glIsVertexArray" ) ) == NULL;

				if( Ret )
				{
					BIT_ARB_vertex_array_object = false;
				}
			}

			// Reset the return flag
			Ret = 0;

			// Load the shader functions if the shader extenstions are available
			// and we are using the right verison of OpenGL
			// WARNING: comparison of unsigned expression >= 0 is always true [-Wtype-limits]|
			if( p_Major >= 2 )
			{
				// Needed for the vertex objects.
				Ret |= ( __glEnableVertexAttribArray = ( PFNGLENABLEVERTEXATTRIBARRAYPROC )
					glGetProcAddress( "glEnableVertexAttribArray" ) ) == NULL;

				Ret |= ( __glVertexAttribPointer = ( PFNGLVERTEXATTRIBPOINTERPROC )
					glGetProcAddress( "glVertexAttribPointer" ) ) == NULL;

				Ret |=  ( __glAttachShader = ( PFNGLATTACHSHADERPROC )
					 glGetProcAddress( "glAttachShader" ) ) == NULL;

				Ret |= ( __glCompileShader = ( PFNGLCOMPILESHADERPROC )
					glGetProcAddress( "glCompileShader" ) ) == NULL;

				Ret |= ( __glCreateProgram = ( PFNGLCREATEPROGRAMPROC )
					glGetProcAddress( "glCreateProgram" ) ) == NULL;

				Ret |= ( __glCreateShader = ( PFNGLCREATESHADERPROC )
					glGetProcAddress( "glCreateShader" ) ) == NULL;

				Ret |= ( __glDeleteProgram = ( PFNGLDELETEPROGRAMPROC )
					glGetProcAddress( "glDeleteProgram" ) ) == NULL;

				Ret |= ( __glDeleteShader = ( PFNGLDELETESHADERPROC )
					glGetProcAddress( "glDeleteShader" ) ) == NULL;

				Ret |= ( __glGetProgramInfoLog = ( PFNGLGETPROGRAMINFOLOGPROC )
					glGetProcAddress( "glGetProgramInfoLog" ) ) == NULL;

				Ret |= ( __glGetProgramiv = ( PFNGLGETPROGRAMIVPROC )
					glGetProcAddress( "glGetProgramiv" ) ) == NULL;

				Ret |= ( __glGetShaderInfoLog = ( PFNGLGETSHADERINFOLOGPROC )
					glGetProcAddress( "glGetShaderInfoLog" ) ) == NULL;

				Ret |= ( __glGetShaderiv = ( PFNGLGETSHADERIVPROC )
					glGetProcAddress( "glGetShaderiv" ) ) == NULL;

				Ret |= ( __glGetUniformLocation = ( PFNGLGETUNIFORMLOCATIONPROC )
					glGetProcAddress( "glGetUniformLocation" ) ) == NULL;

				Ret |= ( __glLinkProgram = ( PFNGLLINKPROGRAMPROC )
					glGetProcAddress( "glLinkProgram" ) ) == NULL;

				Ret |= ( __glShaderSource = ( PFNGLSHADERSOURCEPROC )
					glGetProcAddress( "glShaderSource" ) ) == NULL;

				Ret |= ( __glValidateProgram = ( PFNGLVALIDATEPROGRAMPROC )
					glGetProcAddress( "glValidateProgram" ) ) == NULL;

				Ret |= ( __glBindAttribLocation = ( PFNGLBINDATTRIBLOCATIONPROC )
					glGetProcAddress( "glBindAttribLocation" ) ) == NULL;

				Ret |= ( __glGetAttribLocation = ( PFNGLGETATTRIBLOCATIONPROC )
					glGetProcAddress( "glGetAttribLocation" ) ) == NULL;

				Ret |= ( __glUseProgram = ( PFNGLUSEPROGRAMPROC )
					glGetProcAddress( "glUseProgram" ) ) == NULL;

				// Uniforms
				Ret |= ( __glUniform1f = ( PFNGLUNIFORM1FPROC )
					glGetProcAddress( "glUniform1f" ) ) == NULL;

				Ret |= ( __glUniform1fv = ( PFNGLUNIFORM1FVPROC )
					glGetProcAddress( "glUniform1fv" ) ) == NULL;

				Ret |= ( __glUniform1i = ( PFNGLUNIFORM1IPROC )
					glGetProcAddress( "glUniform1i" ) ) == NULL;

				Ret |= ( __glUniform1iv = ( PFNGLUNIFORM1IVPROC )
					glGetProcAddress( "glUniform1iv" ) ) == NULL;

				Ret |= ( __glUniform2f = ( PFNGLUNIFORM2FPROC )
					glGetProcAddress( "glUniform2f" ) ) == NULL;

				Ret |= ( __glUniform2fv = ( PFNGLUNIFORM2FVPROC )
					glGetProcAddress( "glUniform2fv" ) ) == NULL;

				Ret |= ( __glUniform2i = ( PFNGLUNIFORM2IPROC )
					glGetProcAddress( "glUniform2i" ) ) == NULL;

				Ret |= ( __glUniform2iv = ( PFNGLUNIFORM2IVPROC )
					glGetProcAddress( "glUniform2iv" ) ) == NULL;

				Ret |= ( __glUniform3f = ( PFNGLUNIFORM3FPROC )
					glGetProcAddress( "glUniform3f" ) ) == NULL;

				Ret |= ( __glUniform3fv = ( PFNGLUNIFORM3FVPROC )
					glGetProcAddress( "glUniform3fv" ) ) == NULL;

				Ret |= ( __glUniform3i = ( PFNGLUNIFORM3IPROC )
					glGetProcAddress( "glUniform3i" ) ) == NULL;

				Ret |= ( __glUniform3iv = ( PFNGLUNIFORM3IVPROC )
					glGetProcAddress( "glUniform3iv" ) ) == NULL;

				Ret |= ( __glUniform4f = ( PFNGLUNIFORM4FPROC )
					glGetProcAddress( "glUniform4f" ) ) == NULL;

				Ret |= ( __glUniform4fv = ( PFNGLUNIFORM4FVPROC )
					glGetProcAddress( "glUniform4fv" ) ) == NULL;

				Ret |= ( __glUniform4i = ( PFNGLUNIFORM4IPROC )
					glGetProcAddress( "glUniform4i" ) ) == NULL;

				Ret |= ( __glUniform4iv = ( PFNGLUNIFORM4IVPROC )
					glGetProcAddress( "glUniform4iv" ) ) == NULL;

				Ret |= ( __glUniformMatrix2fv = ( PFNGLUNIFORMMATRIX2FVPROC )
					glGetProcAddress( "glUniformMatrix2fv" ) ) == NULL;

				Ret |= ( __glUniformMatrix3fv = ( PFNGLUNIFORMMATRIX3FVPROC )
					glGetProcAddress( "glUniformMatrix3fv" ) ) == NULL;

				Ret |= ( __glUniformMatrix4fv = ( PFNGLUNIFORMMATRIX4FVPROC )
					glGetProcAddress( "glUniformMatrix4fv" ) ) == NULL;


				// Error check the OpenGL shader functions
				if( Ret == 0 )
				{
					s_AllShaderFunctions = true;
				}

			}


			return true;
		}

		BIT_API Bool IsVertexObjectAvailability( )
		{
			return	BIT_ARB_vertex_array_object;
		}

		BIT_API Bool IsGeneralTextureAvailability( )
		{
			return s_GeneralTextureFunctions;
		}

		BIT_API Bool IsGeneralBufferAvailability( )
		{
			return s_GeneralBuffersFunctions;
		}

		BIT_API Bool IsShaderAvailability( )
		{
			return	s_AllShaderFunctions &&
					BIT_ARB_shading_language_100 &&
					BIT_ARB_shader_objects &&
					BIT_ARB_vertex_shader &&
					BIT_ARB_fragment_shader;
		}

		BIT_API Bool IsFramebufferAvailability( )
		{
			return BIT_ARB_framebuffer_object;
		}

	}

}
