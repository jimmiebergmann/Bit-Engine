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

#ifndef BIT_NETWORK_NET_CLIENT_NETWORK_MANAGER_HPP
#define BIT_NETWORK_NET_CLIENT_NETWORK_MANAGER_HPP

#include <Bit/Build.hpp>
#include "Bit/Network/Net/Variable.hpp"
#include "Bit/Network/Net/Entity.hpp"
#include <string>
#include <map>
#include <vector>

namespace Bit
{
	
	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Client network manager class.
		///
		////////////////////////////////////////////////////////////////
		class ClientEntityManager
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			ClientEntityManager( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~ClientEntityManager( );

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
									Variable<Type> Class::* p_Pointer );

			////////////////////////////////////////////////////////////////
			/// \brief Set entity variable by variable name.
			///
			/// \param p_EntityIndex Index of the entitiy to update.
			/// \param p_Variable Key of the entity variable.
			/// \param p_Value The value to set.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			template<typename T>
			bool SetVariable(	unsigned int p_EntityIndex,
								const std::string & p_Variable,
								const T & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Create a new entity.
			///
			/// \param p_Key Name of the entity, or key value.
			///
			/// \return Pointer to the created entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			Entity * CreateEntityByName( const std::string & p_Key );

			////////////////////////////////////////////////////////////////
			/// \brief Get entity.
			///
			/// \param p_EntityIndex Index of the entity.(SHOULD BE ENTITY ID?!?!)
			///
			/// \return Pointer to the entity, NULL if error.
			///
			////////////////////////////////////////////////////////////////
			Entity * GetEntity( unsigned int p_EntityIndex );

		private:

			// Forward declarations
			struct EntityMetaData;
			struct EntityLink;
			
			// Private typedefs
			typedef std::map<std::string, VariableBase Entity::*>	EntityVariableMap;	///< Map of entity varibles
			typedef std::map<std::string, EntityMetaData*>			EntityMetaDataMap;	///< Map of pointers for creating entities
			typedef std::vector<EntityLink*>						EntityVector;		///< Vector of all entities

			////////////////////////////////////////////////////////////////
			/// \brief	Entity meta data structure. Holding the entity variables
			///			and pointer to the entitiy creating function.
			///
			////////////////////////////////////////////////////////////////
			struct EntityMetaData
			{
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
		
			// Private variable
			EntityMetaDataMap m_EntityMetaDataMap;	///< Map of entity class meta data.
			EntityVector m_Entities;				///< Vector of all entities.

		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/ClientEntityManager.inl>

	}

}

#endif