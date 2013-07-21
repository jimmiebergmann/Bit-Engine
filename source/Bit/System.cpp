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
#include <string.h>
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
	BIT_UINT32 BIT_API SetAbsolutePath( const std::string p_ExecutablePath )
	{
		if( p_ExecutablePath.size( ) == 0 )
		{
			return BIT_ERROR;
		}

		// Set the absolute path
		s_AbsolutePath = p_ExecutablePath;
		s_AbsoluteDirectoryPath = p_ExecutablePath;

		// Set the directory path by finding the first "\" or "/" backwards and then cut off the rest
		BIT_UINT32 Position = p_ExecutablePath.size( ) + 1;
		for( BIT_SINT32 i = p_ExecutablePath.size( ) - 1; i >= 0; i-- )
		{
			if( p_ExecutablePath[ i ] == '/' || p_ExecutablePath[ i ] == '\\' )
			{
				Position = i;
				break;
			}
		}

		// Did we find the position?
		if( Position != p_ExecutablePath.size( ) + 1 )
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

	std::string BIT_API GetDirectoryPath( const std::string p_ExecutablePath )
	{
		if( p_ExecutablePath.size( ) == 0 )
		{
			return "";
		}

		// Set the absolute path
		std::string DirectoryPath = p_ExecutablePath;

		// Set the directory path by finding the first "\" or "/" backwards and then cut off the rest
		BIT_UINT32 Position = p_ExecutablePath.size( ) + 1;
		for( BIT_SINT32 i = p_ExecutablePath.size( ) - 1; i >= 0; i-- )
		{
			if( p_ExecutablePath[ i ] == '/' || p_ExecutablePath[ i ] == '\\' )
			{
				Position = i;
				break;
			}
		}

		// Did we find the position?
		if( Position != p_ExecutablePath.size( ) + 1 )
		{
			DirectoryPath.resize( Position );
		}

		return DirectoryPath;
	}

	std::string BIT_API GetFileExtension( const char * p_pFilePath )
	{
		std::string Extension = "";

		// Search for the extension
		for( BIT_SINT32 i = strlen( p_pFilePath ) - 1; i >= 0; i-- )
		{
			// We found the start of the file extension.
			if( p_pFilePath[ i ] == '.' )
			{
				// Set the extension with upper case letters
				for( BIT_SINT32 j = i + 1; j < (BIT_SINT32)strlen( p_pFilePath ); j++ )
				{
					Extension += toupper( p_pFilePath[ j ] );
				}
				//Extension = &p_pFilePath[ i + 1 ];
				break;
			}
		}

		// We didn't find any extension
		if( !Extension.size( ) )
		{
			return std::string( p_pFilePath );
		}

		// Return the extension
		return Extension;
	}

/*
    Texture * BIT_API GetATest( )
    {

    return BIT_NULL;
    }*/

	// Get line functions.
	BIT_UINT32 BIT_API GetLine( BIT_SCHAR8 * p_Destination, BIT_UINT32 p_DestinationSize, const BIT_SCHAR8 * p_Source )
	{
		// Keep on searching for a line ending.
		for( BIT_UINT32 i = 0; i < p_DestinationSize; i++ )
		{
			// Look for a new line
			if( p_Source[ i ] == '\n' || p_Source[ i ] == 0 )
			{
				// Calculate the size of the line
				BIT_UINT32 Size = i + 1;

				// Copy the new line into the temporary line buffer
				strncpy( (char*)p_Destination, (const char*)p_Source, Size );
				p_Destination[ i ] = 0;

				return Size;
			}
		}

		return 0;
	}

}






