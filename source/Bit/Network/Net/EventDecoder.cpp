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

#include <Bit/Network/Net/EventDecoder.hpp>
#include <Bit/Network/Socket.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		EventDecoder::EventDecoder( const std::string & p_Name,
									Uint8 *				p_pMessage,
									SizeType			p_MessageSize ) :
			m_Name( p_Name )
		{
			// Error check the message size
			if( p_MessageSize == 0 )
			{
				return;
			}

			// Track the position in the message
			SizeType pos = 0;

			// Keep on parsing until we reach the end of the message
			while( pos < p_MessageSize )
			{
				// Parse the variable name
				SizeType stringEnd = 0;
				for( SizeType i = pos; i < p_MessageSize; i++ )
				{
					if( p_pMessage[ i ] == 0 )
					{
						stringEnd = i;
						break;
					}
				}

				// Error check the string length
				if( stringEnd == 0 )
				{
					return;
				}

				// Get string length
				const SizeType nameLength = stringEnd - pos;

				// Copy the string.
				std::string variableName;
				variableName.assign( reinterpret_cast<char*>(p_pMessage + pos), nameLength );

				// Increment the position by the length + 1
				pos += nameLength + 1;

				// Parse the variable size
				if( ( pos + 2 ) >= p_MessageSize )
				{
					break;
				}

				// Get the data size
				Uint16 dataSize =	 static_cast<Uint16>( p_pMessage[ pos ] )				|
									(static_cast<Uint16>( p_pMessage[ pos + 1 ] ) << 8 );
				dataSize = Ntoh16( dataSize );

				// Error check the data size
				if( dataSize == 0 )
				{
					break;
				}

				// Move the position to the varaible data
				pos += 2;

				// Parse the variable size
				if( ( pos + dataSize ) > p_MessageSize )
				{
					break;
				}

				// Check if the varaible already exists
				Variable * pVariable = GetVariable( variableName );
				if( pVariable != NULL )
				{
					// Ignore this variable if it's already parsed.
					pos += dataSize;
					continue;
				}

				// Add the new variable
				pVariable = new Variable;
				m_Variables[ variableName ] = pVariable;
				pVariable->first = dataSize;
				pVariable->second = new Uint8[ dataSize ];

				// Get the variable data
				memcpy( pVariable->second, p_pMessage + pos, dataSize );

				// Move the position to the next varaible
				pos += dataSize;
			}
		}

		EventDecoder::~EventDecoder( )
		{
			for( VariableMap::iterator it = m_Variables.begin( );
				 it != m_Variables.end( );
				 it++ )
			{
				delete [ ] it->second->second;

				if( it->second )
				{
					delete [ ] it->second;
				}
			}
		}

		Uint8 EventDecoder::GetByte( const std::string & p_VariableName )
		{
			Variable * pVariable = GetVariable( p_VariableName );
			if( pVariable == NULL )
			{
				return 0;
			}

			if( pVariable->first < 1 )
			{
				return 0;
			}
			
			return pVariable->second[ 0 ];
		}

		Int32 EventDecoder::GetInt( const std::string & p_VariableName )
		{
			Variable * pVariable = GetVariable( p_VariableName );
			if( pVariable == NULL )
			{
				return 0;
			}

			if( pVariable->first < 4 )
			{
				return 0;
			}

			// Read the integer
			Int32 integer = static_cast<Int32>( pVariable->second[ 0 ] )			|
							(static_cast<Int32>( pVariable->second[ 1 ] ) << 8 )	|
							(static_cast<Int32>( pVariable->second[ 2] ) << 16 )	|
							(static_cast<Int32>( pVariable->second[ 3] ) << 24 );

			return Ntoh32( integer );
		}

		Float32 EventDecoder::GetFloat( const std::string & p_VariableName )
		{
			Variable * pVariable = GetVariable( p_VariableName );
			if( pVariable == NULL )
			{
				return 0.0f;
			}

			if( pVariable->first < 4 )
			{
				return 0.0f;
			}

			// Copy the float.
			Float32 floatingPoint;
			memcpy( &floatingPoint, pVariable->second, 4 );

			// Return the float
			return floatingPoint;
		}

		std::string EventDecoder::GetString( const std::string & p_VariableName )
		{
			Variable * pVariable = GetVariable( p_VariableName );
			if( pVariable == NULL )
			{
				return "";
			}

			if( pVariable->first < 1 )
			{
				return "";
			}

			// Copy the string.
			std::string string;
			string.assign( reinterpret_cast<char*>(pVariable->second ), pVariable->first );

			// Succeeded
			return string;
		}		

		const std::string & EventDecoder::GetName( ) const
		{
			return m_Name;
		}

		EventDecoder::Variable * EventDecoder::GetVariable( const std::string & p_VariableName )
		{
			// Check if the varaible already exists
			VariableMap::iterator it = m_Variables.find( p_VariableName );
			if( it != m_Variables.end( ) )
			{
				return it->second;
			}

			return NULL;
		}

	}

}