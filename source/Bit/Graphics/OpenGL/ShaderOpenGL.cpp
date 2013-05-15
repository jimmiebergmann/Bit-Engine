#include <Bit/Graphics/OpenGL/ShaderOpenGL.hpp>
#include <cstring>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	GLenum ShaderOpenGL::s_ShaderObjectTypes[3] = { 0, GL_VERTEX_SHADER_ARB, GL_FRAGMENT_SHADER_ARB };


	ShaderOpenGL::ShaderOpenGL( eShaderType p_ShaderType ) :
		m_ShaderObject(0)
	{
		m_Compiled = BIT_FALSE;
		m_ShaderType = p_ShaderType;
		m_ShaderSource = "";
		m_FilePath = "";

		// Create the shader object
		m_ShaderObject = glCreateShader( s_ShaderObjectTypes[ p_ShaderType ] );
	}

	ShaderOpenGL::~ShaderOpenGL( )
	{
		if( m_ShaderObject != 0 )
		{
			glDeleteShader( m_ShaderObject );
			m_ShaderObject = 0;
		}

		m_Compiled = BIT_FALSE;
	}

	BIT_UINT32 ShaderOpenGL::Compile( )
	{
		// Already compiled?
		if( m_Compiled == BIT_TRUE )
		{
			bitTrace( NULL, "[ShaderOpenGL::Compile] The shader is already compiled.\n" );
			return BIT_ERROR;
		}

		// Make sure we have a shader source
		if( m_ShaderSource.length() == 0 )
		{
			bitTrace( NULL, "[ShaderOpenGL::Compile] No shader source.\n" );
			return BIT_ERROR;
		}

		const GLcharARB * pTextSource = m_ShaderSource.c_str( );

		// Might be a silly erorr check. But I'm doing it anyway.
		if( pTextSource == BIT_NULL )
		{
			bitTrace( BIT_NULL, "[Bit::ShaderOGL::Compile] <ERROR> "
				"Silly pointer error!" );
			return BIT_ERROR;
		}

		// Compile the vertex shader using it's source
		glShaderSource( m_ShaderObject, 1, &pTextSource, 0 );
		glCompileShader( m_ShaderObject );

		// Did the shader compile without incident?
		GLint Compiled = 0;
		glGetShaderiv( m_ShaderObject, GL_COMPILE_STATUS, &Compiled );

		if( !Compiled )
		{
			GLint LogLength = 0;
			glGetShaderiv( m_ShaderObject, GL_INFO_LOG_LENGTH, &LogLength );

			if( LogLength > 1 )
			{
				char *pLog = new char[ LogLength ];
				glGetShaderInfoLog( m_ShaderObject, LogLength, BIT_NULL, pLog );
				bitTrace( BIT_NULL, "[ShaderOpenGL::Compile] Shader compiler error:\n%s\n", pLog );
				delete [ ] pLog;
			}

			return BIT_ERROR;
		}
		
		m_Compiled = BIT_TRUE;
		return BIT_OK;
	}

	GLuint ShaderOpenGL::GetShaderObject( ) const
	{
		return m_ShaderObject;
	}

}
