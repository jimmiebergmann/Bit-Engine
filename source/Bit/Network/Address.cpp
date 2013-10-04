// ///////////////////////////////////////////////////////////////////////////
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


#include <Bit/Network/Address.hpp>
#include <Bit/Network.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Address::Address() :
            m_Address(0),
            m_Port(0)
        {
		}

		Address::Address(	const BIT_UINT8 p_A, const BIT_UINT8 p_B, const BIT_UINT8 p_C,
							const BIT_UINT8 p_D, const BIT_UINT16 p_Port ) :
			m_Port( p_Port )
		{
			/// Bit swift the adress
			m_Address = (p_A << 24) | (p_B << 16) | (p_C << 8) | p_D;
		}

		Address::Address( const BIT_UINT32 p_Address, const BIT_UINT16 p_Port) :
			m_Address( p_Address ),
			m_Port( p_Port )
		{
		}

		Address::Address( BIT_UINT16 p_Port ) :
			m_Address( 0 ),
			m_Port( p_Port )
		{
		}

		BIT_BOOL Address::SetAddressByString( std::string p_String )
		{
			// Remove everything after the : sign if there are any
			// and use it as the port
			m_Port = 0;
			BIT_SINT32 PortPos = 0;

			if( ( PortPos = static_cast< BIT_SINT32>( p_String.find( ':' )) ) != -1 )
			{
				// Make sure there's any address at all..
				if( PortPos == 0 )
				{
					return BIT_FALSE;
				}

				// Move the position forward
				PortPos++;

				BIT_SINT32 CopyCount = p_String.size() - PortPos;
				// make sure there are any characters to copy
				if( CopyCount > 0 && CopyCount <= 5 )
				{
					// Get the port number in a string of chars
					char PortBuffer[ 8 ];
					p_String.copy( PortBuffer, CopyCount, PortPos );
					PortBuffer[ CopyCount ] = '\0';

					// convert it to a real short
					m_Port = static_cast< BIT_UINT16 >( atoi( PortBuffer ) );		
				}

				// Let's remove the port part of the string
				p_String.resize( PortPos - 1 );
				
			}

			// Is the string a plain ip-address?
			if( inet_addr( p_String.c_str() ) != -1 )
			{
				m_Address = htonl( inet_addr( p_String.c_str() ) );
			}
			// So check if the string is a URL
			else
			{
				struct hostent *he;
				struct in_addr **addr_list;

				if( ( he = gethostbyname( p_String.c_str() ) ) == NULL )
				{
					//gethostbyname failed
					///herror("gethostbyname");
					///cout<<"Failed to resolve hostname\n";
					
					return BIT_FALSE;
				}

				// Get the hostname in string form
				char * Hostname =  inet_ntoa (*( (struct in_addr *) he->h_addr_list[0] ) );

				// Convert it to a string.
				if( (m_Address = inet_addr( Hostname ) )  == -1 )
				{
					return BIT_FALSE;
				}

				// Convert the address to a network side number
				m_Address = htonl( m_Address );
			}

			// Return true, everything went well.
			return BIT_TRUE;
		}

		unsigned int Address::GetAddress() const
		{
			return m_Address;
		}
		BIT_UINT8 Address::GetA() const
		{
			return (BIT_UINT8) (m_Address >> 24);
		}
		BIT_UINT8 Address::GetB() const
		{
			return (BIT_UINT8) (m_Address >> 16);
		}
		BIT_UINT8 Address::GetC() const
		{
			return (BIT_UINT8) (m_Address >> 8);
		}
		BIT_UINT8 Address::GetD() const
		{
			return (BIT_UINT8) (m_Address);
		}
		BIT_UINT16 Address::GetPort() const
		{
			return m_Port;
		}
		Address::AddressIndex  Address::GetAddressIndex() const
		{
			return (m_Address * m_Port) + m_Port;
		}

		void Address::SetPortValue(BIT_UINT16 p_Port)
		{
			m_Port = p_Port;
		}
		/*void Address::SetA( BIT_UINT8 p_A )
		{

		}
		void Address::SetB( BIT_UINT8 p_B )
		{

		}
		void Address::SetC( BIT_UINT8 p_C )
		{

		}
		void Address::SetD( BIT_UINT8 p_D )
		{

		}*/

		BIT_BOOL Address::operator == (const Address & p_Address) const
		{
			return m_Address == p_Address.GetAddress();
		}
		BIT_BOOL Address::operator != (const Address & p_Address) const
		{
			return m_Address != p_Address.GetAddress();
		}
	}

}
