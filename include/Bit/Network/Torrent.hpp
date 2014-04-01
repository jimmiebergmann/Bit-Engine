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

#ifndef BIT_NETWORK_TORRENT_HPP
#define BIT_NETWORK_TORRENT_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Hash.hpp>
#include <Bit/Network/Http.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Network address class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Torrent
	{

	public:

		class TrackerRequest
		{

			public:
		};

		class TrackerResponse
		{

			public:
		};


		Torrent( );

		Bool ReadTorrentFile( const std::string & p_Filename );

		Bool SendTrackerRequest(	const TrackerRequest & p_Request, TrackerResponse & p_Response,
									const Address & p_Address, const Uint16 p_Port );

	private:

		// Private typedefs
		typedef std::vector<std::string> StringVector;

		// Private variables
		StringVector m_Trackers;	///< Vector of all available trackers.
		Hash m_InfoHash;			///< The info hash.


	};

}

#endif