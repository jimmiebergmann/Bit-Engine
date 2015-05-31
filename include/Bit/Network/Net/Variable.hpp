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
#include <Bit/Network/Net/Private/EntityChanger.hpp>
#include <string>

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
			const std::string & GetName( ) const;

		protected:

			// Private variables.
			const SizeType		m_Size;			///< Size of the varaible.
			std::string			m_Name;			///< Variable name.
			Entity *			m_pParent;		///< Parent entity.
			Mutex				m_Mutex;		///< Mutex for set/get value.
			Timer				m_Timer;		///< Timer for interpolation.
			bool				m_IsSet;		///< Flag for checking if the value has been set.

		};

		////////////////////////////////////////////////////////////////
		/// \ingroup Network
		/// \brief Network address class.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		class Variable : public VariableBase
		{

		public:

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
			T Get( const bool p_Interpolated = false );

			void UpdateLastValue()
			{
				Float64 time = m_Timer.GetLapsedTime().AsSeconds();
				m_LastValue = m_Value + ((m_Value - m_LastValue) * (time * 10.0f /*m_pParent->m_pEntityManager->m_FrameTime.Get()*/));
			}

			// Public variables
			// DO NOT CHANGE THE ORDER OF THESE VARIABLES
			T m_Value;		///< Value of the network variable.
			T m_LastValue;	///< Value of the last network variable, used for interpolation.

		};

		////////////////////////////////////////////////////////////////
		// Include the inline file.
		////////////////////////////////////////////////////////////////
		#include <Bit/Network/Net/Variable.inl>

	}

}

#endif