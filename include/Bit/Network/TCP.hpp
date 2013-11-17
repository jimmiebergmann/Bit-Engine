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


#ifndef BIT_NETWORK_TCP_HPP
#define BIT_NETWORK_TCP_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/Network.hpp>
#include <Bit/Network/Address.hpp>

namespace Bit
{

	namespace Net
	{

		enum eType
		{
			None = 0,
			Client = 1,
			Server = 2
		};


		class TCP
		{

		public:

			// Constructors/destructors
			TCP( );
			~TCP( );
			
			// Public general functions
			BIT_UINT32 Connect( const Address & p_Address );
			BIT_UINT32 Disconnect( );
			BIT_UINT32 Host( const BIT_UINT16 p_Port );
			BIT_BOOL IsEstablished( ) const;
			BIT_SINT32 Receive( BIT_BYTE * p_pBuffer, const BIT_UINT32 p_Size );
			BIT_SINT32 Send( const BIT_BYTE * p_Buffer, const BIT_UINT32 p_Size );

		private:

			// Private members
			eType m_Type;
			BIT_BOOL m_Established;
			Socket m_Socket;
			Address m_Address;

		};

	}
}

#endif