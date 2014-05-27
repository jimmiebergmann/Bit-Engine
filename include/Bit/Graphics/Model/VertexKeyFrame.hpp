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

#ifndef BIT_GRAPHICS_MODEL_VERTEX_KEY_FRAME_HPP
#define BIT_GRAPHICS_MODEL_VERTEX_KEY_FRAME_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/Model/KeyFrame.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Vertex key frame class for 3D models.
	///
	/// \see Skeleton
	///
	////////////////////////////////////////////////////////////////
	class BIT_API VertexKeyFrame
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor. 
		///
		////////////////////////////////////////////////////////////////
		VertexKeyFrame( const Time & p_Time );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. 
		///
		////////////////////////////////////////////////////////////////
		~VertexKeyFrame( );

		////////////////////////////////////////////////////////////////
		/// \brief Get time. 
		///
		////////////////////////////////////////////////////////////////
		virtual const Time & GetTime( ) const;

	private:

		Time m_Time;

	};

}

#endif