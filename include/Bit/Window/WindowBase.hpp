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

#ifndef __BIT_WINDOW_BASE_HPP__
#define __BIT_WINDOW_BASE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Window/Event.hpp>
#include <Bit/System/Vector2.hpp>
#include <string>
#include <list>

namespace Bit
{
	class WindowBase
	{

	public:

		// Public functions
		virtual BIT_UINT32 Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
			const std::string p_Title ) = 0;
		virtual BIT_UINT32 Destroy( ) = 0;
		virtual BIT_UINT32 DoEvents( ) = 0;

		// Get functions
		// ...

		// Set functions
		virtual BIT_UINT32 SetTitle( std::string p_Title ) = 0;

		// Public variables
		Vector2_ui32 m_Size;
		BIT_UINT32 m_Bits;
		std::string m_Title;
		std::list< Event > m_EventQueue;

	};

}

#endif
