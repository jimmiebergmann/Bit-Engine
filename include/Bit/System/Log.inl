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

// Pool item class.
/*template <typename .. Params>
inline void Log<T>::Item::SetUsedSize(const SizeType p_UsedSize)
{
	m_UsedSize = p_UsedSize;
}

*/

template <typename ... Params>
Bool Log::New(const char * p_FormatedString, Params ... p_Params)
{
	return New(Log::Info, Log::Client, p_FormatedString, p_Params...);
}

template <typename ... Params>
Bool Log::New(const eType p_Type, const char * p_FormatedString, Params ... p_Params)
{
	return New(p_Type, Log::Client, p_FormatedString, p_Params...);
}

template <typename ... Params>
Bool Log::New(const eType p_Type,const eUser p_User, const char * p_FormatedString, Params ... p_Params)
{
	// Lock the mutex.
	SmartMutex mutex(g_Mutex);
	mutex.Lock();

	// Format the string into the message
	StringFormat(p_FormatedString, p_Params ...);

	// Check the size.
	if (g_LogMessage.message.size() == 0)
	{
		return false;
	}

	// Set type and user.
	g_LogMessage.type = p_Type;
	g_LogMessage.user = p_User;

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

	return true;
}

template <typename ... Params>
Bool Log::NewEngine(const char * p_FormatedString, Params ... p_Params)
{
	return New(Log::Info, Log::Engine, p_FormatedString, p_Params...);
}

template <typename ... Params>
Bool Log::NewEngine(const eType p_Type, const char * p_FormatedString, Params ... p_Params)
{
	return New(p_Type, Log::Engine, p_FormatedString, p_Params...);
}
