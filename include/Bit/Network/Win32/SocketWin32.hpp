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

#ifndef BIT_NETWORK_SOCKET_WIN32_HPP
#define BIT_NETWORK_SOCKET_WIN32_HPP

#include <Bit/Build.hpp>
#ifdef BIT_PLATFORM_WINDOWS
#include <Bit/Network/Private/SocketBase.hpp>

namespace Bit
{

    ////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Win32 socket class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API SocketWin32 : public Private::SocketBase
	{

	public:

        // Friend classes
		friend class TcpListenerWin32;
		friend class SocketSelector;

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		SocketWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		SocketWin32( const SocketHandle & p_SocketHandle );

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor
		///
		////////////////////////////////////////////////////////////////
		~SocketWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Enable/disable socket blocking.
		///
		/// The Receive function wont return until the socket
		/// actually receive any data if blocking is set to true.
		/// Blocking is enabled by default for TCP sockets.
		///
		/// \param p_Blocking The blocking flag.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetBlocking( const Bool p_Blocking );

		////////////////////////////////////////////////////////////////
		/// \brief Get the blocking state
		///
		////////////////////////////////////////////////////////////////
		virtual Bool GetBlocking( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get address from hostname.
		///
		////////////////////////////////////////////////////////////////
		//static Address GetHostByName( const std::string & p_Hostname );

	protected:

        ////////////////////////////////////////////////////////////////
		/// \brief Close socket handle.
		///
		////////////////////////////////////////////////////////////////
		virtual void CloseHandle( );

		////////////////////////////////////////////////////////////////
		/// \brief Set the handle
		///
		/// \param p_Handle The socket handle.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetHandle( const SocketHandle & p_SocketHandle );

		////////////////////////////////////////////////////////////////
		/// \brief Get the socket handle.
		///
		////////////////////////////////////////////////////////////////
		virtual const SocketHandle GetHandle( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Return the the address of the peer to which a socket is connected.
		///
		////////////////////////////////////////////////////////////////
		virtual Address GetPeerAddress( ) const;

	};

}

#endif

#endif
