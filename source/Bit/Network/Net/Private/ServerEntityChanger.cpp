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
/*
#include <Bit/Network/Net/Private/ServerEntityChanger.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
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
			// Error check the parameters.
			if( p_pEntity == NULL || p_pVariableBase == NULL )
			{
				return;
			}

			// Add changed variable
			//m_pEntityManager->AddChangedVariable(p_pEntity, p_pVariableBase);

			
			// Lock the entity manager mutex.
			SmartMutex mutex(m_pEntityManager->m_Mutex);
			mutex.Lock();

			EntityManager::ChangedEntitiesMap * pChangedEntities = &m_pEntityManager->m_ChangedEntities;

			// Check if the entity already is changed at some pointer, if the entity
			EntityManager::ChangedEntitiesMap::iterator it = pChangedEntities->find( p_pEntity->GetName( ) );
			EntityManager::ChangedVariablesMap * pChangedVariables = NULL;
			if( it == pChangedEntities->end( ) )
			{
				// Add the entity
				pChangedVariables = new EntityManager::ChangedVariablesMap;
				(*pChangedEntities)[ p_pEntity->GetName( ) ] = pChangedVariables;
			}
			else
			{
				pChangedVariables = it->second;
			}

			// Check if the variable already is changed
			EntityManager::ChangedVariablesMap::iterator it2 = pChangedVariables->find( p_pVariableBase->GetName( ) );
			EntityManager::ChangedEntityVariableMap * pChangedEntityVariables = NULL;
			if( it2 == pChangedVariables->end( ) )
			{
				// Add the entity
				pChangedEntityVariables = new EntityManager::ChangedEntityVariableMap;
				(*pChangedVariables)[ p_pVariableBase->GetName( ) ] = pChangedEntityVariables;
			}
			else
			{
				pChangedEntityVariables = it2->second;
			}

			// Insert the entity and variable if needed
			EntityManager::ChangedEntityVariableMap::iterator it3 = pChangedEntityVariables->find( p_pEntity );
			if( it3 == pChangedEntityVariables->end( ) )
			{
				(*pChangedEntityVariables)[ p_pEntity ] = p_pVariableBase;
			}

			// Unlock the mutex.
			mutex.Unlock();
			

		}


	}

}

*/