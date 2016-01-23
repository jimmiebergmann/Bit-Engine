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

#include <Bit/Graphics/OpenGL/OpenGLShaderProgram.hpp>
#include <Bit/Graphics/OpenGL/OpenGLShader.hpp>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	OpenGLShaderProgram::OpenGLShaderProgram( ) :
		m_ProgramID( 0 ),
		m_Linked( false ),
		m_AttachedShaderCount( 0 )
	{
		// Create the shader program
		m_ProgramID = glCreateProgram( );
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		if( m_ProgramID != 0 )
		{
			glDeleteProgram( m_ProgramID );
			m_ProgramID = 0;
		}

		m_AttachedShaderCount = 0;
	}

	Bool OpenGLShaderProgram::AttachShader( const Shader & p_pShader )
	{
		// Make sure the shader program is loaded before we attach any shaders.
		if( m_ProgramID == 0 )
		{
			BitLog::NewEngine(Log::Error,  "Shader program was not initialised." );
			return false;
		}

		// Not supporting more than 2 shaders atm
		if( m_AttachedShaderCount == 2 )
		{
			BitLog::NewEngine(Log::Error, "Too many shaders attached already: %i",  m_AttachedShaderCount );
			return false;
		}

		const OpenGLShader * pShaderOpenGL = reinterpret_cast< const OpenGLShader * >( &p_pShader );

		// Attach the shader
		glAttachShader( m_ProgramID, pShaderOpenGL->GetShaderObject( ) );

		// Increment the attachment count
		m_AttachedShaderCount++;

		// Succeeded
		return true;
	}

	Bool OpenGLShaderProgram::Link( )
	{
		// Make sure we have attached any shaders at all to the shader program
		if( m_AttachedShaderCount < 2 )
		{
			BitLog::NewEngine(Log::Error,  "Not enough shader objects are attached: %i", m_AttachedShaderCount );
			return false;
		}

		// Link the program
		glLinkProgram( m_ProgramID );

		// Get log message
		// Get the length
		GLsizei logLength = 0;
		glGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH, &logLength );

		// Get the error message
		if( logLength > 1 )
		{
			GLchar * pLog = new GLchar[ logLength ];
			glGetProgramInfoLog( m_ProgramID, logLength, &logLength, pLog );
			BitLog::NewEngine(Log::Error,  "Shader link error: %s",  pLog );
			delete [ ] pLog;
		}

		// Make sure the shaders linked correctly
		GLint linkStatus = GL_FALSE;
		glGetProgramiv( m_ProgramID, GL_LINK_STATUS, &linkStatus );
		// Get the error
		if( linkStatus == GL_FALSE )
		{
			return false;
		}



		m_Linked = true;
		return true;
	}

	void OpenGLShaderProgram::Bind()
	{
		glUseProgram( m_ProgramID );
	}

	void OpenGLShaderProgram::Unbind()
	{
		glUseProgram( 0 );
	}

	void OpenGLShaderProgram::SetAttributeLocation( const char * p_Name, const Uint32 p_Index )
	{
		glBindAttribLocation( m_ProgramID, (GLuint)p_Index, p_Name);
	}

	void OpenGLShaderProgram::SetUniform1i( const char * p_Name, const Int32 p_A )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniform1i( uniformLocation, (GLint)p_A );
	}

	void OpenGLShaderProgram::SetUniform1f( const char * p_Name, const Float32 p_A )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniform1f( uniformLocation, (GLfloat)p_A );
	}

	void OpenGLShaderProgram::SetUniform2f( const char * p_Name, const Float32 p_A, const Float32 p_B )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniform2f( uniformLocation, (GLfloat)p_A, (GLfloat)p_B );
	}

	void OpenGLShaderProgram::SetUniform3f( const char * p_Name, const Float32 p_A, const Float32 p_B, const Float32 p_C )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniform3f( uniformLocation, (GLfloat)p_A, (GLfloat)p_B, (GLfloat)p_C );
	}

	void OpenGLShaderProgram::SetUniform4f( const char * p_Name, const Float32 p_A, const Float32 p_B, const Float32 p_C, const Float32 p_D )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniform4f( uniformLocation, (GLfloat)p_A, (GLfloat)p_B, (GLfloat)p_C, (GLfloat)p_D );
	}

	void OpenGLShaderProgram::SetUniformMatrix4x4f( const char * p_Name, const Matrix4x4f32 & p_Matrix )
	{
		int uniformLocation = glGetUniformLocation( m_ProgramID, p_Name );
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, (GLfloat*)p_Matrix.m );
	}

}
