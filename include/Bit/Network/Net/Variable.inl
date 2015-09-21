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
	VariableBase(sizeof(T)),
	m_Value(static_cast<T>(0)),
	m_Snapshot(static_cast<T>(0))
{
	
}

template<typename T>
Variable<T>::Variable( const T & p_Value ) :
	VariableBase(sizeof(T)),
	m_Value(p_Value),
	m_Snapshot(static_cast<T>(m_Value))
{
}

template<typename T>
void Variable<T>::Set( const T & p_Value )
{
	// Set the variable.
	m_Mutex.Lock( );
	m_Value = p_Value;
	m_Mutex.Unlock( );

	// Call the on variable function for the entity changer
	if (m_pParent &&
		m_pParent->m_pEntityManager &&
		m_pParent->m_pEntityManager->m_pEntityChanger)
	{
		m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange( m_pParent, this );
	}
}

template<typename T>
T Variable<T>::Get()
{
	m_Mutex.Lock();
	T value = m_Value;
	m_Mutex.Unlock();
	return value;
}

template<typename T>
T Variable<T>::GetSnapshot()
{
	m_Mutex.Lock();
	T value = m_Snapshot;
	m_Mutex.Unlock();
	return value;
}

template<typename T>
void Variable<T>::TakeSnapshot(const Time & p_Time)
{
	m_Mutex.Lock();
	m_Snapshot = m_Value;
	m_Mutex.Unlock();
}

template<typename T>
void * Variable<T>::GetData()
{
	return reinterpret_cast<void *>(&m_Value);
}

template<typename T>
void Variable<T>::SetData(const void * p_pData, const Time & p_Time, const Time & p_MinimumTime)
{
	m_Mutex.Lock();
	memcpy(&m_Value, p_pData, m_Size);
	m_Mutex.Unlock();
}







// Interpolated variable class.
template<typename T>
InterpolatedVariable<T>::InterpolatedVariable() :
	VariableBase(sizeof(T)),
	m_LastValue(static_cast<T>(0)),
	m_Snapshot(static_cast<T>(0))
{
	static_assert(	std::is_same<Float32, T>::value		||
					std::is_same<Vector2f32, T>::value	||
					std::is_same<Vector3f32, T>::value,
					"T must be Float32/Vector2f32/Vector3f32");
}


template<typename T>
InterpolatedVariable<T>::InterpolatedVariable(const T & p_Value) :
	VariableBase(sizeof(T)),
	m_LastValue(p_Value),
	m_Snapshot(static_cast<T>(p_Value))
{
	static_assert(	std::is_same<Float32, T>::value ||
					std::is_same<Vector2f32, T>::value ||
					std::is_same<Vector3f32, T>::value,
					"T must be Float32/Vector2f32/Vector3f32");
}

template<typename T>
void InterpolatedVariable<T>::Set(const T & p_Value)
{
	// Server only.
	// Call the on variable function for the entity changer
	if (m_pParent &&
		m_pParent->m_pEntityManager &&
		m_pParent->m_pEntityManager->m_pEntityChanger)
	{
		// Set the variable.
		m_Mutex.Lock();
		m_LastValue = p_Value;
		m_Mutex.Unlock();

		m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange(m_pParent, this);
	}
}

template<typename T>
T InterpolatedVariable<T>::Get()
{
	// Server only.
	m_Mutex.Lock();
	T value = m_LastValue;
	m_Mutex.Unlock();
	return value;
}

template<typename T>
T InterpolatedVariable<T>::GetSnapshot()
{
	// Client only.
	m_Mutex.Lock();
	T value = m_Snapshot;
	m_Mutex.Unlock();
	return value;
}

