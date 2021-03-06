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

#ifndef BIT_SYSTEM_PHYS2_SHAPE_HPP
#define BIT_SYSTEM_PHYS2_SHAPE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	namespace Phys2
	{

		// Forward declarations
		namespace Private
		{
			class Manifold;
		}

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Shape base class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Shape
		{

		public:

			// Friend classes
			friend class Body;

			////////////////////////////////////////////////////////////////
			/// \ingroup System
			/// \brief Shape type enumerator.
			/// 
			////////////////////////////////////////////////////////////////
			enum eType
			{
				CircleType,
				RectangleType
			};

			////////////////////////////////////////////////////////////////
			/// \brief Get shape type.
			/// 
			////////////////////////////////////////////////////////////////
			eType GetType( ) const;

		protected:

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Shape( const eType p_Type );

			// Protected variables
			eType m_Type;	///< Shape type

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for cloning the shape
			/// 
			////////////////////////////////////////////////////////////////
			virtual Shape * Clone( ) const = 0;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing mass.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeMass( const Float32 p_Density ) = 0;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing inertia.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeInertia( const Float32 p_Mass ) = 0;

		};

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Circle shape class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Circle : public Shape
		{

		public:

			// Friend classes
			friend class Private::Manifold;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Circle( const Float32 p_Radius = 1.0f );

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for getting the radius of the shape(circle only).
			/// 
			////////////////////////////////////////////////////////////////
			Float32 GetRadius( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for setting the radius of the shape(circle only).
			/// 
			////////////////////////////////////////////////////////////////
			void SetRadius( const Float32 p_Radius );

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for cloning the shape
			/// 
			////////////////////////////////////////////////////////////////
			virtual Shape * Clone( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing mass.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeMass( const Float32 p_Density );

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing inertia.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeInertia( const Float32 p_Mass );

			// Private variables
			Float32 m_Radius;

		};

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Rectangle shape class for 2D physics.
		/// 
		////////////////////////////////////////////////////////////////
		class BIT_API Rectangle : public Shape
		{

		public:

			// Friend classes
			friend class Private::Manifold;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			/// 
			////////////////////////////////////////////////////////////////
			Rectangle( const Vector2f32 & p_Size = Vector2f32( 1.0f, 1.0f ) );

			////////////////////////////////////////////////////////////////
			/// \brief Get size( width and height ).
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetSize( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for setting the radius of the shape(circle only).
			/// 
			////////////////////////////////////////////////////////////////
			void SetSize( const Vector2f32 & p_Size );

		private:

			// Private functions

			////////////////////////////////////////////////////////////////
			/// \brief Get extreme point along a direction from the rectangle
			/// 
			////////////////////////////////////////////////////////////////
			Vector2f32 GetExtremePoint( const Vector2f32 & p_Direction ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function for cloning the shape
			/// 
			////////////////////////////////////////////////////////////////
			virtual Shape * Clone( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing mass.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeMass( const Float32 p_Density );

			////////////////////////////////////////////////////////////////
			/// \brief Virtual function computing inertia.
			/// 
			////////////////////////////////////////////////////////////////
			virtual Float32 ComputeInertia( const Float32 p_Mass );

			// Private variables
			Vector2f32 m_Size;

		};

	}

}

#endif