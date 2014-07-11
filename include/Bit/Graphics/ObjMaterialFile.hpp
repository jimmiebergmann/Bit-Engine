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

#ifndef BIT_GRAPHICS_OBJ_MATERIAL_FILE_HPP
#define BIT_GRAPHICS_OBJ_MATERIAL_FILE_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/System/Vector3.hpp>
#include <string>
#include <vector>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Obj model material file class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ObjMaterialFile
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Single material class.
		///
		/// Resource:	- http://cgkit.sourceforge.net/doc2/objmaterial.html
		///				- http://paulbourke.net/dataformats/mtl/				
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Material
		{

		public:

			// Friend classes
			friend class ObjMaterialFile;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor.
			///
			////////////////////////////////////////////////////////////////
			Material(	const std::string p_Name = "default",
						const Uint8 p_Illum = 2,
						const Vector3f32 p_AmbientColor = Vector3f32( 0.2f, 0.2f, 0.2f ),
						const Vector3f32 p_DiffuseColor = Vector3f32( 0.8f, 0.8f, 0.8f ),
						const Vector3f32 p_SpecularColor = Vector3f32( 0.0f, 0.0f, 0.0f ),
						const Vector3f32 p_EmissiveColor = Vector3f32( 0.0f, 0.0f, 0.0f ),
						const Float32 p_Shininess = 0.0f,
						const Float32 p_OpticalDensity = 1.0f,
						const std::string p_AmbienTexture = "",
						const std::string p_DiffuseTexture = "",
						const std::string p_SpecularTexture = "",
						const std::string p_ShininessTexture = "",
						const std::string p_EmissiveTexture = "",
						const std::string p_DisplacementTexture = "",
						const std::string p_BumpTexture = "" );


		private:

			// Private variables
			std::string m_Name;					///< Name of the material.
			Uint8 m_Illum;						///< "illum", render mode, 2 by default.
			Vector3f32 m_AmbientColor;			///< "Ka".
			Vector3f32 m_DiffuseColor;			///< "Kd".
			Vector3f32 m_SpecularColor;			///< "Ks".
			Vector3f32 m_EmissiveColor;			///< "Ke".
			Float32 m_Shininess;				///< "Ns", specular exponent.
			Float32 m_OpticalDensity;			///< "Ni", 0.001 to 10. 1 by default, 1.5 if glass.
			std::string m_AmbienTexture;		///< "map_Ka".
			std::string m_DiffuseTexture;		///< "map_Kd", color texture.
			std::string m_SpecularTexture;		///< "map_Ks". Specular color.
			std::string m_ShininessTexture;		///< "map_Ns" Specular exponent.
			std::string m_EmissiveTexture;		///< "map_Ke".
			std::string m_DisplacementTexture;	///< "disp" / "map_disp", displacement map.
			std::string m_BumpTexture;			///< "bump" / "map_bump", normal map.

		};

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		ObjMaterialFile( );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ MTL from memory.
		///
		/// \param p_pMemory Pointer to the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMemory( const std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ MTL file from stream.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromStream( std::istream & p_Stream );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ MTL from file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ MTL to memory.
		///
		/// \param p_pMemory Reference to the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToMemory( std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ MTL to stream.
		///
		/// \param p_Stream Output stream.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToStream( std::ostream & p_Stream );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ MTL to file.
		///
		/// \param p_Filename Output file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the OBJ data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Add material.
		///
		/// \param p_Material Material to be copied to the material file class.
		///
		////////////////////////////////////////////////////////////////
		void AddMaterial( const Material & p_Material );

		////////////////////////////////////////////////////////////////
		/// \brief Delete material. Nothing happends if the index is invalid.
		///
		/// \param p_Index Index of the material.
		///
		////////////////////////////////////////////////////////////////
		void DeletedMaterial( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get material.
		///
		////////////////////////////////////////////////////////////////
		const Material & GetMaterial( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get material count.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetMaterialCount( ) const;		

	private:

		// Private typedefs
		typedef std::vector<Material *> MaterialVector;

		// Private varaibles
		MaterialVector m_Materials;		///< Vector of all the materials in the file.
		static const Material s_DefaultMaterial;

	};

}

#endif