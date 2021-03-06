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

#ifndef BIT_SYSTEM_JSON_VALUE_HPP
#define BIT_SYSTEM_JSON_VALUE_HPP

#include <Bit/Build.hpp>
#include <unordered_map>
#include <vector>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Json(pronounced "Jason") namespace
	///
	/// The following data types are possible:
	///		- String:	Any length.
	///		- Number:	64 bit floating- point number, or 32 bit signed integer.
	///		- Boolean:	True or false.
	///		- Object:	Table of values.
	///		- Array:	Array of any value.
	///
	////////////////////////////////////////////////////////////////
	namespace Json
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Bencode
		/// \brief Json value class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Value
		{

		public:

			// Public typedefs
			typedef std::vector<Value *> ValueVector;
			typedef std::unordered_map<std::string, Value *> ValueMap;

			////////////////////////////////////////////////////////////////
			/// \brief Json iterator class
			///
			////////////////////////////////////////////////////////////////
			class BIT_API Iterator
			{

			public:

				////////////////////////////////////////////////////////////////
				/// \brief Default constructor
				///
				////////////////////////////////////////////////////////////////
				Iterator( );

				////////////////////////////////////////////////////////////////
				/// \brief Constructor
				///
				////////////////////////////////////////////////////////////////
				Iterator( ValueVector::iterator p_VectorIterator );

				////////////////////////////////////////////////////////////////
				/// \brief Constructor
				///
				////////////////////////////////////////////////////////////////
				Iterator( ValueMap::iterator p_MapIterator );

				////////////////////////////////////////////////////////////////
				/// \brief Copy constructor
				///
				////////////////////////////////////////////////////////////////
				Iterator( const Iterator & p_Iterator );

				////////////////////////////////////////////////////////////////
				/// \brief Destructor
				///
				////////////////////////////////////////////////////////////////
				~Iterator( );

				////////////////////////////////////////////////////////////////
				/// \brief Get current value.
				///
				////////////////////////////////////////////////////////////////
				Value & GetValue( ) const;

				////////////////////////////////////////////////////////////////
				/// \brief Get current keyword.
				///
				////////////////////////////////////////////////////////////////
				const std::string & GetKey( ) const;

				////////////////////////////////////////////////////////////////
				/// \brief Equal to operator.
				///
				////////////////////////////////////////////////////////////////
				Bool operator == ( const Iterator & p_Iterator ) const;

				////////////////////////////////////////////////////////////////
				/// \brief Not equal to operator.
				///
				////////////////////////////////////////////////////////////////
				Bool operator != ( const Iterator & p_Iterator ) const;

				////////////////////////////////////////////////////////////////
				/// \brief Assignment operator.
				///
				////////////////////////////////////////////////////////////////
				Iterator & operator = ( const Iterator & p_Iterator );

				////////////////////////////////////////////////////////////////
				/// \brief Post increment operator.
				///
				////////////////////////////////////////////////////////////////
				Iterator & operator ++ ( int );

			private:

				// Private functions
				Iterator & AssignIterator( const Iterator & p_Iterator );

				// Private variables
				ValueVector::iterator * m_pVectorIterator;
				ValueMap::iterator * m_pMapIterator;

			};


			// Friend classes
			friend class Reader;
			friend class Writer;
			friend class StyledWriter;

			////////////////////////////////////////////////////////////////
			/// \brief Json data type enumerator
			///
			////////////////////////////////////////////////////////////////
			enum eType
			{
				Null,		///< NULL type / invalid type.
				String,		///< String type.
				Number,		///< Double -precision floating -point typ.
				Boolean,	///< Boolean type.
				Array,		///< Array type.
				Object		///< Object type.				
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
			/// \brief Get begin iterator, works like the std::iterator type.
			///
			////////////////////////////////////////////////////////////////
			Iterator IteratorBegin( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get end iterator, works like the std::iterator type.
			///
			////////////////////////////////////////////////////////////////
			Iterator IteratorEnd( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Cleart the value from any data.
			///
			////////////////////////////////////////////////////////////////
			void Clear( );

			////////////////////////////////////////////////////////////////
			/// \brief Append value to array
			/// 
			/// This function will turn the value into an array.
			/// This will internally make a copy of the input value
			/// and all it's content.
			/// This can cause assignments of root values to be slow.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Value & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Append number to array
			/// 
			/// This function will turn the value into an array.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Float64 & p_Number );

			////////////////////////////////////////////////////////////////
			/// \brief Append integer to array
			/// 
			/// This function will turn the value into an array.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Int32 & p_Integer );

			////////////////////////////////////////////////////////////////
			/// \brief Append string to array
			/// 
			/// This function will turn the value into an array.
			///
			////////////////////////////////////////////////////////////////
			void Append( const std::string & p_String );

			////////////////////////////////////////////////////////////////
			/// \brief Append characters(string) to array
			/// 
			/// This function will turn the value into an array.
			///
			////////////////////////////////////////////////////////////////
			void Append( const char * p_Characters );

			////////////////////////////////////////////////////////////////
			/// \brief Append boolean to array
			/// 
			/// This function will turn the value into an array.
			///
			////////////////////////////////////////////////////////////////
			void Append( const Bool p_Boolean );

			////////////////////////////////////////////////////////////////
			/// \brief Erase item from array
			///
			////////////////////////////////////////////////////////////////
			void Erase( const SizeType & p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Erase field from object.
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
			Bool IsNull( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value from object
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
			/// \brief Get value from object
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
			Value & Get( const std::string & p_Key, const Float64 & p_DefaultValue ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get value from object
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
			/// \brief Get value from object
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
			/// \brief Get value from object
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
			Value & Get( const std::string & p_Key, const Bool & p_DefaultValue ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get size of array
			///
			////////////////////////////////////////////////////////////////
			SizeType GetSize( ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get integer flag
			///
			////////////////////////////////////////////////////////////////
			Bool GetIntegerFlag( ) const; 

			////////////////////////////////////////////////////////////////
			/// \brief Get value as string
			///
			////////////////////////////////////////////////////////////////
			const std::string & AsString( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value as number
			///
			////////////////////////////////////////////////////////////////
			Float64 AsNumber( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value as integer(number).
			///
			////////////////////////////////////////////////////////////////
			Int32 AsInteger( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value as bool
			///
			////////////////////////////////////////////////////////////////
			Bool AsBoolean( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get value from object.
			///
			/// This function will add a null value to the dictionary
			/// with the p_key paramter as it's key value
			/// if the value can't be found in the object.
			///
			/// \return NullValue if failed, else the value from the given key.
			///
			////////////////////////////////////////////////////////////////
			Value & operator [ ] ( const std::string & p_Key );

			////////////////////////////////////////////////////////////////
			/// \brief Get value from array
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
			/// \brief Assign the value a number.
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const Float64 & p_Number );

			////////////////////////////////////////////////////////////////
			/// \brief Assign the value an integer(number).
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
			/// \brief Assign the value a boolean
			///
			////////////////////////////////////////////////////////////////
			Value & operator = ( const Bool & p_Boolean );

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
			/// \brief Static default null value.
			///	
			/// This value should be used as an error value.
			///
			////////////////////////////////////////////////////////////////
			static const Value NullValue;

		private:

			// Private unions
			union ValueHolder
			{
				std::string * String;	///< String.
				Float64 FloatingPoint;	///< Number.
				Int32 Integer;			///< Number.
				Bool Boolean;			///< Boolean.
				ValueVector * Array;	///< Array.
				ValueMap * Object;		///< Object.

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
			/// for example an array or object.
			///
			/// \param p_From Source value.
			/// \param p_To Destination value.
			///
			////////////////////////////////////////////////////////////////
			void CopyValue( const Value & p_From, Value & p_To ) const;

			// Private varaibles
			eType m_Type;			///< The data type of the value.
			ValueHolder m_Value;	///< The value.
			Bool m_IntegerFlag;		///< Indicates if the number data type is an integer of floating point.
			
		};

	}

}

#endif