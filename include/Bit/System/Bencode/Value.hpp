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

#ifndef BIT_SYSTEM_BENCODE_VALUE_HPP
#define BIT_SYSTEM_BENCODE_VALUE_HPP

#include <Bit/Build.hpp>
#include <unordered_map>
#include <vector>
#include <string>

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
		/// \brief Bencode value class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Value
		{

		public:

			// Friend classes
			friend class Reader;
			friend class Writer;

			////////////////////////////////////////////////////////////////
			/// \brief Bencode data type enumerator
			///
			////////////////////////////////////////////////////////////////
			enum eType
			{
				Nil,		///< Invalid type.
				String,		///< String type.
				Integer,	///< Integer type.
				List,		///< List type.
				Dictionary	///< Dictionary type.
			};

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Value( );

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			Value( eType p_Type );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Value( );

			////////////////////////////////////////////////////////////////
			/// \brief Gets the type
			///
			////////////////////////////////////////////////////////////////
			eType GetType( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Checks if the value is a nil value.
			///
			////////////////////////////////////////////////////////////////
			Bool IsNil( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets value from dictionary( return 
			///
			////////////////////////////////////////////////////////////////
			const Value & Get( const std::string & p_Key ) const; 
			
			////////////////////////////////////////////////////////////////
			/// \brief Get value as string
			///
			////////////////////////////////////////////////////////////////
			std::string AsString( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value as integer
			///
			////////////////////////////////////////////////////////////////
			Int32 AsInt( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Static default nil value.
			///	
			/// This value should be used as an error value.
			///
			////////////////////////////////////////////////////////////////
			static const Value NilValue;

		private:

			// Private typedefs
			typedef std::vector<Value> ValueVector;
			typedef std::unordered_map<std::string, Value *> ValueMap;

			// Private unions
			union ValueHolder
			{
				std::string * String;
				Int32 Integer;
				ValueVector * List;
				ValueMap * Dictionary;
			};

			// Private varaibles
			eType m_Type;
			ValueHolder m_Value;
		};

	}

}

#endif