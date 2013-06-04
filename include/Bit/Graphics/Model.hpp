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

#ifndef __BIT_GRAPHICS_MODEL_HPP__
#define __BIT_GRAPHICS_MODEL_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/Graphics/VertexObject.hpp>
#include <vector>
#include <string>

namespace Bit
{

	class BIT_API Model
	{

	public:

		// Public structures
		struct Triangle
		{
			BIT_SINT32 PositionIndex[ 3 ];
			BIT_SINT32 TextureIndex[ 3 ];
			BIT_SINT32 NormalIndex[ 3 ];
		};

		// Constructor/destructor
		Model( );
		~Model( );

		// Public functions
		BIT_UINT32 ReadFile( std::string p_FilePath );
		BIT_UINT32 ReadOBJ( std::string p_FilePath );
		BIT_UINT32 LoadVertexObject( VertexObject & p_VertexObject );
		void DeallocateData( );
		BIT_INLINE BIT_BOOL IsLoaded( ) const { return m_Loaded; }

		// Set functions
		// ...

		// Get functions
		BIT_UINT32 GetVertexPositionCount( ) const;
		BIT_UINT32 GetVertexTextureCount( ) const;
		BIT_UINT32 GetVertexNormalCount( ) const;
		BIT_UINT32 GetTriangleCount( ) const;
		BIT_UINT32 GetVertexPosition( BIT_UINT32 p_Index, Vector3_f32 p_Coord ) const;
		BIT_UINT32 GetVertexTexture( BIT_UINT32 p_Index, Vector2_f32 p_Coord ) const;
		BIT_UINT32 GetVertexNormal( BIT_UINT32 p_Index, Vector3_f32 p_Coord ) const;
		BIT_UINT32 GetTriangle( BIT_UINT32 p_Index, Triangle p_Triangle ) const;

	private:

		// Private functions
		void DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
		BIT_SINT32 * p_pTexture, BIT_SINT32 * p_pNormal );

		// Private variables
		BIT_BOOL m_Loaded;
		std::vector< Vector3_f32 > m_VertexPositions;
		std::vector< Vector2_f32 > m_VertexTextures;
		std::vector< Vector3_f32 > m_VertexNormals;
		std::vector< Triangle > m_Triangles;

	};

}

#endif
