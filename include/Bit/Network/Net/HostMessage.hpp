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

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Server;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Entity manager class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API UserMessage
		{

		public:

			// Friend classes
			friend class ServerEntityChanger;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			UserMessage( const std::string & p_Name, Server * p_pServer );

			////////////////////////////////////////////////////////////////
			/// \brief Send user message to user.
			///
			////////////////////////////////////////////////////////////////
			Bool Send( const Uint16 p_User );

			////////////////////////////////////////////////////////////////
			/// \brief Get event name
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

		private:

			// Private variables.
			std::string		m_Name;		///< Event name
			Server *		m_pServer;	///< Pointer to server class.

		};

	}

}

#endif