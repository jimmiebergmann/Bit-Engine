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
		/*
			Structure of a packet(+ is mandatory, / is optionally)
			-------------------------------------------------------------------------
				+	1 byte packet type.
				/	2 bytes sequence number.
				/	1 byte reability status, mandatory if sequence number is set.
				/	~ Data
		*/

		////////////////////////////////////////////////////////////////
		/// \brief Size of the net packet header.
		///
		////////////////////////////////////////////////////////////////
		static const SizeType BufferSize			= 2048;
		static const SizeType PacketTypeSize		= 1;
		static const SizeType SequenceSize			= 2;
		static const SizeType ReliabilityFlagSize	= 1;

		////////////////////////////////////////////////////////////////
		/// \brief Packet type
		///
		////////////////////////////////////////////////////////////////
		struct PacketType
		{
			enum eType
			{
				///								| Reliable  |  Has sequence |	Sender	|  Receiver |
				/// ----------------------------------------------------------------------------------
				Connect			= 0,	///<	|	No		|	   No		|	Client	|	Server	|
				Disconnect		= 1,	///<	|	No		|	   No		|	Both	|	Both	|
				Accept			= 2,	///<	|	No		|	   No 		|	Server	|	Client	|
				Reject			= 3,	///<	|	No		|	   No 		|	Server	|	Client	|
				Acknowledgement = 4,	///<	|	No		|	   Yes 		|	Both	|	Both	|
				Alive			= 5,	///<	|	No		|	   Yes 		|	Both	|	Both	|
				EntityUpdate	= 6,	///<	|	Both	|	   Yes		|	Server	|	Client	|
				EntityDestroyed = 7,	///<	|	Yes		|	   Yes		|	Server	|	Client	|
				UserMessage		= 8,	///<	|	Both	|	   Yes		|	Client	|	Server	|
				HostMessage		= 9,	///<	|	Both	|	   Yes		|	Server	|	Client	|
				Command			= 10,	///<	|	Yes		|	   Yes		|	Client	|	Server	|
				Ping			= 11	///<	|	No		|	   Yes		|	Both	|	Both	|
				/// ---------------------------------------------------------------------------------
			};
		};

		///< Packet type count.
		const SizeType PacketTypeCount = 10;

		///< Packet sizes, excluding data
		const SizeType ConnectPacketSize = 3;
		const SizeType DisconnectPacketSize = 1;
		const SizeType AcceptPacketSize = 11;
		const SizeType RejectPacketSize = 2;
		const SizeType AcknowledgementPacketSize = 3;
		const SizeType AlivePacketSize = 3;
		const SizeType EntityUpdatePacketSize = 4;
		const SizeType EntityDestroyedPacketSize = 6;
		const SizeType UserMessagePacketSize = 4;
		const SizeType HostMessagePacketSize = 4;
		const SizeType CommandPacketSize = 3;
		const SizeType PingPacketSize = 2;

		////////////////////////////////////////////////////////////////
		/// \brief Reject type
		///
		////////////////////////////////////////////////////////////////
		struct RejectType
		{
			enum eType
			{
				Denied	= 0,	///< Denied by server.
				Banned	= 1,	///< The client is banned.
				Full	= 2		///< The server is full.
			};
		};

		////////////////////////////////////////////////////////////////
		/// \brief Disconnect type
		///
		////////////////////////////////////////////////////////////////
		struct DisconnectType
		{
			enum eType
			{
				Closed			= 0,	///< The client/server closed the connection.
				Banned			= 1,	///< The server banned the client.
				Kicked			= 2,	///< The server kicked the client.
				LostConnection	= 3		///< The connection was lost.
			};
		};

		////////////////////////////////////////////////////////////////
		/// \brief Reliability type
		///
		////////////////////////////////////////////////////////////////
		struct ReliabilityType
		{
			enum eType
			{
				Unreliable	= 0,	///< Reliable packet.
				Reliable	= 1		///< Reliable packet.
			};
		};

		////////////////////////////////////////////////////////////////
		/// \brief Message type enumerator (OLD)
		///
		////////////////////////////////////////////////////////////////
		enum eMessageType
		{
			EntityMessageType	= 0,
			UserMessageType		= 1
		};

	}

}

#endif