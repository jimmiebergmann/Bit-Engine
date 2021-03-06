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

#ifndef BIT_NETWORK_NTP_HPP
#define BIT_NETWORK_NTP_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/NtpTimestamp.hpp>
#include <Bit/Network/UdpSocket.hpp>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief NTP protocol class
	///
	/// Documentation: http://www.eecis.udel.edu/~mills/database/rfc/rfc1305/rfc1305c.pdf
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Ntp
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief NTP header class
		///
		/// Internally stores all the bytes in network order.
		/// Sets and gets all the values in host order,
		/// if nothing else is specified in the documentation
		/// for easch function.
		///
		/// The current version of the NTP protocol is 4.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Header
		{

		public:

			// Friend classes
			friend class Ntp;

			////////////////////////////////////////////////////////////////
			/// Leap indicator enumerator.
			///
			////////////////////////////////////////////////////////////////
			enum eLeap
			{
				NoWarning,	///< No leap second warning
				Add,		///< Add one second to last minute of the day(61s).
				Remove,		///< Remove one second from last minute of the day(59s).
				Unknown		///< "Clock unsynchronized"?
			};

			////////////////////////////////////////////////////////////////
			/// Mode enumerator.
			///
			////////////////////////////////////////////////////////////////
			enum eMode
			{
				Reserved,			///< Not currently in use.
				SymmetricActive,	///< ?
				SymmetricPassive,	///< ?
				Client,				///< Client request.
				Server,				///< Server reply.
				Broadcast,			///< Broadcast message.
				ControlMessage,		///< Control message.
				Private				///< Reserved for private use.
			};

			////////////////////////////////////////////////////////////////
			/// Default constructor.
			///
			/// Initialize an empty NTP header.
			///
			////////////////////////////////////////////////////////////////
			Header( );

			////////////////////////////////////////////////////////////////
			/// Constructor
			///
			/// All parameters are in host order
			///	(The current system's way of ordering bytes).
			///
			////////////////////////////////////////////////////////////////
			Header( const eLeap p_LeapIndicator,
					const Uint8 p_Version,
					const eMode p_Mode,
					const Uint8 p_Stratum = 0,
					const Uint8 p_PollInterval = 0,
					const Uint8 p_Precision = 0,
					const Uint32 p_RootDelay = 0,
					const Uint32 p_RootDispersion = 0,
					const Uint32 p_ReferenceClock = 0,
					const NtpTimestamp p_ReferenceTimestamp = 0,
					const NtpTimestamp p_OriginateTimestamp = 0,
					const NtpTimestamp p_ReceiveTimestamp = 0,
					const NtpTimestamp p_TransmitTimestamp = 0 );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the leap indicator flag.
			///
			////////////////////////////////////////////////////////////////
			void SetLeapIndicator( const eLeap p_LeapIndicator );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the protocol version.
			///
			////////////////////////////////////////////////////////////////
			void SetVersion( const Uint8 p_Version );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the mode flag.
			///
			////////////////////////////////////////////////////////////////
			void SetMode( const eMode p_Mode );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the stratum level.
			///
			////////////////////////////////////////////////////////////////
			void SetStratum( const Uint8 p_Stratum );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the poll interval.
			///
			////////////////////////////////////////////////////////////////
			void SetPollInterval( const Uint8 p_PollInterval );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the clock precision.
			///
			////////////////////////////////////////////////////////////////
			void SetPrecision( const Uint8 p_Precision );

			////////////////////////////////////////////////////////////////
			/// \brief Sets root delay(total roundtrip time).
			///
			////////////////////////////////////////////////////////////////
			void SetRootDelay( const Uint32 p_RootDelay );

			////////////////////////////////////////////////////////////////
			/// \brief Sets root dispersion.
			///
			////////////////////////////////////////////////////////////////
			void SetRootDispersion( const Uint32 p_RootDispersion );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the reference clock.
			///
			////////////////////////////////////////////////////////////////
			void SetReferenceClock( const Uint32 p_ReferenceClock );

			////////////////////////////////////////////////////////////////
			/// \brief Sets reference timestamp.
			///
			////////////////////////////////////////////////////////////////
			void SetReferenceTimestamp( const NtpTimestamp p_ReferenceTimestamp );

			////////////////////////////////////////////////////////////////
			/// \brief Sets originate timestamp.
			///
			////////////////////////////////////////////////////////////////
			void SetOriginateTimestamp( const NtpTimestamp p_OriginateTimestamp );

			////////////////////////////////////////////////////////////////
			/// \brief Sets receive timestamp.
			///
			////////////////////////////////////////////////////////////////
			void SetReceiveTimestamp( const NtpTimestamp p_ReceiveTimestamp );

			////////////////////////////////////////////////////////////////
			/// \brief Sets transmit timestamp.
			///
			////////////////////////////////////////////////////////////////
			void SetTransmitTimestamp( const NtpTimestamp p_TransmitTimestamp );

			////////////////////////////////////////////////////////////////
			/// \brief Gets the leap indicator flag.
			///
			////////////////////////////////////////////////////////////////
			eLeap GetLeapIndicator( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the protocol version.
			///
			////////////////////////////////////////////////////////////////
			Uint8 GetVersion( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the mode flag.
			///
			////////////////////////////////////////////////////////////////
			eMode GetMode( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the stratum level
			///
			////////////////////////////////////////////////////////////////
			Uint8 GetStratum( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the poll interval.
			///
			////////////////////////////////////////////////////////////////
			Uint8 GetPollInterval( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Sets the clock precision.
			///
			////////////////////////////////////////////////////////////////
			Uint8 GetPrecision( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets root delay(total roundtrip time)
			///
			////////////////////////////////////////////////////////////////
			Uint32 GetRootDelay( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets root dispersion.
			///
			////////////////////////////////////////////////////////////////
			Uint32 GetRootDispersion( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the reference clock.
			///
			////////////////////////////////////////////////////////////////
			Uint32 GetReferenceClock( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets reference timestamp.
			///
			////////////////////////////////////////////////////////////////
			NtpTimestamp GetReferenceTimestamp( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets originate timestamp.
			///
			////////////////////////////////////////////////////////////////
			NtpTimestamp GetOriginateTimestamp( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets receive timestamp.
			///
			////////////////////////////////////////////////////////////////
			NtpTimestamp GetReceiveTimestamp( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets transmit timestamp.
			///
			////////////////////////////////////////////////////////////////
			NtpTimestamp GetTransmitTimestamp( ) const;

		private:

			Uint8 m_Flags;						///< LI(Leap Indicator), VN(Version Number), Mode.
			Uint8 m_Stratum;					///< Stratum level 0, 1 or 2-255.
			Uint8 m_PollInterval;				///< Poll interval.
			Uint8 m_Precision;					///< Time precision.
			Uint32 m_RootDelay;					///< Total roundtrip delay.
			Uint32 m_RootDispersion;			///< Maximum error.
			Uint32 m_ReferenceClock;			///< Reference of the clock, ip / string.
			Uint64 m_ReferenceTimestamp;		///< Client last time set.
			Uint64 m_OriginateTimestamp;		///< Client Response send time.
			Uint64 m_ReceiveTimestamp;			///< Host request receive time.
			Uint64 m_TransmitTimestamp;			///< Host Response send time.

		};

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		/// \param m_SourcePort The port is set to 0 by default,
		/// this will cause the socket to pick a random port for you.
		/// \param p_DestinationPort The destination port. 123 by default.
		///
		////////////////////////////////////////////////////////////////
		Ntp( Uint16 p_SourcePort = 0, const Uint16 p_DestinationPort = 123 );

		////////////////////////////////////////////////////////////////
		/// \brief Open the UDP socket.
		///
		/// \param m_SourcePort The port is set to 0 by default,
		/// this will cause the socket to pick a random port for you.
		/// \param p_DestinationPort The destination port. 123 by default.
		///
		////////////////////////////////////////////////////////////////
		Bool Open( Uint16 p_SourcePort = 0, const Uint16 p_DestinationPort = 123 );

		////////////////////////////////////////////////////////////////
		/// \brief Close the UDP socket.
		///
		////////////////////////////////////////////////////////////////
		void Close( );

		////////////////////////////////////////////////////////////////
		/// \brief Set UDP Socket blocking status.
		///
		////////////////////////////////////////////////////////////////
		void SetBlocking( const Bool p_Blocking );

		////////////////////////////////////////////////////////////////
		/// \brief Set destination port.
		///
		////////////////////////////////////////////////////////////////
		void SetDestinationPort( const Uint16 p_Port );

		////////////////////////////////////////////////////////////////
		/// \brief Send NTP time request.
		///
		/// \param p_Request The request header.
		/// \param p_Address The address of the receiver.
		///
		////////////////////////////////////////////////////////////////
		Bool SendRequest( const Header & p_Request, const Address & p_Address );

		////////////////////////////////////////////////////////////////
		/// \brief Receive NTP time responze.
		///
		/// \param p_Response The response header.
		/// \param p_Address The address of the sender.
		///
		////////////////////////////////////////////////////////////////
		Bool ReceiveResponse( Header & p_Response, Address & p_Address  );

	private:

		// Private variables
		UdpSocket m_Socket;			///< UDP socket.
		Uint16 m_DestinationPort;	///< Destination port( 123 by default )

	};

}

#endif