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

#ifndef BIT_SYSTEM_DEFAULT_LOG_HPP
#define BIT_SYSTEM_DEFAULT_LOG_HPP

#include <Bit/System/Log.hpp>
#include <Bit/Build.hpp>
#include <string>

namespace Bit
{

	namespace Private
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Default log handle class.
		///
		/// This log handle will post all messages to the program console.
		///
		/// \See LogHandle
		///
		////////////////////////////////////////////////////////////////
		class BIT_API DefaultLogHandle : public LogHandle
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Virual function fired at posted info messages.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnInfo(const LogMessage & p_Message);

			////////////////////////////////////////////////////////////////
			/// \brief Virual function fired at posted warning messages.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnWarning(const LogMessage & p_Message);

			////////////////////////////////////////////////////////////////
			/// \brief Virual function fired at posted error messages.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnError(const LogMessage & p_Message);

		};

	}

}

#endif