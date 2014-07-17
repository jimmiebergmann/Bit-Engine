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

#include <Bit/System/Json/Value.hpp>
#include <sstream>
#include <iomanip>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Json
	{

		// Global varaibles
		static std::string g_EmptyString		= "";
		static std::string g_TemporaryString	= "";
		static std::string g_BooleanTrueString	= "True";
		static std::string g_BooleanFalseString	= "False";
		static std::string g_NullString			= "[Null]";
		static std::string g_ObjectString		= "[Object]";
		static std::string g_ArrayString		= "[Array]";

		// Static variable members
		const Value Value::NullValue( Null );
		Value Value::s_DefaultValue( Null );

		// Value definitions
		Value::Value( ) :
			m_Type( Null )
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
				case String:
				{
					if( m_Value.String )
					{
						delete m_Value.String;
					}
				}
				break;
				case Array:
				{
					if( m_Value.Array )
					{
						// Delete all the values in the array
						for( ValueVector::iterator it = m_Value.Array->begin( ); it != m_Value.Array->end( ); it++ )
						{
							delete *it;
						}

						delete m_Value.Array;
					}
				}
				break;
				case Object:
				{
					if( m_Value.Object )
					{
						// Go through the dictionary and delete all the values
						for( ValueMap::iterator it = m_Value.Object->begin( ); it != m_Value.Object->end( ); it++ )
						{
							delete it->second;
						}

						// Delete the dictionary pointer.
						delete m_Value.Object;
					}
				}
				break;
				default:
				break;
			}

			// Set the type to Nil.
			m_Type = Null;
		}

		void Value::Append( const Value & p_Value )
		{
			// The value is not a list, turn it into a list
			if( m_Type != Array || !m_Value.Array )
			{
				// Clear the old data first
				Clear( );

				m_Type = Array;
				m_Value.Array = new ValueVector;
			}

			// Create a new value
			Value * pValue = new Value;
				
			// Copy the value
			CopyValue( p_Value, *pValue );
	
			// Push the new value value to the vector.
			m_Value.Array->push_back( pValue );
		}

		void Value::Append( const Float64 & p_Number )
		{
			// The value is not a list, turn it into a list
			if( m_Type != Array || !m_Value.Array )
			{
				// Clear the old data first
				Clear( );

				m_Type = Array;
				m_Value.Array = new ValueVector;
			}

			// Create a value and push it to the vector
			Value * pValue = new Value( Number );
			pValue->m_Value.FloatingPoint = p_Number;
			pValue->m_IntegerFag = false;
			m_Value.Array->push_back( pValue );
		}

		void Value::Append( const Int32 & p_Integer )
		{
			// The value is not a list, turn it into a list
			if( m_Type != Array || !m_Value.Array )
			{
				// Clear the old data first
				Clear( );

				m_Type = Array;
				m_Value.Array = new ValueVector;
			}

			// Create a value and push it to the vector
			Value * pValue = new Value( Number );
			pValue->m_Value.Integer = p_Integer;
			pValue->m_IntegerFag = true;
			m_Value.Array->push_back( pValue );
		}

		void Value::Append( const std::string & p_String )
		{
			// The value is not a list, turn it into a list
			if( m_Type != Array || !m_Value.Array )
			{
				// Clear the old data first
				Clear( );

				m_Type = Array;
				m_Value.Array = new ValueVector;
			}

			// Create a new value
			Value * pValue = new Value( String );
			pValue->m_Value.String = new std::string( );
		
			// Set the string data for the new value.
			pValue->m_Value.String->reserve( p_String.size( ) );
			pValue->m_Value.String->append( p_String  );
	
			// Push the new value value to the vector.
			m_Value.Array->push_back( pValue );
		}

		void Value::Append( const char * p_Characters )
		{
			Append( std::string( p_Characters ) );
		}

		void Value::Append( const Bool p_Boolean )
		{
			// The value is not a list, turn it into a list
			if( m_Type != Array || !m_Value.Array )
			{
				// Clear the old data first
				Clear( );

				m_Type = Array;
				m_Value.Array = new ValueVector;
			}

			// Create a value and push it to the vector
			Value * pValue = new Value( Boolean );
			pValue->m_Value.Boolean = p_Boolean;
			m_Value.Array->push_back( pValue );
		}

		void Value::Erase( const SizeType & p_Index )
		{
			// Make sure that this is an array.
			if( m_Type != Array || !m_Value.Array )
			{
				return;
			}

			// Error check the size
			if( p_Index >= m_Value.Array->size( ) )
			{
				return;
			}

			// Delete the value
			Value * pValue = *(m_Value.Array->begin( ) + p_Index);
			delete pValue;

			// Erase the item
			m_Value.Array->erase( m_Value.Array->begin( ) + p_Index );
		}

		void Value::Erase( const std::string & p_Key )
		{
			// Make sure that this is an object.
			if( m_Type != Object || !m_Value.Object )
			{
				return;
			}

			// Find the value
			ValueMap::iterator it = m_Value.Object->find( p_Key );

			// Could we find the value?
			if( it == m_Value.Object->end( ) )
			{
				return;
			}

			// Delete the value
			delete it->second;

			// Erase the value from the dictionary
			m_Value.Object->erase( it );
		}

		Value::eType Value::GetType( ) const
		{
			return m_Type;
		}

		Bool Value::IsNull( ) const
		{
			return m_Type == Null;
		}

		Value & Value::Get( const std::string & p_Key, const Value & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Object )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Object->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::Get( const std::string & p_Key, const Float64 & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Object )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Object->end( ) )
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
			if( m_Type != Object )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Object->end( ) )
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
			if( m_Type != Object )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Object->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::Get( const std::string & p_Key, const Bool & p_DefaultValue ) const
		{
			// This function only works for dictionaries
			if( m_Type != Object )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key
			if( it == m_Value.Object->end( ) )
			{
				s_DefaultValue = p_DefaultValue;
				return s_DefaultValue;
			}

			// Return the found value.
			return *(it->second);
		}

		SizeType Value::GetSize( ) const
		{
			if( m_Type != Array )
			{
				return 0;
			}

			return static_cast<SizeType>( m_Value.Array->size( ) );
		}

		const std::string & Value::AsString( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Null:
				{
					return g_NullString;
				}
				break;
				case String:
				{
					// Return the string if it's allocated.
					return m_Value.String ? *m_Value.String : g_EmptyString;
				}
				break;
				case Number:
				{
					std::stringstream ss;
					if( m_IntegerFag )
					{
						ss << m_Value.Integer;
					}
					else
					{
						ss << std::fixed << std::setprecision( 4 ) << m_Value.FloatingPoint;
					}
					
					g_TemporaryString = ss.str( ); 

					return g_TemporaryString;
				}
				break;
				case Boolean:
				{
					return m_Value.Boolean ? g_BooleanTrueString : g_BooleanFalseString;
				}
				break;
				case Array:
				{
					return g_ArrayString;
				}
				break;
				case Object:
				{
					return g_ObjectString;
				}
				break;
			}
			
			return g_EmptyString;
		}

		Float64 Value::AsNumber( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Null:
				{
					return 0.0f;
				}
				break;
				case String:
				{
					// Error check the string pointer
					if( !m_Value.String )
					{
						return 0.0f;
					}

					// Load the string into a string stream.
					std::stringstream ss;
					ss << m_Value.String;

					// Get the number
					Float64 number = 0.0f;
					ss >> number;

					// Return the number
					return number;
				}
				break;
				case Number:
				{
					if( m_IntegerFag )
					{
						return static_cast<Float64>( m_Value.Integer );
					}
					else
					{
						return m_Value.FloatingPoint;
					}
				}
				break;
				case Boolean:
				{
					return m_Value.Boolean ? 1.0f : 0.0f;
				}
				break;
				case Array:
				{
					return 0.0f;
				}
				break;
				case Object:
				{
					return 0.0f;
				}
				break;
			}

			return 0.0f;
		}

		Int32 Value::AsInteger( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Null:
				{
					return 0;
				}
				break;
				case String:
				{
					// Error check the string pointer
					if( !m_Value.String )
					{
						return 0;
					}

					// Load the string into a string stream.
					std::stringstream ss;
					ss << m_Value.String;

					// Get the number
					Int32 number = 0;
					ss >> number;

					// Return the number
					return number;
				}
				break;
				case Number:
				{
					if( m_IntegerFag )
					{
						return m_Value.Integer;
					}
					else
					{
						return static_cast<Int32>( m_Value.FloatingPoint );
					}
				}
				break;
				case Boolean:
				{
					return m_Value.Boolean ? 1 : 0;
				}
				break;
				case Array:
				{
					return 0;
				}
				break;
				case Object:
				{
					return 0;
				}
				break;
			}

			return 0;
		}

		Bool Value::AsBoolean( ) const
		{
			// Switch the type in order to know which method you
			// are going to use to convert the value into a string.
			switch( m_Type )
			{
				case Null:
				{
					return false;
				}
				break;
				case String:
				{
					// Check for "True"/"true" or "False"/"false" values?
					return false;
				}
				break;
				case Number:
				{
					if( m_IntegerFag )
					{
						return m_Value.Integer >= 1 ? true : false;
					}
					else
					{
						return m_Value.FloatingPoint >= 1.0f ? true : false;
					}
					
				}
				break;
				case Boolean:
				{
					return m_Value.Boolean;
				}
				break;
				case Array:
				{
					return false;
				}
				break;
				case Object:
				{
					return false;
				}
				break;
			}

			return false;
		}

		Value & Value::operator [ ] ( const std::string & p_Key )
		{
			// This function only works for objects.
			// Turn this value into a object.
			if( m_Type != Object )
			{
				// Clear all the old data
				Clear( );
				m_Type = Object;
				m_Value.Object = new ValueMap;
			}

			// Find the key
			ValueMap::iterator it = m_Value.Object->find( p_Key );
			
			// Could not find the key, add a Null value to the object..
			if( it == m_Value.Object->end( ) )
			{
				// Create a value
				Value * pValue = new Value( Null );
				(*m_Value.Object)[ p_Key ] = pValue;

				// Return the nil value you just created.
				return *pValue;
			}

			// Return the found value.
			return *(it->second);
		}

		Value & Value::operator [ ] ( const SizeType & p_Index )
		{
			// Make sure that the value is a list.
			if( m_Type != Array )
			{
				s_DefaultValue.Clear( );
				return s_DefaultValue;
			}

			// Error check the index.
			if( p_Index > m_Value.Array->size( ) )
			{
				s_DefaultValue.Clear( );
				return s_DefaultValue;
			}

			return *(*m_Value.Array)[ p_Index ];
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

		Value & Value::operator = ( const Float64 & p_Number )
		{
			// The value is not an number, turn it into an number value.
			if( m_Type != Number )
			{
				Clear( );
				m_Type = Number;
			}

			// Set the integer flag
			m_IntegerFag = false;

			// Set the string data.
			m_Value.FloatingPoint = p_Number;

			// Return this value.
			return *const_cast<Value *>( this );
		}

		Value & Value::operator = ( const Int32 & p_Integer )
		{
			// The value is not an number, turn it into an number value.
			if( m_Type != Number )
			{
				Clear( );
				m_Type = Number;
			}

			// Set the integer flag
			m_IntegerFag = true;

			// Set the string data.
			m_Value.Integer = p_Integer;

			// Return this value.
			return *const_cast<Value *>( this );
		}

		Value & Value::operator = ( const char * p_Characters )
		{
			return Value::operator=( std::string( p_Characters ) );
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

		Value & Value::operator = ( const Bool & p_Boolean )
		{
			// The value is not an number, turn it into an number value.
			if( m_Type != Boolean )
			{
				Clear( );
				m_Type = Boolean;
			}

			// Set the string data.
			m_Value.Boolean = p_Boolean;

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
				case Number:
				{
					p_To.m_Type = Number;
					p_To.m_IntegerFag = p_From.m_IntegerFag;

					if( p_From.m_IntegerFag )
					{
						p_To.m_Value.Integer = p_From.m_Value.Integer;
					}
					else
					{
						p_To.m_Value.FloatingPoint = p_From.m_Value.FloatingPoint;
					}					
				}
				break;
				case String:
				{
					// Make sure that the string is valid.
					if( !p_From.m_Value.String )
					{
						p_To.m_Type = Null;
						break;
					}

					p_To.m_Type = String;
					p_To.m_Value.String = new std::string;
					p_To.m_Value.String->reserve( p_From.m_Value.String->size( ) );
					p_To.m_Value.String->append( *p_From.m_Value.String );
					
				}
				break;
				
				case Boolean:
				{
					p_To.m_Type = Boolean;
					p_To.m_Value.Boolean = p_From.m_Value.Boolean;
				}
				break;
				
				case Array:
				{
					// Make sure that the array is valid.
					if( !p_From.m_Value.Array )
					{
						p_To.m_Type = Null;
						break;
					}

					p_To.m_Type = Array;
					p_To.m_Value.Array = new ValueVector;

					// Go throguh all the values in the array, and make copies of them.
					for( ValueVector::iterator it = p_From.m_Value.Array->begin( ); it != p_From.m_Value.Array->end( ); it++ )
					{
						// Create a new value
						Value * pValue = new Value( );

						// Copy the value
						CopyValue( *(*(it)), * pValue );

						// Add the new value to the target array
						p_To.m_Value.Array->push_back( pValue );
					}
					
				}
				break;
				case Object:
				{
					// Make sure that the object is valid.
					if( !p_From.m_Value.Object )
					{
						p_To.m_Type = Null;
						break;
					}
					p_To.m_Type = Object;
					p_To.m_Value.Object = new ValueMap;

					// Go throguh all the values in the object, and make copies of them.
					for( ValueMap::iterator it = p_From.m_Value.Object->begin( ); it != p_From.m_Value.Object->end( ); it++ )
					{
						// Create a new value
						Value * pValue = new Value( );

						// Copy the value
						CopyValue( *(it->second), * pValue );

						// Add the new value to the target object
						(*p_To.m_Value.Object)[ it->first ] = pValue;
					}
				}
				break;
				default: // Null value
				{
					p_To.m_Type = Null;
				}
				break;
			}
		}

	}

}