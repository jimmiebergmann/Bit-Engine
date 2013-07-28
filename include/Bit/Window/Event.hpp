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

#ifndef __BIT_WINDOW_EVENT_HPP__
#define __BIT_WINDOW_EVENT_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Keyboard.hpp>
#include <Bit/System/Mouse.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	struct Event
	{

		enum eEventType
		{
			None = 0,
			Closed = 1,
			Moved = 2,
			Resized = 3,
			GainedFocus = 4,
			LostFocus = 5,
			KeyPressed = 6,
			KeyReleased = 7,
			MouseMoved = 8,
			MouseButtonPressed = 9,
			MouseButtonReleased = 10
		};

		eEventType Type;

		Bit::Vector2_si32 Size;
		Bit::Vector2_si32 Position;
		Bit::Vector2_si32 MousePosition;
		Keyboard::eKey Key;
		//Mouse::eButton Button;
		BIT_UINT32 Button;

	};

}

#endif
