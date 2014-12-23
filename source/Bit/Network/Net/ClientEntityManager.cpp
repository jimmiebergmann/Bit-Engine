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

namespace Bit
{

	namespace Net
	{

		ClientEntityManager::ClientEntityManager( )
		{
		}

		ClientEntityManager::~ClientEntityManager( )
		{
			for( int i = 0; i < m_Entities.size( ); i++ )
			{
				delete m_Entities[ i ]->pEntity;
				delete m_Entities[ i ];
			}
			m_Entities.clear( );

			for( EntityMetaDataMap::iterator it =  m_EntityMetaDataMap.begin( ); it !=  m_EntityMetaDataMap.end( ); it++ )
			{
				delete it->second;
			}

			m_EntityMetaDataMap.clear( );
		}

		Entity * ClientEntityManager::CreateEntityByName( const std::string & p_Key )
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
	
			m_Entities.push_back( pEntityLink );
			return pEntity;
		}

		Entity * ClientEntityManager::GetEntity( unsigned int p_EntityIndex )
		{
			if( p_EntityIndex >= m_Entities.size( ) )
			{
				return NULL;
			}

			return m_Entities[ p_EntityIndex ]->pEntity;
		}

	}

}