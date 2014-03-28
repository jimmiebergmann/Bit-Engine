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

#include <Bit/System/Bencode/Writer.hpp>
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Bencode
	{

		// Writer
		Writer::Writer( )
		{
		}

		Bool Writer::Write( std::string & p_Output, Value & p_Value )
		{
			return SetValueString( p_Value, p_Output );
		}

		Bool Writer::SetValueString( const Value & p_Value, std::string & p_Output ) const
		{
			switch( p_Value.m_Type )
			{
				case Value::Integer:
				{
					// Get the string via a stringstream
					std::stringstream integer;
					integer << p_Value.m_Value.Integer;

					// Append encoded integer
					p_Output += "i" + integer.str( ) + "e";
				}
				break;
				case Value::String:
				{
					// Make sure that the string isn't null
					if( !p_Value.m_Value.String )
					{
						return false;
					}

					// Get the string length as a string
					std::stringstream length;
					length << p_Value.m_Value.String->size( );

					// Append encoded string
					p_Output += length.str( ) + ":" + *p_Value.m_Value.String;
				}
				break;
				case Value::List:
				{
					// Make sure that the list isn't null
					if( !p_Value.m_Value.List )
					{
						return false;
					}

					// Add a 'l' to indicate that this is the start of a list
					p_Output += "l";

					// Go through the list
					for( Value::ValueVector::iterator it = p_Value.m_Value.List->begin( ); it != p_Value.m_Value.List->end( ); it++ )
					{
						// Ignore Nil values.
						if( (*it)->m_Type == Value::Nil )
						{
							continue;
						}

						SetValueString( *(*it), p_Output );
					}
				
					// Add a 'e' to indicate that this is the end of a list
					p_Output += "e";
				}
				break;
				case Value::Dictionary:
				{
					// Make sure that the dictionary isn't null
					if( !p_Value.m_Value.Dictionary )
					{
						return false;
					}

					// Add a 'd' to indicate that this is the start of a dictionary
					p_Output += "d";

					// Go through all the value elements
					for( Value::ValueMap::iterator it = p_Value.m_Value.Dictionary->begin( ); it != p_Value.m_Value.Dictionary->end( ); it++ )
					{
						// Ignore Nil values.
						if( it->second->m_Type == Value::Nil )
						{
							continue;
						}

						// Get the string length of the key
						std::stringstream length;
						length << it->first.size( );

						// Append encoded key
						p_Output += length.str( ) + ":" + it->first;

						SetValueString( *(it->second), p_Output );
					}

					// Add a 'e' to indicate that this is the end of a dictionary
					p_Output += "e";
				}
				break;
				default: // Nil value
				{
					return false;
				}
				break;
			}

			return true;
		}

		// Styled writer
		StyledWriter::StyledWriter( )
		{
		}

		Bool StyledWriter::Write( std::string & p_Output, Value & p_Value )
		{
			return SetValueString( p_Value, p_Output, 0 );
		}

		Bool StyledWriter::SetValueString( const Value & p_Value, std::string & p_Output, const Uint32 & p_Layer ) const
		{
			switch( p_Value.m_Type )
			{
				case Value::Integer:
				{
					// Get the string via a stringstream
					std::stringstream ss;
					ss << p_Value.m_Value.Integer;

					p_Output += "(i): " + ss.str( ) + "\n";
				}
				break;
				case Value::String:
				{
					// Make sure that the string isn't null
					if( !p_Value.m_Value.String )
					{
						p_Output.append( p_Layer, '\t' );
						p_Output += "[NULL string]\n";
						break;
					}
					p_Output += "(s): " + *p_Value.m_Value.String + "\n";
				}
				break;
				case Value::List:
				{
					// Make sure that the list isn't null
					if( !p_Value.m_Value.List )
					{
						p_Output += "[NULL list]\n";
						break;
					}

					// Add a 'l' to indicate that this is the start of a list
					p_Output += "(l):\n";

					// Go through the list
					SizeType index = 0;
					for( Value::ValueVector::iterator it = p_Value.m_Value.List->begin( ); it != p_Value.m_Value.List->end( ); it++ )
					{
						// Get the index as a string
						std::stringstream ss;
						ss << index;

						p_Output.append( p_Layer + 1, '\t' );
						p_Output += ss.str( ) + ": ";

						SetValueString( *(*it), p_Output, p_Layer + 1 );

						index++;
					}
				
					// Add a 'e' to indicate that this is the end of a list
					p_Output.append( p_Layer, '\t' );
					p_Output += "e\n";
				}
				break;
				case Value::Dictionary:
				{
					// Make sure that the dictionary isn't null
					if( !p_Value.m_Value.Dictionary )
					{
						p_Output += "[NULL dictionary]\n";
						break;
					}

					// Add a 'd' to indicate that this is the start of a dictionary
					p_Output += "(d):\n";

					// Go through all the value elements
					for( Value::ValueMap::iterator it = p_Value.m_Value.Dictionary->begin( ); it != p_Value.m_Value.Dictionary->end( ); it++ )
					{
						p_Output.append( p_Layer + 1, '\t' );
						p_Output += it->first + " ";
						SetValueString( *(it->second), p_Output, p_Layer + 1 );
					}

					// Add a 'e' to indicate that this is the end of a dictionary
					p_Output.append( p_Layer, '\t' );
					p_Output += "e\n";
				}
				break;
				default: // Nil value
				{
					p_Output.append( p_Layer, '\t' );
					p_Output += "[Nil]\n";
				}
				break;
			}

			return true;
		}

	}

}