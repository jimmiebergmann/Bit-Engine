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

#ifndef BIT_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP
#define BIT_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/VertexArray.hpp>
#include <Bit/Graphics/VertexBuffer.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief OpenGL Vertex buffer object.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLVertexArray : public VertexArray
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		OpenGLVertexArray( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~OpenGLVertexArray( );

		////////////////////////////////////////////////////////////////
		/// \brief Adding VBO to the VBA
		///
		/// The VBO must be fully loaded before adding.
		///
		/// \param p_VertexBufferObject The vertex buffer object to add.
		/// \param p_ComponentCount Number of vertex components,
		///		must be 1, 2, 3 or 4.
		///	\param p_DataType The type of the vertex data.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool AddVertexBuffer(	VertexBuffer & p_VertexBuffer,
										const SizeType p_ComponentCount = 3, 
										const DataType::eType p_DataType = DataType::Float32 );

		////////////////////////////////////////////////////////////////
		/// \brief Render the vertex array object
		///
		////////////////////////////////////////////////////////////////
		virtual void Render( PrimitiveMode::eMode p_PrimitiveMode ) const;

	private:

		GLuint m_VertexArrayObject;	///< The OpenGL vertex array object.
		GLuint m_BufferCount;		///< Amount of attached buffers.
		Uint32 m_VertexSize;		///< The vertex data size( vertexCount * componentCount )

	};

}

#endif