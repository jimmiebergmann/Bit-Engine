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
#include <Bit/System/Timestamp.hpp>
#include <Bit/System/SmartMutex.hpp>
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
		enum eUser
		{
			Engine,	///< Used for setting system message flag.
			Client	///< Used for setting system message flag.
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


		/*template <typename ... Ts>
		static void Test(char *fmt, Ts ... ts)
		{
			printf(fmt, ts...);
		}*/

		template <typename ... Params>
		static Bool New(const char * p_FormatedString, Params ... p_Params);
		template <typename ... Params>
		static Bool New(const eType p_Type, const char * p_FormatedString, Params ... p_Params);
		template <typename ... Params>
		static Bool New(const eType p_Type, const eUser p_User, const char * p_FormatedString, Params ... p_Params);
		template <typename ... Params>
		static Bool NewEngine(const char * p_FormatedString, Params ... p_Params);
		template <typename ... Params>
		static Bool NewEngine(const eType p_Type, const char * p_FormatedString, Params ... p_Params);
		

		////////////////////////////////////////////////////////////////
		/// \brief	Set meta for the log manager.
		///
		////////////////////////////////////////////////////////////////
		static void SetMetaData(const std::string & p_File,
								const Int32 p_Line,
								const std::string & p_Function);

	private:

		// Private functions

		////////////////////////////////////////////////////////////////
		/// \brief	Format a string into a message for the log message class.
		///
		////////////////////////////////////////////////////////////////
		static void StringFormat(const char * p_FormatedString, ...);

		// Private variables
		static LogManager					g_LogManagerInstance;
		static LogHandle *					g_CurrentLogHandle;
		static LogMessage					g_LogMessage;
		static Mutex						g_Mutex;

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
		Log::eUser		user;				///< User type.
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
	/*class BIT_API LogManager
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
		LogManager & operator << (const Int8 * p_pChars);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an array of chars to the log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Uint8 * p_pChars);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding a string log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const std::string & p_String);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 8 bit integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Int8 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 8 bit unsigned integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Uint8 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 16 bit integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Int16 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 16 bit unsigned integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Uint16 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 32 bit integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Int32 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 32 bit unsigned integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Uint32 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 64 bit integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Int64 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 64 bit unsigned integer to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Uint64 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 32 bit ufloating point to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Float32 & p_Integer);

		////////////////////////////////////////////////////////////////
		/// \brief Operation for adding an 64 bit ufloating point to log message.
		///
		////////////////////////////////////////////////////////////////
		LogManager & operator << (const Float64 & p_Integer);

	};*/

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/Log.inl>
	

}

#endif