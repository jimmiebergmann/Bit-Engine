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

#ifndef BIT_BUILD_HPP
#define BIT_BUILD_HPP

////////////////////////////////////////////////////////////////
// Define the bit engine version
////////////////////////////////////////////////////////////////
#define BIT_VERSION_MAJOR 0
#define BIT_VERSION_MINOR 1
#define BIT_VERSION_REVISION 0

////////////////////////////////////////////////////////////////
// Define the platform
////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || defined( __WIN64__ )
	#define BIT_PLATFORM_WINDOWS
#elif defined( linux ) || defined( __linux )
	#define BIT_PLATFORM_LINUX
#else
	#error No platform is defined
#endif

////////////////////////////////////////////////////////////////
// Define the build type ( release / debug )
////////////////////////////////////////////////////////////////
#if defined( NDEBUG ) || !defined( _DEBUG )
	#define BIT_BUILD_RELEASE
#else
	#define BIT_BUILD_DEBUG
#endif

////////////////////////////////////////////////////////////////
// Define BIT_API
// If this build is not a static library build
////////////////////////////////////////////////////////////////
#ifndef BIT_STATIC
	// Turn off microsoft vsc warning
	#ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif
	// Define as export or import depending on if BIT_EXPORTS is defined
	#ifdef BIT_PLATFORM_WINDOWS
		#ifdef BIT_EXPORTS
			#define BIT_API __declspec(dllexport)
		#else
			#define BIT_API __declspec(dllimport)
		#endif
	#elif defined( BIT_PLATFORM_LINUX )
		#ifdef BIT_EXPORTS
			#define BIT_API
		#else
			#define BIT_API
		#endif
	#endif
#else
	// Define it as nothing
	#define BIT_API
#endif

// Header for std::size_t
#include <cstddef>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \addtogroup Bit
	/// @{
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// System data types
	////////////////////////////////////////////////////////////////
	typedef signed char			Int8;		///< Signed 8 bit integer type
	typedef unsigned char		Uint8;		///< Unsigned 8 bit integer type
	typedef signed short		Int16;		///< Signed 16 bit integer type
	typedef unsigned short		Uint16;		///< Unsigned 16 bit integer type
	typedef signed int			Int32;		///< Signed 32 bit integer type
	typedef unsigned int		Uint32;		///< Unsigned 32 bit integer type
	typedef signed long long	Int64;		///< Signed 64 bit integer type
	typedef unsigned long long	Uint64;		///< Unsigned 64 bit integer type
	typedef float				Float32;	///< 32 bit floating point type
	typedef double				Float64;	///< 64 bit floating point type
	typedef bool				Bool;		///< Boolean type
	typedef std::size_t			SizeType;	///< Memory size type

	////////////////////////////////////////////////////////////////
	// System data types
	////////////////////////////////////////////////////////////////
	namespace DataType
	{
		enum eType
		{
			None = 0,	///< No type
			Int8,		///< Signed 8 bit integer type
			Uint8,		///< Unsigned 8 bit integer type
			Int16,		///< Signed 16 bit integer type
			Uint16,		///< Unsigned 16 bit integer type
			Int32,		///< Signed 32 bit integer type
			Uint32,		///< Unsigned 32 bit integer type
			Int64,		///< Signed 64 bit integer type
			Uint64,		///< Unsigned 64 bit integer type
			Float32,	///< 32 bit floating point type
			Float64,	///< 64 bit floating point type
			Bool,		///< Boolean type
			SizeType	///< Memory size type
		};

	}

	////////////////////////////////////////////////////////////////
	// Math constants
	////////////////////////////////////////////////////////////////
	const Float64 Pi					= 3.141592653589793f;
	const Float64 Pi2					= 6.283185307179586f;
	const Float32 PiFloat32				= 3.1415927f;
	const Float32 Pi2Float32			= 6.28318531f;
	const Float64 Epsilon				= 1.0e-6f;
	const Float64 EpsilonHalf			= 1.0e-3f;
	const Float32 EpsilonFloat32		= 1.0e-6f;
	const Float32 EpsilonHalfFloat32	= 1.0e-3f;

	////////////////////////////////////////////////////////////////
	/// @}
	////////////////////////////////////////////////////////////////

}

#endif

////////////////////////////////////////////////////////////////
// Define multiple modules for the documentation software.
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup Bit
/// \brief Base module.
///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup Audio
/// \brief Audio module.
///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup Graphics
/// \brief Graphics module.
///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup Network
/// \brief Network module.
///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup System
/// \brief System module.
///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/// \defgroup Window
/// \brief Window module.
///
////////////////////////////////////////////////////////////////
