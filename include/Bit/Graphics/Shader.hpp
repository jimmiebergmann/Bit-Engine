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

#ifndef __BIT_GRAPHICS_SHADER_HPP__
#define __BIT_GRAPHICS_SHADER_HPP__

#include <Bit/DataTypes.hpp>
#include <string>

namespace Bit
{

	class BIT_API Shader
	{

	public:

		enum eShaderType
		{
			None = 0,
			Vertex = 1,
			Fragment = 2
		};

		// Making a cute friend!
		friend class ShaderProgram;

		// Destructor
		virtual ~Shader() { }

		// General public functions
		BIT_UINT32 ReadFile( const char * p_pFilePath );
		virtual BIT_UINT32 Compile( ) = 0;

		// Set functions
		BIT_INLINE void SetSource( std::string p_Source ) { m_ShaderSource = p_Source; };
		
		// "inline" Get public functions
		BIT_INLINE eShaderType GetType( ) const { return m_ShaderType; }
		BIT_INLINE std::string GetSource( ) const { return m_ShaderSource; }
		BIT_INLINE BIT_BOOL IsCompiled( ) const { return m_Compiled; }

	protected:

		// Protected variables
		BIT_BOOL m_Compiled;
		eShaderType m_ShaderType;
		std::string m_ShaderSource;

	};

}

#endif