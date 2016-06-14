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

#ifndef BIT_NETWORK_NET_CLIENT_ENTITY_MANAGER_HPP
#define BIT_NETWORK_NET_CLIENT_ENTITY_MANAGER_HPP

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
		class Client;


		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Client entity manager class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API ClientEntityManager : public EntityManager
		{

		public:

			// Friend classes
			friend class Client;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			ClientEntityManager(Client * p_pClient);

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
			bool LinkEntity(const std::string & p_Key);

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
			/// \brief Take a snapshot for a specific entity group.
			///
			/// \param p_GroupId The group to take a snapshot of.
			/// \param p_Time The time of the snapshot data, this is for interpolated variables only.
			///
			////////////////////////////////////////////////////////////////
			void TakeSnapshot(const Uint32 p_GroupId, const Bit::Time & p_Time);

		private:

			// Forward declarations
			struct EntityMetaData;
			struct EntityVariable;

			// Private typedefs
			typedef std::map<std::string, VariableBase Entity::*>	EntityVariableByNameMap;	///< Map of entity varibles, varaible name as key.
			typedef std::map<Uint8, VariableBase Entity::*>			EntityVariableByIdMap;		///< Map of entity varibles, varaible id as key.
			typedef std::map<std::string, EntityMetaData*>			EntityMetaDataMap;			///< Map of pointers for creating entities, entity name as key.

			////////////////////////////////////////////////////////////////
			/// \brief	Entity meta data structure. Holding the entity variables
			///			and pointer to the entitiy creating function.
			///
			////////////////////////////////////////////////////////////////
			struct EntityMetaData
			{
				//size_t TypeHash;
				//std::string RawName;
				Entity*(*CreationPointer)();				///< Pointer to function for creating entity.
				EntityVariableByNameMap VariablesByName;	///< Map of all the variables for this entity. By Name.
				EntityVariableByIdMap VariablesById;		///< Map of all the variables for this entity. By Id.

			};

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief	Parse a entity message from the server.
			///			Applying all necessary changes and create/delete entiies if needed.
			///
			///	\param p_pMessage Pointer to the message.
			///	\param p_MessageSize Size of the message.
			///
			/// \return True if succeeded, else false.
			///
			////////////////////////////////////////////////////////////////
			bool ParseEntityMessage(void * p_pMessage, const SizeType p_MessageSize);


			// Private variables
			Client * m_pClient;			///< Pointer to client.
			EntityMetaDataMap			m_EntityMetaDataMap;	///< Map of entity class meta data.
			Time m_InterpolationTime;	///< Interpolation time(delay).
			Time m_ExtrapolationTime;	///< Extrapolation time( for how long we should extra interpolate).

		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/ClientEntityManager.inl>

	}

}

#endif