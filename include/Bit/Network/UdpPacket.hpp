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

#ifndef BIT_NETWORK_UDP_PACKET_HPP
#define BIT_NETWORK_UDP_PACKET_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	namespace Udp
	{

		////////////////////////////////////////////////////////////////
		/// \brief Size of the Udp packet header.
		///
		////////////////////////////////////////////////////////////////
		static const SizeType HeaderSize = 3;

		////////////////////////////////////////////////////////////////
		/// \brief Udp packet type enumerator
		///
		////////////////////////////////////////////////////////////////
		enum ePacketType
		{
			Syn					= 1,
			SynAck				= 2,
			Ack					= 3,
			Alive				= 4,
			Close				= 5,
			UnreliablePacket	= 6,
			ReliablePacket		= 7
		};

		////////////////////////////////////////////////////////////////
		/// \brief Udp packet structure
		///
		////////////////////////////////////////////////////////////////
		struct Packet
		{
			Bit::Uint16		Sequence;	///< Packet sequence.
			Bit::SizeType	DataSize;	///< Size of the data.
			char *			pData;		///< Pointer to the data.
		};

	}

}

#endif