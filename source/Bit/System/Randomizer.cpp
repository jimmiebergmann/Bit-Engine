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


#include <Bit/System/Randomizer.hpp>
#include <Bit/System/MemoryLeak.hpp>

// Help functions
static BIT_UINT32 GetStringSeed( const std::string & p_Seed )
{
	BIT_UINT32 Seed = 0;
	for( BIT_MEMSIZE i = 0; i < p_Seed.size(); i++ )
	{
		Seed += p_Seed[i] * ( i + 1 );
	}

	return Seed;
}


static BIT_UINT32 s_RandomSeed = 0;

namespace Bit
{

	// Randomizer class
	// Constructors
	Randomizer::Randomizer( ) :
		m_Seed( 0 )
	{
	}

	Randomizer::Randomizer( const BIT_UINT32 p_Seed ) :
		m_Seed( p_Seed )
	{
	}

	Randomizer::Randomizer( const std::string & p_Seed ) :
		m_Seed( GetStringSeed( p_Seed ) )
	{
	}

	// Randomizer function
	BIT_SINT32 Randomizer::Randomize( const BIT_SINT32 p_To )
	{
		m_Seed = ( 214013 * m_Seed + 2531011 );
		return ( ( m_Seed >> 16 ) & 0x0000FFFF ) % ( p_To +1 );
	}

	BIT_SINT32 Randomizer::Randomize( const BIT_SINT32 p_From, const BIT_SINT32 p_To )
	{
		m_Seed = ( 214013 * m_Seed + 2531011 );
		return p_From + ( ( m_Seed >> 16 ) & 0x0000FFFF ) % ( p_To - ( p_From - 1 ) );
	}

	// Get functions
	BIT_UINT32 Randomizer::GetSeed( ) const
	{
		return m_Seed;
	}

	// Set functions
	void Randomizer::SetSeed( const BIT_SINT32 p_Seed )
	{
		m_Seed = p_Seed;
	}

	void Randomizer::SetSeed( const std::string & p_Seed )
	{
		m_Seed = GetStringSeed( p_Seed );
	}


	// Global randomizer functions
	BIT_API void SeedRandomizer( const BIT_SINT32 p_Seed )
	{
		s_RandomSeed = p_Seed;
	}

	BIT_API void SeedRandomizer( const std::string & p_Seed )
	{
		s_RandomSeed = GetStringSeed( p_Seed );
	}

	BIT_API BIT_UINT32 GetRandomizerSeed( )
	{
		return s_RandomSeed;
	}

	BIT_API BIT_SINT32 RandomizeNumber( const BIT_SINT32 p_To )
	{
		s_RandomSeed = ( 214013 * s_RandomSeed + 2531011 );
		return ( ( s_RandomSeed >> 16 ) & 0x0000FFFF ) % ( p_To +1 );
	}

	BIT_API BIT_SINT32 RandomizeNumber( const BIT_SINT32 p_From, const BIT_SINT32 p_To )
	{
		s_RandomSeed = ( 214013 * s_RandomSeed + 2531011 );
		return p_From + ( ( s_RandomSeed >> 16 ) & 0x0000FFFF ) % ( p_To - ( p_From - 1 ) );
	}

}
