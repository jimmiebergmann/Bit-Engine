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

#include <Bit/System/Json/Writer.hpp>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Json
	{

		// Writer
		Writer::Writer( )
		{
		}

		Bool Writer::Write( std::string & p_Output, Value & p_Value, Bool p_EnsureRoot )
		{
			if( p_Value.m_Type != Value::Object )
			{
				return false;
			}

			// Clear the output string
			p_Output.clear( );

			// This should be an object...
			return SetObjectString( p_Value, p_Output, 0 );
		}

		void Writer::SetKeywordString( const std::string & p_Keyword, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			p_Output += "\"";
			p_Output += p_Keyword;
			p_Output += "\":";
		}

		Bool Writer::SetObjectString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			// Error check the object pointer
			if( p_Value.m_Value.Object == NULL )
			{
				return false;
			}

			// Add the object start indicator character
			p_Output += "{";

			// Go through all the values in this object
			SizeType valueCount = 0;
			for( Value::ValueMap::iterator it = p_Value.m_Value.Object->begin( ); it != p_Value.m_Value.Object->end( ); it++ )
			{
				// Write keyword string for every single 
				SetKeywordString( it->first, p_Output, p_Layer + 1 );

				// Switch the type
				switch( it->second->m_Type )
				{
					case Value::Object:
					{
						// Set the string data of this object
						if( SetObjectString( *(it->second), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Array:
					{
						// Set the string data of this object
						if( SetArrayString( *(it->second), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::String:
					{
						// Set the string data of this object
						if( SetStringString( *(it->second), p_Output ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Number:
					{
						// Set the string data of this object
						SetNumberString( *(it->second), p_Output);
					}
					break;
					case Value::Boolean:
					{
						// Set the string data of this object
						SetBoolString( *(it->second), p_Output );
					}
					break;
					case Value::Null:
					{
						// Set the string data of this object
						SetNullString( *(it->second), p_Output );
					}
					break;
					default:
						break;
				}

				// Add a comma if there are more values
				if( valueCount + 1 < p_Value.m_Value.Object->size( ) )
				{
					p_Output += ",";
				}

				// Increment the value counter
				valueCount++;
			}


			// Add the object end indicator character
			p_Output += "}";

			// Succeeded.
			return true;
		}

		Bool Writer::SetArrayString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			// Error check the array pointer
			if( p_Value.m_Value.Array == NULL )
			{
				return false;
			}

			// Add the object start indicator character
			p_Output += "[";

			// Go through all the values in this object
			SizeType valueCount = 0;
			for( Value::ValueVector::iterator it = p_Value.m_Value.Array->begin( ); it != p_Value.m_Value.Array->end( ); it++ )
			{
				// Switch the type
				switch( (*it)->m_Type )
				{
					case Value::Object:
					{
						// Set the string data of this object
						if( SetObjectString( *(*it), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Array:
					{
						// Set the string data of this object
						if( SetArrayString( *(*it), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::String:
					{

						// Set the string data of this object
						if( SetStringString( *(*it), p_Output ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Number:
					{
						// Set the string data of this object
						SetNumberString( *(*it), p_Output);
					}
					break;
					case Value::Boolean:
					{
						// Set the string data of this object
						SetBoolString( *(*it), p_Output );
					}
					break;
					case Value::Null:
					{
						// Set the string data of this object
						SetNullString( *(*it), p_Output );
					}
					break;
					default:
						break;
				}

				// Add a comma if there are more values
				if( valueCount + 1 < p_Value.m_Value.Array->size( ) )
				{
					p_Output += ",";
				}


				// Increment the value counter
				valueCount++;
			}	

			// Add the object end indicator character
			p_Output += "]";

			// Succeeded.
			return true;
		}

		Bool Writer::SetStringString( const Value & p_Value, std::string & p_Output ) const
		{
			// Error check the string pointer
			if( p_Value.m_Value.String == NULL )
			{
				return false;
			}

			p_Output += "\"";
			p_Output += *(p_Value.m_Value.String);
			p_Output += "\"";

			return true;
		}

		void Writer::SetNumberString( const Value & p_Value, std::string & p_Output ) const
		{
			// Check if this is an integer or floating point number
			if( p_Value.m_IntegerFlag )
			{
				// Turn the integer into a string via a string stream.
				std::stringstream ss;
				ss << (p_Value.m_Value.Integer);

				// Add the number to the output string.
				p_Output += ss.str( );
			}
			else
			{
				// Turn the integer into a string via a string stream.
				std::stringstream ss;
				ss << (p_Value.m_Value.FloatingPoint);

				// Add the number to the output string.
				p_Output += ss.str( );
			}
		}

		void Writer::SetBoolString( const Value & p_Value, std::string & p_Output ) const
		{
			if( p_Value.m_Value.Boolean )
			{
				p_Output += "true";
			}
			else
			{
				p_Output += "false";
			}
		}

		void Writer::SetNullString( const Value & p_Value, std::string & p_Output ) const
		{
			p_Output += "null";
		}





		// //////////////////////////////////////////////////////////////////////////////////////////////////
		// Styled Writer
		StyledWriter::StyledWriter( )
		{
		}

		Bool StyledWriter::Write( std::string & p_Output, Value & p_Value, Bool p_EnsureRoot )
		{
			if( p_Value.m_Type != Value::Object )
			{
				return false;
			}

			// Clear the output string
			p_Output.clear( );

			// This should be an object...
			return SetObjectString( p_Value, p_Output, 0 );
		}

		void StyledWriter::SetKeywordString( const std::string & p_Keyword, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			p_Output.append( p_Layer, '\t' );
			p_Output += "\"";
			p_Output += p_Keyword;
			p_Output += "\" : ";
		}

		Bool StyledWriter::SetObjectString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			// Error check the object pointer
			if( p_Value.m_Value.Object == NULL )
			{
				return false;
			}

			// Add the object start indicator character
			p_Output.append( p_Layer, '\t' );
			p_Output += "{\n";

			// Go through all the values in this object
			SizeType valueCount = 0;
			for( Value::ValueMap::iterator it = p_Value.m_Value.Object->begin( ); it != p_Value.m_Value.Object->end( ); it++ )
			{
				// Write keyword string for every single 
				SetKeywordString( it->first, p_Output, p_Layer + 1 );

				// Switch the type
				switch( it->second->m_Type )
				{
					case Value::Object:
					{
						// Add newline
						p_Output += "\n";

						// Set the string data of this object
						if( SetObjectString( *(it->second), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Array:
					{
						// Add newline
						p_Output += "\n";

						// Set the string data of this object
						if( SetArrayString( *(it->second), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::String:
					{
						// Set the string data of this object
						if( SetStringString( *(it->second), p_Output ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Number:
					{
						// Set the string data of this object
						SetNumberString( *(it->second), p_Output);
					}
					break;
					case Value::Boolean:
					{
						// Set the string data of this object
						SetBoolString( *(it->second), p_Output );
					}
					break;
					case Value::Null:
					{
						// Set the string data of this object
						SetNullString( *(it->second), p_Output );
					}
					break;
					default:
						break;
				}

				// Add a comma if there are more values
				if( valueCount + 1 < p_Value.m_Value.Object->size( ) )
				{
					p_Output += ",";
				}

				// Add a new line
				p_Output += "\n";

				// Increment the value counter
				valueCount++;
			}


			// Add the object end indicator character
			p_Output.append( p_Layer, '\t' );
			p_Output += "}";

			// Succeeded.
			return true;
		}

		Bool StyledWriter::SetArrayString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			// Error check the array pointer
			if( p_Value.m_Value.Array == NULL )
			{
				return false;
			}

			// Add the object start indicator character
			p_Output.append( p_Layer, '\t' );
			p_Output += "[\n";

			// Go through all the values in this object
			SizeType valueCount = 0;
			for( Value::ValueVector::iterator it = p_Value.m_Value.Array->begin( ); it != p_Value.m_Value.Array->end( ); it++ )
			{
				// Switch the type
				switch( (*it)->m_Type )
				{
					case Value::Object:
					{
						// Set the string data of this object
						if( SetObjectString( *(*it), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Array:
					{
						// Set the string data of this object
						if( SetArrayString( *(*it), p_Output, p_Layer + 1 ) == false )
						{
							return false;
						}
					}
					break;
					case Value::String:
					{
						// Add tabs
						p_Output.append( p_Layer + 1, '\t' );

						// Set the string data of this object
						if( SetStringString( *(*it), p_Output ) == false )
						{
							return false;
						}
					}
					break;
					case Value::Number:
					{
						// Add tabs
						p_Output.append( p_Layer + 1, '\t' );

						// Set the string data of this object
						SetNumberString( *(*it), p_Output);
					}
					break;
					case Value::Boolean:
					{
						// Add tabs
						p_Output.append( p_Layer + 1, '\t' );

						// Set the string data of this object
						SetBoolString( *(*it), p_Output );
					}
					break;
					case Value::Null:
					{
						// Add tabs
						p_Output.append( p_Layer + 1, '\t' );

						// Set the string data of this object
						SetNullString( *(*it), p_Output );
					}
					break;
					default:
						break;
				}

				// Add a comma if there are more values
				if( valueCount + 1 < p_Value.m_Value.Array->size( ) )
				{
					p_Output += ",";
				}

				// Add a new line
				p_Output += "\n";

				// Increment the value counter
				valueCount++;
			}
			


			// Add the object end indicator character
			p_Output.append( p_Layer, '\t' );
			p_Output += "]";

			// Succeeded.
			return true;
		}

		Bool StyledWriter::SetStringString( const Value & p_Value, std::string & p_Output ) const
		{
			// Error check the string pointer
			if( p_Value.m_Value.String == NULL )
			{
				return false;
			}

			p_Output += "\"";
			p_Output += *(p_Value.m_Value.String);
			p_Output += "\"";

			return true;
		}

		void StyledWriter::SetNumberString( const Value & p_Value, std::string & p_Output ) const
		{
			// Check if this is an integer or floating point number
			if( p_Value.m_IntegerFlag )
			{
				// Turn the integer into a string via a string stream.
				std::stringstream ss;
				ss << (p_Value.m_Value.Integer);

				// Add the number to the output string.
				p_Output += ss.str( );
			}
			else
			{
				// Turn the integer into a string via a string stream.
				std::stringstream ss;
				ss << (p_Value.m_Value.FloatingPoint);

				// Add the number to the output string.
				p_Output += ss.str( );
			}
		}

		void StyledWriter::SetBoolString( const Value & p_Value, std::string & p_Output ) const
		{
			if( p_Value.m_Value.Boolean )
			{
				p_Output += "true";
			}
			else
			{
				p_Output += "false";
			}
		}

		void StyledWriter::SetNullString( const Value & p_Value, std::string & p_Output ) const
		{
			p_Output += "null";
		}

	}

}