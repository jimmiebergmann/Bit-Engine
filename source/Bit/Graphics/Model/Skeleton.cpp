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

#include <Bit/Graphics/Model/Skeleton.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/Model/VertexAnimation.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Skeleton::Skeleton(Model * p_pModel) :
		m_pParent(p_pModel)
	{
	}

	Skeleton::~Skeleton( )
	{
		// Go through and delete all the animations
		for( AnimationVector::iterator it = m_Animations.begin( ); it != m_Animations.end( ); it++ )
		{
			delete (*it);
		}
	}

	VertexAnimation * Skeleton::CreateVertexAnimation( )
	{
		// Create, append and return the key frame animation
		VertexAnimation * pAnimation = new VertexAnimation;
		m_Animations.push_back( pAnimation );
		return pAnimation;
	}

	void Skeleton::DeleteAllAnimations( )
	{
		// Go through and delete all the animations
		for( AnimationVector::iterator it = m_Animations.begin( ); it != m_Animations.end( ); it++ )
		{
			delete (*it);
		}
		m_Animations.clear( );
	}

	Bool Skeleton::DeleteAnimation( Animation * p_pAnimation )
	{
		// Search for the pointer and find it's index
		Int32 index = -1;

		// Go through all the animations
		for( Int32 i = 0; i < m_Animations.size( ); i++ )
		{
			if( m_Animations[ i ] == p_pAnimation )
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

		// Remove the animation from the animation from animation state if needed.
		if (m_pParent && m_pParent->GetAnimationState().m_pAnimation == p_pAnimation)
		{
			m_pParent->GetAnimationState().m_pAnimation = NULL;
			m_pParent->GetAnimationState().m_AnimationIndex = 0;
		}

		// Erase the animation from the vector
		m_Animations.erase(m_Animations.begin() + index);

		// Delete the pointer
		delete m_Animations[index];


		// Succeeded.
		return true;
	}

	Bool Skeleton::DeleteAnimation( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Animations.size( ) ) )
		{
			return false;
		}

		// Remove the animation from the animation from animation state if needed.
		if (m_pParent && m_pParent->GetAnimationState().m_AnimationIndex == p_Index)
		{
			m_pParent->GetAnimationState().m_pAnimation = NULL;
			m_pParent->GetAnimationState().m_AnimationIndex = 0;
		}

		// Erase the animation from the vector
		m_Animations.erase( m_Animations.begin( ) + p_Index );

		// Delete the pointer
		delete m_Animations[p_Index];

		// Succeeded.
		return true;
	}

	Animation * Skeleton::GetAnimation( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Animations.size( ) ) )
		{
			return NULL;
		}
		
		return m_Animations[ p_Index ];
	}

	SizeType Skeleton::GetAnimationCount( )
	{
		return static_cast<SizeType>( m_Animations.size( ) );
	}

}