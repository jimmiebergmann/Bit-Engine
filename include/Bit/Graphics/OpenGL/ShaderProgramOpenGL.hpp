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

#ifndef __BIT_GRAPHICS_SHADER_PROGRAM_OPENGL_HPP__
#define __BIT_GRAPHICS_SHADER_PROGRAM_OPENGL_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/ShaderProgram.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	class ShaderProgramOpenGL : public ShaderProgram
	{

	public:

		// Constructor/destructor
		ShaderProgramOpenGL( );
		virtual ~ShaderProgramOpenGL( );

		// Public general functions
		virtual BIT_UINT32 AttachShaders( Shader * p_Shader );
		virtual BIT_UINT32 Link( std::string & p_Validation );
		virtual void Bind( );
		virtual void Unbind( );

		// Public set functions
		virtual void SetUniform1i( const char * p_Location, const BIT_SINT32 );
		virtual void SetUniform1f( const char *, const BIT_FLOAT32 );
		virtual void SetUniform2f( const char *, const BIT_FLOAT32 , const BIT_FLOAT32 );
		virtual void SetUniform3f( const char *, const BIT_FLOAT32 , const BIT_FLOAT32 , const BIT_FLOAT32 );
		virtual void SetUniform4f( const char *, const BIT_FLOAT32 , const BIT_FLOAT32 , const BIT_FLOAT32 , const BIT_FLOAT32 );
		virtual void SetUniformMatrix4x4f( const char * p_Location, Matrix4x4 & p_Matrix );
		virtual void SetAttributeLocation( const char * p_Location, BIT_UINT32 p_Index );

	private:

		// Private variables
		GLuint m_ProgramID;
		BIT_UINT32 m_AttachedShaderCount;

	};


}

#endif