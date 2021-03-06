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

#include <Bit/Network/Net/Variable.hpp>
#include <Bit/Network/Net/Private/EntityChanger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		VariableBase::VariableBase( SizeType p_Size ) :
			m_Size( p_Size ),
			m_pParent( NULL ),
			m_LastSequence( 0 )
		{
		}

		SizeType VariableBase::GetSize( ) const
		{
			return m_Size;
		}

		const std::string & VariableBase::GetName( ) const
		{
			return m_Name;
		}

		Bool VariableBase::SetNewSequence(const Uint16 p_Sequence)
		{
			// Store the return value for later
			bool ret = false;

			m_Mutex.Lock();

			// Validate the new sequence.
			if (p_Sequence >= m_LastSequence ||
				(m_LastSequence > 49152 && p_Sequence < 16384))
			{
				m_LastSequence = p_Sequence;
				ret = true;
			}
			m_Mutex.Unlock();

			// Return the return value.
			return ret;
		}

		Bool VariableBase::IsNewValue()
		{
			return false;
		}

		void VariableBase::SetIsNewValue(const Bool p_Status)
		{
		}

	}

}