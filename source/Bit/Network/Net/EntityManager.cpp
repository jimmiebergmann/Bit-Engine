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


		Bool EntityManager::CreateEntityMessage( std::vector<Uint8> & p_Message )
		{
			// Check if any entities were changed
			if( m_ChangedEntities.size( ) == 0 )
			{
				return false;
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

			
			// Clear the message
			p_Message.clear( );

			// Add message type.
			p_Message.push_back( static_cast<Uint8>( eMessageType::EntityMessageType ) );

			// Add entity count
			Uint16 entityCount = Hton16( static_cast<Uint16>( m_ChangedEntities.size( ) ) );
			p_Message.push_back( static_cast<Uint8>( entityCount ) );
			p_Message.push_back( static_cast<Uint8>( entityCount >> 8 ) );

			// Go through the changed entities(name)
			for( ChangedEntitiesMap::iterator it = m_ChangedEntities.begin( );
				 it != m_ChangedEntities.end( );
				 it++ )
			{
				// Get the changed variables(name)
				ChangedVariablesMap * pChangedVariables = it->second;

				// Store the position for the block size, we need to set it later
				Uint16 eBlockPos = static_cast<Uint16>( p_Message.size( ) );

				// Add block size and entity name length and name
				p_Message.push_back( 0 );
				p_Message.push_back( 0 );

				p_Message.push_back( static_cast<Uint8>( it->first.size( ) ) );

				p_Message.reserve( p_Message.size( ) + it->first.size( ) );
				for( SizeType i = 0; i < it->first.size( ); i++ )
				{
					p_Message.push_back( static_cast<Uint8>( it->first[ i ] ) );
				}

				// Add varaible count
				Uint16 variableCount = Hton16( static_cast<Uint16>( it->second->size( ) ) );
				p_Message.push_back( static_cast<Uint8>( variableCount ) );
				p_Message.push_back( static_cast<Uint8>( variableCount >> 8 ) );

				// Go through the changed entity variables
				for( ChangedVariablesMap::iterator it2 = pChangedVariables->begin( );
					 it2 != pChangedVariables->end( );
					 it2++ )
				{
					ChangedEntityVariableMap * pChangedEntityVariables = it2->second;

					// Store the position for the block size, we need to set it later
					Uint16 vBlockPos = static_cast<Uint16>( p_Message.size( ) );

					// Add block size and variable name length and name
					p_Message.push_back( 0 );
					p_Message.push_back( 0 );

					p_Message.push_back( static_cast<Uint8>( it2->first.size( ) ) );

					p_Message.reserve( p_Message.size( ) + it2->first.size( ) );
					for( SizeType j = 0; j < it2->first.size( ); j++ )
					{
						p_Message.push_back( static_cast<Uint8>( it2->first[ j ] ) );
					}

					// Add id count
					Uint16 idCount = Hton16( static_cast<Uint16>( it2->second->size( ) ) );
					p_Message.push_back( static_cast<Uint8>( idCount ) );
					p_Message.push_back( static_cast<Uint8>( idCount >> 8 ) );

					// Add data size
					if( pChangedEntityVariables->begin( ) == pChangedEntityVariables->end( ) )
					{
						return false;
					}
					p_Message.push_back( static_cast<Uint8>( pChangedEntityVariables->begin( )->second->GetSize( ) ) );

					// Get the changed entity and variable
					for( ChangedEntityVariableMap::iterator it3 = pChangedEntityVariables->begin( );
						 it3 != pChangedEntityVariables->end( );
						 it3++ )
					{
						// Get the entity
						Entity * pEntity = it3->first;

						// Get the varaible
						VariableBase * pVariable = it3->second;
						
						// Add the entity id
						Uint16 entityId = Hton16( static_cast<Uint16>( pEntity->GetId( ) ) );
						p_Message.push_back( static_cast<Uint8>( entityId ) );
						p_Message.push_back( static_cast<Uint8>( entityId >> 8 ) );

						// Add the data
						Uint16 dataPos = static_cast<Uint16>( p_Message.size( ) );
						p_Message.resize( p_Message.size( ) + pVariable->GetSize( ) );

						// Create temporary variable, in order to access the data pointer
						Variable<Uint8> * pTempVar = reinterpret_cast<Variable<Uint8> *>( pVariable );
						void * pDataPointer = reinterpret_cast<void *>( &(pTempVar->m_Value ) );

						// copy the data
						memcpy( &(p_Message[ dataPos ] ), pDataPointer, pVariable->GetSize( ) );
					}

					// Set the variable block size
					Uint16 vBlockSize = Hton16( static_cast<Uint16>( p_Message.size( ) ) - vBlockPos - 2 );
					p_Message[ vBlockPos ] = static_cast<Uint8>( vBlockSize );
					p_Message[ vBlockPos + 1 ] = static_cast<Uint8>( vBlockSize >> 8 );

				}

				// Set the entity block size
				Uint16 eBlockSize = Hton16( static_cast<Uint16>( p_Message.size( ) ) - eBlockPos - 2 );
				p_Message[ eBlockPos ] = static_cast<Uint8>( eBlockSize );
				p_Message[ eBlockPos + 1 ] = static_cast<Uint8>( eBlockSize >> 8 );

			}
			
			// Succeeded
			return true;
		}

		Bool EntityManager::CreateFullEntityMessage(	std::vector<Uint8> & p_Message,
														const Bool p_ClearMessage )
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

			// Clear the message
			if( p_ClearMessage )
			{
				p_Message.clear( );
			}

			// Add message type.
			p_Message.push_back( static_cast<Uint8>( eMessageType::EntityMessageType ) );

			// Add entity count
			Uint16 entityCount = Hton16( static_cast<Uint16>( m_EntityMetaDataMap.size( ) ) );
			p_Message.push_back( static_cast<Uint8>( entityCount ) );
			p_Message.push_back( static_cast<Uint8>( entityCount >> 8 ) );

			///		std::cout << "Entity count: " << m_Entities.size( ) << std::endl;

			// Go through the entity meta data
			for(	EntityMetaDataMap::iterator it = m_EntityMetaDataMap.begin( );
					it != m_EntityMetaDataMap.end( );
					it++ )
			{
				// Store the position for the block size, we need to set it later
				Uint16 eBlockPos = static_cast<Uint16>( p_Message.size( ) );

				// Add block size and entity name length and name
				p_Message.push_back( 0 );
				p_Message.push_back( 0 );

				///		std::cout << "\tBlock size: " << 0 << std::endl;

				p_Message.push_back( static_cast<Uint8>( it->first.size( ) ) );

				///		std::cout << "\tEntity name length: " << it->first.size( ) << std::endl;

				p_Message.reserve( p_Message.size( ) + it->first.size( ) );
				for( SizeType i = 0; i < it->first.size( ); i++ )
				{
					p_Message.push_back( static_cast<Uint8>( it->first[ i ] ) );
				}

				///		std::cout << "\tEntity name: " << it->first << std::endl;


				// Get the current entity variable map
				EntityVariableMap & entityVariables = it->second->EntityVariables;

				// Add varaible count
				Uint16 variableCount = Hton16( static_cast<Uint16>( entityVariables.size( ) ) );
				p_Message.push_back( static_cast<Uint8>( variableCount ) );
				p_Message.push_back( static_cast<Uint8>( variableCount >> 8 ) );

				///		std::cout << "\tVariable count: " << entityVariables.size( ) << std::endl;

				// Go throguh the entities variables
				// Go through the changed entity variables
				for( EntityVariableMap::iterator it2 = entityVariables.begin( );
					 it2 != entityVariables.end( );
					 it2++ )
				{
					// Get the varaible pointer
					Variable<Uint8> Entity::* pVariable = reinterpret_cast<Variable<Uint8> Entity::*>( it2->second );

					// Store the position for the block size, we need to set it later
					Uint16 vBlockPos = static_cast<Uint16>( p_Message.size( ) );

					// Add block size and variable name length and name
					p_Message.push_back( 0 );
					p_Message.push_back( 0 );

					///		std::cout << "\t\tVariable block size: " << 0 << std::endl;

					p_Message.push_back( static_cast<Uint8>( it2->first.size( ) ) );

					///		std::cout << "\t\tVariable name length: " << it2->first.size( ) << std::endl;

					p_Message.reserve( p_Message.size( ) + it2->first.size( ) );
					for( SizeType j = 0; j < it2->first.size( ); j++ )
					{
						p_Message.push_back( static_cast<Uint8>( it2->first[ j ] ) );
					}

					///		std::cout << "\t\tVariable name: " << it2->first << std::endl;

					// Store the position for the block size, we need to set it later
					Uint16 idcBlockPos = static_cast<Uint16>( p_Message.size( ) );

					// Add id count
					p_Message.push_back( 0 );
					p_Message.push_back( 0 );
					
					///		std::cout << "\t\tEntity id count: " << 0 << std::endl;

					// Store the data size position
					Uint16 dataSizePos = static_cast<Uint16>( p_Message.size( ) );
					p_Message.push_back( 0 );
					Uint8 dataSize = 0;

					///		std::cout << "\t\tVariable data size: " << 0 << std::endl;

					// Find all entities with the same entity name
					Uint16 entityCount = 0;
					for(	EntityMap::iterator it3 = m_Entities.begin( );
							it3 != m_Entities.end( );
							it3++ )
					{
						// ignore wrong entities.
						if( it3->second->Class != it->first )
						{
							continue;
						}

	
						// increment the entitiy count of the same name
						entityCount++;

						// Get the entity
						Entity * pEntity = it3->second->pEntity;

						// Get the data size
						dataSize = static_cast<Uint8>( (pEntity->*pVariable).GetSize( ) );

						// Add the entity id
						Uint16 entityId = Hton16( static_cast<Uint16>( pEntity->GetId( ) ) );
						p_Message.push_back( static_cast<Uint8>( entityId ) );
						p_Message.push_back( static_cast<Uint8>( entityId >> 8 ) );

						///		std::cout << "\t\t\tId: " << pEntity->GetId( ) << std::endl;


						// Add the data
						Uint16 dataPos = static_cast<Uint16>( p_Message.size( ) );
						p_Message.resize( p_Message.size( ) + dataSize );

						// Create temporary variable, in order to access the data pointer
						void * pDataPointer = reinterpret_cast<void *>( &(pEntity->*pVariable).m_Value );

						// copy the data
						memcpy( &(p_Message[ dataPos ] ), pDataPointer, dataSize );

						///		std::cout << "\t\t\tData: " << (int)dataSize << std::endl;
						
					}

					// Set the data size
					p_Message[ dataSizePos ] = dataSize;

					// Set the id count
					Uint16 hEntityCount = Hton16( entityCount );
					p_Message[ idcBlockPos ] = static_cast<Uint8>( hEntityCount );
					p_Message[ idcBlockPos + 1 ] = static_cast<Uint8>( hEntityCount >> 8 );


					// Set the variable block size
					Uint16 vBlockSize = Hton16( static_cast<Uint16>( p_Message.size( ) ) - vBlockPos - 2 - 1 );
					p_Message[ vBlockPos ] = static_cast<Uint8>( vBlockSize );
					p_Message[ vBlockPos + 1 ] = static_cast<Uint8>( vBlockSize >> 8 );
					
				}

				// Set the entity block size
				Uint16 eBlockSize = Hton16( static_cast<Uint16>( p_Message.size( ) ) - eBlockPos - 2 - 1 );
				p_Message[ eBlockPos ] = static_cast<Uint8>( eBlockSize );
				p_Message[ eBlockPos + 1 ] = static_cast<Uint8>( eBlockSize >> 8 );
			}

			// Succeeded
			return true;
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