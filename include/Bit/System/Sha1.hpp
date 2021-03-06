///////////////////////////////////////////////////////////////////////////
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

#ifndef BIT_SYSTEM_SHA1_HPP
#define BIT_SYSTEM_SHA1_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Hash.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Hash class.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API Sha1
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////////
		Sha1( );

		////////////////////////////////////////////////////////////////
		/// \brief Generates a hash sum from a byte array.
		/// 
		////////////////////////////////////////////////////////////////
		void Generate( const Uint8 * p_pData, const SizeType p_DataSize );

		////////////////////////////////////////////////////////////////
		/// \brief Generates a hash sum from a string.
		/// 
		////////////////////////////////////////////////////////////////
		void Generate(const std::string & p_String);

		////////////////////////////////////////////////////////////////
		/// \brief Generates a hash sum from a file
		/// 
		////////////////////////////////////////////////////////////////
		Bool GenerateFromFile(const std::string & p_Filename);

		////////////////////////////////////////////////////////////////
		/// \brief Get the hash value
		/// 
		////////////////////////////////////////////////////////////////
		Hash & GetHash( );

	private:

		// Private functions
		////////////////////////////////////////////////////////////////
		/// \brief Resets all the states to the sha1 constants
		/// 
		////////////////////////////////////////////////////////////////
		void ClearStates( );

		////////////////////////////////////////////////////////////////
		/// \brief Computes a new set of states from block data.
		/// 
		////////////////////////////////////////////////////////////////
		void ComputeBlock( const Uint8 * p_Data );

		// Private variables
		Uint32 m_States[ 5 ];	///< The hash key components.
		Hash m_Hash;			///< The calculated hash sum.

	};

}

#endif