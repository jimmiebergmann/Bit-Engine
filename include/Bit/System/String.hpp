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

#ifndef BIT_SYSTEM_STRING_HPP
#define BIT_SYSTEM_STRING_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief String utility class
	///
	////////////////////////////////////////////////////////////////
	class BIT_API String
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Convert string to bool.
		///
		////////////////////////////////////////////////////////////////
		static Int32 ToBool(const std::string & p_String);

		////////////////////////////////////////////////////////////////
		/// \brief Convert bool to string.
		///
		////////////////////////////////////////////////////////////////
		static std::string FromBool(const bool p_Bool);

		////////////////////////////////////////////////////////////////
		/// \brief Convert string to int.
		///
		////////////////////////////////////////////////////////////////
		static Int32 ToInt(const std::string & p_String);

		////////////////////////////////////////////////////////////////
		/// \brief Convert string to int.
		///
		////////////////////////////////////////////////////////////////
		static std::string FromInt(const Int32 p_Int);

		////////////////////////////////////////////////////////////////
		/// \brief Convert string to float.
		///
		////////////////////////////////////////////////////////////////
		static Float32 ToFloat(const std::string & p_String);

		////////////////////////////////////////////////////////////////
		/// \brief Convert float to string.
		///
		////////////////////////////////////////////////////////////////
		static std::string FromFloat(const Float32 p_Float);

	};

}

#endif