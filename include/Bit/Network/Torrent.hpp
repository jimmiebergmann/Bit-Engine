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
#include <Bit/Network/Url.hpp>
#include <Bit/Network/Http.hpp>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Torrent protocol class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Torrent
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Tracker response class.
		///
		////////////////////////////////////////////////////////////////
		class TrackerResponse
		{

			public:

				// Private typedefs
				typedef std::pair<Address, Uint16> PeerPair;
				typedef std::vector<PeerPair> PeerVector;

				// Private varaibles
				std::string m_FailureReason;	///< Reason why the request failed.
				std::string m_WarningMessage;	///< Warning message from the tracker.
				Uint32 m_Interval;				///< Interval in seconds that the client should wait unit sending next request.
				std::string m_TrackerId;		///< Tracker id, might be absent.
				Uint32 m_Complete;				///< Number of clients with the entire file(seeders).
				Uint32 m_Incomplete;			///< Number of non-seeder peers(leechers).
				PeerVector m_peers;				///< Vector of all the available peers.

		};

		////////////////////////////////////////////////////////////////
		/// \brief Tracker class.
		///
		////////////////////////////////////////////////////////////////
		class Tracker
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Tracker( );

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			Tracker( const Url & p_Url );

			////////////////////////////////////////////////////////////////
			/// \brief Set the tracker's URL
			///
			////////////////////////////////////////////////////////////////
			void SetUrl( const Url & p_Url );

			////////////////////////////////////////////////////////////////
			/// \brief Get the tracker's URL
			///
			////////////////////////////////////////////////////////////////
			const Url & GetUrl( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Send request(announce)
			///
			////////////////////////////////////////////////////////////////
			Bool SendRequest( TrackerResponse & p_Response );

		private:

			Url m_Url;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		Torrent( );

		////////////////////////////////////////////////////////////////
		/// \brief Read a torrent file and store it's data.
		///
		/// \param p_Filename Name of the torrent file.
		///
		////////////////////////////////////////////////////////////////
		Bool ReadTorrentFile( const std::string & p_Filename );


		//Bool SendTrackerRequest(	TrackerResponse & p_Response, const Address & p_Address, const Uint16 p_Port );

	private:

		// Private typedefs
		typedef std::vector<Tracker>			TrackerVector;	///< 
		typedef std::vector<std::string>		StringVector;	///< 
		typedef std::vector<Hash>				HashVector;		///< 
		typedef std::pair<std::string, Uint32>	FilePair;		///< Name and file size
		typedef std::vector<FilePair>			FileVector;		///< 

		// Private variables
		TrackerVector m_Trackers;	///< Vector of all available trackers.
		Uint32 m_PieceSize;			///< Size of each piece;
		HashVector m_Pieces;		///< All the pieces.
		Hash m_InfoHash;			///< The info hash.

	};

}

#endif