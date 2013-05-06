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

// Note:
// The original header was written by Stuart ( teh_programerer@interactivelaboratories.com )

#ifndef __BIT_DATATYPES_HPP__
#define __BIT_DATATYPES_HPP__

// Define the bit engine version
#define BIT_VERSION_MAJOR 0
#define BIT_VERSION_MINOR 0

// Define the platform
#if defined( _WIN32 ) || defined( __WIN32__ )
	#define BIT_PLATFORM_WIN32
#elif defined( linux ) || defined( __linux )
	#define BIT_PLATFORM_LINUX
#else
	#error No platform is defined
#endif

// Define the build type ( release / debug )
#if defined( NDEBUG )
	#define BIT_BUILD_RELEASE
#else
	#define BIT_BUILD_DEBUG
#endif

// Define BIT_API
// If this build is not a static library build
#ifndef BIT_STATIC_LIB
	// Turn off microsoft vsc warning
	#ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif
	// Define as export or import depending on if BIT_EXPORTS is defined
	#ifdef BIT_PLATFORM_WIN32
		#ifdef BIT_EXPORTS
			#define BIT_API __declspec(dllexport)
		#else
			#define BIT_API __declspec(dllimport)
		#endif
	#elif BIT_PLATFORM_LINUX
		#define BIT_API
	#endif
#else
	// Define it as nothing
	#define BIT_API
#endif



// Finally some data types
#include <cstddef>

// inline
#define BIT_INLINE	inline

// Standard types
typedef unsigned char		BIT_UCHAR8;
typedef signed char			BIT_SCHAR8;
typedef unsigned short		BIT_UCHAR16;
typedef signed short		BIT_SCHAR16;
typedef unsigned char		BIT_UINT8;
typedef signed char			BIT_SINT8;
typedef unsigned short		BIT_UINT16;
typedef signed short		BIT_SINT16;
typedef unsigned int		BIT_UINT32;
typedef signed int			BIT_SINT32;
typedef unsigned long long	BIT_UINT64;
typedef signed long long	BIT_SINT64;
typedef float				BIT_FLOAT32;
typedef double				BIT_FLOAT64;
typedef BIT_UCHAR8			BIT_BYTE;
typedef bool				BIT_BOOL;
typedef std::size_t			BIT_MEMSIZE;
#define BIT_TRUE	1
#define BIT_FALSE	0
#define BIT_NULL	NULL

// Return codes
const BIT_UINT32 BIT_OK					= 0x00000000;
const BIT_UINT32 BIT_ERROR				= 0x00000001;
const BIT_UINT32 BIT_ERROR_OPEN_FILE	= 0x00000002;

// Constants
const BIT_FLOAT64 BIT_PI = 3.141592653589793238f;
const BIT_FLOAT32 BIT_EPSILON = 1.0e-6f;
const BIT_FLOAT32 BIT_HALFEPSILON = 1.0e-3f;

#endif
