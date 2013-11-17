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


#ifndef BIT_NETWORK_ADDRESS_HPP
#define BIT_NETWORK_ADDRESS_HPP

#include <Bit/DataTypes.hpp>
#include <string>

namespace Bit
{
	namespace Net
	{

		class Address
		{
		public:

			typedef BIT_UINT64 AddressIndex;

			Address();
			Address( const BIT_UINT8 p_A, const BIT_UINT8 p_B, const BIT_UINT8 p_C,
				const BIT_UINT8 p_D, const BIT_UINT16 p_Port );
			Address( const BIT_UINT32 p_Address, BIT_UINT16 p_Port );
			Address( BIT_UINT16 p_Port );
			BIT_BOOL SetAddressByString( std::string p_String );
			BIT_UINT32 GetAddress() const;
			BIT_UINT8 GetA() const;
			BIT_UINT8 GetB() const;
			BIT_UINT8 GetC() const;
			BIT_UINT8 GetD() const;
			BIT_UINT16 GetPort() const;
			AddressIndex GetAddressIndex() const;
			void SetPortValue( BIT_UINT16 p_Port );
			/*void SetA( BIT_UINT8 p_A );
			void SetB( BIT_UINT8 p_B );
			void SetC( BIT_UINT8 p_C );
			void SetD( BIT_UINT8 p_D );*/
			BIT_BOOL operator == (const Address & p_Address) const;
			BIT_BOOL operator != (const Address & p_Address) const;

		private:

			BIT_UINT32 m_Address;
			BIT_UINT16 m_Port;

		};
	}

}

#endif