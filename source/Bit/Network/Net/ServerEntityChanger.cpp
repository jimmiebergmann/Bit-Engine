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

#include <Bit/Network/Net/ServerEntityChanger.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	
	namespace Net
	{

		ServerEntityChanger::ServerEntityChanger( EntityManager * p_pEntityManager ) :
			m_pEntityManager( p_pEntityManager )
		{
		}

		void ServerEntityChanger::OnVariableChange( Entity * p_pEntity, VariableBase * p_pVariableBase )
		{
			EntityManager::ChangedEntitiesMap * pChangedEntities = &m_pEntityManager->m_ChangedEntities;

			// Check if the entity already is changed at some pointer, if the entity
			EntityManager::ChangedEntitiesMap::iterator it = pChangedEntities->find( p_pEntity );
			EntityManager::ChangedVariablesSet * pChangedVariables = NULL;
			if( it == pChangedEntities->end( ) )
			{
				// Add the entity
				pChangedVariables = new EntityManager::ChangedVariablesSet;
				(*pChangedEntities)[ p_pEntity ] = pChangedVariables;
			}
			else
			{
				pChangedVariables = it->second;
			}

			// Insert the variable
			pChangedVariables->insert( p_pVariableBase );
			
			std::cout << "Changed server variable. Changed entity size: " << pChangedEntities->size( ) << "  " << pChangedVariables->size( ) << std::endl;
		}

	}

}