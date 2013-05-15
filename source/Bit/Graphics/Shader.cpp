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


#include <Bit/Graphics/Shader.hpp>
#include <Bit/System/Debugger.hpp>
#include <fstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	BIT_UINT32 Shader::Read( const std::string p_FilePath )
	{
		// Use fstream for reading files.
		std::ifstream File( p_FilePath.c_str( ) );
		if(File.is_open() == false)
		{
			bitTrace( "[Shader::Open] Failed to open file: %s\n", p_FilePath.c_str( ) );
			return BIT_ERROR_OPEN_FILE;
		}

		// Set the file path (if we want to reload the shader later, maybe.
		m_FilePath = p_FilePath;

		// Get the file size
		File.seekg( 0, std::ios::end );
		BIT_UINT32 FileSize = File.tellg( );
		File.seekg( 0, std::ios::beg );

		// Clear the old source, and reserve the new size
		m_ShaderSource.clear( );
		m_ShaderSource.reserve( FileSize );

		// Read all the file's lines
		std::string Line = "";

		while( !File.eof( ) )
		{
			std::getline( File, Line );
			m_ShaderSource += Line + "\n";
		}

		// Close the file, we are done.
		File.close( );

		return BIT_OK;
	}
}
