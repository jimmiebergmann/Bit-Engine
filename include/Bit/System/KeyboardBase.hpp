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

#ifndef BIT_SYSTEM_KEYBOARD_BASE_HPP
#define BIT_SYSTEM_KEYBOARD_BASE_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	namespace Private
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Mouse base class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API KeyboardBase
		{

		public:

			// Public enum
			enum eKey
			{
				// Unknown
				Unknown = -1,

				// Alphabet keys
				A,
				B,
				C,
				D,
				E,
				F,
				G,
				H,
				I,
				J,
				K,
				L,
				M,
				N,
				O,
				P,
				Q,
				R,
				S,
				T,
				U,
				V,
				W,
				X,
				Y,
				Z,

				// Numeric keys
				Num0,
				Num1,
				Num2,
				Num3,
				Num4,
				Num5,
				Num6,
				Num7,
				Num8,
				Num9,

				// Function Keys
				F1,
				F2,
				F3,
				F4,
				F5,
				F6,
				F7,
				F8,
				F9,
				F10,
				F11,
				F12,

				// Arrow keys
				Down,
				Left,
				Right,
				Up,

				// System keys
				AltLeft,
				AltRight,
				Backspace,
				BraceLeft,
				BraceRight,
				BracketLeft,
				BracketRight,
				Break,
				Capslock,
				Colon,
				ControlLeft,
				ControlRight,
				Comma,
				Delete,
				End,
				Escape,
				Greater,
				Home,
				Insert,
				Less,
				Minus,
				NumLock,
				PageDown,
				PageUp,
				Period,
				Plus,
				Print,
				Return,
				ScrollLock,
				SemiColon,
				ShiftLeft,
				ShiftRight,
				Space,
				SuperLeft,
				SuperRight,
				Tab,
				Underscore,
				KeyCount
			};

			// Destructor
			virtual ~KeyboardBase( ) { }

			// Public general functions
			virtual void Update( ) = 0;

			// Key translation function for platform keys
			virtual eKey TranslateKeyToBitKey( const Uint16 p_Key ) = 0;
			virtual Uint16 TranslateKeyToSystemKey( const eKey p_Key ) = 0;

			// Get state functions
			virtual Bool KeyIsDown( const eKey p_Key ) = 0;
			virtual Bool KeyIsUp( const eKey p_Key ) = 0;
			virtual Bool KeyIsJustPressed( const eKey p_Key ) = 0;
			virtual Bool KeyIsJustReleased( const eKey p_Key ) = 0;

		protected:

			// Protected functions
			virtual Bool GetCurrentKeyState( const eKey p_Key ) = 0;
			virtual Bool GetPreviousKeyState( const eKey p_Key ) = 0;
			virtual void SetCurrentKeyState( const eKey p_key, const Bool p_State ) = 0;
			virtual void SetPreviousKeyState( const eKey p_key, const Bool p_State ) = 0;

		};

	}

}

#endif
