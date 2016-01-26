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

#ifndef BIT_SYSTEM_PRIVATE_LOG_MANAGER_HPP
#define BIT_SYSTEM_PRIVATE_LOG_MANAGER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Timestamp.hpp>
#include <string>
#include <sstream>

namespace Bit
{

	// Forward declarations
	class LogHandle;

	namespace Private
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Default log handle class.
		///
		/// Private log class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API LogManager
		{

		public:


			////////////////////////////////////////////////////////////////
			/// \brief	Start a new log entry.
			///			Set meta for the log manager.
			///
			////////////////////////////////////////////////////////////////
			static void Start(	const Uint32 p_Type,
										const Uint32 p_Channel,
										const std::string & p_File,
										const Int32 p_Line,
										const std::string & p_Function);

			////////////////////////////////////////////////////////////////
			/// \brief	Get the data stream for setting messages.
			///
			////////////////////////////////////////////////////////////////
			static std::stringstream & GetStream();

			////////////////////////////////////////////////////////////////
			/// \brief	Post message entry.
			///
			////////////////////////////////////////////////////////////////
			static void End();

			////////////////////////////////////////////////////////////////
			/// \brief Set the log handle.
			///
			////////////////////////////////////////////////////////////////
			static void SetHandle(LogHandle & p_Handle);

			////////////////////////////////////////////////////////////////
			/// \brief Get the default handle.
			///
			////////////////////////////////////////////////////////////////
			static LogHandle & GetDefaultHandle();

		};

	}

}

#endif