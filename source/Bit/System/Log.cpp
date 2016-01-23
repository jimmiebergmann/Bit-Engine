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



	// Static functions
	static std::string StringFormat(const char * p_FormatedString, ...)
	{
		if (p_FormatedString == NULL)
		{
			return "";
		}

		int size = strlen(p_FormatedString) * 2 + 50;   // Use a rubric appropriate for your code
		std::string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, p_FormatedString);
			int n = vsnprintf((char *)str.data(), size, p_FormatedString, ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				return str;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
		return str;
	}

	static void StringFormat(std::string & p_OutString, const char * p_FormatedString, ...)
	{
		if (p_FormatedString == NULL)
		{
			return;
		}

		int size = strlen(p_FormatedString) * 2 + 50;   // Use a rubric appropriate for your code
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			p_OutString.resize(size);
			va_start(ap, p_FormatedString);
			int n = vsnprintf((char *)p_OutString.data(), size, p_FormatedString, ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				p_OutString.resize(n);
				return;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}

	}



	// Static variables
	/*static LogManager					g_LogManagerInstance;
	
	static LogHandle *					g_CurrentLogHandle = &g_DefaultLogHandle;
	static LogMessage					g_LogMessage;
	static std::stringstream			g_MessageStream;
	static Mutex						g_Mutex;
	static Mutex						g_IsLoggingMutex;
	static Bool							g_IsLogging;*/

	//LogManager			Log::g_LogManagerInstance;
	static				Private::DefaultLogHandle	g_DefaultLogHandle;
	LogHandle *			Log::g_CurrentLogHandle = &g_DefaultLogHandle;
	LogMessage			Log::g_LogMessage;
	Mutex				Log::g_Mutex;
	


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



	// Log class
	void Log::SetHandle(LogHandle & p_Handle)
	{
		g_CurrentLogHandle = &p_Handle;
	}

	LogHandle & Log::GetDefaultHandle()
	{
		return g_DefaultLogHandle;
	}
	/*
	Bool Log::New(const char * p_FormatedString, ...)
	{
		va_list argptr;
		va_start(argptr, p_FormatedString);
		Bool status = New(Info, p_FormatedString, argptr);
		va_end(argptr);

		return status;
	}

	Bool Log::New(const eType p_Type, const char * p_FormatedString, ...)
	{
		va_list argptr;
		va_start(argptr, p_FormatedString);

		printf(p_FormatedString, argptr);

		std::string message = StringFormat(p_FormatedString, argptr);
		va_end(argptr);

		


		// Ignore message if it's empty
		if (message.size() == 0)
		{
			// Return the log manager instance.
			return false;
		}


		// Set the log message data
		g_LogMessage.message = message;

		// Remove lambda text in function name


		// Set the timestamp
		g_LogMessage.timestamp = Timestamp::Now();

		// Fire the OnMessage function for the handle.
		g_CurrentLogHandle->OnMessage(g_LogMessage);

		// Also fire the On[eType] function for the handle.
		switch (g_LogMessage.type)
		{
		case Log::Info:
			g_CurrentLogHandle->OnInfo(g_LogMessage);
			break;
		case Log::Warning:
			g_CurrentLogHandle->OnWarning(g_LogMessage);
			break;
		case Log::Error:
			g_CurrentLogHandle->OnError(g_LogMessage);
			break;
		default:
			break;
		};

		// Reset to default flags.
		//g_LogMessage.type = Log::Info;


		return true;
	}

	Bool Log::NewEngine(const char * p_FormatedString, ...)
	{
		va_list argptr;
		va_start(argptr, p_FormatedString);
		Bool status = NewEngine(Info, p_FormatedString, argptr);
		va_end(argptr);

		return status;
	}

	Bool Log::NewEngine(const eType p_Type, const char * p_FormatedString, ...)
	{
		return true;
	}*/

	/*
	LogManager & Log::Post()
	{
		// Ignore message if it's empty
		if (g_MessageStream.str().size() == 0)
		{
			// Return the log manager instance.
			return g_LogManagerInstance;
		}


		// Set the log message data
		g_LogMessage.message = g_MessageStream.str();

		// Remove lambda text in function name
		std::size_t lambdaStart = g_LogMessage.function.find('<');
		if (lambdaStart != std::string::npos)
		{
			g_LogMessage.function.resize(lambdaStart);
		}

		// Set the timestamp
		g_LogMessage.timestamp = Timestamp::Now();

		// Fire the OnMessage function for the handle.
		g_CurrentLogHandle->OnMessage(g_LogMessage);

		// Also fire the On[eType] function for the handle.
		switch (g_LogMessage.type)
		{
		case Log::Info:
			g_CurrentLogHandle->OnInfo(g_LogMessage);
			break;
		case Log::Warning:
			g_CurrentLogHandle->OnWarning(g_LogMessage);
			break;
		case Log::Error:
			g_CurrentLogHandle->OnError(g_LogMessage);
			break;
		default:
			break;
		};

		// Reset to default flags.
		g_LogMessage.type = Log::Info;
		g_LogMessage.isEngineMessage = false;
		g_LogMessage.file = "";
		g_LogMessage.line = 0;
		g_LogMessage.function = "";

		// Clear the message
		Clear();

		g_Mutex.Unlock();

		// Return the log manager instance.
		return g_LogManagerInstance;
	}
	*/

	void Log::SetMetaData(	const std::string & p_File,
							const Int32 p_Line,
							const std::string & p_Function)
	{
		g_LogMessage.file = p_File;
		g_LogMessage.line = p_Line;
		g_LogMessage.function = p_Function;
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

	/*
	// Log manager class
	LogManager & LogManager::operator << (const Log::eType & p_Type)
	{
		// Set the message type
		g_LogMessage.type = p_Type;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Log::eFunction & p_Func)
	{
		// Run the wanted function
		switch (p_Func)
		{
		case Log::End:
			Log::Post();
			break;
		case Log::Engine:
			g_LogMessage.isEngineMessage = true;
			break;
		case Log::User:
			g_LogMessage.isEngineMessage = false;
			break;
		default:
			break;
		};

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Int8 * p_pChars)
	{
		// Add chars to the stream.
		g_MessageStream << p_pChars;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Uint8 * p_pChars)
	{
		// Add chars to the stream.
		g_MessageStream << p_pChars;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const std::string & p_String)
	{
		// Add string to the stream.
		g_MessageStream << p_String;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Int8 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Uint8 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Int16 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Uint16 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Int32 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Uint32 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Int64 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Uint64 & p_Integer)
	{
		// Add string to the stream.
		g_MessageStream << p_Integer;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Float32 & p_Float)
	{
		// Add string to the stream.
		g_MessageStream << p_Float;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & LogManager::operator << (const Float64 & p_Float)
	{
		// Add string to the stream.
		g_MessageStream << p_Float;

		// Return the log manager instance.
		return g_LogManagerInstance;
	}
	*/


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