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

#ifndef BIT_NETWORK_NET_ENTITY_HPP
#define BIT_NETWORK_NET_ENTITY_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Net/Variable.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <string>
#include <set>

namespace Bit
{
	
	namespace Net
	{

		// Forward declaractions
		class BIT_API EntityManager;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Network entity base class.
		///
		////////////////////////////////////////////////////////////////
		class Entity
		{

		public:

			// Friend classes
			friend class EntityManager;
			template <typename T> friend class Variable;
			template <typename T> friend class InterpolatedVariable;

			// Public typdefs
			typedef std::set<Uint32> GroupSet;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			Entity( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			virtual ~Entity( );

			////////////////////////////////////////////////////////////////
			/// \brief Get entity id.
			///
			////////////////////////////////////////////////////////////////
			Uint16 GetId( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get entity name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName() const;

			////////////////////////////////////////////////////////////////
			/// \brief Add entity to group.
			///
			////////////////////////////////////////////////////////////////
			void AddToGroup(const Uint32 p_GroupIndex);

			////////////////////////////////////////////////////////////////
			/// \brief Remove entity from group.
			///
			////////////////////////////////////////////////////////////////
			void RemoveFromGroup(const Uint32 p_GroupIndex);

			////////////////////////////////////////////////////////////////
			/// \brief Att entity to group.
			///
			////////////////////////////////////////////////////////////////
			GroupSet GetGroups();

		private:

			// Private variables
			Uint16					m_Id;				///< Entity id.
			std::string				m_Name;				///< Entity name.
			ThreadValue<GroupSet>	m_Groups;			///< Set of entity groups for this entity.
			EntityManager *			m_pEntityManager;	///< Entity changer class.

		};

	}

}

#endif
