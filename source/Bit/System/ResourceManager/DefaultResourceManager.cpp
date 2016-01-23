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

#include <Bit/System/ResourceManager/DefaultResourceManager.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/Texture.hpp>
#include <Bit/System/Log.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	namespace Private
	{

		DefaultResourceManager::DefaultResourceManager( ) :
			m_pGraphicDevice( NULL )
		{
		}

		DefaultResourceManager::~DefaultResourceManager( )
		{
			Release( );
		}
		
		void DefaultResourceManager::Release( )
		{
			ReleaseTextures( );
		}
			
		void DefaultResourceManager::ReleaseTextures( )
		{
			// Go through and delete all the textures.
			for( TextureIterator it = m_Textures.begin( ); it != m_Textures.end( ); it++ )
			{
				delete it->second;
			}
			m_Textures.clear( );
		}

		void DefaultResourceManager::SetGraphicDevice( GraphicDevice * p_pGraphicDevice )
		{
			m_pGraphicDevice = p_pGraphicDevice;
		}

		Texture * DefaultResourceManager::GetTexture(	const std::string & p_FilePath,
														const Bool p_PropertyInitializing,
														const Bool p_Mipmapping )
		{
			// Find the texture
			TextureIterator it = m_Textures.find( p_FilePath );

			// Check if we've already found the texture
			if( it == m_Textures.end( ) )
			{
				if( m_pGraphicDevice == NULL )
				{
					BitLog::NewEngine(Log::Error,  "No attached graphic device." );
					return NULL;
				}

				// Create the texture
				Texture * pTexture = m_pGraphicDevice->CreateTexture( );
				if( pTexture == NULL )
				{
					return NULL;
				}

				// Get the mipmapping status
				bool mipmapping = false;
				if( p_PropertyInitializing )
				{
					// Get the default mipmapping property from the graphic device.
					mipmapping = m_pGraphicDevice->GetDefaultTextureProperties( ).GetMipmapping( );
				}
				else
				{
					mipmapping = p_Mipmapping;
				}

				// Load the texture
				if( pTexture->LoadFromFile( p_FilePath.c_str( ), mipmapping ) == false )
				{
					// Could not load the texture
					return NULL;
				}

				// Initialize the properties
				if( p_PropertyInitializing )
				{
					// Get the property reference.
					TextureProperties & properties = m_pGraphicDevice->GetDefaultTextureProperties( );

					// Set all the properties
					pTexture->GetProperties( ).SetMagnificationFilter( properties.GetMagnificationFilter( ) );
					pTexture->GetProperties( ).SetMinificationFilter( properties.GetMinificationFilter( ) );
					pTexture->GetProperties( ).SetWrappingX( properties.GetWrappingX( ) );
					pTexture->GetProperties( ).SetWrappingY( properties.GetWrappingY( ) );
					pTexture->GetProperties( ).SetMipmapping( properties.GetMipmapping( ) );
					pTexture->GetProperties( ).SetAnisotropic( properties.GetAnisotropic( ) );

					// Apply the properties
					pTexture->ApplyProperties( );
				}

				// Add the texture to the texture map
				m_Textures[ p_FilePath ] = pTexture;

				// return the texture
				return pTexture;
			}

			// Return the texture pointer
			return it->second;
		}

		GraphicDevice * DefaultResourceManager::GetGraphicDevice( ) const
		{
			return m_pGraphicDevice;
		}

	}

}

