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

#include <Bit/System/Private/LogManager.hpp>
#include <Bit/System/Log.hpp>
#include <Bit/System/SmartMutex.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Private
	{

		// Static data
		static std::stringstream	g_MessageStream;
		static LogMessage			g_Message;
		static Mutex				g_Mutex;
		static LogHandle			g_DefaultLogHandle;
		static LogHandle *			g_CurrentLogHandle = &g_DefaultLogHandle;


		// Log manager class
		void LogManager::Start(	const Uint32 p_Type,
										const Uint32 p_Channel,
										const std::string & p_File,
										const Int32 p_Line,
										const std::string & p_Function)
		{
			// Lock the mutex.
			g_Mutex.Lock();

			// Set message meta data.
			g_Message.type = static_cast<Log::eType>(p_Type);
			g_Message.channel = p_Channel;
			g_Message.file = p_File;
			g_Message.line = p_Line;
			g_Message.function = p_Function;
		}

		std::stringstream & LogManager::GetStream()
		{
			return g_MessageStream;
		}


		void LogManager::End()
		{

			// Check the size.
			if (g_MessageStream.str().size() == 0)
			{
				// Unlock the mutex
				g_Mutex.Unlock();
				return;
			}

			// Set the message.
			g_Message.message = g_MessageStream.str();
			g_MessageStream.str("");

			// Set the timestamp
			g_Message.timestamp = Timestamp::Now();

			// Fire the OnMessage function for the handle.
			g_CurrentLogHandle->OnMessage(g_Message);

			// Also fire the On[eType] function for the handle.
			switch (g_Message.type)
			{
			case Log::Error:
				g_CurrentLogHandle->OnError(g_Message);
				break;
			case Log::Warning:
				g_CurrentLogHandle->OnWarning(g_Message);
				break;
			case Log::Info:
				g_CurrentLogHandle->OnInfo(g_Message);
				break;
			case Log::Debug:
				g_CurrentLogHandle->OnDebug(g_Message);
				break;
			
			default:
				break;
			};

			// Unlock the mutex
			g_Mutex.Unlock();
			
		}

		void LogManager::SetHandle(LogHandle & p_Handle)
		{
			g_CurrentLogHandle = &p_Handle;
		}

		LogHandle & LogManager::GetDefaultHandle()
		{
			return g_DefaultLogHandle;
		}

	}

}