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


#ifndef __BIT_SYSTEM_RESOURCE_MANAGER_HPP__
#define __BIT_SYSTEM_RESOURCE_MANAGER_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <hash_map>
#include <string>

namespace Bit
{

	class ResourceManager
	{

	public:

		// Public functions
		static BIT_UINT32 Initialize( GraphicDevice * p_pGraphicDevice,
			Texture::eFilter * p_DefaultTextureFilters );
		static void Release( );
		
		// Get functions
		static GraphicDevice * GetGraphicDevice( );
		static Texture * GetTexture( const std::string p_FilePath );
		static Texture * GetTexture( const std::string p_FilePath, const Texture::eFilter * p_pTextureFilters );
		static Texture * GetTexture( const std::string p_FilePath, const BIT_BOOL p_Mipmapping );
		static Texture * GetTexture( const std::string p_FilePath, const Texture::eFilter * p_pTextureFilters, const BIT_BOOL p_Mipmapping  );
		static Texture * GetErrorTexture( );

	private:

		// Private functions
		static BIT_UINT32 LoadErrorTexture( );

		// Private typedefs
		typedef stdext::hash_map <std::string, Texture *> TextureMap;
		typedef TextureMap::iterator TextureIterator;

		// Private variables
		static BIT_BOOL m_Initialized;
		static GraphicDevice * m_pGraphicDevice;
		static TextureMap m_Textures;
		static Texture::eFilter * m_DefaultTextureFilters;
		static Texture * m_ErrorTexture;

	};

}

#endif