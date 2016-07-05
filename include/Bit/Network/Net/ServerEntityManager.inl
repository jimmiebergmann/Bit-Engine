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
// Function for creating entity
template <typename T>
Entity * CreateEntityT()
{
	return new T;
}

// Function for linking entities
template<typename T>
bool ServerEntityManager::LinkEntity(const std::string & p_Key)
{
	// Make sure that it's not already added
	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find(p_Key);
	if (it != m_EntityMetaDataMap.end())
	{
		return false;
	}

	// Add to the entity meta data map
	EntityMetaData * pMetaData = new EntityMetaData;
	//pMetaData->TypeHash = typeid(T).hash_code();	// DEBUG??
	//pMetaData->RawName = typeid(T).raw_name();		// DEBUG??
	pMetaData->CreationPointer = &CreateEntityT<T>;
	m_EntityMetaDataMap.insert(std::pair<std::string, EntityMetaData*>(p_Key, pMetaData));

	// Succeeded.
	return true;
}


// Function for registering entity variables.
template < typename Type, typename Class>
bool ServerEntityManager::RegisterVariable(	const std::string & p_Class,
											const std::string & p_Variable,
											Variable<Type> Class::* p_pPointer)
{
	// Find the meta data and make sure it exists.
	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find(p_Class);
	if (it == m_EntityMetaDataMap.end())
	{
		return false;
	}

	// Get the pointer to the meta data
	EntityMetaData * pMetadata = it->second;


	// Add by name variable...
	// Find the variable in the meta data, make sure it's not already registerd
	EntityVariableByNameMap::iterator it2 = pMetadata->VariablesByName.find(p_Variable);
	if (it2 != pMetadata->VariablesByName.end())
	{
		return false;
	}

	// Get the pointer to the varaible
	VariableBase Entity::*pVariable = reinterpret_cast<VariableBase Entity::*>(p_pPointer);

	// Add the variable
	pMetadata->VariablesByName.insert(std::pair<std::string, VariableBase Entity::*>(p_Variable, pVariable));


	// Add by id variable...
	// Add the variable
	pMetadata->VariablesById.insert(std::pair<Uint8, VariableBase Entity::*>(pMetadata->VariablesById.size()+1, pVariable));

	// Succeeded
	return true;
}


// Function for registering entity variables.
template < typename Type, typename Class>
bool ServerEntityManager::RegisterVariable(	const std::string & p_Class,
											const std::string & p_Variable,
											InterpolatedVariable<Type> Class::* p_pPointer)
{
	// Find the meta data and make sure it exists.
	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find(p_Class);
	if (it == m_EntityMetaDataMap.end())
	{
		return false;
	}

	// Get the pointer to the meta data
	EntityMetaData * pMetadata = it->second;


	// Add by name variable...
	// Find the variable in the meta data, make sure it's not already registerd
	EntityVariableByNameMap::iterator it2 = pMetadata->VariablesByName.find(p_Variable);
	if (it2 != pMetadata->VariablesByName.end())
	{
		return false;
	}

	// Get the pointer to the varaible
	VariableBase Entity::*pVariable = reinterpret_cast<VariableBase Entity::*>(p_pPointer);

	// Add the variable
	pMetadata->VariablesByName.insert(std::pair<std::string, VariableBase Entity::*>(p_Variable, pVariable));


	// Add by id variable...
	// Add the variable
	pMetadata->VariablesById.insert(std::pair<Uint8, VariableBase Entity::*>(pMetadata->VariablesById.size() + 1, pVariable));

	// Succeeded
	return true;
}*/