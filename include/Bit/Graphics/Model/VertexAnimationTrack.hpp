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

#ifndef BIT_GRAPHICS_MODEL_VERTEX_ANIMATION_TRACK_HPP
#define BIT_GRAPHICS_MODEL_VERTEX_ANIMATION_TRACK_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/Model/AnimationTrack.hpp>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Vertex animation track class for 3D models.
	///
	/// \see Skeleton
	///
	////////////////////////////////////////////////////////////////
	class BIT_API VertexAnimationTrack : public AnimationTrack
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default destructor. 
		///
		////////////////////////////////////////////////////////////////
		VertexAnimationTrack( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. 
		///
		////////////////////////////////////////////////////////////////
		~VertexAnimationTrack( );

		////////////////////////////////////////////////////////////////
		/// \brief Create and add a key frame.
		///
		/// \return Pointer to the created key frame
		///		if successfully created, else NULL.
		///
		////////////////////////////////////////////////////////////////
		virtual KeyFrame * CreateKeyFrame( const Time & p_Time );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove all the key frames.
		///
		////////////////////////////////////////////////////////////////
		virtual void DeleteAllKeyFrames( );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove a key frame.
		///		Destroying the pointer as well.
		///
		/// \param p_pTrack Pointer to the key frame to delete.
		///
		/// \return true if track was successfully deleted, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool DeleteKeyFrame( KeyFrame * p_pKeyFrame );

		////////////////////////////////////////////////////////////////
		/// \brief Delete and remove a key frame.
		///
		/// \param p_Index Index of the key frame to delete.
		///
		/// \return true if track was successfully deleted, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool DeleteKeyFrame( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get key frame at the given time.
		///
		/// Picking the key frame closest to the time, rounded up.
		///
		////////////////////////////////////////////////////////////////
		virtual KeyFrame * GetKeyFrame( const Time & p_Time );

		////////////////////////////////////////////////////////////////
		/// \brief Get key frame at the given index.
		///
		////////////////////////////////////////////////////////////////
		virtual KeyFrame * GetKeyFrame( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get key frame count
		///
		////////////////////////////////////////////////////////////////
		virtual SizeType GetKeyFrameCount( );

	private:

		// Private typedefs.
		typedef std::vector<KeyFrame *> KeyFrameVector;

		// Private variables.
		KeyFrameVector m_KeyFrames;

	};

}

#endif