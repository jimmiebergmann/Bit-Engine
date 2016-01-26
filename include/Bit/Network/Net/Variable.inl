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
	Bool isNewValue = m_Value != p_Value;
	m_Value = p_Value;
	m_Mutex.Unlock( );

	// Call the on variable function for the entity changer
	if (/*isNewValue &&*/
		m_pParent &&
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
void Variable<T>::TakeSnapshot(const Time & p_Time, const Time & p_InterpolationTime, const Time & p_ExtrapolationTime)
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
	m_Snapshot(static_cast<T>(0)),
	m_NewValue(true),
	m_IsSet(false),
	m_IsExtrapolating(false)
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
	m_Snapshot(static_cast<T>(p_Value)),
	m_NewValue(true),
	m_IsSet(false),
	m_IsExtrapolating(false)
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
		Bool isNewValue = m_LastValue != p_Value;
		m_LastValue = p_Value;
		m_NewValue = true;
		m_Mutex.Unlock();

		//if (isNewValue)
		{
			m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange(m_pParent, this);
		}
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
void InterpolatedVariable<T>::TakeSnapshot(const Time & p_Time, const Time & p_InterpolationTime, const Time & p_ExtrapolationTime)
{
	// Lock mutex.
	m_Mutex.Lock();

	// Is there 0 or 1 values?
	if (m_Values.size() <= 1)
	{
		m_Snapshot = m_LastValue;
		m_Mutex.Unlock();
		return;
	}

	// is there just 1 value?
	/*if (m_Values.size() == 1)
	{
		m_Snapshot = m_Values.front().m_Value;
		m_Mutex.Unlock();
		return;
	}*/

	// Check if the time is lower than the first item in the list.
	// Then, return the last value.
	if (p_Time < m_Values.front().m_Time)
	{
		ValueList::iterator itFirst = m_Values.begin();
		ValueList::iterator itSecond = std::next(itFirst);


		//std::cout << "Not yet: " << m_Values.front().m_Value.x << "    current time: " << p_Time.AsSeconds() << "   time1: " << (*itFirst).m_Time.AsSeconds() << "   time2: " << (*itSecond).m_Time.AsSeconds() << std::endl;
		m_Snapshot = m_Values.front().m_Value;
		m_Mutex.Unlock();
		return;
	}

	// Store the values to interpoalte between
	Value * pFirstValue = NULL;
	Value * pLastValue = NULL;

	// Check if the time is larger than the last item in the list.
	// Then, check if we should extrapolate the value.
	if (p_Time >= m_Values.back().m_Time )
	{
		if (m_Values.back().m_InitialFlag == false)
		{
			m_LastExtrapolationTime = p_Time;
			m_IsExtrapolating = true;
		}

	//	std::cout << "Extrapolate. " << std::endl;
		// have the time exceeded the extrapolation time?
		/*if (p_Time >= m_Values.back().m_Time + p_ExtrapolationTime)
		{

			// Set snapshot to max extrapolation time.

			m_Snapshot = m_Values.back().m_Value;
			m_Mutex.Unlock();
			return;
		}


		// Extrapolate here
		ValueList::iterator itLast = std::prev(m_Values.end( ));
		ValueList::iterator itButOne = std::prev(itLast);

		// Get values to itnerpolate between
		Value * pFirstValue = &(*itButOne);
		Value * pLastValue = &(*itLast);

		// Calculate interpolation factor.
		//Float32 extraFactor = p_ExtrapolationTime.AsSeconds() / p_InterpolationTime.AsSeconds();
		Float32 factor = ((p_Time - pFirstValue->m_Time).AsSeconds() / (pLastValue->m_Time - pFirstValue->m_Time).AsSeconds());

		// calculate the snapshot.
		m_Snapshot = pFirstValue->m_Value + ((pLastValue->m_Value - pFirstValue->m_Value) * factor);
		*/


		/*m_Snapshot = m_Values.back().m_Value;

		// Unlock mutex.
		m_Mutex.Unlock();
		return;*/


		ValueList::iterator itLast = std::prev(m_Values.end());
		ValueList::iterator itLastButOne = std::prev(itLast);

		pFirstValue = &(*itLastButOne);
		pLastValue = &(*itLast);

	}
	else
	{

		// The time is somewhere between the first and last value in the list.
		// We should find 2 values on each side of the current time, then interpolate.
		Bool foundValues = false;
		for (ValueList::iterator it = m_Values.begin(); it != std::prev(m_Values.end()); it++)
		{
			// Get the next item as well
			ValueList::iterator itNext = std::next(it);

			if (p_Time >= (*it).m_Time && p_Time < (*itNext).m_Time)
			{
				// Get values to interpoale between.
				pFirstValue = &(*it);
				pLastValue = &(*itNext);
				foundValues = true;
				break;
			}
		}

		// Check if we actually found any values.
		if (foundValues == false)
		{
			bitLogNetErr(  "Could not find any values." );
			m_Mutex.Unlock();
			return;
		}

	}


	// Calculate interpolation factor.
	Float32 factor = (p_Time - pFirstValue->m_Time).AsSeconds() / (pLastValue->m_Time - pFirstValue->m_Time).AsSeconds();

	
	// calculate the snapshot.
	m_Snapshot = pFirstValue->m_Value + ((pLastValue->m_Value - pFirstValue->m_Value) * factor);

	
	//std::cout << factor << std::endl;// "    " << pFirstValue->m_Value.x << "   " << pLastValue->m_Value.x << "       " << m_Snapshot.x << "    time1: " << p_Time.AsSeconds() << "   time2: " << m_Values.front().m_Time.AsSeconds() << std::endl;

	/*
	std::cout << "Values: " << m_Values.size() << std::endl;
	for (ValueList::iterator it = m_Values.begin(); it != m_Values.end(); it++)
	{
		std::cout << (*it).m_Value.x << std::endl;
	}

	std::cout << std::endl;*/


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
	
	// Set last value
	T oldLastValue;
	
	if (m_IsSet)
	{
		oldLastValue = m_LastValue;
		memcpy(&m_LastValue, p_pData, m_Size);
	}
	else
	{
		memcpy(&m_LastValue, p_pData, m_Size);
		oldLastValue = m_LastValue;
	}
	
	

	//std::cout << "old: " << oldLastValue.x << std::endl;


	// The value is set.
	m_IsSet = true;

	// No previcously added values?
	if (m_Values.size() == 0)
	{
		if (m_IsExtrapolating == false)
		{

			Value value;
			value.m_InitialFlag = false;
			value.m_Time = p_Time - Seconds(1.0f / 22.0f);
			value.m_Value = oldLastValue;
			m_Values.push_back(value);

			value.m_Time = p_Time;
			value.m_Value = m_LastValue;
			m_Values.push_back(value);
		}
		else
		{
			Value value;
			value.m_InitialFlag = false;
			value.m_Time = m_LastExtrapolationTime;
			value.m_Value = m_Snapshot;
			m_Values.push_back(value);

			value.m_Time = p_Time;
			value.m_Value = m_LastValue;
			m_Values.push_back(value);

			m_IsExtrapolating = false;
		}

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

/*	// Set the intial value, if any, to the current one.
	if (m_Values.size() && m_Values.front().m_InitialFlag)
	{
		Value & frontValue = m_Values.front();
		frontValue.m_Time = p_Time - Seconds(1.0f / 22.0f);
		frontValue.m_InitialFlag = false;
	}
	*/

	// Insert the data in the data queue.
	Value value;
	value.m_Time = p_Time;
	value.m_InitialFlag = false;
	value.m_Value = m_LastValue;

	// Check if this is a stop value( have we received this value before?)
	if (value.m_Value == m_Values.back().m_Value)
	{
		value.m_InitialFlag = true;
	}
	else
	{
		// Check if we should modify the back values time.
		Value & backValue = m_Values.back();
		if (backValue.m_InitialFlag)
		{
			backValue.m_InitialFlag = false;
			backValue.m_Time = p_Time - Seconds(1.0f / 22.0f);
		}
	}

	// Add the new value.
	m_Values.push_back(value);

	/*
	ValueList::iterator it = std::prev(std::prev(m_Values.end()));

	Float32 diff = (p_Time - (*it).m_Time).AsSeconds();

	std::cout << "diff: " << diff << std::endl;
	*/


	// Unlock mutex.
	m_Mutex.Unlock();
}


template<typename T>
Bool InterpolatedVariable<T>::IsNewValue()
{
	// Server only.
	m_Mutex.Lock();
	Bool status = m_NewValue;
	m_Mutex.Unlock();
	return status;
}


template<typename T>
void InterpolatedVariable<T>::SetIsNewValue(const Bool p_Status)
{
	// Server only.
	m_Mutex.Lock();
	m_NewValue = p_Status;
	m_Mutex.Unlock();
}

template<typename T>
void InterpolatedVariable<T>::ClearOldData(const Time & p_MinimumTime)
{
	// lock mutex
	m_Mutex.Lock();

	// Do always store at least 1.
	/*if (m_Values.size( ) <= 1)
	{
		m_Mutex.Unlock();
		return;
	}
	*/
	// Go through the items in the list
	for (ValueList::iterator it = m_Values.begin(); it != /*std::prev(*/m_Values.end()/*)*/;)
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

	/*if (m_Values.size() == 1)
	{
		// Get back value and check if we should change the value to the initial value.
		Value & frontValue = m_Values.front();
		if (frontValue.m_Time <= p_MinimumTime)
		{
			frontValue.m_Value = m_Snapshot;
			frontValue.m_InitialFlag = true;
		}
	}´*/

	// Unlock mutex.
	m_Mutex.Unlock();
}