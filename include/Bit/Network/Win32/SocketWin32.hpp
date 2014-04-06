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
#include <Bit/Network/SocketHandle.hpp>
#include <Bit/Network/Address.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Win32 socket address class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API SocketWin32
	{

	public:

		// Friend classes
		friend class TcpListener;

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		SocketWin32( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		SocketWin32( const SocketHandle & m_SocketHandle );

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor
		///
		////////////////////////////////////////////////////////////////
		virtual ~SocketWin32( );

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
		void SetBlocking( Bool p_Blocking );

		////////////////////////////////////////////////////////////////
		/// \brief Get the blocking state
		///
		////////////////////////////////////////////////////////////////
		Bool GetBlocking( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get address from hostname.
		///
		////////////////////////////////////////////////////////////////
		static Address GetHostByName( const std::string & p_Hostname );

	protected:

		////////////////////////////////////////////////////////////////
		/// \brief Set the handle
		///
		/// \param p_Handle The socket handle.
		///
		////////////////////////////////////////////////////////////////
		void SetHandle( const SocketHandle & m_SocketHandle );

		////////////////////////////////////////////////////////////////
		/// \brief Get the socket handle.
		///
		////////////////////////////////////////////////////////////////
		SocketHandle GetHandle( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Return the the address of the peer to which a socket is connected.
		///
		////////////////////////////////////////////////////////////////
		Address GetPeerAddress( ) const;

		// Protected varaibles
		SocketHandle m_Handle;	///< Socket handle
		Bool m_Blocking;		///< Socket blocking state

	};

}

#endif

#endif