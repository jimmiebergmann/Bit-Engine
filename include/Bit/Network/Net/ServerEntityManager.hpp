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

#ifndef BIT_NETWORK_NET_SERVER_ENTITY_MANAGER_HPP
#define BIT_NETWORK_NET_SERVER_ENTITY_MANAGER_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Server;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Server entity manager class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API ServerEntityManager : public EntityManager
		{

		public:

			///< Friend class declaration
			friend class Server;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			ServerEntityManager( Server * p_pServer );
			
			////////////////////////////////////////////////////////////////
			/// \brief Get the type of entity manager.
			///
			////////////////////////////////////////////////////////////////
			virtual eType GetType();

			////////////////////////////////////////////////////////////////
			/// \brief Function called by variable class when a variable is being changed.
			///
			////////////////////////////////////////////////////////////////
			virtual void OnVariableChange(Entity * p_pEntity, VariableBase * p_VariableBase);

			////////////////////////////////////////////////////////////////
			/// \brief Link entity to a certain key.
			///
			/// \param p_Key Name of the entity, or key value.
			///
			/// \return True if the key is valid and not already in use, else false.
			///
			////////////////////////////////////////////////////////////////
			template<typename T>
			bool LinkEntity( const std::string & p_Key ); 

			////////////////////////////////////////////////////////////////
			/// \brief Register entity variable.
			///
			/// \param p_Class Key of an already linked entity class.
			/// \param p_Class Key of the entity variable.
			/// \param p_Pointer Pointer to the variable.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			template < typename Type, typename Class>
			bool RegisterVariable(	const std::string & p_Class,
									const std::string & p_Variable,
									Variable<Type> Class::* p_pPointer);

			////////////////////////////////////////////////////////////////
			/// \brief Register entity interpolated variable.
			///
			/// \param p_Class Key of an already linked entity class.
			/// \param p_Class Key of the entity variable.
			/// \param p_Pointer Pointer to the interpolated variable.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			template < typename Type, typename Class>
			bool RegisterVariable(	const std::string & p_Class,
									const std::string & p_Variable,
									InterpolatedVariable<Type> Class::* p_pPointer);

			////////////////////////////////////////////////////////////////
			/// \brief Create a new entity.
			///		   Make sure to destroy the entity via the DestroyEntity function.
			///
			/// \param p_Key Name of the entity, or key value.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			/// \see DestroyEntity
			/// \see PublishEntity
			///
			////////////////////////////////////////////////////////////////
			Entity * CreateEntityByName(const std::string & p_Key);

			////////////////////////////////////////////////////////////////
			/// \brief Publish an entity after creation.
			///
			/// \param p_pEntity Pointer to the entity to publish.
			///
			/// \see CreateEntityByName
			/// \see DestroyEntity
			///
			////////////////////////////////////////////////////////////////
			void PublishEntity(Entity * p_pEntity);

			////////////////////////////////////////////////////////////////
			/// \brief Destroy entity.
			///
			/// \param p_pEntity	Pointer to the entity.
			/// \param p_Unallocate	Unallocate the memory.
			///
			////////////////////////////////////////////////////////////////
			void DestroyEntity( Entity * p_pEntity);

			////////////////////////////////////////////////////////////////
			/// \brief Get entity.
			///
			/// \param p_EntityId ID of the entity.
			///
			/// \return Pointer to the entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			Entity * GetEntity(const Uint16 p_EntityId);


		private:

			// Forward declarations
			struct EntityMetaData;
			struct EntityMessage;

			// Private typedefs
			typedef std::map<std::string, VariableBase Entity::*>	EntityVariableByNameMap;	///< Map of entity varibles, varaible name as key.
			typedef std::map<Uint8, VariableBase Entity::*>			EntityVariableByIdMap;		///< Map of entity varibles, varaible id as key.
			typedef std::map<std::string, EntityMetaData*>			EntityMetaDataMap;			///< Map of pointers for creating entities, entity name as key.
			typedef std::map<Uint16, Entity*>						EntityMap;					///< Map of all entities, entity id as key.
			typedef std::vector<EntityMessage>						EntityMessageVector;		///< Vector of entity messages.
			typedef std::queue<Uint16>								EntityIdQueue;				///< Queue of Entity Ids.

			////////////////////////////////////////////////////////////////
			/// \brief	Entity meta data structure. Holding the entity variables
			///			and pointer to the entitiy creating function.
			///
			////////////////////////////////////////////////////////////////
			struct EntityMetaData
			{
				Entity*(*CreationPointer)();				///< Pointer to function for creating entity.
				EntityVariableByNameMap VariablesByName;	///< Map of all the variables for this entity. By Name.
				EntityVariableByIdMap VariablesById;		///< Map of all the variables for this entity. By Id.
					
			};

			////////////////////////////////////////////////////////////////
			/// \brief	Entity message struct.
			///			Contains the data, size and the type of entity message.
			///
			////////////////////////////////////////////////////////////////
			struct EntityMessage
			{
				Uint16 Size;
				Uint8 * pData;
			};

			////////////////////////////////////////////////////////////////
			/// \brief	Create a entity message from all the changed entities.
			///
			////////////////////////////////////////////////////////////////
			Bool CreateEntityMessage(	const std::set<Uint32> & p_EntityGroups,
										EntityMessageVector & p_UnreliableMessages,
										EntityMessageVector & p_ReliableMessages);

			////////////////////////////////////////////////////////////////
			/// \brief	The the entity id queue of free ids, this function is called by the server.
			///
			////////////////////////////////////////////////////////////////
			Bool CreateEntityIdQueue(const Uint16 p_Size);

			// Private variables
			Server *					m_pServer;				///< Pointer to server.
			EntityMetaDataMap			m_EntityMetaDataMap;	///< Map of entity class meta data.
			EntityMap					m_Entities;				///< Map of all entities.
			Mutex						m_EntityMutex;			///< Mutex for entities.
			ThreadValue<EntityIdQueue>	m_FreeEntityIDs;		///< Queue of free entity IDs.


		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/ServerEntityManager.inl>

	}

}

#endif