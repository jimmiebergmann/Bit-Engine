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

#ifndef BIT_NETWORK_NET_VARIABLE_HPP
#define BIT_NETWORK_NET_VARIABLE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Mutex.hpp>
#include <Bit/System/ThreadValue.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/Network/Net/Private/EntityChanger.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <list>
#include <type_traits>

namespace Bit
{
	
	namespace Net
	{

		// Forward declarations
		class Entity;

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Network variable base class.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API VariableBase
		{

		public:

			// Friend classes
			template <typename T> friend class Variable;
			friend class EntityManager;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			VariableBase( SizeType p_Size );

			////////////////////////////////////////////////////////////////
			/// \brief Get Size
			///
			////////////////////////////////////////////////////////////////
			SizeType GetSize( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get name
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName() const;

		protected:

			// Protected functions
			////////////////////////////////////////////////////////////////
			/// \brief Function for setting new sequence.
			///
			/// \return true if the new sequence is valid, else false.
			///
			////////////////////////////////////////////////////////////////
			Bool SetNewSequence(const Uint16 p_Sequence);

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for taking a snapshot.
			///
			////////////////////////////////////////////////////////////////
			virtual void TakeSnapshot(const Time & p_Time, const Time & p_InterpolationTime, const Time & p_ExtrapolationTime) = 0;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for getting the "static" data. Not interpoalted one.
			///
			////////////////////////////////////////////////////////////////
			virtual void * GetData() = 0;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for setting the "static" data.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetData(const void * p_pData, const Time & p_Time, const Time & p_MinimumTime) = 0;

			////////////////////////////////////////////////////////////////
			/// \brief Check if this a new value, used for extrapolation in interpolated variable.
			///
			////////////////////////////////////////////////////////////////
			virtual Bool IsNewValue();

			////////////////////////////////////////////////////////////////
			/// \brief Set the "new value" flag, used for extrapolation in interpolated variable.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetIsNewValue(const Bool p_Status);
			

			// Protected variables.
			const SizeType		m_Size;			///< Size of the varaible.
			std::string			m_Name;			///< Variable name.
			Entity *			m_pParent;		///< Parent entity.
			Mutex				m_Mutex;		///< Mutex for set/get value.
			Uint16				m_LastSequence;	///< Last update sequence.

		};

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Entity variable class.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		class Variable : public VariableBase
		{

		public:

			// Friend classes
			friend class EntityManager;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			Variable( );

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			Variable( const T & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Set value.
			///
			////////////////////////////////////////////////////////////////
			void Set( const T & p_Value );

			////////////////////////////////////////////////////////////////
			/// \brief Get value.
			///
			////////////////////////////////////////////////////////////////
			T Get();

			////////////////////////////////////////////////////////////////
			/// \brief Get snapshot value. Make a snapshot via the entity manager class.
			///
			////////////////////////////////////////////////////////////////
			T GetSnapshot();

		protected:

			// Protected functions
			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for taking a snapshot.
			///
			////////////////////////////////////////////////////////////////
			virtual void TakeSnapshot(const Time & p_Time, const Time & p_InterpolationTime, const Time & p_ExtrapolationTime);

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for getting the "static" data. Not interpoalted one.
			///
			////////////////////////////////////////////////////////////////
			virtual void * GetData();

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for setting the "static" data.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetData(const void * p_pData, const Time & p_Time, const Time & p_MinimumTime);

		private:

			// Private variables
			T m_Value;		///< Value of the network variable.
			T m_Snapshot;	///< Snapshot value from the last snapshot.

		};

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Entity interpolated variable class.
		///
		/// Right now, you have to take a snapshot in order to get the interpolated variable to work.
		/// Get the interpolated data via the GetSnapshot snapshot function after
		/// callig the TakeSnapshot function via the entity manager.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		class InterpolatedVariable : public VariableBase
		{

		public:

			// Friend classes
			friend class EntityManager;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			InterpolatedVariable();

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			InterpolatedVariable(const T & p_Value);

			////////////////////////////////////////////////////////////////
			/// \brief Set value.
			///
			////////////////////////////////////////////////////////////////
			void Set(const T & p_Value);

			////////////////////////////////////////////////////////////////
			/// \brief Get value.
			///
			////////////////////////////////////////////////////////////////
			T Get();

			////////////////////////////////////////////////////////////////
			/// \brief Get snapshot value. Make a snapshot via the entity manager class.
			///
			////////////////////////////////////////////////////////////////
			T GetSnapshot();

		protected:

			// Protected functions
			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for taking a snapshot.
			///
			////////////////////////////////////////////////////////////////
			virtual void TakeSnapshot(const Time & p_Time, const Time & p_InterpolationTime, const Time & p_ExtrapolationTime);

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for getting the "static" data. Not interpoalted one.
			///
			////////////////////////////////////////////////////////////////
			virtual void * GetData();

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for setting the "static" data.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetData(const void * p_pData, const Time & p_Time, const Time & p_MinimumTime);

			////////////////////////////////////////////////////////////////
			/// \brief Check if this a new value, used for extrapolation in interpolated variable.
			///
			////////////////////////////////////////////////////////////////
			virtual Bool IsNewValue();

			////////////////////////////////////////////////////////////////
			/// \brief Set the "new value" flag, used for extrapolation in interpolated variable.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetIsNewValue(const Bool p_Status);

		private:

			// Private structs
			////////////////////////////////////////////////////////////////
			/// \brief Value strucutre
			///
			////////////////////////////////////////////////////////////////
			struct Value
			{
				Time m_Time;
				T m_Value;
				Bool m_InitialFlag;	///< This value is an initial value, and should be modified when another value is added.
			};

			
			// Private typedef
			typedef std::list<Value> ValueList;

			// Private functions
			////////////////////////////////////////////////////////////////
			/// \brief Clear old data.
			///
			////////////////////////////////////////////////////////////////
			void ClearOldData(const Time & p_MinimumTime);
			
			// Private variables
			ValueList		m_Values;		///< Values of the network variable.
			T				m_LastValue;	///< Server only.
			T				m_Snapshot;		///< Snapshot value from the last snapshot.
			Bool			m_NewValue;		///< Is this a new value since the entity manager sent this value to the client?
			Bool			m_IsSet;		///< Is this value set from the server yet?
			Time			m_LastExtrapolationTime;	///< 
			Bool			m_IsExtrapolating;
		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/Variable.inl>

	}

}

#endif