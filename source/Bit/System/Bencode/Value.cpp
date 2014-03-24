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

#include <Bit/System/Bencode/Value.hpp>

namespace Bit
{

	namespace Bencode
	{

		const Value Value::NilValue = Value( Nil );

		Value::Value( ) :
			m_Type( Nil )
		{
		}

		Value::Value( eType p_Type ) :
			m_Type( p_Type )
		{
		}

		Value::~Value( )
		{
			switch( m_Type )
			{
				case String:
				{
					delete m_Value.String;
				}
				break;
				case List:
				{
					delete m_Value.List;
				}
				break;
				case Dictionary:
				{
					if( m_Value.Dictionary )
					{
						// Go through the dictionary and delete all the values
						for( ValueMap::iterator it = m_Value.Dictionary->begin( ); it != m_Value.Dictionary->begin( ); it++ )
						{
							delete it->second;
						}

						// Delete the dictionary pointer.
						delete m_Value.Dictionary;
					}
				}
				break;
				default:
				break;
			}
		}

		Value::eType Value::GetType( ) const
		{
			return m_Type;
		}

		Bool Value::IsNil( ) const
		{
			return m_Type == Nil;
		}
	
		const Value & Value::Get( const std::string & p_Key ) const
		{
			// This function only works for dictionaries
			if( m_Type != Dictionary )
			{
				return NilValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Dictionary->end( ) )
			{
				return NilValue;
			}

			// Return the found value.
			return *(it->second);
		}
			
		std::string Value::AsString( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Nil:
				{
					return "[Nil]";
				}
				break;
				case String:
				{
					// Return the string if it's allocated.
					return m_Value.String ? *m_Value.String : "";
				}
				break;
				case Integer:
				{
					return "[Integer]";
				}
				break;
				case List:
				{
					return "[List]";
				}
				break;
				case Dictionary:
				{
					return "[Dictionary]";
				}
				break;
			}

			return "";
		}

		Int32 Value::AsInt( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Nil:
				{
					return 0;
				}
				break;
				case String:
				{
					// Get the string as an int
					return atoi( m_Value.String->c_str( ) );
				}
				break;
				case Integer:
				{
					return m_Value.Integer;
				}
				break;
				case List:
				{
					return 0;
				}
				break;
				case Dictionary:
				{
					return 0;
				}
				break;
			}

			return 0;
		}

	}

}