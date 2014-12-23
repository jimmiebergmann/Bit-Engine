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

// Function for creating entity
template <typename T>
Entity * CreateEntityT( )
{
	return new T;
}

// Function for linking entities
template<typename T>
bool ClientEntityManager::LinkEntity( const std::string & p_Key )
{
	// Make sure that it's not already added
	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( p_Key );
	if( it != m_EntityMetaDataMap.end( ) )
	{
		return false;
	}
	
	// Add to the entity meta data map
	EntityMetaData * pMetaData = new EntityMetaData;
	pMetaData->CreationPointer = &CreateEntityT<T>;
	m_EntityMetaDataMap.insert( it, std::pair<std::string, EntityMetaData*>( p_Key, pMetaData ) );

	// Succeeded.
	return true;
}


// Function for registering entity variables.
template < typename Type, typename Class>
bool ClientEntityManager::RegisterVariable( const std::string & p_Class,
											const std::string & p_Variable,
											Variable<Type> Class::* p_Pointer )
{
	// Find the meta data and make sure it's doesn't already exists.
	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( p_Class );
	if( it == m_EntityMetaDataMap.end( ) )
	{
		return false;
	}

	// Get the pointer to the meta data
	EntityMetaData * pMetadata = it->second;

	// Find the variable in the meta data, make sure it's not already registerd
	EntityVariableMap::iterator it2 = pMetadata->EntityVariables.find( p_Variable );
	if( it2 != pMetadata->EntityVariables.end( ) )
	{
		return false;
	}

	// Get the pointer to the varaible
	VariableBase Entity::*pVariable = reinterpret_cast<VariableBase Entity::*>( p_Pointer);

	// Add the variable
	pMetadata->EntityVariables.insert( it2, std::pair<std::string, VariableBase Entity::*>( p_Variable, pVariable ) );

	// Succeeded
	return true;
}

// Function for setting entity variables
template<typename T>
bool ClientEntityManager::SetVariable( unsigned int p_EntityIndex, const std::string & p_Variable, const T & p_Value )
{
	if( p_EntityIndex >= m_Entities.size( ) )
	{
		return false;
	}

	// Check if the vairable exists for the entitiy
	EntityLink * pEntityLink = m_Entities[ p_EntityIndex ];

	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( pEntityLink->Class );
	if( it == m_EntityMetaDataMap.end( ) )
	{
		false;
	}

	// Find the entitiy variable
	EntityMetaData * pMetadata = it->second;
	EntityVariableMap::iterator it2 = pMetadata->EntityVariables.find( p_Variable );
	if( it2 == pMetadata->EntityVariables.end( ) )
	{
		return false;
	}

	// Get the varaible pointer
	Variable<T> Entity::* pVariable = reinterpret_cast<Variable<T> Entity::*>( it2->second );

	// Get the entity
	Entity * pEntity = pEntityLink->pEntity;

	// Set the variable
	(pEntity->*pVariable).Set( p_Value );

	// Succeeded
	return true;
}