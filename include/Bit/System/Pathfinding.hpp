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

#ifndef BIT_SYSTEM_PATHFINDING_HPP
#define BIT_SYSTEM_PATHFINDING_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/System/PathNode.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	class BIT_API Pathfinding
	{

	public:

		// Destructor
		virtual ~Pathfinding( ) { };

		// Public general functions
		virtual BIT_UINT32 Load( BIT_BOOL * p_Data, Bit::Vector3_ui32 p_Size ) = 0;
		virtual BIT_UINT32 Generate( const Bit::Vector3_ui32 p_Start, const Bit::Vector3_ui32 p_End ) = 0;

		// Get functions
		virtual PathNode * GetNode( const BIT_UINT32 p_Inde ) = 0;
		virtual BIT_UINT32 GetNodeCount( ) const = 0;

	};

}

#endif