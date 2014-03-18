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

#ifndef BIT_NETWORK_TCP_SOCKET_HPP
#define BIT_NETWORK_TCP_SOCKET_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/Network/Address.hpp>
#include <Bit/Network/Packet.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Network address class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API TcpSocket : public Socket
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		TcpSocket( );

		////////////////////////////////////////////////////////////////
		/// \brief Connect the socket to a server.
		///
		/// \param p_Address Server address.
		/// \param p_Port Server port.
		///
		////////////////////////////////////////////////////////////////
		bool Connect( const Address & p_Address, const Uint16 p_Port );

		////////////////////////////////////////////////////////////////
		/// \brief Disconnect the socket from the server.
		///
		////////////////////////////////////////////////////////////////
		void Disconnect( );

		////////////////////////////////////////////////////////////////
		/// \brief Receive data from server.
		///
		/// \param p_pData Data from server.
		/// \param p_Size The size of the data.
		///
		////////////////////////////////////////////////////////////////
		Int32 Receive( void * p_pData, const SizeType p_Size);

		////////////////////////////////////////////////////////////////
		/// \brief Receive packet from server.
		///
		/// NOT AVAILABLE YET.
		///
		/// \param p_Packet Packet from server.
		///
		////////////////////////////////////////////////////////////////
		void Receive( Packet & p_Packet );

		////////////////////////////////////////////////////////////////
		/// \brief Send data to server.
		///
		/// \param p_pData The data to be sent to server.
		/// \param p_Size The size of the data.
		///
		////////////////////////////////////////////////////////////////
		Int32 Send( const void * p_pData, const SizeType p_Size );

		////////////////////////////////////////////////////////////////
		/// \brief Send packet to server.
		///
		/// NOT AVAILABLE YET.
		///
		/// \param p_Packet The packet to be sent to server.
		///
		////////////////////////////////////////////////////////////////
		void Send( const Packet & p_Packet );

	};

}

#endif