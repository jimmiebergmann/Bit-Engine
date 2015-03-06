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

#ifndef BIT_NETWORK_NET_HOST_MESSAGE_DECODER_HPP
#define BIT_NETWORK_NET_HOST_MESSAGE_DECODER_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{
	
	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Host message decoder base class for client side.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API HostMessageDecoder
		{

		public:

			// Friend classes
			friend class Client;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			HostMessageDecoder(	const std::string & p_Name,
								Uint8 *				p_pMessage,
								SizeType			p_MessageSize );

			////////////////////////////////////////////////////////////////
			/// \brief Read byte from message.
			///
			////////////////////////////////////////////////////////////////
			Uint8 ReadByte( );

			////////////////////////////////////////////////////////////////
			/// \brief Read signed integer from message.
			///
			////////////////////////////////////////////////////////////////
			Int32 ReadInt( );

			////////////////////////////////////////////////////////////////
			/// \brief Read floating point from message.
			///
			////////////////////////////////////////////////////////////////
			Float32 ReadFloat( );

			////////////////////////////////////////////////////////////////
			/// \brief Read string from message.
			///
			////////////////////////////////////////////////////////////////
			Bool ReadString( std::string & p_String );

			////////////////////////////////////////////////////////////////
			/// \brief Write string to message.
			///
			/// Integers are not in network or host order.
			///
			////////////////////////////////////////////////////////////////
			Bool ReadArray( void * p_pArray, const SizeType p_Size );

			////////////////////////////////////////////////////////////////
			/// \brief Get message name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get message size.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetMessageSize( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get message size.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetPosition( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get message size.
			///
			////////////////////////////////////////////////////////////////
			void SetPosition( const SizeType p_Position );

		private:


			// Private variables.
			std::string m_Name;			///< Message name.
			SizeType	m_Position;		///< Current position in message
			Uint8 *		m_pMessage;		///< Pointer to message.
			SizeType	m_MessageSize;	///< Size of the message.

		};

	}

}

#endif