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

#include <Bit/System/String.hpp>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Int32 String::ToBool(const std::string & p_String)
	{
		std::stringstream ss;
		ss << p_String;

		// Get the result.
		bool result = false;
		ss >> result;
		
		// Retunr the result.
		return result;
	}

	std::string String::FromBool(const bool p_Bool)
	{
		if (p_Bool)
		{
			return "true";
		}

		return "false";
	}

	Int32 String::ToInt(const std::string & p_String)
	{
		std::stringstream ss;
		ss << p_String;

		// Get the result.
		Int32 result = 0;
		ss >> result;

		// Retunr the result.
		return result;
	}

	std::string String::FromInt(const Int32 p_Int)
	{
		std::stringstream ss;
		ss << p_Int;

		// Get the result.
		std::string result;
		ss >> result;

		// Retunr the result.
		return result;
	}

	Float32 String::ToFloat(const std::string & p_String)
	{
		std::stringstream ss;
		ss << p_String;

		// Get the result.
		Float32 result = 0.0f;
		ss >> result;

		// Retunr the result.
		return result;
	}

	std::string String::FromFloat(const Float32 p_Float)
	{
		std::stringstream ss;
		ss << p_Float;

		// Get the result.
		std::string result;
		ss >> result;

		// Retunr the result.
		return result;
	}

}