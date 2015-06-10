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

#ifndef BIT_NETWORK_NET_SERVER_LIST_HPP
#define BIT_NETWORK_NET_SERVER_LIST_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Address.hpp>
#include <Bit/System/Time.hpp>
#include <Bit/System/Json/Value.hpp>
#include <map>


namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Server list class.
		///
		/// Server list class. This class is being used for connecting to a list server
		/// and add the server to the list. The default server list is None,
		/// which means there's no server list.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API ServerList
		{

		public:

			// Public typedefs
			typedef std::map<std::string, std::string> UrlFields;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			ServerList();

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			/// \param p_UseServerList Needs to be true if you want to add/get server list.
			///
			////////////////////////////////////////////////////////////////
			ServerList(	const std::string & p_ListServerUrl,
						const Uint16 p_ListServerPort,
						const std::string & p_GetPath = "/get.php",
						const std::string & p_AddPath = "/add.php",
						const std::string & p_RemovePath = "/remove.php",
						const bool m_Enabled = true);

			////////////////////////////////////////////////////////////////
			/// \brief Get list of servers
			///
			/// \return Response from list server. Empty json array if failed.
			///
			////////////////////////////////////////////////////////////////
			static Json::Value Get(const ServerList & p_ServerList, const Time & p_Timeout = Seconds(10.0f));

			////////////////////////////////////////////////////////////////
			/// \brief Add your server to the list
			///
			/// \return Response from list server, status field if true if succeeded.
			///
			////////////////////////////////////////////////////////////////
			static Json::Value Add(const ServerList & p_ServerList, const UrlFields & p_Fields = NoFields, const Time & p_Timeout = Seconds(10.0f));

			////////////////////////////////////////////////////////////////
			/// \brief Remove your server from the list
			///
			/// \return Response from list server, status field if true if succeeded.
			///
			////////////////////////////////////////////////////////////////
			static Json::Value Remove(const ServerList & p_ServerList, const Time & p_Timeout = Seconds(10.0f));

			////////////////////////////////////////////////////////////////
			/// \brief Static value for no server list
			///
			////////////////////////////////////////////////////////////////
			static const ServerList None;

			////////////////////////////////////////////////////////////////
			/// \brief Static value for no fields
			///
			////////////////////////////////////////////////////////////////
			static const UrlFields NoFields;

		private:

			// Private variables
			std::string m_ListServerUrl;
			Uint16 m_ListServerPort;
			std::string m_GetPath;
			std::string m_AddPath;
			std::string m_RemovePath;
			bool m_Enabled;			///< used for the Net::Server class.

		};
	}
}


#endif