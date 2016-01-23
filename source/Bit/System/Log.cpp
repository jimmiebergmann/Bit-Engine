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

#include <Bit/System/Log.hpp>
#include <Bit/System/Private/DefaultLogHandle.hpp>
#include <sstream>
#include <memory>
#include <string>
#include <cstdio>
#include <stdio.h>

#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	//LogManager			Log::g_LogManagerInstance;
	static				Private::DefaultLogHandle	g_DefaultLogHandle;
	LogHandle *			Log::g_CurrentLogHandle = &g_DefaultLogHandle;
	LogMessage			Log::g_LogMessage;
	Mutex				Log::g_Mutex;
	

	// Log class
	void Log::SetHandle(LogHandle & p_Handle)
	{
		g_CurrentLogHandle = &p_Handle;
	}

	LogHandle & Log::GetDefaultHandle()
	{
		return g_DefaultLogHandle;
	}

	void Log::SetMetaData(	const std::string & p_File,
							const Int32 p_Line,
							const std::string & p_Function)
	{
		g_LogMessage.file = p_File;
		g_LogMessage.line = p_Line;
		g_LogMessage.function = p_Function;
	}

	void Log::StringFormat(const char * p_FormatedString, ...)
	{
		if (p_FormatedString == NULL)
		{
			return;
		}

		int size = strlen(p_FormatedString) * 2 + 50;   // Use a rubric appropriate for your code
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			g_LogMessage.message.resize(size);
			va_start(ap, p_FormatedString);
			int n = vsnprintf((char *)g_LogMessage.message.data(), size, p_FormatedString, ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				g_LogMessage.message.resize(n);
				return;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
	}

	// Log message structure
	LogMessage::LogMessage() :
		message(""),
		type(Log::Info),
		user(Log::Client),
		file(""),
		line(0),
		function("")
	{
	}

	// Log handle class
	void LogHandle::OnMessage(const LogMessage & p_Message)
	{
	}

	void LogHandle::OnInfo(const LogMessage & p_Message)
	{
	}

	void LogHandle::OnWarning(const LogMessage & p_Message)
	{
	}

	void LogHandle::OnError(const LogMessage & p_Message)
	{
	}

}