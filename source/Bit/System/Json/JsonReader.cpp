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

#include <Bit/System/Json/Reader.hpp>
#include <fstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Json
	{

		Reader::Reader( )
		{
		}

		Bool Reader::Parse( const std::string & p_Input, Value & p_Value )
		{
			// The string should at least contain "{}" (an object).
			if( p_Input.size( ) < 2 )
			{
				p_Value.m_Type = Value::Null;
				return false;
			}

			// Store the index of the current position in the input string
			SizeType position = FindValidCharacter( p_Input, 0 );

			// Check if the first character is a '{'
			if( p_Input[ position ] != '{' )
			{
				p_Value.m_Type = Value::Null;
				return false;
			}

			// Create a root value, set p_Value to this value if the parsing succeeds.
			Value root( Value::Object );

			// The first value should be an object, read it.
			if( ReadObject( root, p_Input, ++position ) == false )
			{
				p_Value.m_Type = Value::Null;
				return false;
			}

			// Set the paramter value to the root value
			p_Value = root;

			// Suceeded
			return true;
		}

		Bool Reader::ParseFromFile( const std::string & p_Filename, Value & p_Value )
		{
			// Open the file
			std::ifstream fin( p_Filename, std::ifstream::binary );
			if( fin.is_open( ) == false )
			{
				return false;
			}

			// Get the file size
			fin.seekg( 0, std::ifstream::end );
			SizeType fileSize = static_cast<SizeType>( fin.tellg( ) );
			fin.seekg( 0, std::ifstream::beg );

			// Create a string for holding the file data
			std::string fileData;
			fileData.reserve( fileSize );

			// Read the data into a buffer
			char * pBuffer = new char[ fileSize ];
			fin.read( pBuffer, fileSize );

			// Append the buffer to a string
			fileData.append( pBuffer, fileSize );

			// Delete the buffer
			delete pBuffer;

			// Close the file
			fin.close( );

			// Parse the file data
			return Parse( fileData, p_Value );
		}

		// Private functions
		SizeType Reader::FindValidCharacter( const std::string & p_Input, const SizeType & p_Position ) const
		{
			SizeType newPosition = p_Position;

			// Go throguh the characters from the given position
			for( ; newPosition < p_Input.size( ) - 1; newPosition++ )
			{
				// Break if the character is in range.
				if( p_Input[ newPosition ] > 32 &&
					p_Input[ newPosition ] < 126  )
				{
					break;
				}
			}

			// Return the new position
			return newPosition;
		}

		std::string Reader::ReadKey( const std::string & p_Input, SizeType & p_Position ) const
		{
			// Make sure that the position isn't out of bound.
			if( p_Position >= p_Input.size( ) )
			{
				return "";
			}

			// Calculate the size of the key word.
			SizeType keySize = 0;
			SizeType tempPosition = p_Position;
			Bool foundKey = false;
			for( ; tempPosition < p_Input.size( ) - 1; tempPosition++, keySize++ )
			{
				// Break if we find a '"' character(except if there's an escape character before)
				if( p_Input[ tempPosition ] == '\"' &&
					p_Input[ tempPosition - 1 ] != '\\' )
				{
					foundKey = true;
					break;
				}
			}

			// Check if we found a key value.
			if( foundKey == false )
			{
				return "";
			}

			// Get the key
			std::string key = p_Input.substr( p_Position, keySize );

			// Increment the position
			p_Position += keySize + 1;

			// Return the key
			return key;
		}

		Bool Reader::ReadObject( Value & p_Value, const std::string & p_Input, SizeType & p_Position ) const
		{
			// Set the value data
			p_Value.m_Type = Value::Object;
			p_Value.m_Value.Object = new Value::ValueMap;

			// Keep on reading values
			Bool expectValue = false;
			SizeType tempPosition = 0;
			while( p_Position < p_Input.size( ) )
			{
				// Find the next valid character
				p_Position = FindValidCharacter( p_Input, p_Position );

				// Is this the end of this object?
				if( p_Input[ p_Position ] == '}' )
				{
					// The object is not complete. We are expecting more values.
					if( expectValue == true )
					{
						return false;
					}

					// The object is complete
					return true;
				}

				// Reset the expect value flag
				expectValue = false;

				// The next character should be the beginning of a key string.
				if( p_Input[ p_Position ] != '\"' )
				{
					return false;
				}

				// Read the key word
				std::string key = ReadKey( p_Input, ++p_Position );
				if( key.size( ) == 0 )
				{
					return false;
				}

				// Make sure that the position isn't out of bound.
				if( p_Position >= p_Input.size( ) )
				{
					return false;
				}

				// The key value should be followed by a ':', find it.
				p_Position = FindValidCharacter( p_Input, p_Position );

				// Check if the current character is a ':'
				if( p_Input[ p_Position ] != ':' )
				{
					return false;
				}

				// Increment the position by one
				p_Position++;

				// Make sure that the position isn't out of bound.
				if( p_Position >= p_Input.size( ) )
				{
					return false;
				}
				
				// Find the next valid character
				p_Position = FindValidCharacter( p_Input, p_Position );

				// Now when we have the key word, read the value followed by the key.
				// Find the right value type by checking the current value
				
				// Object value.
				if( p_Input[ p_Position ] == '{' )
				{
					// Create a object value
					Value * value = new Value( );
						
					// Read the integer
					if( ReadObject( *value, p_Input, ++p_Position ) == false )
					{
						return false;
					}

					// Add the integer to the dictionary
					(*p_Value.m_Value.Object)[ key ] = value;
				}
				// Array value.
				else if( p_Input[ p_Position ] == '[' )
				{
				}
				// String value.
				else if( p_Input[ p_Position ] == '\"' )
				{
					// Create a string value
					Value * value = new Value( );
						
					// Read the integer
					if( ReadString( *value, p_Input, ++p_Position ) == false )
					{
						return false;
					}

					// Add the integer to the dictionary
					(*p_Value.m_Value.Object)[ key ] = value;
				}
				// Boolean value.
				else if( p_Input[ p_Position ] == 't' || p_Input[ p_Position ] == 'f' )
				{
					// Create a string value
					Value * value = new Value( );
						
					// Read the integer
					if( ReadBoolean( *value, p_Input, p_Position ) == false )
					{
						return false;
					}

					// Add the integer to the dictionary
					(*p_Value.m_Value.Object)[ key ] = value;
				}
				// Number value(that's last possible value ).
				else
				{
				}

				// Check for a ','. More values are expected if we find one.
				tempPosition = FindValidCharacter( p_Input, p_Position );

				// Make sure that the position isn't out of bound.
				if( tempPosition >= p_Input.size( ) )
				{
					return false;
				}

				// Check if the current character is a ':'
				if( p_Input[ tempPosition ] == ',' )
				{
					p_Position = tempPosition + 1;
					expectValue = true;
				}

			}

			// Something went wrong.
			return false;
		}

		Bool Reader::ReadString( Value & p_Value, const std::string & p_Input, SizeType & p_Position ) const
		{
			// Using the Readkey function to obtain the string( same functionality )
			std::string string = ReadKey( p_Input, p_Position );
			if( string.size( ) == 0 )
			{
				return false;
			}

			// Set the value data
			p_Value.m_Type = Value::String;
			p_Value.m_Value.String = new std::string( string );
			return true;
		}

		Bool Reader::ReadBoolean( Value & p_Value, const std::string & p_Input, SizeType & p_Position ) const		
		{
			// Set the value type
			p_Value.m_Type = Value::Boolean;

			// Check if this is a "true" keyword
			if( p_Input[ p_Position ] == 't' )
			{
				// Check if there's enought space for the "true" keyword.
				if( p_Position + 3 >= p_Input.size( ) )
				{
					return false;
				}

				// Check the rest of the characters
				if( p_Input[ p_Position + 1 ] == 'r' &&
					p_Input[ p_Position + 2 ] == 'u' &&
					p_Input[ p_Position + 3 ] == 'e' )
				{
					// Set the boolean
					p_Value.m_Value.Boolean = true;

					// Increment the position by 4
					p_Position += 4;

					// Succeeded.
					return true;
				}
			}
			// Check if this is a "false" keyword
			else if( p_Input[ p_Position ] == 'f' )
			{
				// Check if there's enought space for the "false" keyword.
				if( p_Position + 4 >= p_Input.size( ) )
				{
					return false;
				}

				// Check the rest of the characters
				if( p_Input[ p_Position + 1 ] == 'a' &&
					p_Input[ p_Position + 2 ] == 'l' &&
					p_Input[ p_Position + 3 ] == 's' &&
					p_Input[ p_Position + 4 ] == 'e' )
				{
					// Set the boolean
					p_Value.m_Value.Boolean = false;

					// Increment the position by 5
					p_Position += 5;

					// Succeeded.
					return true;
				}
			}

			// Something failed.
			return false;
		}
	}

}