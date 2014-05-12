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

#ifndef BIT_GRAPHICS_OPENGL_HPP
#define BIT_GRAPHICS_OPENGL_HPP

#include <Bit/Build.hpp>

// Include the required headers to run opengl
#ifdef BIT_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <GL/wglext.h>
#elif defined( BIT_PLATFORM_LINUX )
	#include <X11/Xlib.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glext.h>
	#include <GL/glxext.h>
#endif

// Function for grabbing the opengl functions
#if defined(BIT_PLATFORM_WINDOWS)
	#define glGetProcAddress( p_Ext ) wglGetProcAddress( p_Ext )
#elif defined( BIT_PLATFORM_LINUX )
	#define glGetProcAddress( p_Ext ) glXGetProcAddress( ( const GLubyte *) p_Ext )
#endif


// ///////////////////////////////////////////////////////////////////
// Opengl "private" extensions
extern PFNGLGETSTRINGIPROC __glGetStringi;

// Vertex buffer object functions
extern PFNGLBINDVERTEXARRAYPROC __glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC __glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC __glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC __glIsVertexArray;

// Texture, OpenGL 1.3
extern PFNGLACTIVETEXTUREPROC __glActiveTexture;

// Buffers, OpenGL 1.5
extern PFNGLBINDBUFFERPROC __glBindBuffer;
extern PFNGLBUFFERDATAPROC __glBufferData;
extern PFNGLBUFFERSUBDATAPROC __glBufferSubData;
extern PFNGLDELETEBUFFERSPROC __glDeleteBuffers;
extern PFNGLGENBUFFERSPROC __glGenBuffers;

// Framebuffers/renderbuffers, OpenGL 3.0
extern PFNGLBLITFRAMEBUFFERPROC __glBlitFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC __glBindFramebuffer;
extern PFNGLBINDRENDERBUFFERPROC __glBindRenderbuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC __glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC __glDeleteFramebuffers;
extern PFNGLDELETERENDERBUFFERSPROC __glDeleteRenderbuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC __glFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC __glFramebufferTexture2D;
extern PFNGLGENFRAMEBUFFERSPROC __glGenFramebuffers;
extern PFNGLGENRENDERBUFFERSPROC __glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC __glRenderbufferStorage;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC __glRenderbufferStorageMultisample;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC __glTexImage2DMultisample;


// Texture functions
extern PFNGLGENERATEMIPMAPPROC __glGenerateMipmap;

// Shader functions
extern PFNGLENABLEVERTEXATTRIBARRAYPROC __glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC __glVertexAttribPointer;
extern PFNGLATTACHSHADERPROC __glAttachShader;
extern PFNGLCOMPILESHADERPROC __glCompileShader;
extern PFNGLCREATEPROGRAMPROC __glCreateProgram;
extern PFNGLCREATESHADERPROC __glCreateShader;
extern PFNGLDELETEPROGRAMPROC __glDeleteProgram;
extern PFNGLDELETESHADERPROC __glDeleteShader;
extern PFNGLGETPROGRAMINFOLOGPROC __glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC __glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC __glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC __glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC __glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC __glLinkProgram;
extern PFNGLSHADERSOURCEPROC __glShaderSource;
extern PFNGLVALIDATEPROGRAMPROC __glValidateProgram;
extern PFNGLBINDATTRIBLOCATIONPROC __glBindAttribLocation;
extern PFNGLGETATTRIBLOCATIONPROC __glGetAttribLocation;
extern PFNGLUSEPROGRAMPROC __glUseProgram;

// Shader uniforms
extern PFNGLUNIFORM1FPROC __glUniform1f;
extern PFNGLUNIFORM1FVPROC __glUniform1fv;
extern PFNGLUNIFORM1IPROC __glUniform1i;
extern PFNGLUNIFORM1IVPROC __glUniform1iv;
extern PFNGLUNIFORM2FPROC __glUniform2f;
extern PFNGLUNIFORM2FVPROC __glUniform2fv;
extern PFNGLUNIFORM2IPROC __glUniform2i;
extern PFNGLUNIFORM2IVPROC __glUniform2iv;
extern PFNGLUNIFORM3FPROC __glUniform3f;
extern PFNGLUNIFORM3FVPROC __glUniform3fv;
extern PFNGLUNIFORM3IPROC __glUniform3i;
extern PFNGLUNIFORM3IVPROC __glUniform3iv;
extern PFNGLUNIFORM4FPROC __glUniform4f;
extern PFNGLUNIFORM4FVPROC __glUniform4fv;
extern PFNGLUNIFORM4IPROC __glUniform4i;
extern PFNGLUNIFORM4IVPROC __glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC __glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC __glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC __glUniformMatrix4fv;




