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

#ifndef __BIT_GRAPHICS_VERTEX_OBJECT_HPP__
#define __BIT_GRAPHICS_VERTEX_OBJECT_HPP__

#include <Bit/DataTypes.hpp>

// How to use:
// 1. Create the vertex object via your graphic device.
// 2. Add your buffers via AddVertexBuffer( ... )
// 3. Load the vertex object via Load( ... )
// 4. Clean up by called Clear( ) or deleting your vertex object pointer

namespace Bit
{
	class BIT_API VertexObject
	{

	public:

		// Public enum
		enum eRenderMode
		{
			RenderMode_Triangles = 0,
			RenderMode_Lines = 1,
			RenderMode_LineStrip = 2
		};
		
		// Destructor
		virtual ~VertexObject( ) { }

		// Virtual public functions
		virtual BIT_UINT32 Load( const BIT_UINT32 p_PieceCount, const BIT_UINT32 p_PieceSize ) = 0;
		virtual BIT_UINT32 Unload( ) = 0;
		virtual BIT_UINT32 AddVertexBuffer( void * p_pBuffer, const BIT_UINT32 p_VertexDimensions, const BIT_UINT32 p_DataType ) = 0;
		virtual BIT_UINT32 UpdateVertexBuffer( const BIT_UINT32 p_Index, const void * p_pBuffer,
			const BIT_UINT32 p_Offset, const BIT_UINT32 p_DataSize ) = 0;
		virtual void Render( const eRenderMode p_Mode ) = 0;

		// Public inline functions
		BIT_INLINE BIT_BOOL IsLoaded( ) const { return m_Loaded; }

	protected:

		// Protected variables
		BIT_BOOL m_Loaded;

	};

}

#endif