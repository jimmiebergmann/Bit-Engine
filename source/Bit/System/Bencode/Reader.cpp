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

#include <Bit/System/Bencode/Reader.hpp>
#include <fstream>

namespace Bit
{

	namespace Bencode
	{

		Reader::Reader( )
		{
		}

		Bool Reader::Parse( const std::string & p_Input, Value & p_Value )
		{
			// The string should at least contain "de"
			if( p_Input.size( ) < 2 )
			{
				p_Value.m_Type = Value::Nil;
				return false;
			}

			// The first byte should be 'd'
			if( p_Input[ 0 ] != 'd' )
			{
				p_Value.m_Type = Value::Nil;
				return false;
			}

			// Create a root value, set p_Value to this value if the parsing succeeds.
			Value root;

			// Store the index of the current position in the input string
			SizeType position = 1;

			// Read the dictionary
			if( ReadDictionary( root, p_Input, position ) == false )
			{
				p_Value.m_Type = Value::Nil;
				return false;
			}

			// Set the paramter value to the root value
			p_Value = root;

			// Change the root value type to Nil.
			// Otherwise, the destructor will delete the dictionary...
			root.m_Type = Value::Nil;

			// Succeeded
			return true;
		}

		Bool Reader::ParseFromFile( const std::string & p_Filename, Value & p_Value )
		{
			// Open the file
			std::ifstream fin( p_Filename );
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

			// Read line by line
			std::string line;
			while( fin.eof( ) == false )
			{
				std::getline( fin, line );
				fileData += line;
			}

			// Close the file
			fin.close( );

			// Parse the file data
			return Parse( fileData, p_Value );
		}

		Uint32 Reader::ReadKeySize( const std::string & p_Input, SizeType & p_Position )
		{
			// Look for the end sign for key length( ':' )
			bool foundEnd = false;
			SizeType i;
			for( i = p_Position; i < p_Input.size( ) - 1; i++ )
			{
				if( p_Input[ i ] == ':' )
				{
					foundEnd = true;
					break;
				}
			}

			// We did not found the end.
			if( foundEnd == false )
			{
				return 0;
			}

			// We found the end, let's get the length string
			std::string lengthString = p_Input.substr( p_Position, i - p_Position );

			// The string's length is 0, we did not find the end at last...
			if( lengthString.size( ) == 0 )
			{
				return 0;
			}

			// Increment the input string position
			p_Position += lengthString.size( ) + 1;

			// Get the length
			int length = atoi( lengthString.c_str( ) );
			if( length < 0 )
			{
				return 0;
			}

			// Return the length
			return static_cast<Uint32>( length );
		}

		std::string Reader::ReadKey( const std::string & p_Input, SizeType & p_Position )
		{
			// Get the key size
			Uint32 keySize = ReadKeySize( p_Input, p_Position );

			if( keySize == 0 )
			{
				return "";
			}

			// Check if we have enough of input data for this key
			if( keySize + p_Position > p_Input.size( ) )
			{
				return "";
			}

			// Get the key
			std::string key = p_Input.substr( p_Position, keySize );

			// Increment the position
			p_Position += keySize;

			// Return the key
			return key;
		}

		bool Reader::ReadDictionary( Value & p_Value, const std::string & p_Input, SizeType & p_Position )
		{
			// Set the value data
			p_Value.m_Type = Value::Dictionary;
			p_Value.m_Value.Dictionary = new Value::ValueMap;

			// Keep on looking for more values in the dictionary,
			// until the index is running out of bound,
			// or if we found an 'e'(end of dictionary).
			while( p_Position < p_Input.size( ) - 1 && p_Input[ p_Position ] != 'e' )
			{
				// Get the size of the key
				std::string key = ReadKey( p_Input, p_Position );

				// Make sure that the key is valid.
				if( key.size( ) == 0 )
				{
					return false;
				}

				// Check if there's already a value with this key
				if( p_Value.m_Value.Dictionary->find( key ) != p_Value.m_Value.Dictionary->end( ) )
				{
					return false;
				}

				// Check the data type for this data field.
				switch( p_Input[ p_Position ] )
				{
					// Integer
					case 'i':
					{
						// Increment the position( get rid of the 'i' character )
						p_Position++;

						// Create a string value
						Value * value = new Value( );
						
						// Read the string
						if( ReadInteger( *value, p_Input, p_Position ) == false )
						{
							return false;
						}

						// Add the string to the dictionary
						(*p_Value.m_Value.Dictionary)[ key ] = value;
					}
					break;
					// List
					case 'l':
					{
					}
					break;
					// Dictionary
					case 'd':
					{
					}
					break;
					// String
					default:
					{
						// Create a string value
						Value * value = new Value( );
						
						// Read the string
						if( ReadString( *value, p_Input, p_Position ) == false )
						{
							return false;
						}

						// Add the string to the dictionary
						(*p_Value.m_Value.Dictionary)[ key ] = value;
					}
					break;
				}
			}

			// Check if the current character is an 'e'( end of dictionary )
			if( p_Input[ p_Position ] != 'e' )
			{
				return false;
			}

			// Increment the position and return true.
			p_Position++;
			return true;
		}

		bool Reader::ReadString( Value & p_Value, const std::string & p_Input, SizeType & p_Position )
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

		bool Reader::ReadInteger( Value & p_Value, const std::string & p_Input, SizeType & p_Position )
		{
			// Search for an 'e'( end of integer )
			// Look for the end sign for key length( ':' )
			bool foundEnd = false;
			SizeType i;
			for( i = p_Position; i < p_Input.size( ) - 1; i++ )
			{
				if( p_Input[ i ] == 'e' )
				{
					foundEnd = true;
					break;
				}
			}

			// We did not found the end.
			if( foundEnd == false )
			{
				return false;
			}

			// Get the integer string
			std::string integerString = p_Input.substr( p_Position, i - p_Position );

			// Convert the string to a int
			int length = atoi( integerString.c_str( ) );
			if( length < 0 )
			{
				return false;
			}

			// Set the position
			p_Position += integerString.size( ) + 1;

			// Set the value data
			p_Value.m_Type = Value::Integer;
			p_Value.m_Value.Integer = static_cast<Int32>( length );
			return true;
		}

	}

}