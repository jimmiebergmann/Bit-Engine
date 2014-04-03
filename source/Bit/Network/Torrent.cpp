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
#include <Bit/Network/Http.hpp>
#include <iostream>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Global variables
	static Torrent::Tracker g_NullTracker;


	// Tracker class
	Torrent::Tracker::Tracker( )
	{
	}

	Torrent::Tracker::Tracker( const Url & p_Url ) :
		m_Url( p_Url )
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

	Bool Torrent::Tracker::SendRequest( TrackerResponse & p_Response, const Torrent & p_Torrent )
	{
		// Set up the http class
		Http http;
		http.SetPort( m_Url.GetPort( ) );
		http.SetTimeout( 10 * 1000 );

		// Create an request and response class
		std::stringstream path;
		path << "/" + m_Url.GetPath( );
		path << "?info_hash=" + p_Torrent.GetInfoHash( ).AsUrlEncodedString( );
		path << "&peer_id=-UT3320-_v%a0%5c%01%5e%be%8d%e6%40%d3%fb";
		path << "&port=12345";
		path << "&uploaded=0";
		path << "&downloaded=0";
		path << "&left=925892608";
		path << "&compact=1";

		Http::Request request( Http::Get, path.str( ) );
		request.SetField( "Host", m_Url.GetDomain( ) );
		Http::Response response;

		// Send and receive the request
		if( http.SendRequest( request, response, Bit::Address( m_Url.GetDomain( ))) == false )
		{
			std::cout << "Failed to send request." << std::endl;
			return false;
		}

		// Check the response status
		if( response.GetStatusCode( ) != Http::Ok )
		{
			std::cout << "Error in response." << std::endl;
			return false;
		}

		// Parse the response


	
		return true;
	}

	// Torrent class
	Torrent::Torrent( )
	{
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
			std::cerr << "Could not parse the torrent file." << std::endl;
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
			std::cerr << "Could not get the info field" << std::endl;
			return false;
		}

		// Get the piece size.
		Int32 pieceSize = beInfo.Get( "piece length", 0 ).AsInt( );
		if( m_PieceSize <= 0 )
		{
			std::cerr << "Could get the piece size." << std::endl;
			return false;
		}
		m_PieceSize = pieceSize;

		// Read the piece data.
		const std::string & pieceData = beInfo.Get( "pieces", "" ).AsString( );

		// Check if there's any data
		if( pieceData.size( ) == 0  )
		{
			std::cerr << "Could get the piece data." << std::endl;
			return false;
		}

		// Error check the size
		if( pieceData.size( ) % 20 != 0  )
		{
			std::cerr << "Error in piece data." << std::endl;
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
					std::cerr << "Error in multifile path." << std::endl;
					return false;
				}

				if( length <= 0 )
				{
					std::cerr << "Error in multifile length." << std::endl;
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
				std::cerr << "Error in single file name." << std::endl;
				return false;
			}

			if( length <= 0 )
			{
				std::cerr << "Error in single file length." << std::endl;
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
			std::cerr << "Could not get the info dictionary string." << std::endl;
			return false;
		}

		// Generate the info hash
		Sha1 infoHash;
		infoHash.Generate( infoString );
		m_InfoHash = infoHash.GetHash( );

		return true;
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