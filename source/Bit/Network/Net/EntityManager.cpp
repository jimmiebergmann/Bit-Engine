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

#include <Bit/Network/Net/EntityManager.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		EntityManager::EntityManager( EntityChanger * p_pEntityChanger ) :
			m_pEntityChanger( p_pEntityChanger ),
			m_CurrentId( 0 )
		{
		}

		EntityManager::~EntityManager( )
		{
			ClearEntityMessage( );

			for( EntityMap::iterator it =  m_Entities.begin( ); it !=  m_Entities.end( ); it++ )
			{
				delete it->second;
			}
			m_Entities.clear( );

			for( EntityMetaDataMap::iterator it =  m_EntityMetaDataMap.begin( ); it !=  m_EntityMetaDataMap.end( ); it++ )
			{
				delete it->second;
			}

			m_EntityMetaDataMap.clear( );

			// Destroy the enetity changer
			delete m_pEntityChanger;
		}

		Entity * EntityManager::CreateEntityByName( const std::string & p_Key )
		{
			// Check if we have any creating function for the key value.
			EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( p_Key );
			if( it == m_EntityMetaDataMap.end( ) )
			{
				return NULL;
			}

			// Create the entity
			Entity * pEntity = it->second->CreationPointer( );

			// Set id and entity changer
			pEntity->m_Id = m_CurrentId;
			pEntity->m_Name = p_Key;
			pEntity->m_pEntityManager = this;

			// Go through the variables and set the parent(entity)
			EntityMetaData * pMetadata = it->second;
			for( EntityVariableMap::iterator it2 = pMetadata->EntityVariables.begin( );
				 it2 != pMetadata->EntityVariables.end( );
				 it2++ )
			{
				// Get the current variable pointer
				VariableBase Entity::*pVariable = it2->second;
				
				// Set the parent of the variable to the entity.
				(pEntity->*pVariable).m_Name = it2->first;
				(pEntity->*pVariable).m_pParent = pEntity;
			}

			// Create entity link
			EntityLink * pEntityLink = new EntityLink;
			pEntityLink->Class = p_Key;
			pEntityLink->pEntity = pEntity;
	
			// Add the entity to the map
			m_Entities[ m_CurrentId ] = pEntityLink;
			m_CurrentId++;

			// Return the newly created entity
			return pEntity;
		}

		void EntityManager::DestroyEntity( Entity * p_pEntity, const Bool & p_Unallocate )
		{
			// Error check the entity pointer
			if( p_pEntity == NULL )
			{
				return;
			}

			// Get the entity link
			EntityMap::iterator linkIt = m_Entities.find( p_pEntity->GetId( ) );
			if( linkIt == m_Entities.end( ) )
			{
				return;
			}

			// Get the iterator for the entity map
			EntityMetaDataMap::iterator it = m_EntityMetaDataMap.find( p_pEntity->GetName( ) );
			if( it == m_EntityMetaDataMap.end( ) )
			{
				return;
			}

			// Go through the variables and remove the parent
			EntityMetaData * pMetadata = it->second;
			for( EntityVariableMap::iterator it2 = pMetadata->EntityVariables.begin( );
				 it2 != pMetadata->EntityVariables.end( );
				 it2++ )
			{
				// Get the current variable pointer
				VariableBase Entity::*pVariable = it2->second;
				(p_pEntity->*pVariable).m_pParent = NULL;
			}

			// Delete and remove the link
			delete linkIt->second;
			m_Entities.erase( linkIt );

			// Delete the pointer
			if( p_Unallocate )
			{
				delete p_pEntity;
			}
		}

		Entity * EntityManager::GetEntity( const Uint16 p_EntityId )
		{
			EntityMap::iterator it = m_Entities.find( p_EntityId );
			if( it == m_Entities.end( ) )
			{
				return NULL;
			}

			return it->second->pEntity;
		}

		bool EntityManager::ParseEntityMessage( void * p_pMessage, const SizeType p_MessageSize )
		{
			/*
				Message structure:
				- Entity count(2)
					-Entity:
						- Block size(2)
						- Name length(1)
						- Name(...)
						- Variable count(2)
						- Variable:
							- Block Size (2)
							- Name length (1)
							- Name (...)
							- ID count (2)
							- Data size (1)
							- Data:
								- ID	(2)
								- Data	(..)
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
						SizeType valueSize = (pEntity->*pVariable).m_Size;

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
						(pEntity->*pVariable).m_Mutex.Lock( );
						memcpy( pValuePointer, &(pData[dataPos]), valueSize );
						(pEntity->*pVariable).m_Mutex.Unlock( );

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


		void * EntityManager::CreateEntityMessage( SizeType & p_MessageSize )
		{
			// Check if any entities were changed
			if( m_ChangedEntities.size( ) == 0 )
			{
				p_MessageSize = 0;
				return NULL;
			}

			/*
				Message structure:
				- Entity count(2)
					-Entity:
						- Block size(2)
						- Name length(1)
						- Name(...)
						- Variable count(2)
						- Variable:
							- Block Size (2)
							- Name length (1)
							- Name (...)
							- ID count (2)
							- Data size (1)
							- Data:
								- ID	(2)
								- Data	(..)
								- ...
							- ...
						- ...
			*/

			
			// Create the message
			std::vector<Uint8> message;

			// Add message type.
			message.push_back( static_cast<Uint8>( eMessageType::EventMessageType ) );

			// Add entity count
			Uint16 entityCount = Hton16( static_cast<Uint16>( m_ChangedEntities.size( ) ) );
			message.push_back( static_cast<Uint8>( entityCount ) );
			message.push_back( static_cast<Uint8>( entityCount >> 8 ) );

			// Go through the changed entities(name)
			for( ChangedEntitiesMap::iterator it = m_ChangedEntities.begin( );
				 it != m_ChangedEntities.end( );
				 it++ )
			{
				// Get the changed variables(name)
				ChangedVariablesMap * pChangedVariables = it->second;

				// Add block size and entity name length and name
				message.push_back( 0 );
				message.push_back( 0 );

				message.push_back( static_cast<Uint8>( it->first.size( ) ) );

				message.reserve( message.size( ) + it->first.size( ) );
				for( SizeType i = 0; i < it->first.size( ); i++ )
				{
					message.push_back( static_cast<Uint8>( it->first[ i ] ) );
				}

				// Add varaible count
				Uint16 variableCount = Hton16( static_cast<Uint16>( it->second->size( ) ) );
				message.push_back( static_cast<Uint8>( variableCount ) );
				message.push_back( static_cast<Uint8>( variableCount >> 8 ) );

				// Go through the changed entities(name)
				for( ChangedVariablesMap::iterator it2 = pChangedVariables->begin( );
					 it2 != pChangedVariables->end( );
					 it2++ )
				{
					ChangedEntityVariableMap * pChangedEntityVariables = it2->second;

					// Add block size and variable name length and name
					message.push_back( 0 );
					message.push_back( 0 );

					message.push_back( static_cast<Uint8>( it2->first.size( ) ) );

					message.reserve( message.size( ) + it2->first.size( ) );
					for( SizeType j = 0; j < it2->first.size( ); j++ )
					{
						message.push_back( static_cast<Uint8>( it2->first[ j ] ) );
					}

					// Add id count
					Uint16 idCount = Hton16( static_cast<Uint16>( it2->second->size( ) ) );
					message.push_back( static_cast<Uint8>( idCount ) );
					message.push_back( static_cast<Uint8>( idCount >> 8 ) );

					// Add data size
					if( pChangedEntityVariables->begin( ) == pChangedEntityVariables->end( ) )
					{
						p_MessageSize = 0;
						return NULL;
					}
					message.push_back( static_cast<Uint8>( pChangedEntityVariables->begin( )->second->GetSize( ) ) );

					// Get the changed entity and variable
					for( ChangedEntityVariableMap::iterator it3 = pChangedEntityVariables->begin( );
						 it3 != pChangedEntityVariables->end( );
						 it3++ )
					{
						// Get the entity
						Entity * pEntity = it3->first;

						// Get the varaible
						VariableBase * pVariable = it3->second;


					}
				}
			}
			
			// Return the message pointer and set the message size
			p_MessageSize = static_cast<SizeType>( message.size( ) );
			return reinterpret_cast<void *>( message.data( ) );
		}

		void EntityManager::ClearEntityMessage( )
		{
			for( ChangedEntitiesMap::iterator it = m_ChangedEntities.begin( );
				 it != m_ChangedEntities.end( );
				 it++ )
			{

				for( ChangedVariablesMap::iterator it2 = it->second->begin( );
				 it2 != it->second->end( );
					 it2++ )
				{
					delete it2->second;
				}
				delete it->second;
			}

			m_ChangedEntities.clear( );
		}

	}

}