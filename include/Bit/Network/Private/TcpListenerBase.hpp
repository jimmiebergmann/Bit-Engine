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

#ifndef BIT_NETWORK_TCP_LISTENER_BASE_HPP
#define BIT_NETWORK_TCP_LISTENER_BASE_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/TcpSocket.hpp>

namespace Bit
{

    namespace Private
    {

        ////////////////////////////////////////////////////////////////
        /// \ingroup Network
        /// \brief Tcp listener base class.
        ///
        ////////////////////////////////////////////////////////////////
        class BIT_API TcpListenerBase : private Socket
        {

        public:

            ////////////////////////////////////////////////////////////////
            /// \brief Destructor
            ///
            ////////////////////////////////////////////////////////////////
            virtual ~TcpListenerBase( ) { }

            ////////////////////////////////////////////////////////////////
            /// \brief Start the listener.
            ///
            /// \param p_Port The server port.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Bool Start( const Uint16 p_Port ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Stop the listener
            ///
            ////////////////////////////////////////////////////////////////
            virtual void Stop( ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Listen for incoming connections
            ///
            /// This is a modal function.
            ///
            /// \param p_TcpSocket The returned connected socket.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Bool Listen( TcpSocket & p_Connection ) = 0;

            ////////////////////////////////////////////////////////////////
            /// \brief Checks if the listener is hosted.
            ///
            /// \return True if hosted, else false.
            ///
            ////////////////////////////////////////////////////////////////
            virtual Bool IsRunning( ) const = 0;

        };

    }

}

#endif
