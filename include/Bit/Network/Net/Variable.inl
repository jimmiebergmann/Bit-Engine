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
	if( m_pParent )
	{
		if( m_pParent->m_pEntityManager && 
			m_pParent->m_pEntityManager->m_pEntityChanger )
		{
			m_pParent->m_pEntityManager->m_pEntityChanger->OnVariableChange( m_pParent, this );
		}
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
	memcpy(&m_Value, p_pData, m_Size);
}


// Interpolated variable class.
/*
template<typename T>
InterpolatedVariable<T>::InterpolatedVariable() :
	VariableBase(sizeof(T))
{
}
*/
/*
template<>
InterpolatedVariable<Float32>::InterpolatedVariable() :
	VariableBase(sizeof(Float32))
{
}
*/

template<typename T>
InterpolatedVariable<T>::InterpolatedVariable() :
	VariableBase(sizeof(T))
{
	static_assert(	std::is_base_of<Float32, T>::value		||
					std::is_base_of<Vector2f32, T>::value	||
					std::is_base_of<Vector3f32, T>::value,
					"T must be Float32/Vector2f32/Vector3f32");
}


template<typename T>
InterpolatedVariable<T>::InterpolatedVariable(const T & p_Value) :
	VariableBase(sizeof(T))
{
	static_assert(	std::is_base_of<Float32, T>::value ||
					std::is_base_of<Vector2f32, T>::value ||
					std::is_base_of<Vector3f32, T>::value,
					"T must be Float32/Vector2f32/Vector3f32");
}

template<typename T>
void InterpolatedVariable<T>::Set(const T & p_Value)
{
}

template<typename T>
T InterpolatedVariable<T>::Get()
{
	return static_cast<T>(0);
}

template<typename T>
void InterpolatedVariable<T>::SetData(const void * p_pData)
{
}