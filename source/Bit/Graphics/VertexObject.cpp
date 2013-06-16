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


#include <Bit/Graphics/VertexObject.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	
	// Calculate tangents and/or binormals
	void VertexObject::GenerateTangents( const BIT_FLOAT32 * p_pVertexPositions, const BIT_FLOAT32 * p_pTexturePositions,
			BIT_FLOAT32 * p_pTangents, BIT_FLOAT32 * p_pBinormals, const BIT_UINT32 p_TriangleCount )
	{
		if( !p_pVertexPositions || !p_pTexturePositions || ( !p_pTangents && !p_pBinormals ) )
		{
			return;
		}

		// Go through all the triangles
		for( BIT_UINT32 ti = 0; ti < p_TriangleCount; ti++ )
		{
			// Go through all the vertices
			for( BIT_UINT32 vi = 0; vi < 3; vi++ )
			{
				// Triangle indicies
				const BIT_UINT32 PosTriangleIndex = ti * 9;
				const BIT_UINT32 TexTriangleIndex = ti * 6;

				// Vertex indicies, depending on the vertex
				const Vector3_ui32 PosIndices( vi * 3, ( ( vi * 3 ) + 3 ) % 9, ( ( vi * 3 ) + 6 ) % 9 );
				const Vector3_ui32 TexIndices( vi * 2, ( ( vi * 2 ) + 2 ) % 6, ( ( vi * 2 ) + 4 ) % 6 );

				// Vertex positions
				const Vector3_f32 Vertex1(	p_pVertexPositions[ PosTriangleIndex + PosIndices.x ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.x + 1 ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.x + 2 ] );

				const Vector3_f32 Vertex2(	p_pVertexPositions[ PosTriangleIndex + PosIndices.y ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.y + 1 ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.y + 2 ] );

				const Vector3_f32 Vertex3(	p_pVertexPositions[ PosTriangleIndex + PosIndices.z ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.z + 1 ],
											p_pVertexPositions[ PosTriangleIndex + PosIndices.z + 2 ] );

				// Texture positions
				const Vector3_f32 Texture1(	p_pTexturePositions[ TexTriangleIndex + TexIndices.x ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.x + 1 ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.x + 2 ] );

				const Vector3_f32 Texture2(	p_pTexturePositions[ TexTriangleIndex + TexIndices.y ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.y + 1 ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.y + 2 ] );

				const Vector3_f32 Texture3(	p_pTexturePositions[ TexTriangleIndex + TexIndices.z ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.z + 1 ],
											p_pTexturePositions[ TexTriangleIndex + TexIndices.z + 2 ] );


				const BIT_FLOAT32 x1 = Vertex2.x - Vertex1.x;
				const BIT_FLOAT32 x2 = Vertex3.x - Vertex1.x;
				const BIT_FLOAT32 y1 = Vertex2.y - Vertex1.y;
				const BIT_FLOAT32 y2 = Vertex3.y - Vertex1.y;
				const BIT_FLOAT32 z1 = Vertex2.z - Vertex1.z;
				const BIT_FLOAT32 z2 = Vertex3.z - Vertex1.z;

				const BIT_FLOAT32 s1 = Texture2.x - Texture1.x;
				const BIT_FLOAT32 s2 = Texture3.x - Texture1.x;
				const BIT_FLOAT32 t1 = Texture2.y - Texture1.y;
				const BIT_FLOAT32 t2 = Texture3.y - Texture1.y;

				const BIT_FLOAT32 r = 1.0f / ( s1 * t2 - s2 * t1 );

				// Calculate the tangent
				if( p_pTangents )
				{
					Vector3_f32 Tangent( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
						(t2 * z1 - t1 * z2) * r );
					Tangent.Normalize( );

					p_pTangents[ PosTriangleIndex + PosIndices.x + 0 ] = Tangent.x;
					p_pTangents[ PosTriangleIndex + PosIndices.x + 1 ] = Tangent.y;
					p_pTangents[ PosTriangleIndex + PosIndices.x + 2 ] = Tangent.z;
				}

				// Calculate the binormal
				if( p_pBinormals )
				{
					Vector3_f32 Binormal( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
						(s1 * z2 - s2 * z1) * r );
					Binormal.Normalize( );

					p_pBinormals[ PosTriangleIndex + PosIndices.x + 0 ] = Binormal.x;
					p_pBinormals[ PosTriangleIndex + PosIndices.x + 1 ] = Binormal.y;
					p_pBinormals[ PosTriangleIndex + PosIndices.x + 2 ] = Binormal.z;
				}
			}
		}
	}

}