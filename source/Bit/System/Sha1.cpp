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

#include <Bit/System/Sha1.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	static inline Uint32 LeftBitRotate( const Uint32 p_Value, const Uint32 p_Bits )
	{
		return ( p_Value << p_Bits ) | ( p_Value >> ( 32 - p_Bits ) );
	}

	Sha1::Sha1( )
	{
	}

	void Sha1::Generate( const Uint8 * p_pData, const SizeType p_DataSize )
	{
		// Clear the states
		ClearStates( );

		// Calculate the block count and the remainder
		const SizeType blockCount = ( p_DataSize / 64 ) + 1;
		const SizeType remainder = p_DataSize % 64;
		const Uint64 messageSize = static_cast<Uint64>( p_DataSize ) * 8;

		// Go throguh all blocks, except the last one.
		// Do not do anything if there is just one single block.
		for( SizeType i = 0; i < blockCount - 1; i++ )
		{
			// Compute the current block
			const Uint8 * pCurrentData = p_pData + ( i * 64 );
			ComputeBlock( pCurrentData );
		}

		// Now, cumpute the last block
		// There should always be one padded block at the end.
		
		// The block should just be padded.
		if( remainder < 56 )
		{
			// Create a buffer for the padded data
			Uint8 buffer[ 64 ];
			const Uint8 * pCurrentData = p_pData + ( (blockCount - 1 ) * 64 );

			// Copy over the buffer data
			memcpy( buffer, pCurrentData, remainder );

			// Add 0x80 after the message( 10000000 in binary)
			buffer[ remainder ] = 0x80;

			// Pad the message with zeros
			memset( buffer + ( remainder + 1 ), 0, 55 - remainder );

			// Add the the (message size * 8) to the end( 64 bit unsigned integer).
			buffer[ 56 ] = static_cast<Uint8>( messageSize >> 56ULL );
			buffer[ 57 ] = static_cast<Uint8>( messageSize >> 48ULL );
			buffer[ 58 ] = static_cast<Uint8>( messageSize >> 40ULL );
			buffer[ 59 ] = static_cast<Uint8>( messageSize >> 32ULL );
			buffer[ 60 ] = static_cast<Uint8>( messageSize >> 24ULL );
			buffer[ 61 ] = static_cast<Uint8>( messageSize >> 16ULL );
			buffer[ 62 ] = static_cast<Uint8>( messageSize >> 8ULL );
			buffer[ 63 ] = static_cast<Uint8>( messageSize );

			// Compute the block
			ComputeBlock( buffer );
		}
		// We should add another block at the end.
		else
		{
			// Create a buffer for the padded data, but for two blocks
			Uint8 buffer[ 128 ];
			const Uint8 * pCurrentData = p_pData + ( (blockCount - 1 ) * 64 );

			// Copy over the buffer data
			memcpy( buffer, pCurrentData, remainder );

			// Add 0x80 after the message( 10000000 in binary)
			buffer[ remainder ] = 0x80;

			// Pad the message with zeros
			memset( buffer + ( remainder + 1 ), 0, 119 - remainder );

			// Add the the (message size * 8) to the end( 64 bit unsigned integer).
			buffer[ 120 ] = static_cast<Uint8>( messageSize >> 56ULL );
			buffer[ 121 ] = static_cast<Uint8>( messageSize >> 48ULL );
			buffer[ 122 ] = static_cast<Uint8>( messageSize >> 40ULL );
			buffer[ 123 ] = static_cast<Uint8>( messageSize >> 32ULL );
			buffer[ 124 ] = static_cast<Uint8>( messageSize >> 24ULL );
			buffer[ 125 ] = static_cast<Uint8>( messageSize >> 16ULL );
			buffer[ 126 ] = static_cast<Uint8>( messageSize >> 8ULL );
			buffer[ 127 ] = static_cast<Uint8>( messageSize );

			// Compute the first block
			ComputeBlock( buffer );

			// Compute the very last block
			ComputeBlock( buffer + 64 );
		}

		// Set the hash
		m_Hash.Clear( );
		for( SizeType i = 0; i < 5; i++ )
		{
			m_Hash.AddByte( static_cast<Uint8>( m_States[ i ] >> 24 ) );
			m_Hash.AddByte( static_cast<Uint8>( m_States[ i ] >> 16 ) );
			m_Hash.AddByte( static_cast<Uint8>( m_States[ i ] >> 8 ) );
			m_Hash.AddByte( static_cast<Uint8>( m_States[ i ] ) );
		}

	}

	void Sha1::Generate( const std::string & p_String )
	{
		Generate( reinterpret_cast<const Uint8 *>( &p_String[ 0 ] ), p_String.size( ) ) ;
	}

	Hash & Sha1::GetHash( )
	{
		return m_Hash;
	}

	void Sha1::ClearStates( )
	{
		m_States[ 0 ] = 0x67452301;
		m_States[ 1 ] = 0xEFCDAB89;
		m_States[ 2 ] = 0x98BADCFE;
		m_States[ 3 ] = 0x10325476;
		m_States[ 4 ] = 0xC3D2E1F0;
	}
		
	void Sha1::ComputeBlock( const Uint8 * p_Data )
	{
		Uint32 w[ 80 ];

		// Split up the block into sixteen unsigned 32 bit integers.
		for( SizeType i = 0; i < 16; i++ )
		{
			w[ i ] =	p_Data[ i * 4 ]		<< 24;
			w[ i ] |=	p_Data[ i * 4 + 1 ]	<< 16;
			w[ i ] |=	p_Data[ i * 4 + 2 ]	<< 8;
			w[ i ] |=	p_Data[ i * 4 + 3 ];
		}

		// Extend the sixteen 32-bit words into eighty 32-bit words:
		for( SizeType i = 16; i < 80; i++ )
		{
			w[ i ] = LeftBitRotate( w[ i - 3 ] ^ w[ i - 8 ] ^ w[ i - 14 ] ^ w[ i - 16 ], 1 );
		}

		// Initialize the hash values for this chunk.
		Uint32 a = m_States[ 0 ];
		Uint32 b = m_States[ 1 ];
		Uint32 c = m_States[ 2 ];
		Uint32 d = m_States[ 3 ];
		Uint32 e = m_States[ 4 ];

		// Main loop ( It's possibe tp optimize this by splitting up the loop into four different loops. )
		for( SizeType i = 0; i < 80; i++ )
		{
			Uint32 k = 0;
			Uint32 f = 0;
			if( i < 20 )
			{
				f = ( b & c ) | ( ( ~b ) & d );
				k = 0x5A827999;
			}
			else if( i < 40 )
			{
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if( i < 60 )
			{
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else
			{
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}

			Uint32 temp = LeftBitRotate( a, 5 ) + f + e + k + w[ i ];
			e = d;
			d = c;
			c = LeftBitRotate( b, 30 );
			b = a;
			a = temp;
		}
		
		// Ass this chunk's hash to result so far.
		m_States[ 0 ] += a;
		m_States[ 1 ] += b;
		m_States[ 2 ] += c;
		m_States[ 3 ] += d;
		m_States[ 4 ] += e;
	}
	
}