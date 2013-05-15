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

#include <Bit/Graphics/OpenGL/ShaderProgramOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderOpenGL.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	ShaderProgramOpenGL::ShaderProgramOpenGL( ) :
		m_ProgramID(0),
		m_AttachedShaderCount(0)
	{
		m_Linked = BIT_FALSE;

		// Create the shader program
		m_ProgramID = glCreateProgram( );
	}

	ShaderProgramOpenGL::~ShaderProgramOpenGL()
	{
		m_AttachedShaderCount = 0;
	}

	BIT_UINT32 ShaderProgramOpenGL::AttachShaders( Shader *p_pShader )
	{
		// Make sure the shader program is loaded before we attach any shaders.
		if( m_ProgramID == 0 )
		{
			bitTrace( BIT_NULL, "[Bit::ShaderProgramOGL::AttachShaders] <ERROR> "
				"Shader program was not initialised\n" );
			return BIT_ERROR;
		}

		// Not supporting more than 2 shaders atm
		if( m_AttachedShaderCount == 2 )
		{
			bitTrace( NULL, "[Bit::ShaderProgramOGL::AttachShaders] <ERROR> "
				"Too many shaders attached already: %i/2\n", m_AttachedShaderCount );
			return BIT_ERROR;
		}

		ShaderOpenGL * pShaderOpenGL = reinterpret_cast< ShaderOpenGL * >( p_pShader );

		// Attach the shader
		glAttachShader( m_ProgramID, pShaderOpenGL->GetShaderObject( ) );

		m_AttachedShaderCount++;

		return BIT_OK;
	}

	BIT_UINT32 ShaderProgramOpenGL::Link( std::string & p_Validation )
	{
		// Make sure we have attached any shaders at all to the shader program
		if( m_AttachedShaderCount < 2 )
		{
			bitTrace( BIT_NULL, "[Bit::ShaderProgramOGL::Link] <ERROR> "
				"Not enough shader objects are attached: %ld reported\n",
				m_AttachedShaderCount );
			return BIT_ERROR;
		}

		// Link the program
		glLinkProgram( m_ProgramID );

		// Make sure the shaders linked correctly
		GLint LinkStatus = GL_FALSE;
		glGetProgramiv( m_ProgramID, GL_LINK_STATUS, &LinkStatus );
		if( LinkStatus == GL_FALSE )
		{
			bitTrace( BIT_NULL, "[Bit::ShaderProgramOGL::Link] <ERROR> "
				"Failed to link program:\n" );

			GLsizei Length = 0;
			GLchar *pLog;

			glGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH, &Length );
			pLog = new GLchar[ Length ];

			glGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );

			bitTrace( BIT_NULL, "\t%s\n", pLog );

			p_Validation = pLog;
			delete [ ] pLog;

			return BIT_ERROR;
		}

		// Get the validation string (Useful for debugging shaders)
//		p_Validation = ShaderOGL::ValidateShader( m_ProgramID );

		m_Linked = BIT_TRUE;
		return BIT_OK;
	}

	void ShaderProgramOpenGL::Bind()
	{
		glUseProgram( m_ProgramID );
	}

	void ShaderProgramOpenGL::Unbind()
	{
		glUseProgram( 0 );
	}


	void ShaderProgramOpenGL::SetUniform1i( const char * p_Location,  const BIT_SINT32 p_A )
	{
		//int UniformLocation = glGetUniformLocation( m_ProgramID, p_Location );
		//glUniform1i( UniformLocation, (GLint)p_A );
	}

	void ShaderProgramOpenGL::SetUniform1f( const char * p_Location, const BIT_FLOAT32 p_A )
	{
		//int UniformLocation = glGetUniformLocation( m_ProgramID, p_Location );
		//glUniform1f( UniformLocation, (GLfloat)p_A );
	}

	void ShaderProgramOpenGL::SetUniform2f( const char * p_Location, const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B )
	{
		//int UniformLocation = glGetUniformLocation( m_ProgramID, p_Location );
		//glUniform2f( UniformLocation, (GLfloat)p_A, (GLfloat)p_B );
	}

	void ShaderProgramOpenGL::SetUniform3f( const char * p_Location, const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_C )
	{
		//int UniformLocation = glGetUniformLocation( m_ProgramID, p_Location );
		//glUniform3f( UniformLocation, (GLfloat)p_A, (GLfloat)p_B, (GLfloat)p_C );
	}

	void ShaderProgramOpenGL::SetUniform4f( const char * p_Location, const BIT_FLOAT32 p_A, const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_C, const BIT_FLOAT32 p_D )
	{
		//int UniformLocation = glGetUniformLocation( m_ProgramID, p_Location );
		//glUniform4f( UniformLocation, (GLfloat)p_A, (GLfloat)p_B, (GLfloat)p_C, (GLfloat)p_D );
	}

	void ShaderProgramOpenGL::SetUniformMatrix4x4f( const char * p_Location, Matrix4x4 & p_Matrix )
	{

		/*int UniformLocation = bglGetUniformLocation( m_ProgramID, p_Location );

		BIT_FLOAT32 MatrixData[BIT_MATRIX4X4_SIZE];
		p_Matrix.GetMatrix( MatrixData );
		glUniformMatrix4fv( UniformLocation, 1, GL_FALSE, (GLfloat*)MatrixData );
*/
	}
	void ShaderProgramOpenGL::SetAttributeLocation( const char * p_Location, BIT_UINT32 p_Index )
	{

		//glBindAttribLocation(m_ProgramID, (GLuint)p_Index, p_Location);
	}

}
