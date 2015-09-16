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

#include <Bit/Network/Net/Entity.hpp>
#include <Bit/Network/Net/EntityManager.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		Entity::Entity( ) :
			m_pEntityManager( NULL )
		{
		}

		Uint16 Entity::GetId( ) const
		{
			return m_Id;
		}

		const std::string & Entity::GetName( ) const
		{
			return m_Name;
		}

		void Entity::AddToGroup(const Uint32 p_GroupIndex)
		{
			m_Groups.Mutex.Lock();
			m_Groups.Value.insert(p_GroupIndex);
			m_Groups.Mutex.Unlock();
		}

		void Entity::RemoveFromGroup(const Uint32 p_GroupIndex)
		{
			m_Groups.Mutex.Lock();
			m_Groups.Value.erase(p_GroupIndex);
			m_Groups.Mutex.Unlock();
		}

		Entity::GroupSet Entity::GetGroups()
		{
			SmartMutex mutex(m_Groups.Mutex);
			mutex.Lock();

			GroupSet groupSet(m_Groups.Value);

			return groupSet;
		}

		Entity::~Entity()
		{
		}

	}

}