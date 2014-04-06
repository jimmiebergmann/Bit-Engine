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

#include <Bit/Network/Ntp.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Ntp::Header::Header( ) :
		m_Flags( 0 ), //  0x1b =  LI: No warning(0), VN: 3, Mode: client( 3 )
		m_Stratum( 0 ),
		m_PollInterval( 0 ),
		m_Precision( 0 ),
		m_RootDelay( 0 ),
		m_RootDispersion( 0 ),
		m_ReferenceClock( 0 ),
		m_ReferenceTimestamp( 0 ),
		m_OriginateTimestamp( 0 ),
		m_ReceiveTimestamp( 0 ),
		m_TransmitTimestamp( 0 )
	{
	}

	Ntp::Header::Header(	const eLeap p_LeapIndicator,
							const Uint8 p_Version,
							const eMode p_Mode,
							const Uint8 p_Stratum,
							const Uint8 p_PollInterval,
							const Uint8 p_Precision,
							const Uint32 p_RootDelay,
							const Uint32 p_RootDispersion,
							const Uint32 p_ReferenceClock,
							const NtpTimestamp p_ReferenceTimestamp,
							const NtpTimestamp p_OriginateTimestamp,
							const NtpTimestamp p_ReceiveTimestamp,
							const NtpTimestamp p_TransmitTimestamp ) :
		m_Flags( 0 ),
		m_Stratum( p_Stratum ),
		m_PollInterval( p_PollInterval ),
		m_Precision( p_Precision ),
		m_RootDelay( Hton32( p_RootDelay ) ),
		m_RootDispersion( Hton32( p_RootDispersion ) ),
		m_ReferenceClock( Hton32( p_ReferenceClock ) ),
		m_ReferenceTimestamp( Hton64( p_ReferenceTimestamp.GetTimestamp( ) ) ),
		m_OriginateTimestamp( Hton64( p_OriginateTimestamp.GetTimestamp( ) ) ),
		m_ReceiveTimestamp( Hton64( p_ReceiveTimestamp.GetTimestamp( ) ) ),
		m_TransmitTimestamp( Hton64( p_TransmitTimestamp.GetTimestamp( ) ) )
	{
		// Set the leap flag
		Uint8 leap = ( static_cast<Uint8>( p_LeapIndicator ) << 6 ) & 0xC0;
		m_Flags |= leap;

		// Set the version flag
		Uint8 version = ( static_cast<Uint8>( p_Version ) << 3 ) & 0x38;
		m_Flags |= version;
		
		// Set the mode flag
		Uint8 mode = ( static_cast<Uint8>( p_Mode ) ) & 0x07;
		m_Flags |= mode;
	}

	void Ntp::Header::SetLeapIndicator( const eLeap p_LeapIndicator )
	{
		Uint8 leap = ( static_cast<Uint8>( p_LeapIndicator ) << 6 ) & 0xC0;
		m_Flags &= 0x3F;
		m_Flags |= leap;
	}

	void Ntp::Header::SetVersion( const Uint8 p_Version )
	{
		Uint8 version = ( p_Version << 3 ) & 0x38;
		m_Flags &= 0xC7;
		m_Flags |= version;
	}

	void Ntp::Header::SetMode( const eMode p_Mode )
	{
		Uint8 mode = ( static_cast<Uint8>( p_Mode ) ) & 0x07;
		m_Flags &= 0xF8;
		m_Flags |= mode;
	}

	void Ntp::Header::SetStratum( const Uint8 p_Stratum )
	{
		m_Stratum = p_Stratum;
	}

	void Ntp::Header::SetPollInterval( const Uint8 p_PollInterval )
	{
		m_PollInterval = p_PollInterval;
	}

	void Ntp::Header::SetPrecision( const Uint8 p_Precision )
	{
		m_Precision = p_Precision;
	}

	void Ntp::Header::SetRootDelay( const Uint32 p_RootDelay )
	{
		m_RootDelay = Hton32( p_RootDelay );
	}

	void Ntp::Header::SetRootDispersion( const Uint32 p_RootDispersion )
	{
		m_RootDispersion = Hton32( p_RootDispersion );
	}

	void Ntp::Header::SetReferenceClock( const Uint32 p_ReferenceClock )
	{
		m_ReferenceClock = Hton32( p_ReferenceClock );
	}

	void Ntp::Header::SetReferenceTimestamp( const NtpTimestamp p_ReferenceTimestamp )
	{
		m_ReferenceTimestamp = Hton64( p_ReferenceTimestamp.GetTimestamp( ) );
	}

	void Ntp::Header::SetOriginateTimestamp( const NtpTimestamp p_OriginateTimestamp )
	{
		m_OriginateTimestamp = Hton64( p_OriginateTimestamp.GetTimestamp( ) );
	}

	void Ntp::Header::SetReceiveTimestamp( const NtpTimestamp p_ReceiveTimestamp )
	{
		m_ReceiveTimestamp = Hton64( p_ReceiveTimestamp.GetTimestamp( ) );
	}

	void Ntp::Header::SetTransmitTimestamp( const NtpTimestamp p_TransmitTimestamp )
	{
		m_TransmitTimestamp = Hton64( p_TransmitTimestamp.GetTimestamp( ) );
	}

	Ntp::Header::eLeap Ntp::Header::GetLeapIndicator( ) const
	{
		Uint8 leap = ( m_Flags >> 6 ) & 0x03;
		return static_cast<eLeap>( leap );
	}

	Uint8 Ntp::Header::GetVersion( ) const
	{
		Uint8 version = ( m_Flags >> 3 ) & 0x07;
		return version;
	}

	Ntp::Header::eMode Ntp::Header::GetMode( ) const
	{
		Uint8 mode = m_Flags & 0x07;
		return static_cast<eMode>( mode );
	}

	Uint8 Ntp::Header::GetStratum( ) const
	{
		return m_Stratum;
	}

	Uint8 Ntp::Header::GetPollInterval( ) const
	{
		return m_PollInterval;
	}

	Uint8 Ntp::Header::GetPrecision( ) const
	{
		return m_Precision;
	}

	Uint32 Ntp::Header::GetRootDelay( ) const
	{
		return Ntoh32( m_RootDelay );
	}

	Uint32 Ntp::Header::GetRootDispersion( ) const
	{
		return Ntoh32( m_RootDispersion );
	}

	Uint32 Ntp::Header::GetReferenceClock( ) const
	{
		return Ntoh32( m_ReferenceClock );
	}

	NtpTimestamp Ntp::Header::GetReferenceTimestamp( ) const
	{
		return NtpTimestamp( Ntoh64( m_ReferenceTimestamp ) );
	}

	NtpTimestamp Ntp::Header::GetOriginateTimestamp( ) const
	{
		return NtpTimestamp( Ntoh64( m_OriginateTimestamp ) );
	}

	NtpTimestamp Ntp::Header::GetReceiveTimestamp( ) const
	{
		return NtpTimestamp( Ntoh64( m_ReceiveTimestamp ) );
	}

	NtpTimestamp Ntp::Header::GetTransmitTimestamp( ) const
	{
		return NtpTimestamp( Ntoh64( m_TransmitTimestamp ) );
	}


	// Ntp constructor
	Ntp::Ntp( Uint16 p_SourcePort, const Uint16 p_DestinationPort )
	{
		Open( p_SourcePort, p_DestinationPort );
	}

	Bool Ntp::Open( Uint16 p_SourcePort, const Uint16 p_DestinationPort )
	{
		m_DestinationPort = p_DestinationPort;
		return m_Socket.Open( p_SourcePort );
	}

	void Ntp::Close( )
	{
		m_Socket.Close( );
	}

	void Ntp::SetBlocking( const Bool p_Blocking )
	{
		m_Socket.SetBlocking( p_Blocking );
	}

	void Ntp::SetDestinationPort( const Uint16 p_Port )
	{
		m_DestinationPort = p_Port;
	}

	Bool Ntp::SendRequest( const Header & p_Request, const Address & p_Address )
	{
		SizeType headerSize = sizeof( p_Request );
		return m_Socket.Send( static_cast<const void *>( &p_Request ), headerSize, p_Address, m_DestinationPort ) == headerSize;
	}
	
	Bool Ntp::ReceiveResponse( Header & p_Response, Address & p_Address )
	{
		SizeType headerSize = sizeof( p_Response );
		Uint16 port = 0;

		// Receive the message
		const Int32 size = m_Socket.Receive( static_cast<void *>( &p_Response ), headerSize, p_Address, port );

		// Return true if the size is ok.
		return size == headerSize;
	}


}