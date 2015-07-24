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
#include <Bit/Graphics/Drawable.hpp>
#include <Bit/Graphics/ModelMaterial.hpp>
#include <Bit/Graphics/Model/ModelVertexData.hpp>
#include <Bit/Graphics/Model/Skeleton.hpp>
#include <Bit/Graphics/Model/AnimationState.hpp>
#include <string>

namespace Bit
{

	// Forward declaractions
	class GraphicDevice;
	class VertexArray;
	class ObjFile;
	class Md2File;
	class VertexAnimationTrack;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model class.
	///
	/// The vertex data is stored by default in the following order:
	///		- 0: Position
	///		- 1: Texture coordinate
	///		- 2: Normal
	///		- 3: Tangnet
	///		- 4: Binormal
	/// You can add your own model funcion and loading your own format
	/// and store the vertex data in any order, but then you have to
	/// create your own model renderer(which is recommened anyway).
	///
	/// Current model formats supported:
	///		- OBJ
	///
	/// Default material property names:
	///		- Color			( RGBA : 0.0f - 1.0f each channel )
	///		- Shininess		( 0.0f - 1.0f )
	///		- ColorMap		( Color texture path )
	///		- NormalMap		( Normal texture path )
	///		- SpecularMap	( Specular texture path );
	///
	/// \see ModelRenderer
	/// \see ModelVertexData
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Model : public Drawable
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
		/// \brief Draw the model
		///
		////////////////////////////////////////////////////////////////
		virtual void Draw(Renderer & p_Renderer);

		////////////////////////////////////////////////////////////////
		/// \brief Load 3D model from a file.
		///
		/// The filename must contain a file suffix in order to let the function
		/// know what kind of model format you are trying to load.
		///
		/// \param p_LoadTextureCoords Try to load texture coordinates if true.
		/// \param p_LoadNormals Try to load normals if true.
		/// \param p_LoadTangents Try to load tangents if true. Ignored if p_LoadNormals is false.
		/// \param p_LoadBinormals Try to load binormals if true. Ignored if p_LoadNormals is false.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile(	const std::string & p_Filename,
							const Bool p_LoadTextureCoords = true,
							const Bool p_LoadNormals = true,
							const Bool p_LoadTangents = true,
							const Bool p_LoadBinormals = true );

		////////////////////////////////////////////////////////////////
		/// \brief Load 3D model from a OBJ file.
		///
		/// \param p_LoadTextureCoords Try to load texture coordinates if true.
		/// \param p_LoadNormals Try to load normals if true.
		/// \param p_LoadTangents Try to load tangents if true. Ignored if p_LoadNormals is false.
		/// \param p_LoadBinormals Try to load binormals if true. Ignored if p_LoadNormals is false.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromObjFile(	const std::string & p_Filename,
								const Bool p_LoadTextureCoords = true,
								const Bool p_LoadNormals = true,
								const Bool p_LoadTangents = true,
								const Bool p_LoadBinormals = true );

		////////////////////////////////////////////////////////////////
		/// \brief Load 3D model from a MD2 file.
		///
		/// \param p_LoadTextureCoords Try to load texture coordinates if true.
		/// \param p_LoadNormals Try to load normals if true.
		/// \param p_LoadTangents Try to load tangents if true. Ignored if p_LoadNormals is false.
		/// \param p_LoadBinormals Try to load binormals if true. Ignored if p_LoadNormals is false.
		///
		/// \return true if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMd2File(	const std::string & p_Filename,
								const Bool p_LoadTextureCoords = true,
								const Bool p_LoadNormals = true,
								const Bool p_LoadTangents = true,
								const Bool p_LoadBinormals = true );

		////////////////////////////////////////////////////////////////
		/// \brief Get animation type.
		///
		////////////////////////////////////////////////////////////////
		Skeleton & GetSkeleton();

		////////////////////////////////////////////////////////////////
		/// \brief Get animation state. You handle the animations via this class.
		///
		////////////////////////////////////////////////////////////////
		AnimationState & GetAnimationState();

		////////////////////////////////////////////////////////////////
		/// \brief Get vertex data.
		///
		////////////////////////////////////////////////////////////////
		ModelVertexGroup & GetVertexGroup( );

		////////////////////////////////////////////////////////////////
		/// \brief Get material count.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetMaterialCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get material.
		///
		////////////////////////////////////////////////////////////////
		ModelMaterial & GetMaterial( const SizeType p_Index );

	private:

		// Private typedefs
		typedef std::vector<ModelMaterial *> ModelMaterialVector;

		// Private functions

		////////////////////////////////////////////////////////////////
		/// \brief Render the initial pose, or "static" model.
		///
		/// This function could be used for static model rendering, such as props.
		///
		////////////////////////////////////////////////////////////////
		void RenderInitialPose(Renderer & p_Renderer);

		////////////////////////////////////////////////////////////////
		/// \brief	Render vertex animation. Uses the AnimationState class from
		///			the model in order to render the right pose.
		///
		////////////////////////////////////////////////////////////////
		void RenderVertexAnimation(Renderer & p_Renderer);

		////////////////////////////////////////////////////////////////
		/// \brief	Render skeletal animation. Uses the AnimationState class from
		///			the model in order to render the right pose.
		///
		////////////////////////////////////////////////////////////////
		void RenderSkeletalAnimation(Renderer & p_Renderer);

		///////////////////////////////////////////////////////////////
		/// \brief Load md2 frame from md2 file into the model vertex group.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadMd2Frame(	Md2File & p_Md2File,
							ModelVertexGroup * p_pModelVertexGroup,
							const SizeType p_FrameIndex,
							const Bool p_LoadTextureCoords = true,
							const Bool p_LoadNormals = true,
							const Bool p_LoadTangents = true,
							const Bool p_LoadBinormals = true);

		///////////////////////////////////////////////////////////////
		/// \brief Fix the interpolation for vertex animations.
		///		   We fix this by adding the keyframes VBOs to each other.
		///
		////////////////////////////////////////////////////////////////
		void FixVertexInterpolation(	VertexAnimationTrack * p_pAnimationTrack,
										const Bool p_LoadTextureCoords = true,
										const Bool p_LoadNormals = true,
										const Bool p_LoadTangents = true,
										const Bool p_LoadBinormals = true);

		// Private variables
		const GraphicDevice &	m_GraphicDevice;	///< Reference of the parent graphic device.
		Skeleton				m_Skeleton;			///< Skeleton, contains animations of any kind.
		AnimationState			m_AnimationState;	///< Stores the current animation state.
		ModelMaterialVector		m_Materials;		///< Materials used by this model.
		ModelVertexGroup		m_VertexGroup;		///< Vertex group of idle static pose.

	};

}

#endif
