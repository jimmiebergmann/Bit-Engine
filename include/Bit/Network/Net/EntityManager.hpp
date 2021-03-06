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

#ifndef BIT_NETWORK_NET_ENTITY_MANAGER_HPP
#define BIT_NETWORK_NET_ENTITY_MANAGER_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Socket.hpp>
#include <Bit/Network/Net/Variable.hpp>
#include <Bit/Network/Net/Entity.hpp>
#include <Bit/Network/Net/Private/NetPacket.hpp>
#include <Bit/System/Log.hpp>
#include <string>
#include <vector> 
#include <map>
#include <set>
#include <queue>
#include <typeinfo>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Client;
		class Server;
		class ServerEntityChanger;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Entity manager class.
		///
		/// Supports up to 65536 entities, 256 variables each.
		///
		/// There are 3 types of entity messages:
		///		1 - New entities.
		///		2 - Removed entities.
		///		3 - Updated entities.
		/// 
		/// Structure of each message type:
		///		1 - New entities:
		///			- Time(8)
		///			- Entity count(2)
		///			* Entity;
		///				- Name length(1)
		///				- Name(...)
		///				- Entity Id(2)
		///				- Variable count(1)
		///				* Variable:
		///					- Name length(1)
		///					- Name(...)
		///					- Variable Id(1)
		///					- Data size(1)
		///					- Data(..)
		///
		///		2 - Removed entities:
		///			- Time(8)
		///			- Entity count(2)
		///			* Entity:
		///				- Entity Id(2)
		///
		///		3 - Updated entitie:
		///			- Time(8)
		///			- variable count(1)
		///			* Variable:
		///				- Entity Id(2)
		///				- Variable Id(1)
		///				- Data size(1)
		///				- Data(..)
		///
		////////////////////////////////////////////////////////////////
		/*
		Message structure:
		- Time
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
		
		class BIT_API EntityManager
		{

		public:

			// Friend classes
			friend class ServerEntityChanger;
			friend class VariableBase;
			template<typename T> friend class Variable;
			template<typename T> friend class InterpolatedVariable;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			EntityManager(	EntityChanger * p_pEntityChanger,
							Server * p_pServer,
							Client * p_pClient);

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~EntityManager( );

			////////////////////////////////////////////////////////////////
			/// \brief Set interpolation time.
			///
			////////////////////////////////////////////////////////////////
			void SetInterpolationTime(const Time & p_Time);

			////////////////////////////////////////////////////////////////
			/// \brief Set extrapolation time.
			///
			////////////////////////////////////////////////////////////////
			void SetExtrapolationTime(const Time & p_Time);

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
			/// \brief Set entity variable by variable name.
			///
			/// \param p_EntityId ID of the entitiy to update.
			/// \param p_Variable Key of the entity variable.
			/// \param p_Value The value to set.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			template<typename T>
			bool SetVariable(	const Uint16 p_EntityId,
								const std::string & p_Variable,
								const T & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Take a snapshot for a specific entity group.
			///
			/// \param p_GroupId The group to take a snapshot of.
			/// \param p_Time The time of the snapshot data, this is for interpolated variables only.
			///
			////////////////////////////////////////////////////////////////
			void TakeSnapshot(const Uint32 p_GroupId, const Bit::Time & p_Time);

			////////////////////////////////////////////////////////////////
			/// \brief Create a new entity.
			///		   Make sure to destroy the entity via the DestroyEntity function.
			///
			/// \param p_Key Name of the entity, or key value.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			/// \see DestroyEntity
			///
			////////////////////////////////////////////////////////////////
			Entity * CreateEntityByName( const std::string & p_Key );

			////////////////////////////////////////////////////////////////
			/// \brief Destroy entity.
			///
			/// \param p_pEntity	Pointer to the entity.
			/// \param p_Unallocate	Unallocate the memory.
			///
			////////////////////////////////////////////////////////////////
			void DestroyEntity( Entity * p_pEntity, const Bool & p_Unallocate = true );

			////////////////////////////////////////////////////////////////
			/// \brief Get entity.
			///
			/// \param p_EntityId ID of the entity.
			///
			/// \return Pointer to the entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			Entity * GetEntity( const Uint16 p_EntityId );

			////////////////////////////////////////////////////////////////
			/// \brief	Parse a entity message from the server.
			///			Applying all necessary changes and create/delete entiies if needed.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			bool ParseEntityMessage( const Uint16 p_Sequence, void * p_pMessage, const SizeType p_MessageSize );

			////////////////////////////////////////////////////////////////
			/// \brief	Create a entity message for a single entity, varaible and Id.
			///
			/// \return Pointer to the message data.
			///
			////////////////////////////////////////////////////////////////
			template<typename T>
			void * CreateSingleEntityMessage(	const std::string & p_Class,
												const std::string & p_Variable,
												const Uint16 p_Id,
												const T & p_Data,
												SizeType & p_MessageSize );

			////////////////////////////////////////////////////////////////
			/// \brief	Create a entity message from all the changed entities.
			///
			////////////////////////////////////////////////////////////////
			Bool CreateEntityMessage(	std::vector<Uint8> & p_Message,
										const Bool p_ClearMessage = false );

			////////////////////////////////////////////////////////////////
			/// \brief	Create a entity message from all the entities.
			///
			////////////////////////////////////////////////////////////////
			Bool CreateFullEntityMessage(	std::vector<Uint8> & p_Message,
											const Bool p_ClearMessage = false );

			////////////////////////////////////////////////////////////////
			/// \brief	Clear the list of all changed entities.
			///
			////////////////////////////////////////////////////////////////
			void ClearChangedEntities();

		private:

			// Forward declarations
			struct EntityMetaData;
			struct EntityLink;
			
			// Private typedefs
			typedef std::map<std::string, VariableBase Entity::*>		EntityVariableMap;			///< Map of entity varibles, varaible name as key.
			typedef std::map<std::string, EntityMetaData*>				EntityMetaDataMap;			///< Map of pointers for creating entities, entity name as key.
			typedef std::map<Uint16, EntityLink*>						EntityMap;					///< Map of all entities, entity id as key.
			typedef std::map<Entity *, VariableBase *>					ChangedEntityVariableMap;	///< Mapping entities to variables
			typedef std::map<std::string, ChangedEntityVariableMap *>	ChangedVariablesMap;		///< Set of changed variables
			typedef std::map<std::string, ChangedVariablesMap *>		ChangedEntitiesMap;			///< Map of changed entities.
			typedef std::set<Entity *>									EntitySet;					///< Set of entities.
			
			////////////////////////////////////////////////////////////////
			/// \brief	Entity meta data structure. Holding the entity variables
			///			and pointer to the entitiy creating function.
			///
			////////////////////////////////////////////////////////////////
			struct EntityMetaData
			{

				size_t TypeHash;
				std::string RawName;
				Entity*(*CreationPointer)();		///< Pointer to function for creating entity.
				EntityVariableMap EntityVariables;	///< Map of all the variables for this entity.
			};

			////////////////////////////////////////////////////////////////
			/// \brief	Structure for connecting entity with it's name.
			///
			////////////////////////////////////////////////////////////////
			struct EntityLink
			{
				std::string Class;	///< Name of the entity's class.
				Entity * pEntity;	///< Pointer to the entity.
			};

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief This function is being called when a network variable is changed.
			///
			/// \param p_pEntity Pointer to entitiy
			/// \param p_VariableBase Pointer to variable base class.
			///
			////////////////////////////////////////////////////////////////
			void OnVariableChange(Entity * p_pEntity, VariableBase * p_VariableBase);

			////////////////////////////////////////////////////////////////
			/// \brief Create a new entity at the given id.
			///		   Make sure to destroy the entity by yourself.
			///
			/// \param p_Key Name of the entity, or key value.
			/// \param p_Id Id for the entity.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			Entity * CreateEntityAtId(const std::string & p_Key, const Bit::SizeType p_Id);

			////////////////////////////////////////////////////////////////
			/// \brief Delete all the entities in the deletion queue.
			///
			////////////////////////////////////////////////////////////////
			void DeleteEntitiesInDeletionQueue();
		
			// Private variable
			EntityChanger *			m_pEntityChanger;		///< Poiter to entity changer base class
			Client *				m_pClient;				///< Null for server owner.
			Server *				m_pServer;				///< Null for client owner.
			EntityMetaDataMap		m_EntityMetaDataMap;	///< Map of entity class meta data.
			ChangedEntitiesMap		m_ChangedEntities;		///< Map of changed entitites
			EntityMap				m_Entities;				///< Map of all entities.
			EntitySet				m_EntitiyDeletionQueue;	///< Queue or set of entities to be deleted.
			Uint32					m_CurrentId;			///< Temporary solution for incremening the ID.
			Time					m_InterpolationTime;	///< Interpolation time(delay).
			Time					m_ExtrapolationTime;	///< Extrapolation time( for how long we should extra interpolate).
			Mutex					m_Mutex;				///< Mutex for making sure we're not destroying entities at the same time we create entity messages.

			/// NEW!

			/*
				typedef std::map<std::string, VariableBase Entity::*>		EntityVariableMap;			///< Map of entity varibles, varaible name as key.
				typedef std::map<std::string, EntityMetaData*>				EntityMetaDataMap;			///< Map of pointers for creating entities, entity name as key.
				typedef std::map<Uint16, EntityLink*>						EntityMap;					///< Map of all entities, entity id as key.
				typedef std::map<Entity *, VariableBase *>					ChangedEntityVariableMap;	///< Mapping entities to variables
				typedef std::map<std::string, ChangedEntityVariableMap *>	ChangedVariablesMap;		///< Set of changed variables
				typedef std::map<std::string, ChangedVariablesMap *>		ChangedEntitiesMap;			///< Map of changed entities.
				typedef std::set<Entity *>									EntitySet;					///< Set of entities.
			*/

			struct ChangedVariableStruct
			{
				VariableBase Entity::* pVariableBase;
				Entity * pEntity;
			};

			//typedef std::map< std::pair<Uint16, Uint8>, ChangedVariableStruct> ChangeVariableEntityPairMap; ///< Pair of var and entity id to link them, struct.
			typedef std::map< Uint16, ChangedVariableStruct> ChangeVariableMap;						///< Changed Id, struct
			typedef std::map<Uint32, ChangeVariableMap *> ChangeVariablesGroupMap;							///< Group id, struct
			ChangeVariablesGroupMap m_ChangeVariablesGroups;


			/*
			
			
			c1 group 3

			m_ChangeVariablesGroups[3] finns? Inneh�ller data?

			Ta changed id och j�mf�r med c1 senaste connection:changedvar(std::pair<entityId, VariableId>) = 5

			Changed if f�r variabel �r 6, Skicka nytt.

			H�mta senaste ChangeVariableEntityPairMap.

			*/

		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/EntityManager.inl>

	}

}

#endif