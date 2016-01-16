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
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{



	// Static variables
	static LogManager					g_LogManagerInstance;
	static Private::DefaultLogHandle	g_DefaultLogHandle;
	static LogHandle *					g_CurrentLogHandle = &g_DefaultLogHandle;
	static LogMessage					g_LogMessage;
	static std::stringstream			g_MessageStream;


	// Log class
	void Log::SetHandle(LogHandle & p_Handle)
	{
		g_CurrentLogHandle = &p_Handle;
	}

	LogHandle & Log::GetDefaultHandle()
	{
		return g_DefaultLogHandle;
	}

	LogManager & Log::New(const Log::eType p_Type)
	{
		// Post the old message data.
		Post();

		// Set the new type
		g_LogMessage.type = p_Type;

		return g_LogManagerInstance;
	}

	LogManager & Log::NewEngine(const Log::eType p_Type)
	{
		// Post the old message data.
		Post();

		// Set the new type
		g_LogMessage.type = p_Type;
		g_LogMessage.isEngineMessage = true;

		return g_LogManagerInstance;
	}

	LogManager & Log::Get()
	{
		// Return the log manager instance.
		return g_LogManagerInstance;
	}

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

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	LogManager & Log::Clear()
	{
		g_MessageStream.str("");

		// Return the log manager instance.
		return g_LogManagerInstance;
	}

	Log::eType Log::GetType()
	{
		// Return the log manager instance.
		return g_LogMessage.type;
	}

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
		file(""),
		line(0),
		function(""),
		isEngineMessage(false)
	{
	}


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

	LogManager & LogManager::operator << (const char * p_pChars)
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