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

#ifndef BIT_GRAPHICS_MODEL_MATERIAL_HPP
#define BIT_GRAPHICS_MODEL_MATERIAL_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Json/Value.hpp>

namespace Bit
{

	// Forward declarations
	class Texture;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model material typedef.
	///
	/// Default property names:
	///		- Color			( RGB : 0.0f - 1.0f each channel )
	///		- Opacity		( 0.0f - 1.0f )
	///		- Shininess		( 0.0f - 1.0f )
	///		- ColorMap		( Color texture path )
	///		- NormalMap		( Normal Texture path)
	///		- SpecularMap	( Specular Texture path);
	///
	/// Supporting 3 default textures:
	///		- Color
	///		- Normal
	///		- Specular
	/// Use the extension function for up to 16 extra textures.
	/// The extension area uses a bitmask to let the user keep track
	/// of what kind of extended textures the material contain.
	///
	/// \see Model
	///
	////////////////////////////////////////////////////////////////
	class ModelMaterial : public Json::Value
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default .
		///
		////////////////////////////////////////////////////////////////
		ModelMaterial( );

		////////////////////////////////////////////////////////////////
		/// \brief Set color texture.
		///
		////////////////////////////////////////////////////////////////
		void SetColorTexture( Texture * p_pTexture );

		////////////////////////////////////////////////////////////////
		/// \brief Set normal texture.
		///
		////////////////////////////////////////////////////////////////
		void SetNormalTexture( Texture * p_pTexture );

		////////////////////////////////////////////////////////////////
		/// \brief Set specular texture.
		///
		////////////////////////////////////////////////////////////////
		void SetSpecularTexture( Texture * p_pTexture );

		////////////////////////////////////////////////////////////////
		/// \brief Get color texture.
		///
		////////////////////////////////////////////////////////////////
		Texture * GetColorTexture( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get normal texture.
		///
		////////////////////////////////////////////////////////////////
		Texture * GetNormalTexture( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief GSet specular texture.
		///
		////////////////////////////////////////////////////////////////
		Texture * GetSpecularTexture( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Add extended texture to material.
		///
		/// \param p_pTexture Pointer of the texture to add.
		/// \param p_FlagIndex Index of the flag. This is an index and not a bitmask.
		///
		/// \return False if the material is full of textures(max 16),
		///			or if the flag index already exists, else true.
		///
		////////////////////////////////////////////////////////////////
		Bool AddExtendedTexture( Texture * p_pTexture, const SizeType p_FlagIndex );

		////////////////////////////////////////////////////////////////
		/// \brief Get number of extended textures.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetExtendedTextureCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get extended textures by index.
		///
		////////////////////////////////////////////////////////////////
		Texture * GetExtendedTexture( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get texture flags
		///
		////////////////////////////////////////////////////////////////
		Uint16 GetTextureFlags( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Default error material.
		///
		////////////////////////////////////////////////////////////////
		static const ModelMaterial ErrorMaterial;

	private:

		// Private typedefs
		typedef std::vector<Texture *> TextureVector;

		// Private variables
		Texture * m_pColorTexture;
		Texture * m_pNormalTexture;
		Texture * m_pSpecularTexture;
		TextureVector m_ExtendedTextures;
		Uint16 m_Flags;


	};

}

#endif
