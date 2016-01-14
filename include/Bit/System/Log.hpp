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
#include <string>

namespace Bit
{

	/*
	Example 1:
	--------------------------------------------------------------------------------------
	BitLog::New() << "This is an info message!" << Bit::Log::End;
	--------------------------------------------------------------------------------------
	
	Example 2:
	--------------------------------------------------------------------------------------
		BitLog::New() << Bit::Log::Error << "This is an error message!" << Bit::Log::End();
	--------------------------------------------------------------------------------------

	Example 3:
	--------------------------------------------------------------------------------------
		BitLog::New(Bit::Log::Error) << "This is an error message!" << Bit::Log::End();
	--------------------------------------------------------------------------------------

	Example 4:
	--------------------------------------------------------------------------------------
		BitLog::SetHandle(MyCustomLogHandle);
		BitLog::New(Bit::Log::Warning);
		BitLog::Get() << "Warning message!";
		BitLog::Post();
	--------------------------------------------------------------------------------------
	*/


	// Macro for setting up log, line and function name from where the log message is done.
	#define BitLog Bit::Log::SetMetaData(__FILE__, __LINE__, __FUNCTION__); Bit::Log


	// Forward declarations
	class LogHandle;
	class LogManager;
	struct LogMessage;

	////////////////////////////////////////////////////////////////
	/// \brief	Log static class, this class makes it much cleaner to post messages.
	///			See examples at the top.
	///
	/// Set the message type with the enums Info/Waring/Error. The type are restored to the default
	/// type(Info) at message posting.
	///
	/// Set the output log handle with the SetHandle function. The default handle will print every single message in the program console.
	/// You can get the default handle via the GetDefaultHandle function.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Log
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Type enum
		///
		////////////////////////////////////////////////////////////////
		enum eType
		{
			Info,		///< Used for setting the current message as an info message
			Warning,	///< Used for setting the current message as aa warning message.
			Error		///< Used for setting the current message as an error message.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Function enum
		///
		////////////////////////////////////////////////////////////////
		enum eFunction
		{
			End,	///< used for posting messages.
			Engine,	///< Used for setting system message flag.
			User	///< Used for setting system message flag.
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

		////////////////////////////////////////////////////////////////
		/// \brief	Start a new message, this function makes sure to post any old message.
		///			You can also specify the type of the message.
		///			Also, you will get the reference to the log manager.
		///
		////////////////////////////////////////////////////////////////
		static LogManager & New(const Log::eType p_Type = Log::Info);

		////////////////////////////////////////////////////////////////
		/// \brief	Start a new Engine message, this function makes sure to post any old message.
		///			You can also specify the type of the message.
		///			Also, you will get the reference to the log manager.
		///			Use this function instead of the function New for system messages.
		///
		////////////////////////////////////////////////////////////////
		static LogManager & NewEngine(const Log::eType p_Type = Log::Info);

		////////////////////////////////////////////////////////////////
		/// \brief	Get the log manager instance.
		///
		////////////////////////////////////////////////////////////////
		static LogManager & Get();

		////////////////////////////////////////////////////////////////
		/// \brief	End the log message, this will fire the log handle events.
		///
		////////////////////////////////////////////////////////////////
		static LogManager & Post();

		////////////////////////////////////////////////////////////////
		/// \brief	Clear the log message.
		///
		////////////////////////////////////////////////////////////////
		static LogManager & Clear();

		////////////////////////////////////////////////////////////////
		/// \brief	Get the current log message type
		///
		////////////////////////////////////////////////////////////////
		static eType GetType();

		////////////////////////////////////////////////////////////////
		/// \brief	Set meta for the log manager.
		///
		////////////////////////////////////////////////////////////////
		static void SetMetaData(const std::string & p_File,
								const Int32 p_Line,
								const std::string & p_Function);

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
		std::string		message;			///< Message data.
		Log::eType		type;				///< Message type.
		std::string		file;				///< From what file is the logging being done?
		Int32			line;				///< At what line is the logging being done?
		std::string		function;			///< In what file is the logging being done?
		Bool			isEngineMessage;	///< Is this a system message?

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
		/// \brief Virual function fired at posted info messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnInfo(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted warning messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnWarning(const LogMessage & p_Message);

		////////////////////////////////////////////////////////////////
		/// \brief Virual function fired at posted error messages.
		///
		////////////////////////////////////////////////////////////////
		virtual void OnError(const LogMessage & p_Message);

	};


	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Log manager class. Call the operations of this class to add data to the log message.
	/// Post the log message by using the << operation with eSpecial::Endl as parameter,
	/// which will fire the overloaded LogHandle function OnMessage and OnInfo/OnWarning/OnError.
	///
	/// Use the log manager via the class Log, this class contains functions for getting,starting and posting messages for the log manager.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API LogManager
	{

	public:

		// Public operations

		////////////////////////////////////////////////////////////////
		/// \brief Operation for setting the message type of the message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Log::eType & p_Type);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for firing special functions.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Log::eFunction & p_Func);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an array of chars to the log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const char * p_pChars);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding a string log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const std::string & p_String);

	};
	

}

#endif