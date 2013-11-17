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

#include <Bit/System/ResourceManager.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/Image.hpp>
//#include <Bit/Audio/XM.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Set the private static variables
	BIT_BOOL ResourceManager::m_Initialized = BIT_FALSE;
	GraphicDevice * ResourceManager::m_pGraphicDevice = BIT_NULL;
	ResourceManager::TextureMap ResourceManager::m_Textures;
	Texture::eFilter * ResourceManager::m_DefaultTextureFilters = BIT_NULL;
	Texture * ResourceManager::m_ErrorTexture = BIT_NULL;

	// Public functions
	BIT_UINT32 ResourceManager::Initialize( GraphicDevice * p_pGraphicDevice,
		Texture::eFilter * p_DefaultTextureFilters )
	{
		// Release old data
		Release( );

		// Set the graphic device
		if( !( m_pGraphicDevice = p_pGraphicDevice ) )
		{
			bitTrace( "[ResourceManager::Initialize] NULL graphic device\n" );
			return BIT_ERROR;
		}


		// Set the default texture filters
		if( p_DefaultTextureFilters != BIT_NULL )
		{
			// Find the filter size
			BIT_UINT32 FilterSize = 8;
			for( BIT_MEMSIZE i = 0; i < 8; i++ )
			{
				if( p_DefaultTextureFilters[ i ] == Texture::Filter_None )
				{
					FilterSize = i + 1;
					break;
				}
			}

			// Get rid of grabage data
			if( FilterSize % 2 )
			{
				FilterSize--;
			}

			// Alocate our own filters and set them
			m_DefaultTextureFilters = new Texture::eFilter[ FilterSize + 2 ];
			for( BIT_MEMSIZE i = 0; i < FilterSize; i++ )
			{
				m_DefaultTextureFilters[ i ] = p_DefaultTextureFilters[ i ];
			}

			m_DefaultTextureFilters[ FilterSize ] = Texture::Filter_None;
			m_DefaultTextureFilters[ FilterSize + 1 ] = Texture::Filter_None;
		}

		// Load the error texture
		if( LoadErrorTexture( ) != BIT_OK )
		{
			return BIT_ERROR;
		}

		// Everything went ok
		m_Initialized = BIT_TRUE;
		return BIT_OK;
	}

	void ResourceManager::Release( )
	{
		// Go through and delete all the textures.
		for( TextureIterator it = m_Textures.begin( ); it != m_Textures.end( ); it++ )
		{
			delete it->second;
		}
		m_Textures.clear( );

		// Delete the default texture filters
		if( m_DefaultTextureFilters )
		{
			delete [ ] m_DefaultTextureFilters;
			m_DefaultTextureFilters = BIT_NULL;
		}

		// Clear the graphic device
		m_pGraphicDevice = BIT_NULL;

		// Set the initialized flag to false
		m_Initialized = BIT_FALSE;
	}

	// Get functions
	GraphicDevice * ResourceManager::GetGraphicDevice( )
	{
		return m_pGraphicDevice;
	}

	Texture * ResourceManager::GetTexture( const std::string p_FilePath )
	{
		return GetTexture( p_FilePath, m_DefaultTextureFilters, BIT_FALSE );
	}

	Texture * ResourceManager::GetTexture( const std::string p_FilePath, const Texture::eFilter * p_pTextureFilters )
	{
		return GetTexture( p_FilePath, p_pTextureFilters, BIT_FALSE );
	}

	Texture * ResourceManager::GetTexture( const std::string p_FilePath, const BIT_BOOL p_Mipmapping )
	{
		return GetTexture( p_FilePath, m_DefaultTextureFilters, p_Mipmapping );
	}

	Texture * ResourceManager::GetTexture( const std::string p_FilePath, const Texture::eFilter * p_pTextureFilters, const BIT_BOOL p_Mipmapping  )
	{
		// Find the texture
		TextureIterator it = m_Textures.find( p_FilePath );

		// Check if we've already found the texture
		if( it == m_Textures.end( ) )
		{
			// We have to load the texture. Read the image
			Image ImageFile;

			if( ImageFile.ReadFile( p_FilePath.c_str( ) ) != BIT_OK )
			{
				// We could not read the image file
				return BIT_NULL;
			}

			// Create the texture
			Texture * pTexture = m_pGraphicDevice->CreateTexture( );
			if( pTexture == BIT_NULL )
			{
				return BIT_NULL;
			}

			// Load the texture
			if( pTexture->Load( ImageFile, p_Mipmapping ) != BIT_OK )
			{
				// Could not load the texture
				return BIT_NULL;
			}

			// Set the filters
			if( p_pTextureFilters )
			{
				pTexture->SetFilters( p_pTextureFilters );
			}

			// Add the texture to the texture map
			m_Textures[ p_FilePath ] = pTexture;

			// return the texture
			return pTexture;
		}

		// Return the texture pointer
		return it->second;
	}

	Texture * ResourceManager::GetErrorTexture( )
	{
		return m_ErrorTexture;
	}


	// Private functions
	BIT_UINT32 ResourceManager::LoadErrorTexture( )
	{
		return BIT_OK;
	}



}

