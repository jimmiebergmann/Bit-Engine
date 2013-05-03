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
// The original data type headers was written by Stuart ( teh_programerer@interactivelaboratories.com )

#ifndef __BIT_SYSTEM_DATATYPES_HPP__
#define __BIT_SYSTEM_DATATYPES_HPP__

// Make sure any platform is defined.
#ifndef PLATFORM_WINDOWS
	#ifndef PLATFORM_LINUX
		#error No platform is defined
	#endif
#endif

// Make sure any build is defined
#ifndef BUILD_DEBUG
	#ifndef BUILD_RELEASE
		#error No build is defined
	#endif
#endif


#include <cstddef>

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
typedef unsigned __int64	BIT_UINT64;
typedef signed __int64		BIT_SINT64;
typedef float				BIT_FLOAT32;
typedef double				BIT_FLOAT64;
typedef BIT_UCHAR8			BIT_BYTE;
typedef BIT_UCHAR8			BIT_BOOL;
typedef std::size_t			BIT_MEMSIZE;
#define BIT_TRUE	1
#define BIT_FALSE	0
#define BIT_NULL	NULL

// Return codes
const BIT_UINT32 BIT_OK					= 0x00000000;
const BIT_UINT32 BIT_ERROR				= 0x00000001;
const BIT_UINT32 BIT_ERROR_OPEN_FILE	= 0x00000002;

const BIT_FLOAT64 BIT_PI = 3.14159265359f;
const BIT_FLOAT32 BIT_EPSILON = 1.0e-6f;
const BIT_FLOAT32 BIT_HALFEPSILON = 1.0e-3f;

#endif