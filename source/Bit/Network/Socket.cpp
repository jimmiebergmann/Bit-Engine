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

#include <Bit/Network/Socket.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static const Uint16 g_Endian16 = 1;
	static const Uint8 g_Endian8 =  *(reinterpret_cast<const Uint8 *>( &g_Endian16 ) );

	BIT_API Uint16 Hton16( const Uint16 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( (p_Value << 8 )	& 0xFF00 ) |
					( ( p_Value >> 8 )	& 0x00FF );
		}

		// Are the bytes already in network order?
		return p_Value;
	}

	BIT_API Uint32 Hton32( const Uint32 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( ( p_Value << 24 )	& 0xFF000000 )	|
					( ( p_Value << 8 )	& 0x00FF0000 )	|
					( ( p_Value >> 8 )	& 0x0000FF00 )	|
					( ( p_Value >> 24 )	& 0x000000FF );
		}

		// Are the bytes already in network order?
		return p_Value;
	}

	BIT_API Uint64 Hton64( const Uint64 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( ( p_Value << 56ULL )	& 0xFF00000000000000ULL )	|
					( ( p_Value << 40ULL )	& 0x00FF000000000000ULL )	|
					( ( p_Value << 24ULL )	& 0x0000FF0000000000ULL )	|
					( ( p_Value << 8ULL )	& 0x000000FF00000000ULL )	|
					( ( p_Value >> 8ULL )	& 0x00000000FF000000ULL )	|
					( ( p_Value >> 24ULL )	& 0x0000000000FF0000ULL )	|
					( ( p_Value >> 40ULL )	& 0x000000000000FF00ULL )	|
					( ( p_Value >> 56ULL )	& 0x00000000000000FFULL );
		}

		// Are the bytes already in network order?
		return p_Value;
	}


	BIT_API Uint16 Ntoh16( const Uint16 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( ( p_Value << 8 )	& 0xFF00 ) |
					( ( p_Value >> 8 )	& 0x00FF );
		}

		// Are the bytes already in network order?
		return p_Value;
	}

	BIT_API Uint32 Ntoh32( const Uint32 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( ( p_Value << 24 )	& 0xFF000000 )	|
					( ( p_Value << 8 )	& 0x00FF0000 )	|
					( ( p_Value >> 8 )	& 0x0000FF00 )	|
					( ( p_Value >> 24 )	& 0x000000FF );
		}

		// Are the bytes already in network order?
		return p_Value;
	}

	BIT_API Uint64 Ntoh64( const Uint64 p_Value )
	{
		// Is this little endian system? Then swap byte order.
		if( g_Endian16 == g_Endian8 )
		{
			return	( ( p_Value << 56ULL )	& 0xFF00000000000000ULL )	|
					( ( p_Value << 40ULL )	& 0x00FF000000000000ULL )	|
					( ( p_Value << 24ULL )	& 0x0000FF0000000000ULL )	|
					( ( p_Value << 8ULL )	& 0x000000FF00000000ULL )	|
					( ( p_Value >> 8ULL )	& 0x00000000FF000000ULL )	|
					( ( p_Value >> 24ULL )	& 0x0000000000FF0000ULL )	|
					( ( p_Value >> 40ULL )	& 0x000000000000FF00ULL )	|
					( ( p_Value >> 56ULL )	& 0x00000000000000FFULL );
		}

		// Are the bytes already in network order?
		return p_Value;
	}

}