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

#ifndef BIT_NETWORK_SOCKET_SELECTOR_HPP
#define BIT_NETWORK_SOCKET_SELECTOR_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/System/Time.hpp>
#include <map>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Socket selector class.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API SocketSelector
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////////
		SocketSelector( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		/// 
		////////////////////////////////////////////////////////////////
		~SocketSelector( );

		////////////////////////////////////////////////////////////////
		/// \brief Select an active socket.
		///
		/// \return True if paramter m_Socket is set to an active socket,
		///			else false.
		/// 
		////////////////////////////////////////////////////////////////
		Bool Select( Socket & p_Socket, const Time & p_Timeout = Time::Infinite );
		
		////////////////////////////////////////////////////////////////
		/// \brief Add socket to the selector.
		/// 
		////////////////////////////////////////////////////////////////
		void AddSocket( Socket & p_Socket );

	private:

		// Private typedefs
		typedef std::map<SocketHandle, Socket *> SocketMap;

		// Private variables
		fd_set m_SocketSet;
		SocketMap m_Sockets;

	};

}

#endif