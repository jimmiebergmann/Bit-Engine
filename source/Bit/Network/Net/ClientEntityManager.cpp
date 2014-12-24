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

#include <Bit/Network/Net/ClientEntityManager.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		ClientEntityManager::ClientEntityManager( ) :
			m_CurrentId( 0 )
		{
		}

		ClientEntityManager::~ClientEntityManager( )
		{
			for( EntityMap::iterator it =  m_Entities.begin( ); it !=  m_Entities.end( ); it++ )
			{
				delete it->second->pEntity;
				delete it->second;
			}
			m_Entities.clear( );

			for( EntityMetaDataMap::iterator it =  m_EntityMetaDataMap.begin( ); it !=  m_EntityMetaDataMap.end( ); it++ )
			{
				delete it->second;
			}

			m_EntityMetaDataMap.clear( );
		}

		Entity * ClientEntityManager::CreateEntityByName( const std::string & p_Key, Uint16 & p_EntityId )
		{
			// Check if we have any creating function for the key value.
			EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( p_Key );
			if( it == m_EntityMetaDataMap.end( ) )
			{
				return NULL;
			}

			// Create the entity
			Entity * pEntity = it->second->CreationPointer( );

			// Create entity link
			EntityLink * pEntityLink = new EntityLink;
			pEntityLink->Class = p_Key;
			pEntityLink->pEntity = pEntity;
	
			// Add the entity to the map
			p_EntityId = m_CurrentId;
			m_Entities[ m_CurrentId ] = pEntityLink;
			m_CurrentId++;
			return pEntity;
		}

		Entity * ClientEntityManager::GetEntity( const Uint16 p_EntityId )
		{
			EntityMap::iterator it = m_Entities.find( p_EntityId );
			if( it == m_Entities.end( ) )
			{
				return NULL;
			}

			return it->second->pEntity;
		}

		bool ClientEntityManager::ParseEntityMessage( void * p_pMessage, const SizeType p_MessageSize )
		{
			/*
				Message structure:
				- Entity count
					-Entity:
						- Block size
						- Name length
						- Name
						- Variable count
						- Variable:
							- Block Size
							- Name length
							- Name
							- ID count
							- Data size
							- Data:
								- ID
								- Data
								- ...
							- ...
						- ...
			*/


			// Error check the parameters
			if( p_pMessage == NULL || p_MessageSize < 14 )
			{
				return false;
			}

			// Get the message pointer as an unsigned char pointer
			Uint8 * pData = reinterpret_cast<Uint8 *>( p_pMessage );

			// Read entity count
			Uint16 entityCount = Ntoh16(	( static_cast<Uint16>(  pData[ 0 ] ) ) |
											( static_cast<Uint16>(  pData[ 1 ] ) << 8 ) );

			// Sture the current position in the message
			Uint16 dataPos = 2;

			// Go throguh the entities
			for( Uint16 i = 0; i < entityCount; i++ )
			{
				// Get the current entity block size
				Uint16 entityBlockSize = Ntoh16(	( static_cast<Uint16>(  pData[ dataPos ] ) ) |
													( static_cast<Uint16>(  pData[ dataPos + 1 ] ) << 8 ) );

				// Increase the data position.
				dataPos += 2;

				// Error check the block size, make sure there's enough data in the message.
				if( entityBlockSize == 0 || entityBlockSize + dataPos > p_MessageSize )
				{
					std::cout << "Entity manager: Entity block size error: \"" << entityBlockSize << "\"\n";
					return false;
				}

				// Read the entity name
				Uint8 entityNameLength = pData[ dataPos++ ];
				char * pEntityName = new char[ entityNameLength + 1 ];
				memcpy( pEntityName, pData + dataPos, entityNameLength );
				pEntityName[ entityNameLength ] = 0;

				// Ignore the entity if the entity name is unknown
				EntityMetaDataMap::iterator entityIt = m_EntityMetaDataMap.find( std::string( pEntityName ) );
				if( entityIt == m_EntityMetaDataMap.end( ) )
				{
					// Move to the next entity
					dataPos += entityBlockSize - 1 - entityNameLength;
					std::cout << "Entity manager: Unknown entity: \"" << pEntityName << "\"\n";
					continue;
				}

				// Move to the variable count
				dataPos += entityNameLength;

				// Read variable count
				Uint16 variableCount = Ntoh16(	( static_cast<Uint16>(  pData[ dataPos ] ) ) |
												( static_cast<Uint16>(  pData[ dataPos + 1 ] ) << 8 ) );

				// Increase the data position.
				dataPos += 2;

				// Go throguh the variables
				for( Uint16 j = 0; j < variableCount; j++ )
				{
					// Get the current entity block size
					Uint16 variableBlockSize = Ntoh16(	( static_cast<Uint16>(  pData[ dataPos ] ) ) |
														( static_cast<Uint16>(  pData[ dataPos + 1 ] ) << 8 ) );

					// Increase the data position.
					dataPos += 2;

					// Error check the block size, make sure there's enough data in the message.
					if( variableBlockSize == 0 || variableBlockSize + dataPos > p_MessageSize )
					{
						std::cout << "Entity manager: Variable block size error: \"" << variableBlockSize << "\"\n";
						return false;
					}

					// Read the variable name
					Uint8 variableNameLength = pData[ dataPos++ ];
					char * pVariableName = new char[ variableNameLength + 1 ];
					memcpy( pVariableName, pData + dataPos, variableNameLength );
					pVariableName[ variableNameLength ] = 0;

					// Ignore the varaible if it's unknown
					EntityMetaData * pMetaData = entityIt->second;
					EntityVariableMap::iterator variableIt = pMetaData->EntityVariables.find( std::string( pVariableName ) );

					if( variableIt == pMetaData->EntityVariables.end( ) )
					{
						// Move to the next entity
						dataPos += variableBlockSize - 1 - variableNameLength;
						std::cout << "Entity manager: Unknown entity variable(" << pEntityName << "): \"" << pVariableName << "\"\n";
						continue;
					}

					// Move to the IDs
					dataPos += variableNameLength;

					// Read the ID count
					Uint16 IdCount = Ntoh16(	( static_cast<Uint16>(  pData[ dataPos ] ) ) |
												( static_cast<Uint16>(  pData[ dataPos + 1 ] ) << 8 ) );

					// Increase the data position.
					dataPos += 2;

					// Read the data size
					Uint8 dataSize = pData[ dataPos++ ];

					// Go throguh the IDs
					for( Uint16 k = 0; k < IdCount; k++ )
					{
						// Read the current ID
						Uint16 entityId = Ntoh16(	( static_cast<Uint16>(  pData[ dataPos ] ) ) |
													( static_cast<Uint16>(  pData[ dataPos + 1 ] ) << 8 ) );
						
						// Increase the data position.
						dataPos += 2;

						// Find the entity
						EntityMap::iterator entityIt = m_Entities.find( entityId );
						if( entityIt == m_Entities.end( ) )
						{
							std::cout << "Entity manager: Unknown id: \"" << entityId << "\"\n";
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
						EntityVariableMap::iterator it2 = pMetadata->EntityVariables.find( std::string( pVariableName ) );
						if( it2 == pMetadata->EntityVariables.end( ) )
						{
							return false;
						}

						// Get the entity
						Entity * pEntity = pEntityLink->pEntity;

						// Get the varaible pointer
						Variable<Uint8> Entity::* pVariable = reinterpret_cast<Variable<Uint8> Entity::*>( it2->second );

						// Get the value size
						SizeType valueSize = (pEntity->*pVariable).Size;

						// Make sure that the values size is the same as the data
						if( valueSize != dataSize )
						{
							std::cout << "Entity manager: Unknown size variable(" << pEntityName << ")(" << valueSize << "): \"" << dataSize << "\"\n";
							dataPos += dataSize;
							continue;
						}

						// Get the pointer to the value
						void * pValuePointer = reinterpret_cast<void *>( &((pEntity->*pVariable).m_Value) );

						// Copy the data to the value
						memcpy( pValuePointer, &(pData[dataPos]), valueSize );

						// Move to the next Id
						dataPos += dataSize;

					}

					// Delete the varaible name pointer
					delete [ ] pVariableName;
				}

				// Delete the entitiy name pointer
				delete [ ] pEntityName;

			}


			// Succeeded
			return true;
		}

	}

}