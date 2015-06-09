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

#include <Bit/Network/Net/ServerList.hpp>
#include <Bit/System/Json/Reader.hpp>
#include <Bit/Network/Http.hpp>
#include <Bit/Network/Url.hpp>
#include <iostream>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		// Static server list variables
		const ServerList ServerList::None = ServerList("", 0, "", "", false);
		const ServerList::UrlFields ServerList::NoFields;

		// Server list class
		ServerList::ServerList() :
			m_ListServerUrl(""),
			m_ListServerPort(0),
			m_GetPath(""),
			m_AddPath(""),
			m_Enabled(false)
		{
		}

		ServerList::ServerList(	const std::string & p_ListServerUrl,
								const Uint16 p_ListServerPort,
								const std::string & p_GetPath,
								const std::string & p_AddPath,
								const bool p_Enabled) :
			m_ListServerUrl(p_ListServerUrl),
			m_ListServerPort(p_ListServerPort),
			m_GetPath(p_GetPath),
			m_AddPath(p_AddPath),
			m_Enabled(p_Enabled)
		{
		}

		Json::Value ServerList::Get(const ServerList & p_ServerList, const Time & p_Timeout)
		{
			// Check if you really want to send a get request.
			if (p_ServerList.m_Enabled == false)
			{
				return Json::Value::NullValue;
			}

			Http http(p_ServerList.m_ListServerPort, p_Timeout);
			Http::Response response;
			Http::Request request(Http::Get, p_ServerList.m_GetPath);
			request.SetField("Host", p_ServerList.m_ListServerUrl);
			if (http.SendRequest(request, response, Address(p_ServerList.m_ListServerUrl)) == false || response.GetStatusCode() != Http::Ok )
			{
				return Json::Value::NullValue;
			}
			
			// Parse the response
			Json::Value jsonResponse;
			Json::Reader jsonReader;
			if (jsonReader.Parse(response.GetBody(), jsonResponse ) == false )
			{
				return Json::Value::NullValue;
			}

			// Return the json response.
			return jsonResponse;
		}

		Json::Value ServerList::Add(const ServerList & p_ServerList, const UrlFields & p_Fields, const Time & p_Timeout)
		{
			// Check if you really want to send a get request.
			if (p_ServerList.m_Enabled == false)
			{
				return Json::Value::NullValue;
			}

			// Create the full add path

			std::stringstream ss;
			ss << p_ServerList.m_AddPath << "?";
			for (UrlFields::const_iterator it = p_Fields.begin(); it != p_Fields.end(); it++)
			{
				// Add the parameter name
				ss << "&" << Url::GetBinaryEncodedString(it->first) << "=" << Url::GetBinaryEncodedString(it->second);
			}

			Http http(p_ServerList.m_ListServerPort, p_Timeout);
			Http::Response response;
			Http::Request request(Http::Get, ss.str());
			request.SetField("Host", p_ServerList.m_ListServerUrl);
			if (http.SendRequest(request, response, Address(p_ServerList.m_ListServerUrl)) == false || response.GetStatusCode() != Http::Ok)
			{
				return Json::Value::NullValue;
			}

			// Parse the response
			Json::Value jsonResponse;
			Json::Reader jsonReader;
			if (jsonReader.Parse(response.GetBody(), jsonResponse) == false)
			{
				return Json::Value::NullValue;
			}

			// Return the json response.
			return jsonResponse;
		}

	}

}