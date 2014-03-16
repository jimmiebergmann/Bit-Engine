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

#ifndef BIT_GRAPHICS_SHADER_PROGRAM_HPP
#define BIT_GRAPHICS_SHADER_PROGRAM_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Matrix4x4.hpp>
#include <string>

namespace Bit
{

	// Forward declaraction
	class Shader;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Shader program base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ShaderProgram
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor
		///
		////////////////////////////////////////////////////////////////
		virtual ~ShaderProgram( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Atttach a shader to the shader program.
		///
		////////////////////////////////////////////////////////////////
		virtual bool AttachShader( const Shader & p_Shader ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Link the attached shaders into a shader program.
		///
		////////////////////////////////////////////////////////////////
		virtual bool Link( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Bind the shader program and make it to the current one.
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Unbind the shader program
		///
		////////////////////////////////////////////////////////////////
		virtual void Unbind( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set attribute location of a non uniform varaible.
		///
		/// \param p_Name The name of the varaible.
		/// \param p_Index The index of the varaible
		///
		////////////////////////////////////////////////////////////////
		virtual void SetAttributeLocation( const char * p_Name, const Int32 p_Index ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Int32 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_A First varaible paramter.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniform1i( const char * p_Name, const Int32 p_A ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Float32 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_A First varaible paramter.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniform1f( const char * p_Name, const Float32 p_A ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Float32 vector2 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_A First varaible paramter.
		/// \param p_B Second varaible paramter.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniform2f( const char * p_Name, const Float32 p_A, const Float32 p_B ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Float32 vector3 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_A First varaible paramter.
		/// \param p_B Second varaible paramter.
		/// \param p_C Second varaible paramter.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniform3f( const char * p_Name, const Float32 p_A, const Float32 p_B, const Float32 p_C ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Float32 vector3 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_A First varaible paramter.
		/// \param p_B Second varaible paramter.
		/// \param p_C Second varaible paramter.
		/// \param p_D Second varaible paramter.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniform4f( const char * p_Name, const Float32 p_A, const Float32 p_B, const Float32 p_C, const Float32 p_D ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Set Matrix4x4f32 shader uniform variable
		///
		/// \param p_Name The name of the varaible.
		/// \param p_Matrix The matrix.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetUniformMatrix4x4f( const char * p_Name, const Matrix4x4f32 & p_Matrix ) = 0;

	};

}

#endif
