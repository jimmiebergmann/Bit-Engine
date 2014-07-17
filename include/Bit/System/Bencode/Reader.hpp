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

#ifndef BIT_SYSTEM_BENCODE_READER_HPP
#define BIT_SYSTEM_BENCODE_READER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Bencode/Value.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Bencode(pronounced B Encode)
	///
	////////////////////////////////////////////////////////////////
	namespace Bencode
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Bencode
		/// \brief Bencode reader class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Reader
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Reader( );

			////////////////////////////////////////////////////////////////
			/// \brief Parse a bencode string
			///
			/// \param p_Input The text input to parse.
			/// \param p_Value The value output from the parsed text string.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool Parse( const std::string & p_Input, Value & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Parse a bencode file
			///
			/// \param p_Filename The name of the file to read from.
			/// \param p_Value The value output from the parsed file.
			///
			/// \see Parse
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool ParseFromFile( const std::string & p_Filename, Value & p_Value );

		private:

			////////////////////////////////////////////////////////////////
			/// \brief Read key size from input string at given location.
			///
			/// This function will modify and increment p_Position if
			/// the function successfully read any key size.
			///
			/// \param p_Input Input string
			/// \param p_Position Current input string location.
			///
			////////////////////////////////////////////////////////////////
			Uint32 ReadKeySize( const std::string & p_Input, SizeType & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Read key from input string at given location.
			///
			////////////////////////////////////////////////////////////////
			std::string ReadKey( const std::string & p_Input, SizeType & p_Position );
			
			////////////////////////////////////////////////////////////////
			/// \brief Read dictionary
			///
			////////////////////////////////////////////////////////////////
			Bool ReadDictionary( Value & p_Value, const std::string & p_Input, SizeType & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Read string
			///
			////////////////////////////////////////////////////////////////
			Bool ReadString( Value & p_Value, const std::string & p_Input, SizeType & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Read integer
			///
			////////////////////////////////////////////////////////////////
			Bool ReadInteger( Value & p_Value, const std::string & p_Input, SizeType & p_Position );

			////////////////////////////////////////////////////////////////
			/// \brief Read list
			///
			////////////////////////////////////////////////////////////////
			Bool ReadList( Value & p_Value, const std::string & p_Input, SizeType & p_Position );

		};

	}

}

#endif