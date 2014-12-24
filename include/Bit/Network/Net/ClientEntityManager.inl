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
	VariableBase Entity::*pVariable = reinterpret_cast<VariableBase Entity::*>( p_Pointer );

	// Add the variable
	pMetadata->EntityVariables.insert( it2, std::pair<std::string, VariableBase Entity::*>( p_Variable, pVariable ) );

	// Succeeded
	return true;
}

// Function for setting entity variables
template<typename T>
bool ClientEntityManager::SetVariable( const Uint16 p_EntityId, const std::string & p_Variable, const T & p_Value )
{
	// Find the entity
	EntityMap::iterator entityIt = m_Entities.find( p_EntityId );
	if( entityIt == m_Entities.end( ) )
	{
		return false;
	}

	// Check if the vairable exists for the entitiy
	EntityLink * pEntityLink = entityIt->second;

	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( pEntityLink->Class );
	if( it == m_EntityMetaDataMap.end( ) )
	{
		return false;
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

template<typename T>
void * ClientEntityManager::CreateSingleEntityMessage(	const std::string & p_Class,
														const std::string & p_Variable,
														const Uint16 p_Id,
														const T & p_Data,
														SizeType & p_MessageSize )
{
	// Error check the parameters
	if( p_Class.size( ) == 0 || p_Variable.size( ) == 0 )
	{
		p_MessageSize = 0;
		return NULL;
	}

	// Create the message.
	SizeType messageSize =	15 + static_cast<SizeType>( p_Class.size( ) ) +
							static_cast<SizeType>( p_Variable.size( ) ) + sizeof( p_Data );
	Uint8 * pData = new Uint8[ messageSize ];
	SizeType dataPos = 0;

	// Pre calculated variables
	Uint16 entityCount = Hton16( 1 );
	Uint16 variableCount = Hton16( 1 );
	Uint16 IdCount = Hton16( 1 );
	Uint16 entityBlockSize = Hton16( 11 + p_Class.size( ) + p_Variable.size( ) + sizeof( p_Data ) );
	Uint16 variableBlockSize = Hton16( 6 + p_Variable.size( ) + sizeof( p_Data ) );
	
	
	// Entity count
	pData[ dataPos++ ] = 0;
	pData[ dataPos++ ] = 1;
	// Entity block size
	pData[ dataPos++ ] = static_cast<Uint8>( entityBlockSize );
	pData[ dataPos++ ] = static_cast<Uint8>( entityBlockSize >> 8 );

	// Entity name length
	pData[ dataPos++ ] = static_cast<Uint8>( p_Class.size( ) );

	// Entity name
	memcpy( &(pData[ dataPos ]), p_Class.data( ), p_Class.size( ) );
	dataPos += static_cast<SizeType>( p_Class.size( ) );

	// Varaible count
	pData[ dataPos++ ] = 0;
	pData[ dataPos++ ] = 1;

	// Variable block size
	pData[ dataPos++ ] = static_cast<Uint8>( variableBlockSize );
	pData[ dataPos++ ] = static_cast<Uint8>( variableBlockSize >> 8 );

	// Variable name length
	pData[ dataPos++ ] = static_cast<Uint8>( p_Variable.size( ) );

	// Entity name
	memcpy( &(pData[ dataPos ]), p_Variable.data( ), p_Variable.size( ) );
	dataPos += static_cast<SizeType>( p_Variable.size( ) );

	// Id count
	pData[ dataPos++ ] = 0;
	pData[ dataPos++ ] = 1;

	// Data size
	pData[ dataPos++ ] = sizeof( p_Data );

	// Id
	const Uint16 nId = Hton16( p_Id );
	pData[ dataPos++ ] = static_cast<Uint8>( nId );
	pData[ dataPos++ ] = static_cast<Uint8>( nId >> 8 );

	// Data
	memcpy( &(pData[ dataPos ] ), &p_Data, sizeof( p_Data ) );

	// Return the message pointer and set the message size
	p_MessageSize = messageSize;
	return reinterpret_cast<void *>( pData );
}