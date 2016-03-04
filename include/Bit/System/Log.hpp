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

#ifndef BIT_SYSTEM_LOG_HPP
#define BIT_SYSTEM_LOG_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Private/LogManager.hpp>

/// Main macro for logging. pass the Bit::Log::eType of the message, and then the message itself.
/// The following macros are thread safe.
/// CAUTION: The message actually is supposed to be in the format: << data << more data << "This is a string".
/// Example of usage:
///		bitLog(Bit::Log::Error, MY_CHANNEL_NUMBER, "This is an error, id: " << 123 );
#define bitLog(type, channel, message) \
	Bit::Private::LogManager::Start(type, channel, __FILE__, __LINE__, __FUNCTION__); \
	Bit::Private::LogManager::GetStream() << message; \
	Bit::Private::LogManager::End();


// Macros for logging Errors, Warnings and Info. Just pass the message for respective macro, and the channel.
#define bitLogErr(channel, message)		bitLog( Bit::Log::Error,	channel, message)
#define bitLogWarn(channel, message)	bitLog( Bit::Log::Warning,	channel, message)
#define bitLogInfo(channel, message)	bitLog( Bit::Log::Info,		channel, message)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin Debug messages, only for debug profile.
#define bitLogDebug(channel, message)	bitLog( Bit::Log::Debug,	channel, message)
#else
#define bitLogDebug(message)
#endif


// Audio library logging macros
#define bitLogAud(type, message)	bitLog( type,				Bit::Log::Audio, message)
#define bitLogAudErr(message)		bitLog( Bit::Log::Error,	Bit::Log::Audio, message)
#define bitLogAudWarn(message)		bitLog( Bit::Log::Warning,	Bit::Log::Audio, message)
#define bitLogAudInfo(message)		bitLog( Bit::Log::Info,		Bit::Log::Audio)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin Audio Debug messages, only for debug profile.
#define bitLogAudDebug(message) bitLog( Bit::Log::Debug,	Bit::Log::Audio, message)
#else
#define bitLogAudDebug(message)
#endif


// Graphics library logging macros
#define bitLogGra(type, message)	bitLog( type,				Bit::Log::Graphics, message)
#define bitLogGraErr(message)		bitLog( Bit::Log::Error,	Bit::Log::Graphics, message)
#define bitLogGraWarn(message)		bitLog( Bit::Log::Warning,	Bit::Log::Graphics, message)
#define bitLogGraInfo(message)		bitLog( Bit::Log::Info,		Bit::Log::Graphics)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin Graphics Debug messages, only for debug profile.
#define bitLogGraDebug(message)		bitLog( Bit::Log::Debug,	Bit::Log::Graphics, message)
#else
#define bitLogGraDebug(message)
#endif


// Network library logging macros
#define bitLogNet(type, message)	bitLog( type,				Bit::Log::Network, message)
#define bitLogNetErr(message)		bitLog( Bit::Log::Error,	Bit::Log::Network, message)
#define bitLogNetWarn(message)		bitLog( Bit::Log::Warning,	Bit::Log::Network, message)
#define bitLogNetInfo(message)		bitLog( Bit::Log::Info,		Bit::Log::Network)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin Network Debug messages, only for debug profile.
#define bitLogNetDebug(message)		bitLog( Bit::Log::Debug,	Bit::Log::Network, message)
#else
#define bitLogNetDebug(message)
#endif


// System library logging macros
#define bitLogSys(type, message)	bitLog( type,				Bit::Log::System, message)
#define bitLogSysErr(message)		bitLog( Bit::Log::Error,	Bit::Log::System, message)
#define bitLogSysWarn(message)		bitLog( Bit::Log::Warning,	Bit::Log::System, message)
#define bitLogSysInfo(message)		bitLog( Bit::Log::Info,		Bit::Log::System)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin System Debug messages, only for debug profile.
#define bitLogSysDebug(message)		bitLog( Bit::Log::Debug,	Bit::Log::System, message)
#else
#define bitLogSysDebug(message)
#endif


// Window library logging macros
#define bitLogWnd(type, message)	bitLog( type,				Bit::Log::Window, message)
#define bitLogWndErr(message)		bitLog( Bit::Log::Error,	Bit::Log::Window, message)
#define bitLogWndWarn(message)		bitLog( Bit::Log::Warning,	Bit::Log::Window, message)
#define bitLogWndInfo(message)		bitLog( Bit::Log::Info,		Bit::Log::Window)

#ifdef BIT_BUILD_DEBUG
// Macro for loggin Window Debug messages, only for debug profile.
#define bitLogWndDebug(message)		bitLog( Bit::Log::Debug,	Bit::Log::Window, message)
#else
#define bitLogWndDebug(message)
#endif


namespace Bit
{

	
	/// Example 1:
	/// --------------------------------------------------------------------------------------
	///	   bitLogNetInfo("This is a network info message nr. " << 123 );
	/// --------------------------------------------------------------------------------------
	///
	/// Example 2:
	/// --------------------------------------------------------------------------------------
	///	   bitLogErr( "This is an error message!");
	/// --------------------------------------------------------------------------------------
	

	////////////////////////////////////////////////////////////////
	/// \brief	Log static class.
	///
	/// Use the macros to post messages.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Log
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Message type enum
		///
		////////////////////////////////////////////////////////////////
		enum eType
		{
			Error,		///< Error message type.
			Warning,	///< Warning message type.
			Info,		///< Info message type.
			Debug		///< Debug message type.
		};


		////////////////////////////////////////////////////////////////
		/// \brief Channel enum.
		///
		////////////////////////////////////////////////////////////////
		enum eChannel
		{
			Unspecified,
			Audio,
			Graphics,
			Network,
			System,
			Window,
			Reserved
		};

		// Static functions

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


	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Log message structure.
	///
	/// Used for delivering message data to the LogHandle class.
	///
	/// \See Log
	///
	////////////////////////////////////////////////////////////////
	struct LogMessage
	{
		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		LogMessage();

		// Public variables.
		Timestamp		timestamp;			///< Timestamp of the message.
		std::string		message;			///< Message data.
		Log::eType		type;				///< Message type.
		Uint32			channel;			///< Message channel.
		std::string		file;				///< From what file is the logging being done?
		Int32			line;				///< At what line is the logging being done?
		std::string		function;			///< In what file is the logging being done?

	};


	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Log handleclass.
	///
	/// Create your own LogHandle and set it for the Log class, via the SetHandle function.
	/// The virtual function OnMessage is being fired for all incoming messages.
	/// The functions OnInfo/OnWarning/OnError are fired depending on the message type(Info by default).
	///
	/// \See Log
	///
	////////////////////////////////////////////////////////////////
	class BIT_API LogHandle
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual estructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~LogHandle() { }

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at any message posted.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnMessage(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted error messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnError(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted warning messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnWarning(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted info messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnInfo(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted debug messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnDebug(const LogMessage & p_Message);

	};

}

#endif