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

#ifndef BIT_NETWORK_NET_RECIPIENT_FILTER_HPP
#define BIT_NETWORK_NET_RECIPIENT_FILTER_HPP

#include <Bit/Build.hpp>
#include <string>
#include <set>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Server;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief User recipient filter class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API UserRecipientFilter
		{

		public:

			friend class Server;
			friend class UserMessage;

			////////////////////////////////////////////////////////////////
			/// \brief Add user to receipient list
			///
			////////////////////////////////////////////////////////////////
			Bool AddUser( const Uint16 p_UserId );

			////////////////////////////////////////////////////////////////
			/// \brief Add all users to receipient list
			///
			////////////////////////////////////////////////////////////////
			Bool AddAllUsers( );

			////////////////////////////////////////////////////////////////
			/// \brief Add group to receipient list
			///
			////////////////////////////////////////////////////////////////
			Bool AddGroup( const std::string & p_GroupName );

			////////////////////////////////////////////////////////////////
			/// \brief Set reliable filter to true.
			///
			////////////////////////////////////////////////////////////////
			void MakeReliable( );

			////////////////////////////////////////////////////////////////
			/// \brief Set reliable filter to false.
			///
			////////////////////////////////////////////////////////////////
			void MakeUnreliable( );

			////////////////////////////////////////////////////////////////
			/// \brief Set reliable filter to false.
			///
			////////////////////////////////////////////////////////////////
			Bool IsReliable( ) const;

		private:

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			UserRecipientFilter( Server * p_pServer, const Bool p_Reliable = true );

			// Private typedefs
			typedef std::set<Uint16> UserSet;

			// Private variables.
			Server *	m_pServer;	///< Server pointer.
			UserSet		m_Users;	///< Set of all the users
			bool		m_Reliable;	///< Reliable flag.


		};

	}

}

#endif