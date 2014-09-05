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
	/// Supporting up to 16 textures for the bitmask.
	/// The following indices are the default values of the bitmask:
	///		- 0: Color
	///		- 1: Normal
	///		- 2: Specular
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
		/// \brief Add texture to material.
		///
		/// \param p_pTexture Pointer of the texture to add.
		/// \param p_FlagIndex Index of the flag. This is an index and not a bitmask.
		///
		/// \return False if the material is full of textures(max 16),
		///			or if the flag index already exists, else true.
		///
		////////////////////////////////////////////////////////////////
		Bool AddTexture( Texture * p_pTexture, const SizeType p_FlagIndex );

		////////////////////////////////////////////////////////////////
		/// \brief Get number of textures.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetTextureCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get textures by index.
		///
		////////////////////////////////////////////////////////////////
		Texture * GetTexture( const SizeType p_Index ) const;

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
		TextureVector m_Textures;
		Uint16 m_Flags;


	};

}

#endif
