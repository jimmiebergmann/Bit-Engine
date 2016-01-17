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

#include <Bit/Network/Net/HostMessageDecoder.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		HostMessageDecoder::HostMessageDecoder( const std::string & p_Name,
												Uint8 *		p_pMessage,
												SizeType	p_MessageSize ) :
			m_Name( p_Name ),
			m_Position( 0 ),
			m_pMessage( p_pMessage ),
			m_MessageSize( p_MessageSize )
		{
		}

		Uint8 HostMessageDecoder::ReadByte( )
		{
			if( m_Position > m_MessageSize )
			{
				return 0;
			}

			return m_pMessage[ m_Position++ ];
		}

		Int32 HostMessageDecoder::ReadInt( )
		{
			if( m_Position + 4 > m_MessageSize )
			{
				return 0;
			}

			// Read the integer
			Int32 integer = static_cast<Int32>( m_pMessage[ m_Position ] )				|
							(static_cast<Int32>( m_pMessage[ m_Position + 1 ] ) << 8 )	|
							(static_cast<Int32>( m_pMessage[ m_Position + 2] ) << 16 )	|
							(static_cast<Int32>( m_pMessage[ m_Position + 3] ) << 24 );

			// Move the position
			m_Position += 4;

			return Ntoh32( integer );
		}
	
		Float32 HostMessageDecoder::ReadFloat( )
		{
			if( m_Position + 4 > m_MessageSize )
			{
				return 0.0f;
			}

			// Copy the float.
			Float32 floatingPoint;
			memcpy( &floatingPoint, m_pMessage + m_Position, 4 );

			// Increase the position
			m_Position += 4;

			// Return the float
			return floatingPoint;
		}

		Bool HostMessageDecoder::ReadString( std::string & p_String )
		{
			// Get the message name
			SizeType stringEnd = 0;
			for( SizeType i = m_Position; i < m_MessageSize; i++ )
			{
				if( m_pMessage[ i ] == 0 )
				{
					stringEnd = i;
					break;
				}
			}

			// Error check the string length
			if( stringEnd == 0 )
			{
				return false;
			}

			// Get string length
			const SizeType length = stringEnd - m_Position;

			// Copy the string.
			p_String.assign( reinterpret_cast<char*>(m_pMessage + m_Position), length );

			// Increment the position by the length + 1
			m_Position += length + 1;

			// Succeeded
			return true;
		}

		Bool HostMessageDecoder::ReadArray( void * p_pArray, const SizeType p_Size )
		{
			if( m_Position + p_Size > m_MessageSize )
			{
				return 0.0f;
			}

			// Copy the bytes
			memcpy( p_pArray, m_pMessage + m_Position, p_Size );

			// Move the position
			m_Position += p_Size;
			
			// Succeeded.
			return true;
		}

		const std::string & HostMessageDecoder::GetName( ) const
		{
			return m_Name;
		}

		SizeType HostMessageDecoder::GetMessageSize( ) const
		{
			return m_MessageSize;
		}

		SizeType HostMessageDecoder::GetPosition( ) const
		{
			return m_Position;
		}

		void HostMessageDecoder::SetPosition( const SizeType p_Position )
		{
			SizeType position = p_Position;
			if( position >= m_MessageSize )
			{
				position = m_MessageSize - 1;
			}
			m_Position = position;
		}


	}

}