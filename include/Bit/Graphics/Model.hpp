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

#ifndef BIT_GRAPHICS_MODEL_HPP
#define BIT_GRAPHICS_MODEL_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/Graphics/ModelMaterial.hpp>
#include <Bit/Graphics/Model/ModelVertexData.hpp>
#include <Bit/Graphics/Model/Skeleton.hpp>
#include <string>

namespace Bit
{

	// Forward declaractions
	class GraphicDevice;
	class VertexArray;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model class.
	///
	/// The vertex data is stored by default in the following order:
	///		- 0: Position
	///		- 1: Texture coordinate
	///		- 2: Normal
	/// You can add your own model funcion and loading your own format
	/// and store the vertex data in any order, but then you have to
	/// create your own model renderer(which is recommened anyway).
	///
	/// Current model formats supported:
	///		- OBJ
	///
	/// \see ModelRenderer
	/// \see ModelVertexData
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Model : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Model( const GraphicDevice & p_GraphicDevice );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~Model( );

		////////////////////////////////////////////////////////////////
		/// \brief Load 3D model from a file.
		///
		/// The filename must contain a file suffix in order to let the function
		/// know what kind of model format you are trying to load.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Load 3D model from a OBJ file.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromObjFile(	const std::string & p_Filename,
								const Bool p_LoadTextureCoords = true,
								const Bool p_LoadNormalCoords = true );

		////////////////////////////////////////////////////////////////
		/// \brief Get animation type.
		///
		////////////////////////////////////////////////////////////////
		Skeleton & GetSkeleton( );

		////////////////////////////////////////////////////////////////
		/// \brief Get vertex data.
		///
		////////////////////////////////////////////////////////////////
		ModelVertexData & GetVertexData( );

	private:

		// Private typedefs
		typedef std::vector<ModelMaterial*> ModelMaterialVector;

		// Private functions

		// Private variables
		const GraphicDevice & m_GraphicDevice;	///< Reference of the parent graphic device.
		Skeleton m_Skeleton;					///< Skeleton, contains animations of any kind.
		ModelMaterialVector m_Materials;		///< Materials used by this model.
		ModelVertexData m_VertexData;			///< Idle static pose.


		/*
		// Public enum
		enum eModelType
		{
			Model_None = 1,
			Model_OBJ = 2
		};

		// Destructor
		virtual ~Model( ) { }

		// Virtual public functions
		virtual BIT_UINT32 ReadFile( const char * p_pFilePath ) = 0;
		virtual BIT_UINT32 Load( const BIT_UINT32 m_VertexElementBits, const Texture::eFilter * p_pTextureFilters,
			const BIT_BOOL p_Mipmapping ) = 0;
		virtual void Unload( ) = 0;
		virtual void Render( const VertexObject::eRenderMode p_Mode ) = 0;

		// Virtual get functions
		virtual std::string GetName( ) const = 0;
		virtual BIT_UINT32 GetVertexGroupCount( ) const = 0;
		virtual BIT_UINT32 GetTriangleCount( ) const = 0;
		virtual BIT_UINT32 GetPositionCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetTextureCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetNormalCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetTextureCount( ) const = 0;
		virtual BIT_UINT32 GetAnimationCount( ) const = 0;
		virtual BIT_BOOL ContainsRagdoll( ) const = 0;

		// Public inline get functions
		BIT_INLINE BIT_BOOL IsLoaded( ) { return m_Loaded; }
		BIT_INLINE eModelType GetType( ) { return m_Type; }

	protected:

		BIT_BOOL m_Loaded;
		eModelType m_Type;
		*/
	};

}

#endif
