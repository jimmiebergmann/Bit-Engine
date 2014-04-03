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

#ifndef BIT_SYSTEM_MOUSE_BASE_HPP
#define BIT_SYSTEM_MOUSE_BASE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	namespace Private
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Mouse base class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API MouseBase
		{

		public:

			// Public enum
			enum eButton
			{
				Unknown = -1,
				Left,
				Middle,
				Right,
				ButtonCount
			};

			// Destructor
			virtual ~MouseBase( ) { }

			// Public general functions
			virtual void Update( ) = 0;

			// Button translation function for platform keys
			virtual eButton TranslateButtonToBitButton( const Uint16 p_Button ) = 0;
			virtual Uint16 TranslateButtonToSystemButton( const eButton p_Button ) = 0;

			// Get state functions
			virtual Vector2i32 GetPosition( ) const = 0;
			virtual Bool ButtonIsDown( const eButton p_Button ) = 0;
			virtual Bool ButtonIsUp( const eButton p_Button ) = 0;
			virtual Bool ButtonIsJustPressed( const eButton p_Button ) = 0;
			virtual Bool ButtonIsJustReleased( const eButton p_Button ) = 0;

		protected:

			// Protected functions
			virtual Bool GetCurrentButtonState( const eButton p_Button ) = 0;
			virtual Bool GetPreviousButtonState( const eButton p_Button ) = 0;
			virtual void SetCurrentButtonState( const eButton p_Button, const Bool p_State ) = 0;
			virtual void SetPreviousButtonState( const eButton p_Button, const Bool p_State ) = 0;

		};

	}

}

#endif
