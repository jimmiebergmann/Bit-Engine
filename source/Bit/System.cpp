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


#include <Bit/System.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

#ifdef BIT_PLATFORM_WIN32
    #define SeparationSign '\\'
#elif defined( BIT_PLATFORM_LINUX )
    #define SeparationSign '/'
#endif

namespace Bit
{

	// Static variables
	static std::string s_AbsolutePath = "";
	static std::string s_AbsoluteDirectoryPath = "";

	// Path functions
	BIT_UINT32 BIT_API SetAbsolutePath( const std::string p_Path )
	{
		if( p_Path.size( ) == 0 )
		{
			return BIT_ERROR;
		}

		// Set the absolute path
		s_AbsolutePath = p_Path;
		s_AbsoluteDirectoryPath = p_Path;

		// Set the directory path by finding the first "\" or "/" backwards and then cut off the rest
		BIT_UINT32 Position = p_Path.size( ) + 1;
		for( BIT_SINT32 i = p_Path.size( ) - 1; i >= 0; i-- )
		{
			if( p_Path[ i ] == SeparationSign )
			{
				Position = i;
				break;
			}
		}

		// Did we find the position?
		if( Position != p_Path.size( ) + 1 )
		{
			s_AbsoluteDirectoryPath.resize( Position );
		}

		return BIT_OK;
	}

	std::string BIT_API GetAbsolutePath( )
	{
		return s_AbsolutePath;
	}

	std::string BIT_API GetAbsolutePath( const std::string p_Path )
	{
		return s_AbsoluteDirectoryPath + std::string( 1, SeparationSign ) + p_Path;
	}

	std::string BIT_API GetAbsoluteDirectoryPath( )
	{
		return s_AbsoluteDirectoryPath;
	}

}
