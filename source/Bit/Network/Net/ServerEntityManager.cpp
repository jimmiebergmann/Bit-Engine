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

#include <Bit/Network/Net/ServerEntityManager.hpp>
#include <Bit/Network/Net/Server.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	
	namespace Net
	{

		ServerEntityManager::ServerEntityManager(Server * p_pServer) :
			m_pServer(p_pServer)
		{
		}

		EntityManager::eType ServerEntityManager::GetType()
		{
			return EntityManager::ServerType;
		}

		void ServerEntityManager::OnVariableChange(Entity * p_pEntity, VariableBase * p_VariableBase)
		{

		}

		Entity * ServerEntityManager::CreateEntityByName(const std::string & p_Key)
		{
			// Create a smart mutex.
			SmartMutex mutex(m_EntityMutex);
			mutex.Lock();

			// Check if we have any creating function for the key value.
			EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find(p_Key);
			if (it == m_EntityMetaDataMap.end())
			{
				bitLogNetErr("Can not find any registred entity called \"" << p_Key << "\".");
				return NULL;
			}

			// Get the next id * LOCKING MUTEX.*
			m_FreeEntityIDs.Mutex.Lock();
			Uint16 nextId = m_FreeEntityIDs.Value.front();
			m_FreeEntityIDs.Value.pop();
			m_FreeEntityIDs.Mutex.Unlock();


			// Make sure that the id isn't already in use
			EntityMap::iterator it2 = m_Entities.find(nextId);
			if (it2 != m_Entities.end())
			{
				bitLogNetErr("Entity Id is already in use.");
				return NULL;
			}

			// Create the entity
			Entity * pEntity = it->second->CreationPointer();

			// Set id and entity changer
			pEntity->m_Id = nextId;
			pEntity->m_Name = p_Key;
			pEntity->m_pEntityManager = this;

			// Go through the variables and set the parent(entity)
			EntityMetaData * pMetadata = it->second;
			for (EntityVariableByNameMap::iterator it2 = pMetadata->VariablesByName.begin();
			it2 != pMetadata->VariablesByName.end();
				it2++)
			{
				// Get the current variable pointer
				VariableBase Entity::*pVariable = it2->second;

				// Set the parent of the variable to the entity.
				(pEntity->*pVariable).m_Name = it2->first;
				(pEntity->*pVariable).m_pParent = pEntity;
			}

			// Add the entity to the map
			m_Entities[nextId] = pEntity;

			// Return the newly created entity
			return pEntity;

		}

		void ServerEntityManager::PublishEntity(Entity * p_pEntity)
		{

		}

		void ServerEntityManager::DestroyEntity(Entity * p_pEntity)
		{

		}

		Entity * ServerEntityManager::GetEntity(const Uint16 p_EntityId)
		{
			return NULL;
		}

		Bool ServerEntityManager::CreateEntityMessage(	const std::set<Uint32> & p_EntityGroups, 
														EntityMessageVector & p_UnreliableMessages,
														EntityMessageVector & p_ReliableMessages)
		{
			return false;
		}

		Bool ServerEntityManager::CreateEntityIdQueue(const Uint16 p_Size)
		{
			SmartMutex mutex(m_FreeEntityIDs.Mutex);
			mutex.Lock();

			if (m_FreeEntityIDs.Value.size())
			{
				return false;
			}

			for (Uint16 i = 0; i < p_Size; i++)
			{
				m_FreeEntityIDs.Value.push(i);
			}

			return true;
		}

	}

}