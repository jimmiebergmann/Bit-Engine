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

#ifndef __BIT_GRAPHICS_OPENGL_HPP__
#define __BIT_GRAPHICS_OPENGL_HPP__

#include <Bit/DataTypes.hpp>

// Include the required headers to run opengl
#ifdef BIT_PLATFORM_WIN32
	#include <Windows.h>
	#undef CreateWindow
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <GL/wglext.h>
#elif BIT_PLATFORM_LINUX
	/*#include <X11/Xlib.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glext.h>
	#include <GL/glxext.h>*/
#endif

// Function for grabbing the opengl functions
#ifdef BIT_PLATFORM_WIN32
	#define glGetProcAddress( p_Ext ) wglGetProcAddress( p_Ext )
#elif BIT_PLATFORM_LINUX
	#error glGetProcAddress function is defined
#endif


// ///////////////////////////////////////////////////////////////////
// Opengl "private" extensions
extern PFNGLGETSTRINGIPROC __glGetStringi;
//static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;

// Vertex buffer object functions
extern PFNGLBINDVERTEXARRAYPROC __glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC __glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC __glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC __glIsVertexArray;

// Buffers, OpenGL 1.5
extern PFNGLBINDBUFFERPROC __glBindBuffer;
extern PFNGLBUFFERDATAPROC __glBufferData;
extern PFNGLBUFFERSUBDATAPROC __glBufferSubData;
extern PFNGLDELETEBUFFERSPROC __glDeleteBuffers;
extern PFNGLGENBUFFERSPROC __glGenBuffers;


// Shader functions
extern PFNGLATTACHSHADERPROC __glAttachShader;
extern PFNGLCOMPILESHADERPROC __glCompileShader;
extern PFNGLCREATEPROGRAMPROC __glCreateProgram;
extern PFNGLCREATESHADERPROC __glCreateShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC __glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC __glVertexAttribPointer;


/*
static PFNGLDELETEPROGRAMPROC __glGetStringi = BIT_NULL;
static PFNGLDELETESHADERPROC __glGetStringi = BIT_NULL;
static PFNGLGETPROGRAMINFOLOGPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
static PFNGLBINDBUFFERPROC __glGetStringi = BIT_NULL;
*/

// ///////////////////////////////////////////////////////////////////
// Opengl extensions, use these
#define glGetStringi __glGetStringi

// Vertex buffer object functions
#define glBindVertexArray __glBindVertexArray
#define glDeleteVertexArrays __glDeleteVertexArrays
#define glGenVertexArrays __glGenVertexArrays
#define glIsVertexArray __glIsVertexArray

// Buffers, OpenGL 1.5
#define  glBindBuffer __glBindBuffer
#define  glBufferData __glBufferData
#define  glBufferSubData __glBufferSubData
#define  glDeleteBuffers __glDeleteBuffers
#define  glGenBuffers __glGenBuffers

// Shader functions
#define glAttachShader __glAttachShader
#define glCompileShader __glCompileShader
#define glCreateProgram __glCreateProgram
#define glCreateShader __glCreateShader
#define glEnableVertexAttribArray __glEnableVertexAttribArray
#define glVertexAttribPointer __glVertexAttribPointer


namespace Bit
{
	namespace OpenGL
	{

		BIT_API BIT_UINT32 BindOpenGLExtensions( const BIT_UINT32 p_Major, const BIT_UINT32 p_Minor );
		BIT_API BIT_BOOL GetVertexObjectAvailability( );
		BIT_API BIT_BOOL GetGeneralBufferAvailability( );
		BIT_API BIT_BOOL GetShaderAvailability( ); // Check if shaders are available

	}
}


#endif