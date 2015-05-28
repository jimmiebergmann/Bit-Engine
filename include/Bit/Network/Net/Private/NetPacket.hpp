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

// Documentation about the packet structure, please:
// ... NIL


#ifndef BIT_NETWORK_NET_PACKET_HPP
#define BIT_NETWORK_NET_PACKET_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \brief Size of the net packet header.
		///
		////////////////////////////////////////////////////////////////
		static const SizeType HeaderSize = 3;

		////////////////////////////////////////////////////////////////
		/// \brief Packet type enumerator
		///
		////////////////////////////////////////////////////////////////
		enum ePacketType
		{
			Syn					= 1, 
			SynAck				= 2,
			Sync				= 3,
			Ack					= 4,
			Alive				= 5,
			Close				= 6,
			Ban					= 7,
			UnreliablePacket	= 8,
			ReliablePacket		= 9
		};

		/*
		// FUTURE STRUCKTURE?


		enum ePacketType
		{
			Connect				= 1,	//< Unreliable packet.
			Disconnect			= 2,	//< Unreliable packet.
			Accept				= 3,	//< Unreliable packet.
			Reject				= 4,	//< Unreliable packet.
			Ack					= 5,	//< Unreliable packet.
			Alive				= 6,	//< Reliable packet.
			EntityUpdate		= 7,	//< [Un]reliable packet availability.
			UserMessage			= 8,	//< [Un]reliable packet availability.
			HostMessage			= 9		//< [Un]reliable packet availability.
		};

		*/

		
		////////////////////////////////////////////////////////////////
		/// \brief Message type enumerator
		///
		////////////////////////////////////////////////////////////////
		enum eMessageType
		{
			EntityMessageType	= 0,
			UserMessageType		= 1
		};

		////////////////////////////////////////////////////////////////
		/// \brief Packet structure
		///
		////////////////////////////////////////////////////////////////
		struct Packet
		{
			Uint16		Sequence;	///< Packet sequence.
			SizeType	DataSize;	///< Size of the data.
			char *		pData;		///< Pointer to the data.
		};

	}

}

#endif