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

#ifndef BIT_GRAPHICS_MODEL_SKELETON_HPP
#define BIT_GRAPHICS_MODEL_SKELETON_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/Graphics/Model/Skeleton.hpp>
#include <vector>

namespace Bit
{

	// Forward declaractions
	class Animation;
	class VertexAnimation;
	class Bone;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model skeleton class.
	///
	/// \see Model
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Skeleton : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor. 
		///
		////////////////////////////////////////////////////////////////
		Skeleton( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. 
		///
		////////////////////////////////////////////////////////////////
		~Skeleton( );

		////////////////////////////////////////////////////////////////
		/// \brief Create and add vertex animation.
		///
		/// \return Pointer to the created key frame animation
		///		if successfully created, else NULL.
		///
		////////////////////////////////////////////////////////////////
		VertexAnimation * CreateVertexAnimation( );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove all the animations.
		///
		////////////////////////////////////////////////////////////////
		void DeleteAllAnimations( );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove an animation.
		///		Destroying the pointer as well.
		///
		/// \param p_pAnimation Pointer to the animation to delete.
		///
		/// \return true if track was successfully deleted, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool DeleteAnimation( Animation * p_pAnimation );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove an animation.
		///
		/// \param p_Index Index of the animation to delete.
		///
		/// \return true if track was successfully deleted, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool DeleteAnimation( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get animation at the given index.
		///
		////////////////////////////////////////////////////////////////
		Animation * GetAnimation( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get animation count.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetAnimationCount( );

	private:

		// Private typedefs
		typedef std::vector<Animation *> AnimationVector;
		//typedef std::vector<Bone *> BoneVector;

		// Private variables
		AnimationVector m_Animations; ///< Animations of for this skeleton.
		//BoneVector m_Bones; ///< Bone nodes of this skeleton(bones has children).

	};

}

#endif