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

#ifndef BIT_SYSTEM_MEMORY_POOL_HPP
#define BIT_SYSTEM_MEMORY_POOL_HPP

#include <Bit/Build.hpp>
#include <queue>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Memory pool class.
	///
	/// Please, do not resize the pool at runtime for better performance.
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class MemoryPool
	{

	public:


		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Memory pool item class.
		///
		////////////////////////////////////////////////////////////////
		class Item
		{

		public:

			// Friend classes
			friend class MemoryPool;

			////////////////////////////////////////////////////////////////
			/// \brief	Help function for letting
			///			the user store the ammount of used data in the item.
			///			Does not affect the data or size in any way.
			///
			////////////////////////////////////////////////////////////////
			void SetUsedSize(const SizeType p_UsedSize);

			////////////////////////////////////////////////////////////////
			/// \brief Get memory pool parent.
			///
			////////////////////////////////////////////////////////////////
			MemoryPool<T> * GetParent() const;

			////////////////////////////////////////////////////////////////
			/// \brief Get item data.
			///
			////////////////////////////////////////////////////////////////
			T * GetData() const;

			////////////////////////////////////////////////////////////////
			/// \brief Get item data size.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetDataSize() const;

			////////////////////////////////////////////////////////////////
			/// \brief Get used data.
			///
			/// \see SetUsedSize
			///
			////////////////////////////////////////////////////////////////
			SizeType GetUsedSize() const;

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			/// \param p_DataSize Size of the data to allocate.
			/// \param p_DestroyAtReturn Destroy the item when you return it if true.
			///
			////////////////////////////////////////////////////////////////
			Item(	MemoryPool<T> *	p_pParent,
					const SizeType & p_DataSize,
					const bool p_DestroyAtReturn = false );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			~Item();

			// Private variables.
			T *				m_pData;
			SizeType		m_UsedSize;
			bool			m_DestroyAtReturn;
			MemoryPool<T> *	m_pParent;

		};


		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		///	\param p_PoolSize Number of initial created and maximum number of pool items.
		/// \param p_ItemSize Size of each pool item. Not in bytes,
		///					  depending on the data type, int = 4 bytes for example.
		/// \param p_AllocateNewIfEmpty Allocate new item if the memory pool is empty,
		///								and remove them at return.
		///
		////////////////////////////////////////////////////////////////
		MemoryPool( const SizeType p_PoolSize,
					const SizeType p_ItemSize,
					const bool p_AllocateNewIfEmpty = true );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~MemoryPool();

		////////////////////////////////////////////////////////////////
		/// \brief Get pool item.
		///
		////////////////////////////////////////////////////////////////
		Item * Get();

		////////////////////////////////////////////////////////////////
		/// \brief Return item to the memory pool.
		///
		/// \return true if you could return the item to the pool.
		///
		////////////////////////////////////////////////////////////////
		bool Return(Item * p_pItem);

		////////////////////////////////////////////////////////////////
		/// \brief Get current item count in the memory pool.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetItemCount() const;

		////////////////////////////////////////////////////////////////
		/// \brief Add more items to the pool, increases the max item count.
		///
		////////////////////////////////////////////////////////////////
		void Add(const SizeType p_Count = 1);

		////////////////////////////////////////////////////////////////
		/// \brief	Set the pool size. Deallocating pool items at return if
		///			the new pool size is less than the current number of items,
		///			also deallocating items not in use if needed.
		///
		////////////////////////////////////////////////////////////////
		void SetPoolSize(const SizeType p_Size);

		////////////////////////////////////////////////////////////////
		/// \brief	Get max pool size.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetPoolSize() const;

	private:

		// Private typedefs
		typedef std::queue<Item *> ItemQueue;

		// Private variables
		SizeType	m_PoolSize;
		SizeType	m_ItemSize;
		bool		m_AllocateNewIfEmpty;
		ItemQueue	m_Items;
		SizeType	m_DeleteAtReturnCount;
		Mutex		m_Mutex;

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/System/MemoryPool.inl>

}

#endif
