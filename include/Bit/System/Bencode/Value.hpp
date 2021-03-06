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
			friend class StyledWriter;

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
			/// \brief Copy constructor
			///
			////////////////////////////////////////////////////////////////
			Value( const Value & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Value( );

			////////////////////////////////////////////////////////////////
			/// \brief Cleart the value from any data.
			///
			////////////////////////////////////////////////////////////////
			void Clear( );

			////////////////////////////////////////////////////////////////
			/// \brief Append value to list
			/// 
			/// This function will turn the value into a list.
			/// This will internally make a copy of the input value
			/// and all it's content.
			/// This can cause assignments of root values to be slow.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Value & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Append integer to list
			/// 
			/// This function will turn the value into a list.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Int32 & p_Integer );

			////////////////////////////////////////////////////////////////
			/// \brief Append string to list
			/// 
			/// This function will turn the value into a list.
			///
			////////////////////////////////////////////////////////////////
			void Append( const std::string & p_String );

			////////////////////////////////////////////////////////////////
			/// \brief Erase item from list
			///
			////////////////////////////////////////////////////////////////
			void Erase( const SizeType & p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Erase field from dictionary.
			///
			////////////////////////////////////////////////////////////////
			void Erase( const std::string & p_Key );

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
			/// \brief Get value from dictionary
			///
			/// This function will return the requested value.
			/// A new value will be created with the default value,
			///	but not stored in the current value,
			/// if the value with the given key can't be found.
			///
			/// \param p_Key Key for requested value..
			/// \param p_DefaultValue Default value returned if
			///		the value can't be found.
			///
			////////////////////////////////////////////////////////////////
			Value & Get( const std::string & p_Key, const Value & p_DefaultValue ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get value from dictionary
			///
			/// This function will return the requested value.
			/// A new value will be created with the default value,
			///	but not stored in the current value,
			/// if the value with the given key can't be found.
			///
			/// \param p_Key Key for requested value..
			/// \param p_DefaultValue Default value returned if
			///		the value can't be found.
			///
			////////////////////////////////////////////////////////////////
			Value & Get( const std::string & p_Key, const Int32 & p_DefaultValue ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get value from dictionary
			///
			/// This function will return the requested value.
			/// A new value will be created with the default value,
			///	but not stored in the current value,
			/// if the value with the given key can't be found.
			///
			/// \param p_Key Key for requested value..
			/// \param p_DefaultValue Default value returned if
			///		the value can't be found.
			///
			////////////////////////////////////////////////////////////////
			Value & Get( const std::string & p_Key, const std::string & p_DefaultValue ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get size of list
			///
			////////////////////////////////////////////////////////////////
			SizeType GetSize( ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get value as string
			///
			////////////////////////////////////////////////////////////////
			const std::string &  AsString( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value as integer
			///
			////////////////////////////////////////////////////////////////
			Int32 AsInt( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value from dictionary
			///
			/// This fucntion will add a nil value to the dictionary
			/// with the p_key paramter as it's key value
			/// if the value can't be found in the dictionary.
			///
			////////////////////////////////////////////////////////////////
			Value & operator [ ] ( const std::string & p_Key );

			////////////////////////////////////////////////////////////////
			/// \brief Get value from list
			///
			////////////////////////////////////////////////////////////////
			Value & operator [ ] ( const SizeType & p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Assign the value antother value
			///
			/// This will internally make a copy of the input value
			/// and all it's content.
			/// This can cause assignments of root values to be slow.
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const Value & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Assign the value an integer.
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const Int32 & p_Integer );

			////////////////////////////////////////////////////////////////
			/// \brief Assign the value a character array. (string)
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const char * p_Characters );

			////////////////////////////////////////////////////////////////
			/// \brief Assign the value a string
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const std::string & p_String );

			////////////////////////////////////////////////////////////////
			/// \brief Compares value types, not the data.
			///
			////////////////////////////////////////////////////////////////
			bool operator == ( const Value & p_Value ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Compares value types, not the data.
			///
			////////////////////////////////////////////////////////////////
			bool operator != ( const Value & p_Value ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Static default nil value.
			///	
			/// This value should be used as an error value.
			///
			////////////////////////////////////////////////////////////////
			static const Value NilValue;

		private:

			// Private typedefs
			typedef std::vector<Value *> ValueVector;
			typedef std::unordered_map<std::string, Value *> ValueMap;

			// Private unions
			union ValueHolder
			{
				std::string * String;
				Int32 Integer;
				ValueVector * List;
				ValueMap * Dictionary;
			};

			////////////////////////////////////////////////////////////////
			/// \brief Static default value
			///	
			/// This value is being return by the Get function if
			/// the requested value can't be found.
			/// This value should just be temporary used and instantly copied
			/// after receiving it from the Get function.
			///
			/// \see Get
			///
			////////////////////////////////////////////////////////////////
			static Value s_DefaultValue;

			////////////////////////////////////////////////////////////////
			/// \brief Copy a value to another
			///
			/// This function will internally copy every single value in
			/// for example a list or dictionary.
			///
			/// \param p_From Source value.
			/// \param p_To Destination value.
			///
			////////////////////////////////////////////////////////////////
			void CopyValue( const Value & p_From, Value & p_To ) const;

			// Private varaibles
			eType m_Type;
			ValueHolder m_Value;
		};

	}

}

#endif