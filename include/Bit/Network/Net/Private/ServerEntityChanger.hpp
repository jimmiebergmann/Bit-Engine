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

#ifndef BIT_NETWORK_NET_SERVER_ENTITY_CHANGER_HPP
#define BIT_NETWORK_NET_SERVER_ENTITY_CHANGER_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/Private/EntityChanger.hpp>

namespace Bit
{
	
	namespace Net
	{

		// Forward declaractions
		class EntityManager;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Server entity changer class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API ServerEntityChanger : public EntityChanger
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			ServerEntityChanger( EntityManager * p_pEntityManager );
			
			////////////////////////////////////////////////////////////////
			/// \brief This function is being called when a network variable is changed.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnVariableChange( Entity * p_pEntity, VariableBase * p_VariableBase );

		private:

			// Private variables
			EntityManager * m_pEntityManager;

		};

	}

}

#endif