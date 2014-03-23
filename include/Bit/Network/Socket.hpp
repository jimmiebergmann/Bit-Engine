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

#ifndef BIT_NETWORK_SOCKET_HPP
#define BIT_NETWORK_SOCKET_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS
	#include <Bit/Network/Win32/SocketWin32.hpp>
#elif BIT_PLATFORM_LINUX
	#include <Bit/Network/Linux/SocketLinux.hpp>
#endif

namespace Bit
{
	#ifdef BIT_PLATFORM_WINDOWS
		typedef SocketWin32 Socket;
	#elif BIT_PLATFORM_LINUX
		#error No socket is available for this platform.
	#endif

	BIT_API Uint16 Hton16( const Uint16 p_Value );
	BIT_API Uint32 Hton32( const Uint32 p_Value );
	BIT_API Uint64 Hton64( const Uint64 p_Value );
	BIT_API Uint16 Ntoh16( const Uint16 p_Value );
	BIT_API Uint32 Ntoh32( const Uint32 p_Value );
	BIT_API Uint64 Ntoh64( const Uint64 p_Value );


};

#endif