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

#ifndef BIT_NETWORK_NET_EVENT_DECODER_HPP
#define BIT_NETWORK_NET_EVENT_DECODER_HPP

#include <Bit/Build.hpp>
#include <string>

namespace Bit
{
	
	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Event decoder base class for client side.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API EventDecoder
		{

		public:

			// Friend classes
			friend class Client;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			EventDecoder( const std::string & p_Name );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~EventDecoder( );

			////////////////////////////////////////////////////////////////
			/// \brief Get byte variable from event.
			///
			////////////////////////////////////////////////////////////////
			Uint8 GetByte( const std::string & p_VariableName );

			////////////////////////////////////////////////////////////////
			/// \brief Get signed integer variable from event.
			///
			////////////////////////////////////////////////////////////////
			Int32 GetInt( const std::string & p_VariableName );

			////////////////////////////////////////////////////////////////
			/// \brief Get floating point variable from event.
			///
			////////////////////////////////////////////////////////////////
			Float32 GetFloat( const std::string & p_VariableName );

			////////////////////////////////////////////////////////////////
			/// \brief Get string variable from event.
			///
			////////////////////////////////////////////////////////////////
			std::string GetString( const std::string & p_VariableName );

			////////////////////////////////////////////////////////////////
			/// \brief Get event name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

		private:

			// Private variables.
			std::string m_Name;			///< Message name.

		};

	}

}

#endif