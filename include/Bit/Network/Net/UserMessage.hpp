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

#ifndef BIT_NETWORK_NET_USER_MESSAGE_HPP
#define BIT_NETWORK_NET_USER_MESSAGE_HPP

#include <Bit/Build.hpp>
#include <string>
#include <vector>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Client;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief User message class for server side.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API UserMessage
		{

		public:

			// Friend classes
			friend class Client;

			////////////////////////////////////////////////////////////////
			/// \brief Write byte to message.
			///
			////////////////////////////////////////////////////////////////
			void WriteByte( const Uint8 p_Byte );

			////////////////////////////////////////////////////////////////
			/// \brief Write signed integer to message.
			///
			////////////////////////////////////////////////////////////////
			void WriteInt( const Int32 p_Int );

			////////////////////////////////////////////////////////////////
			/// \brief Write floating point to message.
			///
			////////////////////////////////////////////////////////////////
			void WriteFloat( const Float32 p_Float );

			////////////////////////////////////////////////////////////////
			/// \brief Write string to message.
			///
			////////////////////////////////////////////////////////////////
			void WriteString( const std::string & p_String );

			////////////////////////////////////////////////////////////////
			/// \brief Write string to message.
			///
			/// Integers are not in network or host order.
			///
			////////////////////////////////////////////////////////////////
			void WriteArray( const void * p_pArray, const SizeType p_Size );

			////////////////////////////////////////////////////////////////
			/// \brief Send user message.
			///
			////////////////////////////////////////////////////////////////
			Bool Send( const Bool p_Reliable = true );

			////////////////////////////////////////////////////////////////
			/// \brief Get event name
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

		private:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			UserMessage( );

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			/// \param p_Name Name of the user message.
			/// \param p_pClient Pointer to client.
			/// \param p_MessageSize Message size. 0 < if dynamically allocated.
			///
			////////////////////////////////////////////////////////////////
			UserMessage( const std::string & p_Name, Client * p_pClient, Int32 p_MessageSize = -1 );

			// Private typedefs
			typedef std::vector<Uint8> MessageVector;

			// Private variables.
			std::string			m_Name;		///< Event name
			Client *			m_pClient;	///< Pointer to client class.
			MessageVector		m_Message;	///< Vector of message data.

		};

	}

}

#endif