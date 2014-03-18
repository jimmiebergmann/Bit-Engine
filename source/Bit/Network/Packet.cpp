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

#include <Bit/Network/Packet.hpp>

namespace Bit
{

	Packet::Packet( )
	{
	}

	Packet::Packet( const void * p_pData, const SizeType p_Size )
	{
	}

	void Packet::Set( const void * p_pData, const SizeType p_Size )
	{
	}

	void Packet::Clear( )
	{
		m_Data.clear( );
	}

	SizeType Packet::GetSize( ) const
	{
		return m_Data.size( );
	}

	SizeType Packet::GetPosition( ) const
	{
		return 0;
	}

	const void * Packet::GetRawData( ) const
	{
		return reinterpret_cast<const void *>( m_Data.data( ) );
	}

	void Packet::SetPosition( const SizeType m_Position ) const
	{
		
	}

}