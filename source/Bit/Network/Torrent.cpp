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
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

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

	Bool Torrent::Tracker::SendRequest( TrackerResponse & p_Response )
	{
	
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

		// Read all the files.




		return true;
	}


}