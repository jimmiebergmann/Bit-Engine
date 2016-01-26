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

#include <Bit/Graphics/Model/VertexAnimation.hpp>
#include <Bit/Graphics/Model/VertexAnimationTrack.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	VertexAnimation::VertexAnimation( )
	{
	}

	VertexAnimation::~VertexAnimation( )
	{
		// Go through and delete all the animations
		for( TrackVector::iterator it = m_Tracks.begin( ); it != m_Tracks.end( ); it++ )
		{
			delete (*it);
		}
	}

	AnimationTrack * VertexAnimation::CreateTrack( )
	{
		// Create, append and return the key frame animation
		VertexAnimationTrack * pTrack = new VertexAnimationTrack;
		m_Tracks.push_back( pTrack );
		return pTrack;
	}

	void VertexAnimation::DeleteAllTracks( )
	{
		// Go through and delete all the animations
		for( TrackVector::iterator it = m_Tracks.begin( ); it != m_Tracks.end( ); it++ )
		{
			delete (*it);
		}
		m_Tracks.clear( );
	}

	Bool VertexAnimation::DeleteTrack( AnimationTrack * p_pTrack )
	{
		// Search for the pointer and find it's index
		Int32 index = -1;

		// Go through all the track
		for( Int32 i = 0; i < m_Tracks.size( ); i++ )
		{
			if( m_Tracks[ i ] == p_pTrack )
			{
				index = i;
				break;
			}
		}

		// Return if we couldn't find the pointer
		if( index < 0 )
		{
			return false;
		}

		// Delete the pointer
		delete m_Tracks[ index ];

		// Erase the track from the vector
		m_Tracks.erase( m_Tracks.begin( ) + index );

		// Succeeded.
		return true;
	}

	Bool VertexAnimation::DeleteTrack( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Tracks.size( ) ) )
		{
			return false;
		}

		// Delete the pointer
		delete m_Tracks[ p_Index ];

		// Erase the track from the vector
		m_Tracks.erase( m_Tracks.begin( ) + p_Index );

		// Succeeded.
		return true;
	}

	AnimationTrack * VertexAnimation::GetTrack( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Tracks.size( ) ) )
		{
			return NULL;
		}
		
		return m_Tracks[ p_Index ];
	}

	SizeType VertexAnimation::GetTrackCount( )
	{
		return static_cast<SizeType>( m_Tracks.size( ) );
	}

	Animation::eType VertexAnimation::GetType( )
	{
		return PerVertex;
	}

}