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

#include <Bit/Network/Private/SocketBase.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

    namespace Private
    {

        SocketBase::SocketBase( const SocketHandle & p_SocketHandle,
                                const Bool p_Blocking ) :
            m_Handle( p_SocketHandle ),
            m_Blocking( p_Blocking )
        {
        }

        Address SocketBase::GetHostByName( const std::string & p_Hostname )
        {
            struct hostent *he;
            if( ( he = gethostbyname( p_Hostname.c_str() ) ) == NULL )
            {
                return Address::NoAddress;
            }

            // Get the hostname in string form
            char * hostname =  inet_ntoa( *( (struct in_addr *) he->h_addr_list[0] ) );

            // Convert it to a string.
            Uint32 address = inet_addr( hostname );
            if( address == INADDR_NONE )
            {
                return Address::NoAddress;
            }

            // Convert the address to a network side number
            return Address( htonl( address ) );
        }


    }

}
