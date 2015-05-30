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

#include <Bit/Network/Net/UserMessage.hpp>
#include <Bit/Network/Net/Client.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		UserMessage::UserMessage( )
		{
		}

		UserMessage::UserMessage( const std::string & p_Name, Client * p_pClient, Int32 p_MessageSize ) :
			m_Name( p_Name ),
			m_pClient( p_pClient )
		{
			if( p_MessageSize > 0 )
			{
				m_Message.reserve( static_cast<MessageVector::size_type>( p_MessageSize ) +
								   static_cast<MessageVector::size_type>( p_Name.size( ) ) + 2 );
			}
			else
			{
				m_Message.reserve( static_cast<MessageVector::size_type>( p_Name.size( ) ) + 2  );
			}

			// Add the message type and message name
			m_Message.push_back( static_cast<Uint8>( eMessageType::UserMessageType ) );
			for( std::string::size_type i = 0; i < p_Name.size( ); i++ )
			{
				m_Message.push_back( static_cast<Uint8>( p_Name[ i ] ) );
			}
			m_Message.push_back( 0 );
		}

		void UserMessage::WriteByte( const Uint8 p_Byte )
		{
			m_Message.push_back( p_Byte );
		}

		void UserMessage::WriteInt( const Int32 p_Int )
		{
			Uint32 integer = Hton32( static_cast<Uint32>( p_Int ) );

			m_Message.push_back( static_cast<Uint8>( integer ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 8 ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 16 ) );
			m_Message.push_back( static_cast<Uint8>( integer >> 24 ) );
		}

		void UserMessage::WriteFloat( const Float32 p_Float )
		{
			// Get void pointer to float
			const Uint8 * pPointer = reinterpret_cast<const Uint8 *>( &p_Float );

			m_Message.push_back( pPointer[ 0 ] );
			m_Message.push_back( pPointer[ 1 ] );
			m_Message.push_back( pPointer[ 2 ] );
			m_Message.push_back( pPointer[ 3 ] );
		}

		void UserMessage::WriteString( const std::string & p_String )
		{
			for( std::string::size_type i = 0; i < p_String.size( ); i++ )
			{
				m_Message.push_back( static_cast<Uint8>( p_String[ i ] ) );
			}
			m_Message.push_back( 0 );
		}

		void UserMessage::WriteArray( const void * p_pArray, const SizeType p_Size )
		{
			for( SizeType i = 0; i < p_Size; i++ )
			{
				m_Message.push_back( reinterpret_cast<const Uint8 *>( p_pArray )[ i ] );
			}
		}

		Bool UserMessage::Send( const Bool p_Reliable )
		{
			// Send the message
			if( p_Reliable )
			{
				m_pClient->SendReliable( PacketType::UserMessage, reinterpret_cast<void*>( m_Message.data( ) ), m_Message.size( ), true );
			}
			else
			{
				m_pClient->SendUnreliable(PacketType::UserMessage, reinterpret_cast<void*>(m_Message.data()), m_Message.size(), true, true);
			}

			return true;
		}

		const std::string & UserMessage::GetName( ) const
		{
			return m_Name;
		}

	}

}