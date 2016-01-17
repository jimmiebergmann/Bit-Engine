// ///////////////////////////////////////////////////////////////////////////
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

#include <Bit/Network/Torrent.hpp>
#include <Bit/System/Bencode/Reader.hpp>
#include <Bit/System/Bencode/Writer.hpp>
#include <Bit/System/Sha1.hpp>
#include <Bit/System/Randomizer.hpp>
#include <Bit/Network/Http.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/System/Log.hpp>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Global variables
	static Torrent::Tracker g_NullTracker;
	static Torrent::Peer g_NullPeer;

	// Torrent peer class
	Torrent::Peer::Peer( ) :
		m_Address( 0 ),
		m_Port( 0 )
	{
	}

	Torrent::Peer::Peer( const Address p_Address, const Uint16 p_Port ) :
		m_Address( p_Address ),
		m_Port( p_Port )
	{
	}

	// Tracker response class
	const std::string & Torrent::Tracker::Response::GetFailureReason( ) const
	{
		return m_FailureReason;
	}

	const std::string & Torrent::Tracker::Response::GetWarningMessage( ) const
	{
		return m_WarningMessage;
	}

	Uint32 Torrent::Tracker::Response::GetInterval( ) const
	{
		return m_Interval;
	}

	const std::string & Torrent::Tracker::Response::GeTrackerId( ) const
	{
		return m_TrackerId;
	}

	Uint32 Torrent::Tracker::Response::GetCompleteCount( ) const
	{
		return m_Complete;
	}

	Uint32 Torrent::Tracker::Response::GetIncompleteCount( ) const
	{
		return m_Incomplete;
	}

	SizeType Torrent::Tracker::Response::GetPeerCount( ) const
	{
		return m_Peers.size( );
	}

	Torrent::Peer & Torrent::Tracker::Response::GetPeer( const SizeType p_Index )
	{
		if( p_Index < 0 || p_Index >= m_Peers.size( ) )
		{
			return g_NullPeer;
		}

		return m_Peers[ p_Index ];
	}

	const Torrent::Tracker::Response::PeerVector & Torrent::Tracker::Response::GetPeers( ) const
	{
		return m_Peers;
	}

	// Tracker class
	Torrent::Tracker::Tracker( const Uint16 p_PeerRequestCount ) :
		m_PeerRequestCount( p_PeerRequestCount )
	{
	}

	Torrent::Tracker::Tracker( const Url & p_Url, const Uint16 p_PeerRequestCount ) :
		m_Url( p_Url ),
		m_PeerRequestCount( p_PeerRequestCount )
	{
	}

	void Torrent::Tracker::SetUrl( const Url & p_Url )
	{
		m_Url = p_Url;
	}

	const Url & Torrent::Tracker::GetUrl( ) const
	{
		return m_Url;
	}

	Bool Torrent::Tracker::SendRequest( Response & p_Response, const Torrent & p_Torrent, const Time & p_Timeout )
	{
		// Set up the http class
		Http http;
		http.SetPort( m_Url.GetPort( ) );
		http.SetTimeout( p_Timeout );

		// Create an request and response class
		std::stringstream path;
		path << "/" + m_Url.GetPath( );
		path << "?info_hash=" + p_Torrent.GetInfoHash( ).AsUrlEncodedString( );
		path << "&peer_id=" + p_Torrent.GetPeerId( ).AsUrlEncodedString( );
		path << "&port=" << p_Torrent.GetPort( );
		path << "&uploaded=0";
		path << "&downloaded=0";
		path << "&left=925892608";
		path << "&numwant=" << m_PeerRequestCount;
		path << "&compact=1";

		Http::Request request( Http::Get, path.str( ) );
		request.SetField( "Host", m_Url.GetDomain( ) );
		Http::Response response;

		// Send and receive the request
		if( http.SendRequest( request, response, Bit::Address( m_Url.GetDomain( ))) == false )
		{
			BitLog::NewEngine(Log::Error) << "Failed to send request." << Log::End;
			return false;
		}

		// Check the response status
		if( response.GetStatusCode( ) != Http::Ok )
		{
			// Set the failure reason
			if( response.GetBody( ).size( ) )
			{
				p_Response.m_FailureReason = response.GetBody( );
			}

			return false;
		}

		// Parse the response
		Bencode::Reader beReader;
		Bencode::Value beRoot;
		if( beReader.Parse( response.GetBody( ), beRoot ) == false )
		{
			BitLog::NewEngine(Log::Error) << "Failed to parse the response." << Log::End;
			return false;
		}

		// Parse the warning message
		p_Response.m_WarningMessage = beRoot.Get( "warning message", "" ).AsString( );

		// Parse the interval
		Int32 interval = beRoot.Get( "interval", 0 ).AsInt( );
		if( interval < 0 )
		{
			interval = 0;
		}
		p_Response.m_Interval = interval;

		// Parse the tracker id
		p_Response.m_TrackerId = beRoot.Get( "tracker id", "" ).AsString( );

		// Parse the complete count
		Int32 complete = beRoot.Get( "complete", 0 ).AsInt( );
		if( complete < 0 )
		{
			complete = 0;
		}
		p_Response.m_Complete = complete;

		// Parse the incomplete count
		Int32 incomplete = beRoot.Get( "incomplete", 0 ).AsInt( );
		if( incomplete < 0 )
		{
			incomplete = 0;
		}
		p_Response.m_Incomplete = incomplete;

		// Parse the peers
		Bencode::Value bePeers = beRoot.Get( "peers", Bencode::Value::NilValue );
		if( bePeers != Bencode::Value::NilValue )
		{
			// Make sure that this is a string.
			if( bePeers.GetType( ) == Bencode::Value::String )
			{

				// Parse the peer string
				const std::string & peerString = bePeers.AsString( );
				ParsePeers( peerString, p_Response.m_Peers );
			}
			else
			{
				p_Response.m_FailureReason = "[Bit] Currently not supporting list representation of peers.";
				return false;
			}
		}

/*		* std::string m_FailureReason;	///< Reason why the request failed.
		* std::string m_WarningMessage;	///< Warning message from the tracker.
		* Uint32 m_Interval;				///< Interval in seconds that the client should wait unit sending next request.
		* std::string m_TrackerId;		///< Tracker id, might be absent.
		* Uint32 m_Complete;				///< Number of clients with the entire file(seeders).
		* Uint32 m_Incomplete;			///< Number of non-seeder peers(leechers).
		PeerVector m_peers;					*/


		return true;
	}

	Bool Torrent::Tracker::ParsePeers( const std::string & p_PeerString, Response::PeerVector & p_Peers )
	{
		// Make sure that the string contains at least one peer.
		if( p_PeerString.size( ) < 6 )
		{
			return false;
		}

		// Go through(parse) all the peers in the string.
		for( SizeType i = 0; i < p_PeerString.size( ) / 6; i++ )
		{
			// Current position in the peer string.
			SizeType curPos = i * 6;

			// Get the ip for the current peer.
			Uint32 ip = 0;
			ip |= static_cast<Uint32>( p_PeerString[ curPos ] );
			ip |= static_cast<Uint32>( p_PeerString[ curPos + 1 ] ) << 8;
			ip |= static_cast<Uint32>( p_PeerString[ curPos + 2 ] ) << 16;
			ip |= static_cast<Uint32>( p_PeerString[ curPos + 3 ] ) << 24;
			ip = Ntoh32( ip );

			// Get the port for the current peer.
			Uint16 port = 0;
			port |= static_cast<Uint16>( p_PeerString[ curPos + 4 ] );
			port |= static_cast<Uint16>( p_PeerString[ curPos + 5 ] ) << 8;
			port = Ntoh16( port );

			// Add the peer to the output vector.
			p_Peers.push_back( Peer( Address( ip ), port ) );
		}

		return true;
	}

	// Torrent class
	Torrent::Torrent( const Uint16 p_Port ) :
		m_Port( p_Port ),
		m_Trackers( ),
		m_PieceSize( 0 ),
		m_Pieces( ),
		m_InfoHash( ),
		m_Files( )
	{
		// Create a random peer ID.
		for( SizeType i = 0; i < 20; i++ )
		{
			Int32 randomValue = RandomizeNumber( 0, 255 );
			m_PeerId.AddByte( static_cast<Uint8>( randomValue ) );
		}
	}

	Bool Torrent::ReadTorrentFile( const std::string & p_Filename )
	{
		// Clear the torrent class.
		// ...

		// Parse the torrentfile.
		Bencode::Reader beReader;
		Bencode::Value beRoot;

		if( beReader.ParseFromFile( p_Filename, beRoot ) == false )
		{
			BitLog::NewEngine(Log::Error) << "Could not parse the torrent file." << Log::End;
			return false;
		}

		// Make a list of all the trackers.
		std::vector<std::string> trackerAddresses;

		// Get the tracker.
		std::string tracker = beRoot.Get( "announce", "" ).AsString( );
		if( tracker.size( ) )
		{
			trackerAddresses.push_back(  tracker );
		}

		// Add trackers from the tracker list if it's available.
		Bencode::Value beTrackers = beRoot.Get( "announce-list", Bencode::Value::NilValue );
		for( SizeType i = 0; i <  beTrackers.GetSize( ); i++ )
		{
			// Get the tracker URI from the list.
			tracker = beTrackers[ i ][ 0 ].AsString( );

			// Error check the string size.
			if( tracker.size( ) == 0 )
			{
				continue;
			}

			// Make sure the string isn't already in the tracker vector.
			bool unique = true;
			for( SizeType j = 0; j < trackerAddresses.size( ); j++ )
			{
				if( trackerAddresses[ j ] == tracker )
				{
					unique = false;
					break;
				}
			}

			// Add the string if it's unique.
			if( unique )
			{
				trackerAddresses.push_back( tracker );
			}
		}

		// Now, add tracker objects.
		for( SizeType i = 0; i < trackerAddresses.size( ); i++ )
		{
			m_Trackers.push_back( Tracker( Url( trackerAddresses[ i ] ) ) );
		}


		// Read the torrent info.
		Bencode::Value beInfo = beRoot.Get( "info", Bencode::Value::NilValue );

		// Make sure that the torrent ifo is a dictionary.
		if( beInfo.GetType( ) != Bencode::Value::Dictionary )
		{
			BitLog::NewEngine(Log::Error) << "Could not get the info field" << Log::End;
			return false;
		}

		// Get the piece size.
		Int32 pieceSize = beInfo.Get( "piece length", 0 ).AsInt( );
		if( pieceSize <= 0 )
		{
			BitLog::NewEngine(Log::Error) << "Could get the piece size." << Log::End;
			return false;
		}
		m_PieceSize = pieceSize;

		// Read the piece data.
		const std::string & pieceData = beInfo.Get( "pieces", "" ).AsString( );

		// Check if there's any data
		if( pieceData.size( ) == 0  )
		{
			BitLog::NewEngine(Log::Error) << "Could get the piece data." << Log::End;
			return false;
		}

		// Error check the size
		if( pieceData.size( ) % 20 != 0  )
		{
			BitLog::NewEngine(Log::Error) << "Error in piece data." << Log::End;
			return false;
		}

		// Parse the piece data into the pieces(hash) vector
		for( SizeType i = 0; i < pieceData.size( ); i += 20 )
		{
			Hash piece;

			// Add the 20 piece bytes to the piece
			for( SizeType j = 0; j < 20; j++ )
			{
				piece.AddByte( pieceData[ i + j ] );
			}

			// Push back the piece
			m_Pieces.push_back( piece );

		}

		// Read all the files.
		Bencode::Value beFiles = beInfo.Get( "files", Bencode::Value::NilValue );
		if( beFiles.GetType( ) == Bencode::Value::List )
		{
			// This is the dictionary the file should be placed in.
			std::string dic = beInfo.Get( "name", "" ).AsString( );

			// Go through the list of files
			for( SizeType i = 0; i < beFiles.GetSize( ); i++ )
			{
				// Get the filename and file size
				std::string path = beFiles[ i ].Get( "path", "" ).AsString( );
				Int32 length = beFiles[ i ].Get( "length", 0 ).AsInt( );

				// Error check the file variables
				if( path.size( ) == 0 )
				{
					BitLog::NewEngine(Log::Error) << "Error in multifile path." << Log::End;
					return false;
				}

				if( length <= 0 )
				{
					BitLog::NewEngine(Log::Error) << "Error in multifile length." << Log::End;
					return false;
				}

				// Add the file
				m_Files.push_back( FilePair( path, length ) );
			}

		}
		// Could not find the "files" list, this should be a single file torrent
		else
		{
			// Get the filename and file size
			std::string filename = beInfo.Get( "name", "" ).AsString( );
			Int32 length = beInfo.Get( "length", 0 ).AsInt( );

			// Error check the file variables
			if( filename.size( ) == 0 )
			{
				BitLog::NewEngine(Log::Error) << "Error in single file name." << Log::End;
				return false;
			}

			if( length <= 0 )
			{
				BitLog::NewEngine(Log::Error) << "Error in single file length." << Log::End;
				return false;
			}

			// Add the file
			m_Files.push_back( FilePair( filename, length ) );
		}


		// Calculate the info hash, first get the info dictionary as a string
		Bencode::Writer beWriter;
		std::string infoString;

		if( beWriter.Write( infoString, beInfo ) == false )
		{
			BitLog::NewEngine(Log::Error) << "Could not get the info dictionary string." << Log::End;
			return false;
		}

		// Generate the info hash
		Sha1 infoHash;
		infoHash.Generate( infoString );
		m_InfoHash = infoHash.GetHash( );

		return true;
	}

	void Torrent::SetPort( const Uint16 p_Port )
	{
		m_Port = p_Port;
	}

	Uint16 Torrent::GetPort( ) const
	{
		return m_Port;
	}

	const Hash & Torrent::GetPeerId( ) const
	{
		return m_PeerId;
	}

	const Hash & Torrent::GetInfoHash( ) const
	{
		return m_InfoHash;
	}

	SizeType Torrent::GetTrackerCount( ) const
	{
		return static_cast<SizeType>( m_Trackers.size( ) );
	}

	Torrent::Tracker & Torrent::GetTracker( const SizeType p_Index )
	{
		if( p_Index < 0 || p_Index >= m_Trackers.size( ) )
		{
			return g_NullTracker;
		}

		return m_Trackers[ p_Index ];
	}

}
