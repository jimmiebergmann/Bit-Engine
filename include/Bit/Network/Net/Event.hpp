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

#ifndef BIT_NETWORK_NET_EVENT_HPP
#define BIT_NETWORK_NET_EVENT_HPP

#include <Bit/Build.hpp>
#include <string>
#include <map>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Server;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Event class for server side.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Event
		{

		public:

			// Friend classes
			friend class Server;

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Event( );

			////////////////////////////////////////////////////////////////
			/// \brief Fire the event
			///
			////////////////////////////////////////////////////////////////
			Bool FireEvent( );

			////////////////////////////////////////////////////////////////
			/// \brief Get event name
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Set byte variable.
			///
			////////////////////////////////////////////////////////////////
			void SetByte( const std::string & p_VariableName, const Uint8 p_Byte );

			////////////////////////////////////////////////////////////////
			/// \brief Set signed integer variable.
			///
			////////////////////////////////////////////////////////////////
			void SetInt( const std::string & p_VariableName, const Int32 p_Int );

			////////////////////////////////////////////////////////////////
			/// \brief Set floating point variable.
			///
			////////////////////////////////////////////////////////////////
			void SetFloat( const std::string & p_VariableName, const Float32 p_Float );

			////////////////////////////////////////////////////////////////
			/// \brief Set string variable.
			///
			////////////////////////////////////////////////////////////////
			void SetString( const std::string & p_VariableName, const std::string & p_String );

		private:

			// Typedefs
			typedef std::pair<Uint16, Uint8 *>			Variable;
			typedef std::map<std::string, Variable *>	VariableMap;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			Event( const std::string & p_Name, Server * p_pServer );

			////////////////////////////////////////////////////////////////
			/// \brief Get variable pointer, or create a new one if it doesn't extist.
			///
			////////////////////////////////////////////////////////////////
			Variable * GetVariable( const std::string & p_VariableName );

			// Private variables.
			std::string		m_Name;			///< Event name
			Server *		m_pServer;		///< Pointer to server class.
			VariableMap		m_Variables;	///< Map of varaibes.

		};

	}

}

#endif