template<typename T>
void InterpolatedVariable<T>::TakeSnapshot(const Time & p_Time)
{
	// Lock mutex.
	m_Mutex.Lock();

	// Is there no values?
	if (m_Values.size() == 0)
	{
		m_Mutex.Unlock();
		return;
	}

	// is there just 1 value?
	if (m_Values.size() == 1)
	{
		m_Snapshot = m_Values.front().m_Value;
		m_Mutex.Unlock();
		return;
	}

	// Check if the time is lower than the first item in the list.
	// Then, return the last value.
	if (p_Time < m_Values.front().m_Time)
	{
		m_Snapshot = m_Values.front().m_Value;
		m_Mutex.Unlock();
		return;
	}

	// Check if the time is larger than the last item in the list.
	// Then, check if we should extrapolate the value.
	if (p_Time >= m_Values.back().m_Time)
	{
		//std::cout << "Overtime." << std::endl;
		m_Snapshot = m_Values.back().m_Value;
		m_Mutex.Unlock();
		return;
	}

	Value * pFirstValue = NULL;
	Value * pLastValue = NULL;
	Bool foundValues = false;

	// The time is somewhere between the first and last value in the list.
	// We should find 2 values on each side of the current time, then interpolate.
	for (ValueList::iterator it = m_Values.begin(); it != std::prev(m_Values.end()); it++)
	{
		// Get the next item as well
		ValueList::iterator itNext = std::next(it);
		
		if (p_Time >= (*it).m_Time && p_Time < (*itNext).m_Time)
		{
			pFirstValue = &(*it);
			pLastValue = &(*itNext);
			foundValues = true;
			break;
		}
	}

	// Check if we actually found any values.
	if (foundValues == false)
	{
		std::cout << "InterpolatedVariable<T>::TakeSnapshot: Could not find any values." << std::endl;
		m_Mutex.Unlock();
		return;
	}


	// Calculate interpolation factor.
	Float32 factor = (p_Time - pFirstValue->m_Time).AsSeconds() / (pLastValue->m_Time - pFirstValue->m_Time).AsSeconds();

	

	// calculate the snapshot.
	m_Snapshot = pFirstValue->m_Value + ((pLastValue->m_Value - pFirstValue->m_Value) * factor);

	//std::cout << factor << "   " << pFirstValue->m_Time.AsSeconds() << "   " << "   " << pLastValue->m_Time.AsSeconds() << "   " << p_Time.AsSeconds() << "     " << m_Snapshot.x << std::endl;


	// Unlock mutex.
	m_Mutex.Unlock();
}

template<typename T>
void * InterpolatedVariable<T>::GetData()
{
	return reinterpret_cast<void *>(&m_LastValue);
}

template<typename T>
void InterpolatedVariable<T>::SetData(const void * p_pData, const Time & p_Time, const Time & p_MinimumTime)
{
	// Clear old data.
	ClearOldData(p_MinimumTime);

	// This is for server only.
	m_Mutex.Lock();
	
	// No previcously added values?
	if (m_Values.size() == 0)
	{

		Value value;
		value.m_Time = p_Time - Seconds(1.0f / 22.0f);
		value.m_InitialFlag = false;
		memcpy(&(value.m_Value), p_pData, m_Size);
		m_Values.push_back(value);

		value.m_Time = p_Time;
		m_Values.push_back(value);

		m_Mutex.Unlock();
		return;
	}

	// Error check the time, make sure it's larger than the last value added.
	if (m_Values.size())
	{
		if (m_Values.back().m_Time >= p_Time)
		{
			m_Mutex.Unlock();
			return;
		}

	}

	// Set the intial value, if any, to the current one.
	if (m_Values.size() && m_Values.front().m_InitialFlag)
	{
		Value & frontValue = m_Values.front();
		//memcpy(&(frontValue.m_Value), p_pData, m_Size);
		frontValue.m_Time = p_Time - Seconds(1.0f / 22.0f);
		frontValue.m_InitialFlag = false;
	}

	// Insert the data in the data queue.
	Value value;
	value.m_Time = p_Time;
	value.m_InitialFlag = false;
	memcpy(&(value.m_Value), p_pData, m_Size);
	m_Values.push_back(value);

	// Unlock mutex.
	m_Mutex.Unlock();
}

template<typename T>
void InterpolatedVariable<T>::ClearOldData(const Time & p_MinimumTime)
{
	// lock mutex
	m_Mutex.Lock();

	// Do always store at least 1.
	if (m_Values.size( ) <= 1)
	{
		m_Mutex.Unlock();
		return;
	}

	// Go through the items in the list
	for (ValueList::iterator it = m_Values.begin(); it != std::prev(m_Values.end());)
	{
		if ((*it).m_Time <= p_MinimumTime )
		{
			it = m_Values.erase(it);
		}
		else
		{
			break;
		}
	}

	if (m_Values.size() == 1)
	{
		// Get back value and check if we should change the value to the initial value.
		Value & frontValue = m_Values.front();
		if (frontValue.m_Time <= p_MinimumTime)
		{
			frontValue.m_Value = m_Snapshot;
			frontValue.m_InitialFlag = true;
		}
	}

	// Unlock mutex.
	m_Mutex.Unlock();
}