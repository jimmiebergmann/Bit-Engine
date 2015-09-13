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
	m_Value(static_cast<T>(0))
{
	
}

template<typename T>
Variable<T>::Variable( const T & p_Value ) :
	VariableBase(sizeof(T)),
	m_Value( p_Value )
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
	m_Mutex.Lock( );
	T value = m_Value;
	m_Mutex.Unlock( );
	return value;
}

template<typename T>
void Variable<T>::SetData(const void * p_pData)
{
	m_Mutex.Lock();
	memcpy(&m_Value, p_pData, m_Size);
	m_Mutex.Unlock();
}

template<typename T>
void * Variable<T>::GetData()
{
	return reinterpret_cast<void *>( &m_Value );
}

// Interpolated variable class.
template<typename T>
InterpolatedVariable<T>::InterpolatedVariable() :
	VariableBase(sizeof(T))
{
	static_assert(	std::is_same<Float32, T>::value		||
					std::is_same<Vector2f32, T>::value	||
					std::is_same<Vector3f32, T>::value,
					"T must be Float32/Vector2f32/Vector3f32");
}


template<typename T>
InterpolatedVariable<T>::InterpolatedVariable(const T & p_Value) :
	VariableBase(sizeof(T))
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
	if (m_pParent &&
		m_pParent->m_pEntityManager &&
		m_pParent->m_pEntityManager->m_pEntityChanger)
	{
		// Set the variable.
		m_Mutex.Lock();
		m_LastValue = p_Value;
		m_Mutex.Unlock();

		// Call the on variable function for the entity changer
		m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange(m_pParent, this);
	}
}

template<typename T>
T InterpolatedVariable<T>::Get()
{
	// Server only
	if (m_pParent && m_pParent->m_pEntityManager->m_pEntityChanger)
	{
		m_Mutex.Lock();
		T value = m_LastValue;
		m_Mutex.Unlock();
		return value;
	}

	// Error check the snapshot count
	if (m_Snapshots.size() < 2)
	{
		return static_cast<T>(0);
	}

	// Get two snapshots to interpolate between.
	SnapshotList::iterator firstIt = m_Snapshots.end();
	InterpolatedVariable<T>::Snapshot * pFirst = NULL;
	InterpolatedVariable<T>::Snapshot * pLast = NULL;

	// Get the current time.
	Time clientTime = Microseconds(Timer::GetSystemTime());
	Time renderTime = clientTime - m_pParent->m_pEntityManager->m_InterpolationTime;

	// Find the first snapshot
	//int counter = 0;
	for (SnapshotList::iterator it = m_Snapshots.begin(); it != --m_Snapshots.end(); it++)
	{
		// Get the current snapshot
		InterpolatedVariable<T>::Snapshot * pSnapshot = &(*it);

		if (pSnapshot->m_Time > renderTime)
		{
			//std::cout << counter << "    " << (clientTime - m_pParent->m_pEntityManager->m_InterpolationTime).AsMilliseconds() << "   " << (pSnapshot->m_Time - m_pParent->m_pEntityManager->m_InterpolationTime).AsMilliseconds() << std::endl;
			break;
		}

		//counter++;

		firstIt = it;
		pFirst = pSnapshot;
		
	}

	// We found a first snapshot
	if (pFirst == NULL)
	{
		return static_cast<T>(0);
	}

	// Get the last snapshot
	firstIt++;
	pLast = &(*firstIt);
	
	// Calculate the lerp factor
	// pFirst->m_Time
	// pLast->m_Time
	// clientTime
	// renderTime
	Float32 factor = (renderTime - pFirst->m_Time).AsSeconds() / (pLast->m_Time - pFirst->m_Time).AsSeconds();

	//std::cout << factor << std::endl;


	//
	return pFirst->m_Value + ((pLast->m_Value - pFirst->m_Value) * factor);
}

template<typename T>
void InterpolatedVariable<T>::SetData(const void * p_pData)
{
	m_Mutex.Lock();

	// Create a new snapshot
	InterpolatedVariable<T>::Snapshot snapshot;
	snapshot.m_Time = Microseconds(Timer::GetSystemTime() );
	memcpy(&(snapshot.m_Value), p_pData, m_Size);

	// Add the snapshot to the new list
	m_Snapshots.push_back(snapshot);

	// Remove snapshots that are too old
	RemoveOldSnapshots();

	m_Mutex.Unlock();
}

template<typename T>
void * InterpolatedVariable<T>::GetData()
{
	return reinterpret_cast<void *>(&m_LastValue);
}

template<typename T>
void InterpolatedVariable<T>::RemoveOldSnapshots()
{
	Time currentTime = Microseconds(Timer::GetSystemTime() );
	//m_Time = (CurrentTime - m_StartTime);

	// Get the limit time.
	Time limitTime =	m_pParent->m_pEntityManager->m_InterpolationTime * 2;/* +
						m_pParent->m_pEntityManager->m_ExtrapolationTime;*/
	
	

	// Go throguh all the snapshots
	for (SnapshotList::iterator it = m_Snapshots.begin(); it != m_Snapshots.end();)
	{
		// Get the current snapshot
		InterpolatedVariable<T>::Snapshot * pSnapshot = &(*it);

		//std::cout << pSnapshot->m_Timer.GetLapsedTime().AsSeconds() << std::endl;


		//std::cout << m_Snapshots.size() << "   " << (limitTime - pSnapshot->m_Time).AsSeconds() << "    ";

		// Check if we should remove the object
		if (currentTime - pSnapshot->m_Time >= limitTime)
		{
			it = m_Snapshots.erase(it);
		}
		else
		{
			++it;
		}
	}
	//std::cout << std::endl;

	//std::cout << m_Snapshots.size()  << std::endl;

}