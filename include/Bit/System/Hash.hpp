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

#ifndef BIT_SYSTEM_HASH_HPP
#define BIT_SYSTEM_HASH_HPP

#include <Bit/Build.hpp>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Hash class.
	/// 
	////////////////////////////////////////////////////////////////
	class BIT_API Hash
	{

	public:

		// Public typedefs
		typedef std::vector<Uint8> ByteVector; ///< Vector of bytes

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////////
		Hash( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// Intiailizes a hash with the given string.
		/// Note that the string is not string of hexvalues,
		/// it's a string of raw bytes.
		///
		/// \param p_String String of the hash data.
		/// 
		////////////////////////////////////////////////////////////////
		Hash( const std::string & p_String );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// Intiailizes a hash with the given bytes.
		///
		/// \param p_pBytes Pointer to the bytes.
		/// \param p_Count The numbers of bytes to copy from the passed byte pointer.
		/// 
		////////////////////////////////////////////////////////////////
		Hash( const Uint8 * p_pBytes, const SizeType p_Count );

		////////////////////////////////////////////////////////////////
		/// \brief Set the hash value from a string
		///
		/// Note that the string is not string of hexvalues,
		/// it's a string of raw bytes.
		///
		/// Example input:
		/// 5gsu3oFgh4!5%dg
		/// 
		////////////////////////////////////////////////////////////////
		void Set( const std::string & p_String );

		////////////////////////////////////////////////////////////////
		/// \brief Set the hash value from an array of bytes
		///
		/// \param p_pBytes Pointer to the bytes.
		/// \param p_Count The numbers of bytes to copy from the passed byte pointer.
		/// 
		////////////////////////////////////////////////////////////////
		void Set( const Uint8 * p_pBytes, const SizeType p_Count );

		////////////////////////////////////////////////////////////////
		/// \brief Set the hash value from a hex string
		///
		/// Reads the input from left to right.
		/// The string's last character will be treated as a 4 bit value
		/// if the input strings size is uneven.
		///
		/// Example input:
		/// 6FEd28AaB27D
		/// 
		////////////////////////////////////////////////////////////////
		Bool SetHexString( const std::string & p_HexString );

		////////////////////////////////////////////////////////////////
		/// \brief Add a byte to the hash value.
		/// 
		////////////////////////////////////////////////////////////////
		void AddByte( const Uint8 p_Byte );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the hash value
		/// 
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the hash as a string
		/// 
		////////////////////////////////////////////////////////////////
		std::string AsString( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the hash as a hex string
		/// 
		////////////////////////////////////////////////////////////////
		std::string AsHexString( const bool p_Uppercase = true ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the hash as a URI encoded string
		/// 
		////////////////////////////////////////////////////////////////
		std::string AsUrlEncodedString( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the raw hash data.
		/// 
		////////////////////////////////////////////////////////////////
		const ByteVector & AsRaw( ) const;

	private:

		// Private variables
		ByteVector m_Bytes;		///< Vector of all the hash bytes

	};
	
}

#endif