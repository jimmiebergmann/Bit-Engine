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

#ifndef BIT_SYSTEM_RANDOMIZER_HPP
#define BIT_SYSTEM_RANDOMIZER_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Randimizer class
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Randomizer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Randomizer( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_Seed Seed value to initialize the randomizer with.
		///
		////////////////////////////////////////////////////////////////
		Randomizer( const UInt32 p_Seed );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		/// \param p_Seed Seed string to initialize the randomizer with.
		///
		////////////////////////////////////////////////////////////////
		Randomizer( const std::string & p_Seed );

		////////////////////////////////////////////////////////////////
		/// \brief Eandomizing a number.
		///
		/// Randomizing a value between 0 and p_To.
		///
		/// \param p_To The max value to be randomized.
		///
		/// \return A random number.
		///
		////////////////////////////////////////////////////////////////
		Int32 Randomize( const Int32 p_To );

		////////////////////////////////////////////////////////////////
		/// \brief Eandomizing a number.
		///
		/// Randomizing a value between p_From and p_To.
		///
		/// \param p_From The minimum value to be randomized.
		/// \param p_To The maximum value to be randomized.
		///
		/// \return A random number.
		///
		////////////////////////////////////////////////////////////////
		Int32 Randomize( const Int32 p_From, const Int32 p_To );

		////////////////////////////////////////////////////////////////
		/// \return The seed value.
		///
		////////////////////////////////////////////////////////////////
		UInt32 GetSeed( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Set the seed.
		///
		/// \param p_Seed The value to set the seed to. 
		///
		////////////////////////////////////////////////////////////////
		void SetSeed( const Int32 p_Seed );

		////////////////////////////////////////////////////////////////
		/// \brief Set the seed.
		///
		/// \param p_Seed The string to calculate the seed with.
		///
		////////////////////////////////////////////////////////////////
		void SetSeed( const std::string & p_Seed );

	private:

		
		////////////////////////////////////////////////////////////////
		// Private variable members
		////////////////////////////////////////////////////////////////
		UInt32 m_Seed; ///< The current seed.

	};

	////////////////////////////////////////////////////////////////
	// Global randomizer functions.
	////////////////////////////////////////////////////////////////
	BIT_API void SeedRandomizer( const Int32 p_Seed );
	BIT_API void SeedRandomizer( const std::string & p_Seed );
	BIT_API UInt32 GetRandomizerSeed( );
	BIT_API Int32 RandomizeNumber( const Int32 p_To );
	BIT_API Int32 RandomizeNumber( const Int32 p_From, const Int32 p_To );
	
}

#endif