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

#include <Bit/System/Private/DefaultLogHandle.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Private
	{

		void DefaultLogHandle::OnInfo(const LogMessage & p_Message)
		{
			if (p_Message.user == Log::Client)
			{
				std::cout << "Engine: ";
			}
			else
			{
				std::cout << "Client: ";
			}

			if (p_Message.function.size())
			{
				std::cout << p_Message.function << "(" << p_Message.line << "): ";
			}

			std::cout << "Info: " << p_Message.message << std::endl;
		}

		void DefaultLogHandle::OnWarning(const LogMessage & p_Message)
		{
			if (p_Message.user == Log::Client)
			{
				std::cout << "Engine: ";
			}
			else
			{
				std::cout << "Client: ";
			}

			if (p_Message.function.size())
			{
				std::cout << p_Message.function << "(" << p_Message.line << "): ";
			}

			std::cout << "Warning: " << p_Message.message << std::endl;
		}

		void DefaultLogHandle::OnError(const LogMessage & p_Message)
		{
			if (p_Message.user == Log::Client)
			{
				std::cout << "Engine: ";
			}
			else
			{
				std::cout << "Client: ";
			}

			if (p_Message.function.size())
			{
				std::cout << p_Message.function << "(" << p_Message.line << "): ";
			}

			std::cout << "Error: " << p_Message.message << std::endl;
		}

	}

}