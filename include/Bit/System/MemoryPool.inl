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

// Pool item class.
template <typename T>
inline void MemoryPool<T>::Item::SetUsedSize(const SizeType p_UsedSize)
{
	m_UsedSize = p_UsedSize;
}

template <typename T>
inline MemoryPool<T> * MemoryPool<T>::Item::GetParent() const
{
	return m_pParent;
}

template <typename T>
inline T * MemoryPool<T>::Item::GetData() const
{
	return m_pData;
}

template <typename T>
inline SizeType MemoryPool<T>::Item::GetDataSize() const
{
	return m_pParent->m_ItemSize;
}

template <typename T>
inline SizeType MemoryPool<T>::Item::GetUsedSize() const
{
	return m_UsedSize;
}

template <typename T>
inline MemoryPool<T>::Item::Item(	MemoryPool<T> *	p_pParent,
									const SizeType & p_DataSize,
									const bool p_DestroyAtReturn) :
	m_pData( new T[p_DataSize] ),
	m_UsedSize( 0 ),
	m_DestroyAtReturn( p_DestroyAtReturn ),
	m_pParent( p_pParent )
{
}

template <typename T>
inline MemoryPool<T>::Item::~Item()
{
	delete [] m_pData;
}

// Memory pool class
template <typename T>
inline MemoryPool<T>::MemoryPool(	const SizeType p_PoolSize,
									const SizeType p_ItemSize,
									const bool p_AllocateNewIfEmpty) :
	m_PoolSize(p_PoolSize),
	m_ItemSize(p_ItemSize),
	m_AllocateNewIfEmpty(p_AllocateNewIfEmpty),
	m_DeleteAtReturnCount( 0 )
{
	for (SizeType i = 0; i < m_PoolSize; i++)
	{
		m_Items.push(new Item(this, m_ItemSize));
	}
}

template <typename T>
inline MemoryPool<T>::~MemoryPool()
{
	while (!m_Items.empty())
	{
		delete m_Items.front();
		m_Items.pop();
	}
}

template <typename T>
typename inline MemoryPool<T>::Item * MemoryPool<T>::Get()
{
	if (m_Items.size() == 0)
	{
		if (m_AllocateNewIfEmpty)
		{
			return new Item(this, m_ItemSize, true);
		}
		else
		{
			return NULL;
		}
	}

	// Get the front item and pop it fron the pool.
	Item * pFront = m_Items.front();
	m_Items.pop();
	return pFront;
}

template <typename T>
inline bool MemoryPool<T>::Return(Item * p_pItem)
{
	// Validate the item.
	if (p_pItem == NULL || p_pItem->m_pParent != this)
	{
		return false;
	}

	// Should we delete this packet at return because of pool resize?
	if (m_DeleteAtReturnCount)
	{
		m_DeleteAtReturnCount--;
		delete p_pItem;
		return true;
	}

	// Check if we should destroy the item.
	if (p_pItem->m_DestroyAtReturn)
	{
		delete p_pItem;
		return true;
	}

	// Return the item to the pool
	p_pItem->m_UsedSize = 0;
	m_Items.push(p_pItem);

	// Succeeded.
	return true;
}

template <typename T>
inline SizeType MemoryPool<T>::GetItemCount() const
{
	return m_Items.size();
}

template <typename T>
inline void MemoryPool<T>::Add(const SizeType p_Count = 1)
{
	// Create new pool items and add them to the pool
	for (SizeType i = 0; i < p_Count; i++)
	{
		m_Items.push(new Item(this, m_ItemSize));
	}

	// Increment the pool size.
	m_PoolSize += p_Count;
}

template <typename T>
inline void MemoryPool<T>::SetPoolSize(const SizeType p_Size)
{
	// Add items.
	if (p_Size > m_PoolSize)
	{
		for (SizeType i = m_PoolSize; i < p_Size; i++)
		{
			m_Items.push(new Item<T>(this, m_ItemSize));
		}
	}
	// Remove items if needed.
	else if (pSize < m_PoolSize)
	{
		const SizeType leftToDelete = m_PoolSize - pSize;

		// Delete items
		while(leftToDelete)
		{
			// Break the loop if there are no more items to delete
			if (m_Item.size() == 0)
			{
				break;
			}

			// Decrement the left to delete counter
			leftToDelete--;
		}

		// Delete items at return if we need to delete more items.
		m_DeleteAtReturnCount += leftToDelete;
	}

	// Set pool size
	m_PoolSize = p_Size;
}

template <typename T>
inline SizeType MemoryPool<T>::GetPoolSize() const
{
	return m_PoolSize;
}