// ///////////////////////////////////////////////////////////////////
// Opengl extensions, use these
#define glGetStringi __glGetStringi

// Vertex buffer object functions
#define glBindVertexArray __glBindVertexArray
#define glDeleteVertexArrays __glDeleteVertexArrays
#define glGenVertexArrays __glGenVertexArrays
#define glIsVertexArray __glIsVertexArray

// Texture, OpenGL 1.3
#define glActiveTexture __glActiveTexture

// Buffers, OpenGL 1.5
#define glBindBuffer __glBindBuffer
#define glBufferData __glBufferData
#define glBufferSubData __glBufferSubData
#define glDeleteBuffers __glDeleteBuffers
#define glGenBuffers __glGenBuffers

// Framebuffers/renderbuffers, OpenGL 3.0

#define glBlitFramebuffer __glBlitFramebuffer
#define glBindFramebuffer __glBindFramebuffer
#define glBindRenderbuffer __glBindRenderbuffer
#define glCheckFramebufferStatus __glCheckFramebufferStatus
#define glDeleteFramebuffers __glDeleteFramebuffers
#define glDeleteRenderbuffers __glDeleteRenderbuffers
#define glFramebufferRenderbuffer __glFramebufferRenderbuffer
#define glFramebufferTexture2D __glFramebufferTexture2D
#define glGenFramebuffers __glGenFramebuffers
#define glGenRenderbuffers __glGenRenderbuffers
#define glRenderbufferStorage __glRenderbufferStorage
#define glRenderbufferStorageMultisample __glRenderbufferStorageMultisample
#define glTexImage2DMultisample __glTexImage2DMultisample

// Texture functions
#define glGenerateMipmap __glGenerateMipmap

// Shader functions
#define glEnableVertexAttribArray __glEnableVertexAttribArray
#define glVertexAttribPointer __glVertexAttribPointer
#define glAttachShader __glAttachShader
#define glCompileShader __glCompileShader
#define glCreateProgram __glCreateProgram
#define glCreateShader __glCreateShader
#define glDeleteProgram __glDeleteProgram
#define glDeleteShader __glDeleteShader
#define glGetProgramInfoLog __glGetProgramInfoLog
#define glGetProgramiv __glGetProgramiv
#define glGetShaderInfoLog __glGetShaderInfoLog
#define glGetShaderiv __glGetShaderiv
#define glGetUniformLocation __glGetUniformLocation
#define glLinkProgram __glLinkProgram
#define glShaderSource __glShaderSource
#define glValidateProgram __glValidateProgram
#define glBindAttribLocation __glBindAttribLocation
#define glGetAttribLocation __glGetAttribLocation
#define glUseProgram __glUseProgram

#define glUniform1f __glUniform1f
#define glUniform1fv __glUniform1fv
#define glUniform1i __glUniform1i
#define glUniform1iv __glUniform1iv
#define glUniform2f __glUniform2f
#define glUniform2fv __glUniform2fv
#define glUniform2i __glUniform2i
#define glUniform2iv __glUniform2iv
#define glUniform3f __glUniform3f
#define glUniform3fv __glUniform3fv
#define glUniform3i __glUniform3i
#define glUniform3iv __glUniform3iv
#define glUniform4f __glUniform4f
#define glUniform4fv __glUniform4fv
#define glUniform4iP __glUniform4i
#define glUniform4iv __glUniform4iv
#define glUniformMatrix2fv __glUniformMatrix2fv
#define glUniformMatrix3fv __glUniformMatrix3fv
#define glUniformMatrix4fv __glUniformMatrix4fv


namespace Bit
{
	namespace OpenGL
	{

		BIT_API Bool BindOpenGLExtensions( const Uint32 p_Major, const Uint32 p_Minor );
		BIT_API Bool IsVertexObjectAvailable( );
		BIT_API Bool IsGeneralTextureAvailable( );
		BIT_API Bool IsGeneralBufferAvailable( );
		BIT_API Bool IsShaderAvailable( );				///< Check if shaders are available
		BIT_API Bool IsFramebufferAvailable( );			///< Check if framebuffers/renderbuffers are available
		BIT_API Bool IsAnisotropicFilterAvailable( );	///< Check if anisotropic filter is available
		BIT_API Uint32 GetAnisotropicMaxLevel( );		///< Get the max anisotropic level.

	}
}


#endif
