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


#ifndef __BIT_GRAPHICS_MODEL_OBJ_HPP__
#define __BIT_GRAPHICS_MODEL_OBJ_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/Texture.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <vector>

namespace Bit
{

	class BIT_API ModelOBJ : public Model
	{

	public:

		// public structures
		struct Triangle
		{
			BIT_SINT32 PositionIndices[ 3 ];
			BIT_SINT32 TextureIndices[ 3 ];
			BIT_SINT32 NormalIndices[ 3 ];
		};
		
		struct MaterialGroup
		{
			Texture * pTexture;
			std::string MaterialName;

			// Triangles - flat or smooth triangles
			std::vector< Triangle > TrianglesFlat;
			std::vector< Triangle > TrianglesSmooth;
		};

		struct VertexGroup
		{
			std::vector< MaterialGroup * > Materials;
		};


		// Constructor/Destructor
		ModelOBJ( const GraphicDevice & p_GraphicDevice );
		~ModelOBJ( );

		// Public functions
		virtual BIT_UINT32 ReadFile( const char * p_pFilePath );
		virtual void Unload( );
		virtual void Render( );

		// Get functions
		virtual std::string GetName( ) const;
		virtual BIT_UINT32 GetTriangleCount( ) const;
		virtual BIT_UINT32 GetTriangleIndexCount( ) const;
		virtual BIT_UINT32 GetPositionCoordinateCount( ) const;
		virtual BIT_UINT32 GetTextureCoordinateCount( ) const;
		virtual BIT_UINT32 GetNormalCoordinateCount( ) const;
		virtual BIT_UINT32 GetTextureCount( ) const;
		virtual BIT_UINT32 GetAnimationCount( ) const;
		virtual BIT_BOOL ContainsRagdoll( ) const;

	private:

		// Private functions
		BIT_UINT32 ReadData( const char * p_pFilePath );
		BIT_UINT32 LoadGraphics( );
		void DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
			BIT_SINT32 * p_pTexture, BIT_SINT32 * p_pNormal );

		// Private variables
		std::string m_Name;
		std::vector< Vector3_f32 > m_VertexPositions;
		std::vector< Vector2_f32 > m_TexturePositions;
		std::vector< Vector3_f32 > m_NormalPositions;
		std::vector< VertexGroup * > m_VertexGroups;
		std::vector< Texture * > m_Textures;
		BIT_UINT32 m_TotalTriangleCount;
		BIT_UINT32 m_TotalTriangleIndexCount;
		const GraphicDevice & m_GraphicDevice;

		// Private render variables

	};

}

#endif