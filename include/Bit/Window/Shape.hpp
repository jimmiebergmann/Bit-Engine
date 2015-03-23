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

#ifndef BIT_WINDOW_SHAPE_HPP
#define BIT_WINDOW_SHAPE_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	// Forward declarations
	class GraphicDevice;
	class VertexArray;
	class VertexBuffer;

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Shape for simple render window
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Shape : public NonCopyable
	{

	public:

		// Friend classes
		friend class SimpleRenderWindow;

		////////////////////////////////////////////////////////////////
		/// \brief Set position
		///
		////////////////////////////////////////////////////////////////
		void SetPosition( const Vector2f32 & p_Position );

		////////////////////////////////////////////////////////////////
		/// \brief Set Size
		///
		////////////////////////////////////////////////////////////////
		void SetSize( const Vector2f32 & p_Size );

		////////////////////////////////////////////////////////////////
		/// \brief Set Radius( circle only )
		///
		////////////////////////////////////////////////////////////////
		void SetRadius( const Float32 p_Radius );

		////////////////////////////////////////////////////////////////
		/// \brief Get position
		///
		////////////////////////////////////////////////////////////////
		Vector2f32 GetPosition( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get Size
		///
		////////////////////////////////////////////////////////////////
		Vector2f32 GetSize( ) const;

	private:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		Shape( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~Shape( );

		////////////////////////////////////////////////////////////////
		/// \brief Load quad function
		///
		////////////////////////////////////////////////////////////////
		Bool LoadQuad( GraphicDevice * p_pGraphicDevice );

		////////////////////////////////////////////////////////////////
		/// \brief Load circle function
		///
		////////////////////////////////////////////////////////////////
		Bool LoadCircle( GraphicDevice * p_pGraphicDevice, const Uint32 p_Parts );

		////////////////////////////////////////////////////////////////
		/// \brief Get vertex array
		///
		////////////////////////////////////////////////////////////////
		VertexArray * GetVertexArray( ) const;

		// Private variables
		Vector2f32		m_Position;
		Vector2f32		m_Size;
		VertexArray *	m_pVertexArray;
		VertexBuffer *	m_pVertexBuffer;

	};

}

#endif
