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

#ifndef BIT_NETWORK_PACKET_HPP
#define BIT_NETWORK_PACKET_HPP

#include <Bit/Build.hpp>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief Packet class
	///
	/// Stores the data in network order.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Packet
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Packet( );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the packet data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get the packet size.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the read/write position
		///
		////////////////////////////////////////////////////////////////
		SizeType GetPosition( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the raw packet data
		///
		////////////////////////////////////////////////////////////////
		const void * GetRawData( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Set the read/write position
		///
		/// \param m_Position Position index.
		///
		////////////////////////////////////////////////////////////////
		void SetPosition( const SizeType m_Position ) const;

		// Write functions
		void WriteInt8( const Int8 p_Input );
		void WriteUint8( const Uint8 p_Input );
		void WriteInt16( const Int16 p_Input );
		void WriteUint16( const Uint16 p_Input );
		void WriteInt32( const Int32 p_Input );
		void WriteUint32( const Uint32 p_Input );
		void WriteInt64( const Int64 p_Input );
		void WriteUint64( const Uint64 p_Input );
		void WriteFloat32( const Float32 p_Input );
		void WriteFloat64( const Float64 p_Input );
		void WriteBool( const Bool p_Input );

		// Read functions
		Int8 ReadInt8( );
		Uint8 ReadUint8( );
		Int16 ReadInt16( );
		Uint16 ReadUint16( );
		Int32 ReadInt32( );
		Uint32 ReadUint32( );
		Int64 ReadInt64( );
		Uint64 ReadUint64( );
		Float32 ReadFloat32( );
		Float64 ReadFloat64( );
		Bool ReadBool( );

		// Write operator
		// ...

		// Read operator
		// ...
		
	private:

		// Private variables
		std::vector<Uint8> m_Data; ///< The raw packet data.

	};

}

#endif