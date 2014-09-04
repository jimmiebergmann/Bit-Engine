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


#ifndef BIT_SYSTEM_DEFAULT_RESOURCE_MANAGER_HPP
#define BIT_SYSTEM_DEFAULT_RESOURCE_MANAGER_HPP

#include <Bit/Build.hpp>
#include <Bit/System/ResourceManager.hpp>
#include <string>
#include <unordered_map>

namespace Bit
{

	namespace Private
	{

		////////////////////////////////////////////////////////////////
		/// \ingroup System
		/// \brief Resource manager class.
		///
		/// Get the default resource manager by calling the function GetDefault.
		/// You will have to manually set the audio/graphic device
		/// for any resource manager(including the default one) in order
		/// of being able to get audio and textures.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API DefaultResourceManager : public ResourceManager
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor.
			///
			////////////////////////////////////////////////////////////////
			DefaultResourceManager( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor.
			///
			////////////////////////////////////////////////////////////////
			virtual ~DefaultResourceManager( );

			////////////////////////////////////////////////////////////////
			/// \brief Release the entire resource manager,
			///		including audio and textures.
			///
			////////////////////////////////////////////////////////////////
			virtual void Release( );

			////////////////////////////////////////////////////////////////
			/// \brief Release all textures.
			///
			////////////////////////////////////////////////////////////////
			virtual void ReleaseTextures( );

			////////////////////////////////////////////////////////////////
			/// \brief Set the graphic device.
			///		This is required in order to get textures.
			///
			/// \param p_pGraphicDevice Pointer of the graphic device.
			///
			////////////////////////////////////////////////////////////////
			virtual void SetGraphicDevice( GraphicDevice * p_pGraphicDevice );

			////////////////////////////////////////////////////////////////
			/// \brief Get texture by file path.
			///
			/// \param p_FilePath Path of the the texture.
			/// \param p_PropertyInitializing Initialize any new texture with default properties
			///								and ignores the p_Mipmapping param if true.
			/// \param p_Mipmapping Generating mipmaps for the texture if true.
			///
			////////////////////////////////////////////////////////////////
			virtual Texture * GetTexture(	const std::string & p_FilePath,
											const Bool p_PropertyInitializing = true,
											const Bool p_Mipmapping = false );

			////////////////////////////////////////////////////////////////
			/// \brief Get the graphic device.
			///
			////////////////////////////////////////////////////////////////
			virtual GraphicDevice * GetGraphicDevice( ) const;

		private:

			// Private typedefs
			typedef std::unordered_map <std::string, Bit::Texture *> TextureMap;
			typedef TextureMap::iterator TextureIterator;

			// Private variables
			GraphicDevice * m_pGraphicDevice;	///< Pointer of the graphic device.
			TextureMap m_Textures;				///< Map of the textures.

		};

	}

}


#endif
