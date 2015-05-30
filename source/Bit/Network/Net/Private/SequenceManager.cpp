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

#include <Bit/Network/Net/Private/SequenceManager.hpp>
#include <stdio.h>
#include <string.h>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Net
	{

		// Acknowledgement data struct
		SequenceManager::SequenceManager()
		{
			memset(m_SequenceBits, 0, m_SequenceArraySize * 4);
			m_CurrentBlocks[0] = 0;
			m_CurrentBlocks[1] = 1;
		}

		Bool SequenceManager::AddSequence(const Uint16 p_Sequence)
		{
			const static Uint32 blockSize = 16384;
			const static Uint32 blockCount = 4;

			// Find the right array and bit index.
			Uint32 arrayIndex = static_cast<Uint32>(p_Sequence) / 32;
			Uint32 bitIndex = static_cast<Uint32>(p_Sequence) % 32;

			// Check if the bit already is set
			if ((m_SequenceBits[arrayIndex] >> bitIndex) & 0x01)
			{
				return false;
			}

			// Check the block bounding, if we are going to change blocks
			Uint8 sequenceBlock = static_cast<Uint8>(p_Sequence / blockSize);
			if (sequenceBlock != m_CurrentBlocks[0] &&
				sequenceBlock != m_CurrentBlocks[1] &&
				sequenceBlock != ((m_CurrentBlocks[1] + 2) % 4))
			{
				// Clear the last block.
				Uint32 * pClearSequence = m_SequenceBits + (512 * m_CurrentBlocks[0]);
				memset(pClearSequence, 0, m_SequenceArraySize);

				// Move the block index forward.
				m_CurrentBlocks[0] = m_CurrentBlocks[1];
				m_CurrentBlocks[1] = (m_CurrentBlocks[1] + 1) % 4;
			}

			// Set the current bit to 1.
			Uint32 bitMask = 1 << bitIndex;
			m_SequenceBits[arrayIndex] = m_SequenceBits[arrayIndex] | bitMask;
			return true;
		}


	}

}