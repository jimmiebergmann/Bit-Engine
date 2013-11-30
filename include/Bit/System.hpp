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

#ifndef BIT_SYSTEM_HPP
#define BIT_SYSTEM_HPP

#include <Bit/DataTypes.hpp>
#include <string>

// Very useful macro for check single bits
#define bitBitShiftLeft(n)       (1 << (n))
#define bitCheckBit(n,i) ((bool)(((n) & bitBitShiftLeft(i)) != 0)) // <- Use this one

// Sleep
#ifdef BIT_PLATFORM_WIN32
#include <windows.h>
#undef CreateWindow
#define bitSleep(x) ( Sleep( x ) )
#elif BIT_PLATFORM_LINUX
#include <unistd.h>
#define bitSleep(x) ( usleep( x * 1000 ) )
#endif

// Is nan
#include <float.h>

#ifdef BIT_PLATFORM_WIN32
	#define bitIsNan _isnan
#elif BIT_PLATFORM_LINUX
	define bitIsNan isnan
#endif

namespace Bit
{

	// Path functions
	BIT_UINT32 BIT_API SetAbsolutePath( const std::string p_ExecutablePath );
	std::string BIT_API GetAbsolutePath( );
	std::string BIT_API GetAbsolutePath( const std::string p_Path );
	std::string BIT_API GetAbsoluteDirectoryPath( );
	std::string BIT_API GetDirectoryPath( const std::string p_ExecutablePath );
	std::string BIT_API GetFileExtension( const char * p_pFilePath );

	// Get line functions. From strings and char arrays
	BIT_UINT32 BIT_API GetLine( BIT_SCHAR8 * p_Destination, BIT_UINT32 p_DestinationSize, const BIT_SCHAR8 * p_Source );

}



#endif
