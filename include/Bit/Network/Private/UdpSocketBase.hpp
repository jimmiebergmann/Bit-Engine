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

#ifndef BIT_NETWORK_UDP_SOCKET_BASE_HPP
#define BIT_NETWORK_UDP_SOCKET_BASE_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/Network/Address.hpp>
#include <Bit/Network/Packet.hpp>
#include <Bit/System/Time.hpp>

namespace Bit
{

    namespace Private
    {

        ////////////////////////////////////////////////////////////////
        /// \ingroup Network
        /// \brief Linux Udp socket base class.
        ///
        ////////////////////////////////////////////////////////////////
        class BIT_API UdpSocketBase : public Socket
        {

        public:

            ////////////////////////////////////////////////////////////////
            /// \brief Destructor
            ///
            ////////////////////////////////////////////////////////////////
            virtual ~UdpSocketBase( ) { }

            ////////////////////////////////////////////////////////////////
            /// \brief Open the socket
            ///
            /// The port is set to 0 by default,
            /// this will cause the socket to pick a random port for you.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Bool Open( const Uint16 p_Port = 0 ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Close the socket
            ///
            ////////////////////////////////////////////////////////////////
            virtual void Close( ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Send packet
            ///
            /// \param p_pData The data to be sent to server.
            /// \param p_Size The size of the data.
            /// \param p_Address The destination address.
            /// \param p_Port The destination port.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Int32 Send( const void * p_pData, const SizeType p_Size, const Address & p_Address, const Uint16 p_Port ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Receive data packet.
            ///
            /// \param p_pData Received data.
            /// \param p_Size The size of the data.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Int32 Receive( void * p_pData, const SizeType p_Size, Address & p_Address, Uint16 & p_Port ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Receive data packet.
            ///
            /// \param p_pData Received data.
            /// \param p_Size The size of the data.
            /// \param m_Timeout Time in milliseconds until
            ///		the attemp in receiving a message timeouts.
            ///
            ////////////////////////////////////////////////////////////////
           virtual Int32 Receive( void * p_pData, const SizeType p_Size, Address & p_Address, Uint16 & p_Port, const Time & p_Timeout ) = 0;

        private:

        };

    }

}

#endif
