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

#include <Bit/Graphics/Model/VertexAnimationTrack.hpp>
#include <Bit/Graphics/Model/VertexKeyFrame.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	VertexAnimationTrack::VertexAnimationTrack( )
	{
	}

	VertexAnimationTrack::~VertexAnimationTrack( )
	{
		// Go through and delete all the key frames
		for( KeyFrameVector::iterator it = m_KeyFrames.begin( ); it != m_KeyFrames.end( ); it++ )
		{
			delete (*it);
		}
	}

	KeyFrame * VertexAnimationTrack::CreateKeyFrame( const Time & p_Time )
	{
		VertexKeyFrame * pKeyFrame = new VertexKeyFrame( p_Time );
		if( m_KeyFrames.size( ) && p_Time < m_KeyFrames.back( )->GetTime( ) )
		{
			// Find the right location for the key frame with a binary search
			SizeType min = 0;
			SizeType max = static_cast<SizeType>( m_KeyFrames.size( ) ) - 1;
			SizeType pivot = 0;

			// Loop until min < max(min != max) 
			while( min < max )
			{
				pivot = ( min + max ) / 2;

				if( p_Time.AsMicroseconds( ) >= m_KeyFrames[ pivot ]->GetTime( ).AsMicroseconds( ) )
				{
					min = pivot + 1;
				}
				else
				{
					max = pivot;
				}
			}

			// Insert the key frame
			m_KeyFrames.insert( m_KeyFrames.begin( ) + min, pKeyFrame );

			// Return the key frame 
			return pKeyFrame;
		}
			
		// Add the key frame to the back
		m_KeyFrames.push_back( pKeyFrame );

		// Return the key frame 
		return pKeyFrame;

	}

	void VertexAnimationTrack::DeleteAllKeyFrames( )
	{
		// Go through and delete all the key frames
		for( KeyFrameVector::iterator it = m_KeyFrames.begin( ); it != m_KeyFrames.end( ); it++ )
		{
			delete (*it);
		}
		m_KeyFrames.clear( );
	}

	Bool VertexAnimationTrack::DeleteKeyFrame( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_KeyFrames.size( ) ) )
		{
			return false;
		}

		// Delete the pointer
		delete m_KeyFrames[ p_Index ];

		// Erase the key frame from the vector
		m_KeyFrames.erase( m_KeyFrames.begin( ) + p_Index );

		// Succeeded.
		return true;
	}

	Bool VertexAnimationTrack::DeleteKeyFrame( KeyFrame * p_pKeyFrame )
	{
		// Search for the pointer and find it's index
		Int32 index = -1;

		// Go through all the key frame
		for( Int32 i = 0; i < m_KeyFrames.size( ); i++ )
		{
			if( m_KeyFrames[ i ] == p_pKeyFrame )
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
		delete m_KeyFrames[ index ];

		// Erase the key frame from the vector
		m_KeyFrames.erase( m_KeyFrames.begin( ) + index );

		// Succeeded.
		return true;
	}

	KeyFrame * VertexAnimationTrack::GetKeyFrame( const Time & p_Time )
	{
		if( m_KeyFrames.size( ) == 0 )
		{
			return NULL;
		}

		// Find the key frame with a binary search
		SizeType min = 0;
		SizeType max = static_cast<SizeType>( m_KeyFrames.size( ) ) - 1;
		SizeType pivot = 0;

		// Loop until min < max(min != max) 
		while( min < max )
		{
			pivot = ( min + max ) / 2;

			if( p_Time.AsMicroseconds( ) > m_KeyFrames[ pivot ]->GetTime( ).AsMicroseconds( ) )
			{
				min = pivot + 1;
			}
			else
			{
				max = pivot;
			}
		}

		// We found our spot( min or max ).
		return m_KeyFrames[ min ];
	}

	KeyFrame * VertexAnimationTrack::GetKeyFrame( const SizeType p_Index )
	{
		if( p_Index >= static_cast<SizeType>( m_KeyFrames.size( ) ) )
		{
			return NULL;
		}

		return m_KeyFrames[ p_Index ];
	}

	SizeType VertexAnimationTrack::GetKeyFrameCount( )
	{
		return static_cast<SizeType>( m_KeyFrames.size( ) );
	}

}