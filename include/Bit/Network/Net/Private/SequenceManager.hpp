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

// Documentation about the packet structure, please:
// ... NIL


#ifndef BIT_NETWORK_NET_SEQUENCE_MANAGER_HPP
#define BIT_NETWORK_NET_SEQUENCE_MANAGER_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	namespace Net
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Sequence manager class.
		///
		////////////////////////////////////////////////////////////////
		class SequenceManager
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			SequenceManager();

			////////////////////////////////////////////////////////////////
			/// \brief Add the sequence.
			///
			/// \return False if the sequence already is added, else true.
			///
			////////////////////////////////////////////////////////////////
			Bool AddSequence(const Uint16 p_Sequence);

		private:

			// Private variables
			static const Uint32		m_SequenceArraySize = 2048;
			Uint32					m_SequenceBits[m_SequenceArraySize];
			Uint8					m_CurrentBlocks[2];

		};

	}

}

#endif