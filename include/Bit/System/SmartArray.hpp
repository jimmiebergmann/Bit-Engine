// ///////////////////////////////////////////////////////////////////////////
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


#ifndef __BIT_SYSTEM_SMART_ARRAY_HPP__
#define __BIT_SYSTEM_SMART_ARRAY_HPP__

#include <Bit/DataTypes.hpp>

namespace Bit
{

	template <typename T>
	class SmartArray
	{

	public:

		// Construcotrs/destructor
		SmartArray( const BIT_MEMSIZE p_Size )
		{
			m_pPointer = new T[ p_Size ];
		}

		SmartArray( T * p_pPointer )
		{
			m_pPointer = p_pPointer;
		}

		~SmartArray( )
		{
			if( m_pPointer )
			{
				delete [ ] m_pPointer;
			}
		}

		// Public functions
		T * Get( ) const
		{
			return m_pPointer;
		}

		// Operators
		operator T * ( ) const
		{
			return m_pPointer;
		}

	private:

		// Private variables
		T * m_pPointer;

	};

}

#endif