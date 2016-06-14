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
#include <Bit/Network/Net/Client.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		ClientEntityManager::ClientEntityManager(Client * p_pClient) :
			m_pClient(p_pClient)
		{
		}

		EntityManager::eType ClientEntityManager::GetType()
		{
			return EntityManager::ClientType;
		}

		void ClientEntityManager::OnVariableChange(Entity * p_pEntity, VariableBase * p_VariableBase)
		{

		}

		void ClientEntityManager::SetInterpolationTime(const Time & p_Time)
		{
			m_InterpolationTime = p_Time;
		}

		void ClientEntityManager::SetExtrapolationTime(const Time & p_Time)
		{
			m_ExtrapolationTime = p_Time;
		}

		void ClientEntityManager::TakeSnapshot(const Uint32 p_GroupId, const Bit::Time & p_Time)
		{
		}

		bool ClientEntityManager::ParseEntityMessage(void * p_pMessage, const SizeType p_MessageSize)
		{
			return false;
		}

	}

}