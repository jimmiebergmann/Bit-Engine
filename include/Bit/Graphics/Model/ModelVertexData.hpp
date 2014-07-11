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

#ifndef BIT_GRAPHICS_MODEL_VERTEX_DATA_HPP
#define BIT_GRAPHICS_MODEL_VERTEX_DATA_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <vector>

namespace Bit
{

	// Forward declaractions
	class VertexBuffer;
	class VertexArray;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model vertex data class.
	///
	/// Supporting up to 16 vertex buffers for the bitmask.
	/// The following indices are the default values of the bitmask:
	///		- 0: Position
	///		- 1: Texture coordinate
	///		- 2: Normal
	///
	/// \see Model
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ModelVertexData : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor. 
		///
		////////////////////////////////////////////////////////////////
		ModelVertexData( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. 
		///
		////////////////////////////////////////////////////////////////
		~ModelVertexData( );

		////////////////////////////////////////////////////////////////
		/// \brief Add a vertex array. Only the pointer will be copied,
		///		which means that ModelVertexData will take over
		///		the responsibility of the vertex buffer.
		///
		///	\param p_pVertexBuffer Pointer of the vertex buffer.
		/// \param p_Bitmask Value to OR together with the bitmask. 
		///
		/// \return false if the pointer is NULL or if the vertex buffer
		///		isn't loaded, else true.
		///
		////////////////////////////////////////////////////////////////
		Bool AddVertexBuffer( VertexBuffer * p_pVertexBuffer, const Uint16 p_Bitmask );

		////////////////////////////////////////////////////////////////
		/// \brief Set the vertex array. Only the pointer will be copied,
		///		which means that ModelVertexData will take over
		///		the responsibility of the vertex buffer.
		///		The old vertex array, if any, will be destroyed and replaced.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SetVertexArray( VertexArray * p_pVertexArray );

		////////////////////////////////////////////////////////////////
		/// \brief Set the bitmask
		///
		////////////////////////////////////////////////////////////////
		void SetBitmask( const Uint16 p_Bitmask );

		////////////////////////////////////////////////////////////////
		/// \brief Get the number of vertex buffers. 
		///
		////////////////////////////////////////////////////////////////
		SizeType GetVertexBufferCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the vertex buffer at the given index.
		///
		////////////////////////////////////////////////////////////////
		const VertexBuffer * GetVertexBuffer( const SizeType & p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the vertex array.
		///
		////////////////////////////////////////////////////////////////
		const VertexArray * GetVertexArray(  ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the bitmask
		///
		////////////////////////////////////////////////////////////////
		Uint16 GetBitmask(  ) const;

	private:

		// Private typedefs.
		typedef std::vector<VertexBuffer *> VertexBufferVector;

		// Private varaibles.
		VertexBufferVector m_VertexBuffers; ///< Vector of vertex buffers.
		VertexArray * m_pVertexArray;		///< Vertex array with the vertex buffers bound to it.
		Uint16 m_Bitmask;					///< Bitmask of all the buffers that are bound to the vertex array.


	};

}

#endif
