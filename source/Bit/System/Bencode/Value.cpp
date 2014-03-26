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
#include <sstream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Bencode
	{

		// Static variable members
		const Value Value::NilValue( Nil );
		Value Value::s_DefaultValue( Nil );

		// Value definitions
		Value::Value( ) :
			m_Type( Nil )
		{
		}

		Value::Value( eType p_Type ) :
			m_Type( p_Type )
		{
		}

		Value::Value( const Value & p_Value )
		{
			// Always clear the old data
			Clear( );

			// Get this value's pointer
			Value * pValue = const_cast<Value *>( this );

			// Copy the value
			CopyValue( p_Value, *pValue );
		}

		Value::~Value( )
		{
			Clear( );
		}

		void Value::Clear( )
		{
			switch( m_Type )
			{
				case Integer:
				{
					m_Value.Integer = 0;
				}
				break;
				case String:
				{
					if( m_Value.String )
					{
						delete m_Value.String;
					}
				}
				break;
				case List:
				{
					if( m_Value.List )
					{
						// Delete all the values in the list
						for( ValueVector::iterator it = m_Value.List->begin( ); it != m_Value.List->end( ); it++ )
						{
							delete *it;
						}

						delete m_Value.List;
					}
				}
				break;
				case Dictionary:
				{
					if( m_Value.Dictionary )
					{
						// Go through the dictionary and delete all the values
						for( ValueMap::iterator it = m_Value.Dictionary->begin( ); it != m_Value.Dictionary->end( ); it++ )
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

			// Set the type to Nil.
			m_Type = Nil;
		}

		void Value::Append( const Value & p_Value )
		{
			// The value is not a list, turn it into a list
			if( m_Type != List || !m_Value.List )
			{
				// Clear the old data first
				Clear( );

				m_Type = List;
				m_Value.List = new ValueVector;
			}

			// Create a new value
			Value * pValue = new Value;
				
			// Copy the value
			CopyValue( p_Value, *pValue );
	
			// Push the new value value to the vector.
			m_Value.List->push_back( pValue );
		}

		void Value::Append( const Int32 & p_Integer )
		{
			// The value is not a list, turn it into a list
			if( m_Type != List || !m_Value.List )
			{
				// Clear the old data first
				Clear( );

				m_Type = List;
				m_Value.List = new ValueVector;
			}

			// Create a value and push it to the vector
			Value * pValue = new Value( Integer );
			pValue->m_Value.Integer = p_Integer;
			m_Value.List->push_back( pValue );
		}

		void Value::Append( const std::string & p_String )
		{
			// The value is not a list, turn it into a list
			if( m_Type != List || !m_Value.List )
			{
				// Clear the old data first
				Clear( );

				m_Type = List;
				m_Value.List = new ValueVector;
			}

			// Create a new value
			Value * pValue = new Value( String );
			pValue->m_Value.String = new std::string( );
		
			// Set the string data for the new value.
			pValue->m_Value.String->reserve( p_String.size( ) );
			pValue->m_Value.String->append( p_String  );
	
			// Push the new value value to the vector.
			m_Value.List->push_back( pValue );
		}

		void Value::Erase( const SizeType & p_Index )
		{
			// Make sure that this is a list.
			if( m_Type != List || !m_Value.List )
			{
				return;
			}

			// Error check the size
			if( p_Index >= m_Value.List->size( ) )
			{
				return;
			}

			// Delete the value
			Value * pValue = *(m_Value.List->begin( ) + p_Index);
			delete pValue;

			// Erase the item
			m_Value.List->erase( m_Value.List->begin( ) + p_Index );
		}

		void Value::Erase( const std::string & p_Key )
		{
			// Make sure that this is a dictionary.
			if( m_Type != Dictionary || !m_Value.Dictionary )
			{
				return;
			}

			// Find the value
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );

			// Could we find the value?
			if( it == m_Value.Dictionary->end( ) )
			{
				return;
			}

			// Delete the value
			delete it->second;

			// Erase the value from the dictionary
			m_Value.Dictionary->erase( it );
		}

		Value::eType Value::GetType( ) const
		{
			return m_Type;
		}

		Bool Value::IsNil( ) const
		{
			return m_Type == Nil;
		}
	
		Value & Value::Get( const std::string & p_Key, const Value & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Dictionary )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Dictionary->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::Get( const std::string & p_Key, const Int32 & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Dictionary )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Dictionary->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::Get( const std::string & p_Key, const std::string & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Dictionary )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Dictionary->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		SizeType Value::GetSize( ) const
		{
			if( m_Type != List )
			{
				return 0;
			}

			return static_cast<SizeType>( m_Value.List->size( ) );
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
					std::stringstream ss;
					ss << m_Value.Integer;
					return ss.str( );
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
					if( !m_Value.String )
					{
						return 0;
					}

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

		Value & Value::operator [ ] ( const std::string & p_Key )
		{
			// This function only works for dictionaries.
			// Turn this value into a dictionary.
			if( m_Type != Dictionary )
			{
				// Clear all the old data
				Clear( );
				m_Type = Dictionary;
				m_Value.Dictionary = new ValueMap;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Dictionary->find( p_Key );
			
			// Could not find the key, add a Nil value to the dictionary.
			if( it == m_Value.Dictionary->end( ) )
			{
				// Create a value
				Value * pValue = new Value( Nil );
				(*m_Value.Dictionary)[ p_Key ] = pValue;

				// Return the nil value you just created.
				return *pValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::operator [ ] ( const SizeType & p_Index )
		{
			// Make sure that the value is a list.
			if( m_Type != List )
			{
				s_DefaultValue.Clear( );
				return s_DefaultValue;
			}

			// Error check the index.
			if( p_Index > m_Value.List->size( ) )
			{
				s_DefaultValue.Clear( );
				return s_DefaultValue;
			}

			return *(*m_Value.List)[ p_Index ];
		}


		Value & Value::operator = ( const Value & p_Value )
		{
			// Always clear the old data
			Clear( );

			// Get this value's pointer
			Value * pValue = const_cast<Value *>( this );

			// Copy the value
			CopyValue( p_Value, *pValue );

			// Return this value.
			return *pValue;
		}

		Value & Value::operator = ( const Int32 & p_Integer )
		{
			// The value is not an integer, turn it into an integer value.
			if( m_Type != Integer )
			{
				Clear( );
				m_Type = Integer;
			}

			// Set the string data.
			m_Value.Integer = p_Integer;

			// Return this value.
			return *const_cast<Value *>( this );
		}
		
		Value & Value::operator = ( const std::string & p_String )
		{
			// The value is not a string, turn it into a string value.
			if( m_Type != String )
			{
				Clear( );
				m_Type = String;
				m_Value.String = new std::string( );
			}

			// Set the string data.
			m_Value.String->reserve( p_String.size( ) );
			m_Value.String->append( p_String );

			// Return this value.
			return *const_cast<Value *>( this );
		}

		bool Value::operator == ( const Value & p_Value ) const
		{
			return m_Type == p_Value.m_Type;
		}

		bool Value::operator != ( const Value & p_Value ) const
		{
			return m_Type != p_Value.m_Type;
		}

		// Private functions
		void Value::CopyValue( const Value & p_From, Value & p_To ) const
		{
			// Set the type
			p_To.m_Type = p_From.m_Type;
			
			// Set the data
			switch( p_From.m_Type )
			{
				case Integer:
				{
					p_To.m_Type = Integer;
					p_To.m_Value.Integer = p_From.m_Value.Integer;
				}
				break;
				case String:
				{
					// Make sure that the string is valid.
					if( !p_From.m_Value.String )
					{
						p_To.m_Type = Nil;
						break;
					}

					p_To.m_Type = String;
					p_To.m_Value.String = new std::string;
					p_To.m_Value.String->reserve( p_From.m_Value.String->size( ) );
					p_To.m_Value.String->append( *p_From.m_Value.String );
					
				}
				break;
				case List:
				{
					// Make sure that the list is valid.
					if( !p_From.m_Value.List )
					{
						p_To.m_Type = Nil;
						break;
					}

					p_To.m_Type = List;
					p_To.m_Value.List = new ValueVector;

					// Go throguh all the values in the list, and make copies of them.
					for( ValueVector::iterator it = p_From.m_Value.List->begin( ); it != p_From.m_Value.List->end( ); it++ )
					{
						// Create a new value
						Value * pValue = new Value( );

						// Copy the value
						CopyValue( *(*(it)), * pValue );

						// Add the new value to the target list
						p_To.m_Value.List->push_back( pValue );
					}
					
				}
				break;
				case Dictionary:
				{
					// Make sure that the dictionary is valid.
					if( !p_From.m_Value.Dictionary )
					{
						p_To.m_Type = Nil;
						break;
					}
					p_To.m_Type = Dictionary;
					p_To.m_Value.Dictionary = new ValueMap;

					// Go throguh all the values in the dictionary, and make copies of them.
					for( ValueMap::iterator it = p_From.m_Value.Dictionary->begin( ); it != p_From.m_Value.Dictionary->end( ); it++ )
					{
						// Create a new value
						Value * pValue = new Value( );

						// Copy the value
						CopyValue( *(it->second), * pValue );

						// Add the new value to the target dictionary
						(*p_To.m_Value.Dictionary)[ it->first ] = pValue;
					}
				}
				break;
				default: // Nil value
				{
					p_To.m_Type = Nil;
				}
				break;
			}
		}

	}

}