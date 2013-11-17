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

#ifndef BIT_NETWORK_HPP
#define BIT_NETWORK_HPP

#include <Bit/DataTypes.hpp>

#ifdef BIT_PLATFORM_WIN32
	#include <windows.h> // <winsock2.h>
#elif BIT_PLATFORM_LINUX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include "../Utility/Thread/Thread.h"
#endif

namespace Bit
{

	namespace Net
	{

#ifdef BIT_PLATFORM_WIN32
		#define CloseSocket( x ) closesocket( x )
		typedef SOCKET Socket;
#elif BIT_PLATFORM_LINUX
		#define CloseSocket( x ) close( x )
		typedef unsigned int Socket;
#endif

		BIT_UINT32 StartUp( );
		void ShutDown( );

	}

}

#endif