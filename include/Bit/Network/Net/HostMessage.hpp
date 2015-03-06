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

#ifndef BIT_NETWORK_NET_HOST_MESSAGE_HPP
#define BIT_NETWORK_NET_HOST_MESSAGE_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/HostRecipientFilter.hpp>
#include <string>
#include <vector>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Server;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Host message class for server side.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API HostMessage
		{

		public:

			// Friend classes
			friend class Server;

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
			/// \brief Send user message to user.
			///
			////////////////////////////////////////////////////////////////
			Bool Send( HostRecipientFilter * p_pFilter );

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
			HostMessage( );

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			/// \param p_Name Name of the host message.
			/// \param p_pServer Pointer to server.
			/// \param p_MessageSize Message size. 0 < if dynamically allocated.
			///
			////////////////////////////////////////////////////////////////
			HostMessage( const std::string & p_Name, Server * p_pServer, Int32 p_MessageSize = -1 );

			// Private typedefs
			typedef std::vector<Uint8> MessageVector;

			// Private variables.
			std::string			m_Name;		///< Event name
			Server *			m_pServer;	///< Pointer to server class.
			MessageVector		m_Message;	///< Vector of message data.

		};

	}

}

#endif