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

#include <Bit/Network/NtpTimestamp.hpp>
#include <ctime>


namespace Bit
{

	NtpTimestamp::NtpTimestamp( ) :
		m_Timestamp( 0ULL )
	{
	}

	NtpTimestamp::NtpTimestamp( const Bit::Uint64 p_Timestamp ) :
		m_Timestamp( p_Timestamp )
	{
	}

	void NtpTimestamp::SetTimestamp( const Uint64 p_Timestamp )
	{
		m_Timestamp = p_Timestamp;
	}

	void NtpTimestamp::SetSeconds( const Uint32 p_Seconds )
	{
		m_Timestamp &= 0x00000000FFFFFFFFULL;
		m_Timestamp |= static_cast<Uint64>( p_Seconds ) << 32ULL;
	}

	void NtpTimestamp::SetFractions( const Uint32 p_Fractions )
	{
		m_Timestamp &= 0xFFFFFFFF00000000ULL;
		m_Timestamp |= static_cast<Uint64>( p_Fractions );
	}

	Uint64 NtpTimestamp::GetTimestamp( ) const
	{
		return m_Timestamp;
	}

	Uint32 NtpTimestamp::GetSeconds( ) const
	{
		return static_cast<Uint32>( m_Timestamp >> 32ULL );
	}

	Uint32 NtpTimestamp::GetFractions( ) const
	{
		return static_cast<Uint32>( m_Timestamp );
	}

	std::string NtpTimestamp::GetTimeString( ) const
	{
		Uint32 seconds = GetSeconds( );
		if( seconds < 2208988800 )
		{
			seconds = 2208988800;
		}

		time_t rawtime;
		struct tm * timeinfo;

		// Get global time
		rawtime = seconds - 2208988800;
		timeinfo = gmtime( &rawtime );

		// Get the time cstring
		char * charTime = asctime(timeinfo);
		
		// Remove the newline
		SizeType stringSize = strlen( charTime );
		if( stringSize > 2 && charTime[ stringSize - 1 ] == '\n' )
		{
			charTime[ stringSize - 1 ] = 0;
		}

		// return the time cstring
		return charTime;
	}

	std::string NtpTimestamp::GetLocalTimeString( ) const
	{
		Uint32 seconds = GetSeconds( );
		if( seconds < 2208988800 )
		{
			seconds = 2208988800;
		}

		time_t rawtime;
		struct tm * timeinfo;

		// Get local time
		rawtime = seconds - 2208988800;
		timeinfo = localtime( &rawtime );

		// Get the time cstring
		char * charTime = asctime(timeinfo);
		
		// Remove the newline
		SizeType stringSize = strlen( charTime );
		if( stringSize > 2 && charTime[ stringSize - 1 ] == '\n' )
		{
			charTime[ stringSize - 1 ] = 0;
		}

		// return the time cstring
		return charTime;
	}
	
}
