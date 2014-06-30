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

#ifndef BIT_GRAPHICS_OPENGL_SHADER_HPP
#define BIT_GRAPHICS_OPENGL_SHADER_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/Shader.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL shader class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLShader : public Shader
	{

	public:

		// Friend classes
		friend class OpenGLShaderProgram;

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// \param p_Type Enumerator of the shader type.
		///
		////////////////////////////////////////////////////////////////
		OpenGLShader( const ShaderType::eType p_Type );

		////////////////////////////////////////////////////////////////
		/// \brief Donstructor
		///
		////////////////////////////////////////////////////////////////
		~OpenGLShader( );

		////////////////////////////////////////////////////////////////
		/// \brief Compile the shader from a file.
		///
		/// \param p_Filename File path to the shader source file.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool CompileFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Compile the shader from memory.
		///
		/// \param p_Memory String of the shader source code.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool CompileFromMemory( const std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Check if the shader is compiled.
		///
		/// \return True if compiled, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsCompiled( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the shader type.
		///
		/// \return Enumerator of the shader type.
		///
		////////////////////////////////////////////////////////////////
		virtual ShaderType::eType GetType( );

	protected:

		// Get functions
		GLuint GetShaderObject( ) const;

	private:

		// Private variable
		Bool m_Compiled;
		GLuint m_ShaderObject;
		ShaderType::eType m_Type;

	};
}

#endif
