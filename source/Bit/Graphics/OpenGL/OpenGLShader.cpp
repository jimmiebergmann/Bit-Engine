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


#include <Bit/Graphics/OpenGL/OpenGLShader.hpp>
#include <cstring>
#include <fstream>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// global variables
	static GLenum g_ShaderObjectTypes[ 2 ] = { GL_VERTEX_SHADER_ARB, GL_FRAGMENT_SHADER_ARB };

	OpenGLShader::OpenGLShader( const ShaderType::eType p_Type ) :
		m_Compiled( false ),
		m_ShaderObject( 0 ),
		m_Type( p_Type )
	{
		// Create the shader object
		m_ShaderObject = glCreateShader( g_ShaderObjectTypes[ m_Type ] );
	}

	OpenGLShader::~OpenGLShader( )
	{
		if( m_ShaderObject != 0 )
		{
			glDeleteShader( m_ShaderObject );
			m_ShaderObject = 0;
		}

		m_Compiled = false;
	}

	Bool OpenGLShader::CompileFromFile( const std::string & p_Filename )
	{
		// Use fstream for reading files.
		std::ifstream fin( p_Filename.c_str( ) );
		if( fin.is_open( ) == false )
		{
			return false;
		}

		// Get the file size
		fin.seekg( 0, std::ios::end );
		SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
		fin.seekg( 0, std::ios::beg );

		// Create a source string
		std::string source;
		source.reserve( fileSize );

		// Read all the file's lines
		std::string line = "";

		while( !fin.eof( ) )
		{
			std::getline( fin, line );
			source += line + "\n";
		}

		// Close the file, we are done.
		fin.close( );

		// Compile the shader from memory( the soruce we just read from the file )
		return CompileFromMemory( source );
	}

	Bool OpenGLShader::CompileFromMemory( const std::string & p_Memory )
	{
		// Already compiled?
		if( m_Compiled == true )
		{
			BitLog::NewEngine(Log::Error) << "The shader is already compiled." << Log::End;
			return false;
		}

		// Make sure we have a shader source
		if( p_Memory.length() == 0 )
		{
			BitLog::NewEngine(Log::Error) << "No shader source." << Log::End;
			return false;
		}

		const GLcharARB * pTextSource = reinterpret_cast<const GLcharARB *>( p_Memory.c_str( ) );

		// Might be a silly erorr check. But I'm doing it anyway.
		if( pTextSource == NULL )
		{
			BitLog::NewEngine(Log::Error) << "Silly pointer error!" << Log::End;
			return false;
		}

		// Compile the vertex shader using it's source
		glShaderSource( m_ShaderObject, 1, &pTextSource, 0 );
		glCompileShader( m_ShaderObject );

		// Did the shader compile without incident?
		GLint Compiled = 0;
		glGetShaderiv( m_ShaderObject, GL_COMPILE_STATUS, &Compiled );

		if( !Compiled )
		{
			// Get the length of the error
			GLint logLength = 0;
			glGetShaderiv( m_ShaderObject, GL_INFO_LOG_LENGTH, &logLength );

			if( logLength > 1 )
			{
				// Get the error message
				GLchar * pLog = new GLchar[ logLength ];
				glGetShaderInfoLog( m_ShaderObject, logLength, &logLength, pLog );
				BitLog::NewEngine(Log::Error) << "Shader compiler error:" << pLog << Log::End;
				delete [ ] pLog;
			}

			return false;
		}

		m_Compiled = true;
		return true;
	}

	Bool OpenGLShader::IsCompiled( )
	{
		return m_Compiled;
	}

	ShaderType::eType OpenGLShader::GetType( )
	{
		return m_Type;
	}

	GLuint OpenGLShader::GetShaderObject( ) const
	{
		return m_ShaderObject;
	}

}
