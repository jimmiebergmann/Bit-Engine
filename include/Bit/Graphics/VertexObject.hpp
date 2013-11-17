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

#ifndef BIT_GRAPHICS_VERTEX_OBJECT_HPP
#define BIT_GRAPHICS_VERTEX_OBJECT_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector2.hpp>

// How to use:
// 1. Create the vertex object via your graphic device.
// 2. Add your buffers via AddVertexBuffer( ... )
// 3. Load the vertex object via Load( ... )
// 4. Clean up by calling Clear( ) or deleting your vertex object pointer

namespace Bit
{
	class BIT_API VertexObject
	{

	public:

		// Public enums
		enum eRenderMode
		{
			RenderMode_Triangles = 0,
			RenderMode_Lines = 1,
			RenderMode_LineStrip = 2
		};

		enum eVertexType
		{
			Vertex_None = 0,
			Vertex_Position = 1,
			Vertex_Texture = 2,
			Vertex_Normal = 4,
			Vertex_Tangent = 8,
			Vertex_Binormal = 16
		};
		
		// Destructor
		virtual ~VertexObject( ) { }

		// Virtual public functions
		virtual BIT_UINT32 Load( const BIT_UINT32 p_PieceCount, const BIT_UINT32 p_PieceSize ) = 0;
		virtual BIT_UINT32 LoadFullscreenQuad( const Vector2_ui32 p_Size ) = 0;
		virtual BIT_UINT32 Unload( ) = 0;
		virtual BIT_UINT32 AddVertexBuffer( void * p_pBuffer, const BIT_UINT32 p_VertexDimensions, const eDataType p_DataType ) = 0;
		virtual BIT_UINT32 UpdateVertexBuffer( const BIT_UINT32 p_Index, const void * p_pBuffer,
			const BIT_UINT32 p_Offset, const BIT_UINT32 p_DataSize ) = 0;
		virtual void Render( const eRenderMode p_Mode ) = 0;
		virtual void Render( const eRenderMode p_Mode, BIT_UINT32 p_Start, BIT_UINT32 p_Length ) = 0;

		// Static publuc function
		static void GenerateTangents( const BIT_FLOAT32 * p_pVertexPositions, const BIT_FLOAT32 * p_pTexturePositions,
			BIT_FLOAT32 * p_pTangents, BIT_FLOAT32 * p_pBinormals, const BIT_UINT32 p_TriangleCount );

		// Public inline functions
		BIT_INLINE BIT_BOOL IsLoaded( ) const { return m_Loaded; }

	protected:

		// Protected variables
		BIT_BOOL m_Loaded;

	};

}

#endif