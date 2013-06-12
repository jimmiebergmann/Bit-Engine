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
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/Texture.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <vector>

// To do:
// -------------------------------------
// 1. Support negative face indices. f -3/-3 -2/-1 -1/-2
// 2. Support multiple material files.
// 3. Support 4 point polygons.

namespace Bit
{

	class BIT_API ModelOBJ : public Model
	{

	public:

		// public structures
		struct Material
		{
			std::string Name;
			std::string DiffuseTexture;
			std::string NormalTexture;
		};

		struct Triangle
		{
			BIT_SINT32 PositionIndices[ 3 ];
			BIT_SINT32 TextureIndices[ 3 ];
			BIT_SINT32 NormalIndices[ 3 ];
		};
		
		struct MaterialGroup
		{
			std::string MaterialName;

			// Triangles - flat and smooth triangles
			std::vector< Triangle > TrianglesFlat;
			std::vector< Triangle > TrianglesSmooth;
		};

		struct VertexGroup
		{
			std::vector< MaterialGroup * > Materials;
		};

		struct RenderObject
		{
			Texture * pTextureDiffuse;
			Texture * pTextureNormal;
			VertexObject * pVertexObject;
		};

		// Constructor/Destructor
		ModelOBJ( const GraphicDevice & p_GraphicDevice );
		~ModelOBJ( );

		// Public functions
		virtual BIT_UINT32 ReadFile( const char * p_pFilePath );
		virtual BIT_UINT32 Load( const BIT_UINT32 m_VertexElementBits, const Texture::eFilter * p_pTextureFilters,
			const BIT_BOOL p_Mipmapping );
		virtual void Unload( );
		virtual void Render( const VertexObject::eRenderMode p_Mode );

		// Get functions
		virtual std::string GetName( ) const;
		virtual BIT_UINT32 GetVertexGroupCount( ) const;
		virtual BIT_UINT32 GetTriangleCount( ) const;
		virtual BIT_UINT32 GetPositionCoordinateCount( ) const;
		virtual BIT_UINT32 GetTextureCoordinateCount( ) const;
		virtual BIT_UINT32 GetNormalCoordinateCount( ) const;
		virtual BIT_UINT32 GetTextureCount( ) const;
		virtual BIT_UINT32 GetAnimationCount( ) const;
		virtual BIT_BOOL ContainsRagdoll( ) const;

	private:

		// Private functions
		BIT_UINT32 ReadData( const char * p_pFilePath );
		BIT_UINT32 ReadMaterialFile( const char * p_pFilePath, const char * p_pMainFilePath );
		void DecodeOBJFaces( BIT_SCHAR8 * p_String, BIT_SINT32 * p_pPosition,
			BIT_SINT32 * p_pTexture, BIT_SINT32 * p_pNormal );
		BIT_UINT32 ValidateTriangles(  );

		// Private typedefs
		typedef std::vector< Material * > MaterialVector;
		typedef MaterialVector::iterator MaterialIterator;
		typedef std::vector< VertexGroup * > VertexGroupVector;
		typedef VertexGroupVector::iterator VertexGroupIterator;
		typedef std::vector< MaterialGroup * > MaterialGroupVector;
		typedef MaterialGroupVector::iterator MaterialGroupIterator;
		typedef std::vector< Triangle > TriangleVector;
		typedef TriangleVector::iterator TriangleIterator;

		// Private variables
		std::string m_Name;
		std::vector< std::string > m_MaterialLibraries;
		std::vector< Material * > m_Materials;
		std::vector< Vector3_f32 > m_VertexPositions;
		std::vector< Vector2_f32 > m_TexturePositions;
		std::vector< Vector3_f32 > m_NormalPositions;
		std::vector< VertexGroup * > m_VertexGroups;
		std::vector< Texture * > m_Textures;
		BIT_UINT32 m_TotalTriangleCount;

		// Private render variables
		const GraphicDevice & m_GraphicDevice;
		std::vector< RenderObject * > m_RenderObjects;

	};

}

#endif