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

#ifndef BIT_SYSTEM_JSON_WRITER_HPP
#define BIT_SYSTEM_JSON_WRITER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Json/Value.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Json(pronounced "Jason") namespace
	///
	////////////////////////////////////////////////////////////////
	namespace Json
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Bencode
		/// \brief Json compact writer class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Writer
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Writer( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the value as a string.
			///
			/// \param p_Output The text output.
			/// \param p_Value The value to turn into a string.
			/// \param p_EnsureRoot If true, put any value that's not an object into a root value.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool Write( std::string & p_Output, Value & p_Value, Bool p_EnsureRoot = true );

		private:

			// Private functions
			void SetKeywordString( const std::string & p_Keyword, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetObjectString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetArrayString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetStringString( const Value & p_Value, std::string & p_Output ) const;

			void SetNumberString( const Value & p_Value, std::string & p_Output ) const;

			void SetBoolString( const Value & p_Value, std::string & p_Output ) const;

			void SetNullString( const Value & p_Value, std::string & p_Output ) const;


		};

		////////////////////////////////////////////////////////////////
		/// \ingroup Bencode
		/// \brief Json styled writer class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API StyledWriter
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			StyledWriter( );

			////////////////////////////////////////////////////////////////
			/// \brief Get the value as a string.
			///
			/// \param p_Output The text output.
			/// \param p_Value The value to turn into a string.
			/// \param p_EnsureRoot If true, put any value that's not an object into a root value.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool Write( std::string & p_Output, Value & p_Value, Bool p_EnsureRoot = true );

		private:

			// Private functions
			void SetKeywordString( const std::string & p_Keyword, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetObjectString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetArrayString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const;

			Bool SetStringString( const Value & p_Value, std::string & p_Output ) const;

			void SetNumberString( const Value & p_Value, std::string & p_Output ) const;

			void SetBoolString( const Value & p_Value, std::string & p_Output ) const;

			void SetNullString( const Value & p_Value, std::string & p_Output ) const;

			
		};

	}

}

#endif