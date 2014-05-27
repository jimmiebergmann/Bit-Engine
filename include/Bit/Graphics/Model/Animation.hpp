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

#ifndef BIT_GRAPHICS_MODEL_ANIMATION_HPP
#define BIT_GRAPHICS_MODEL_ANIMATION_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	// Forward declaractions
	class AnimationTrack;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model animation base class.
	///
	/// \see Skeleton
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Animation
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Animation type enumerator
		///
		////////////////////////////////////////////////////////////////
		enum eType
		{
			None = 0,	///< No animation
			PerVertex,	///< Per vertex animation.
			Skeletal	///< Skeletal animation.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor. 
		///
		////////////////////////////////////////////////////////////////
		virtual ~Animation( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Create and add an animation track.
		///
		/// \return Pointer to the created animation track
		///		if successfully created, else NULL.
		///
		////////////////////////////////////////////////////////////////
		AnimationTrack * CreateAnimationTrack( );
	};

}

#endif