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

#include <Bit/Graphics/ModelMaterial.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Static declarations of the model material class
	const ModelMaterial ModelMaterial::ErrorMaterial;

	// Model material class.
	ModelMaterial::ModelMaterial( ) :
		m_DiffuseColor( 1.0f, 1.0f, 1.0f, 1.0f ),
		m_pColorTexture( NULL ),
		m_pNormalTexture( NULL ),
		m_pSpecularTexture( NULL ),
		m_Flags( 0 )
	{
	}

	void ModelMaterial::SetDiffuseColor(const Vector4f32 & p_Color)
	{
		m_DiffuseColor = p_Color;
	}

	void ModelMaterial::SetColorTexture( Texture * p_pTexture )
	{
		m_pColorTexture = p_pTexture;
	}

	void ModelMaterial::SetNormalTexture( Texture * p_pTexture )
	{
		m_pNormalTexture = p_pTexture;
	}

	void ModelMaterial::SetSpecularTexture( Texture * p_pTexture )
	{
		m_pSpecularTexture = p_pTexture;
	}

	Texture * ModelMaterial::GetColorTexture( ) const
	{
		return m_pColorTexture;
	}

	const Vector4f32 & ModelMaterial::GetDiffuseColor() const
	{
		return m_DiffuseColor;
	}

	Texture * ModelMaterial::GetNormalTexture( ) const
	{
		return m_pNormalTexture;
	}
	
	Texture * ModelMaterial::GetSpecularTexture( ) const
	{
		return m_pSpecularTexture;
	}

	Bool ModelMaterial::AddExtendedTexture( Texture * p_pTexture, const SizeType p_FlagIndex )
	{
		// Make sure that we don't have too many textures.
		if( m_ExtendedTextures.size( ) == 16 )
		{
			return false;
		}

		// Error check the flag index
		if( p_FlagIndex >= 16 )
		{
			return false;
		}

		// Create the bitmask out of the flag index.
		Uint16 bitmask = 1;
		bitmask <<= static_cast<SizeType>( p_FlagIndex );

		// Check if the texture type already exists
		if( m_Flags & bitmask )
		{
			return false;
		}

		// Now, everything is fine, so let's add the texture and apply the bitmask.
		m_ExtendedTextures.push_back( p_pTexture );
		m_Flags |= bitmask;

		return true;
	}

	SizeType ModelMaterial::GetExtendedTextureCount( ) const
	{
		return static_cast<SizeType>( m_ExtendedTextures.size( ) );
	}

	Texture * ModelMaterial::GetExtendedTexture( const SizeType p_Index ) const
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_ExtendedTextures.size( ) ) )
		{
			return NULL;
		}

		// Return the texture pointer
		return m_ExtendedTextures[ p_Index ];
	}

	Uint16 ModelMaterial::GetTextureFlags( ) const
	{
		return m_Flags;
	}

}