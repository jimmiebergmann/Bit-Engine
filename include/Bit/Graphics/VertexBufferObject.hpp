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

#ifndef BIT_GRAPHICS_VERTEX_BUFFER_OBJECT_HPP
#define BIT_GRAPHICS_VERTEX_BUFFER_OBJECT_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Vertex buffer object base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API VertexBufferObject
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor
		///
		////////////////////////////////////////////////////////////////
		virtual ~VertexBufferObject( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Binding the vertex buffer object
		///
		////////////////////////////////////////////////////////////////
		virtual void Bind( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Loading the vertex buffer object
		///
		/// \param p_DataSize The vertex data size, in bytes.
		/// \param p_pVertexData The vertex data.
		/// \param p_DataType The data type of the vertex data.
		///
		////////////////////////////////////////////////////////////////
		virtual bool Load( const SizeType p_DataSize, void * p_pVertexData = NULL ) = 0;

		virtual SizeType GetBufferSize( ) const = 0;

	};

}

#endif