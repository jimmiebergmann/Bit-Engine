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

#ifndef BIT_SYSTEM_PATH_NODE_HPP
#define BIT_SYSTEM_PATH_NODE_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	class BIT_API PathNode
	{

	public:

		// Destructor
		PathNode(	PathNode * p_pTail,
						PathNode * p_pHead,
						Bit::Vector3_f32 p_Position );

		// Get functions
		PathNode * GetTail( ) const;
		PathNode * GetHead( ) const;
		Bit::Vector3_f32 GetPosition( ) const;

	private:

		PathNode * m_pTail;
		PathNode * m_pHead;
		Bit::Vector3_f32 m_Position;

	};

}

#endif