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

#ifndef BIT_GRAPHICS_SHADER_HPP
#define BIT_GRAPHICS_SHADER_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Shader base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Shader
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Shader type enumerator.
		///
		////////////////////////////////////////////////////////////////
		enum eType
		{
			Vertex,
			Fragment
		};

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~Shader() { }

		////////////////////////////////////////////////////////////////
		/// \brief Read shader source from file.
		///
		/// \param p_FilePath File path to the shader source file.
		///
		////////////////////////////////////////////////////////////////
		virtual bool ReadFromFile( const std::string & p_FilePath ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Read shader source from memory.
		///
		/// \param p_Memory String of the shader source code.
		///
		////////////////////////////////////////////////////////////////
		virtual bool ReadFromMemory( const std::string & p_Memory ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Compile the shader.
		///
		/// Make sure to read a valid shader source code before compiling.
		///
		/// \see ReadFromFile
		/// \see ReadFromMemory
		///
		////////////////////////////////////////////////////////////////
		virtual bool Compile( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Check if the shader is compiled.
		///
		/// \return True if compiled, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsCompiled( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the shader type.
		///
		/// \return Enumerator of the shader type.
		///
		////////////////////////////////////////////////////////////////
		virtual eType GetType( ) = 0;

	};

}


#endif
