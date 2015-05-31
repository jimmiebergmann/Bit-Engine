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

template<typename T>
Variable<T>::Variable( ) :
	VariableBase( sizeof( T ) ),
	m_Value(static_cast<T>(0)),
	m_LastValue(static_cast<T>(0))
{
}

template<typename T>
Variable<T>::Variable( const T & p_Value ) :
	VariableBase(sizeof(T)),
	m_Value(p_Value).
	m_LastValue(p_Value)
{
}

template<typename T>
void Variable<T>::Set( const T & p_Value )
{
	m_Mutex.Lock( );
	/*if (m_IsSet)
	{
		m_LastValue = m_Value;
		m_Value = static_cast<T>(p_Value);
	}
	else*/
	{
		m_Value = static_cast<T>(p_Value);
		//m_LastValue = m_Value;
	}
	//m_IsSet = true;
	m_Mutex.Unlock( );

	// Call the on variable function for the entity changer
	if( m_pParent )
	{
		if( m_pParent->m_pEntityManager && 
			m_pParent->m_pEntityManager->m_pEntityChanger )
		{
			m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange( m_pParent, this );
		}
	}
};

template<typename T>
T Variable<T>::Get(const bool p_Interpolated)
{
	m_Mutex.Lock( );
	T value = m_Value;
	if (p_Interpolated && m_pParent && m_pParent->m_pEntityManager)
	{
		Float64 time = m_Timer.GetLapsedTime().AsSeconds();
		value += (m_Value - m_LastValue) * (time / m_pParent->m_pEntityManager->m_FrameTime.Get());
	}
	m_Mutex.Unlock( );
	return value;
};