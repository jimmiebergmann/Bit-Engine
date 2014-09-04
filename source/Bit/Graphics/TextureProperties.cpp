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

#include <Bit/Graphics/TextureProperties.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Texture properties class
	TextureProperties::TextureProperties( ) :
		m_MagnificationFilter( Nearest ),	
		m_MinificationFilter( Nearest ),
		m_WrappingX( Repeat ),
		m_WrappingY( Repeat ),
		m_Mipmapping( false ),
		m_AnisotropicLevel( 0 ),
		m_Flags( 0 )
	{
	}

	void TextureProperties::SetMagnificationFilter( const eFilter p_Filter )
	{
		m_MagnificationFilter = p_Filter;
		m_Flags |= 0x01;
	}

	void TextureProperties::SetMinificationFilter( const eFilter p_Filter )
	{
		m_MinificationFilter = p_Filter;
		m_Flags |= 0x02;
	}

	void TextureProperties::SetWrapping( const eWarpping p_WrapX, const eWarpping p_WrapY )
	{
		m_WrappingX = p_WrapX;
		m_WrappingY = p_WrapY;
		m_Flags |= 0x0c; /// 0x04 + 0x08
	}

	void TextureProperties::SetWrappingX( const eWarpping p_WrapX )
	{
		m_WrappingX = p_WrapX;
		m_Flags |= 0x04;
	}

	void TextureProperties::SetWrappingY( const eWarpping p_WrapY )
	{
		m_WrappingY = p_WrapY;
		m_Flags |= 0x08;
	}

	void TextureProperties::SetMipmapping( Bool p_Status )
	{
		m_Mipmapping = p_Status;
	}

	void TextureProperties::SetAnisotropic( const Uint32 p_Level )
	{
		m_AnisotropicLevel = p_Level;
		m_Flags |= 0x10;
	}

	void TextureProperties::SetFlags( const Uint8 p_Flags )
	{
		m_Flags = p_Flags;
	}

	TextureProperties::eFilter TextureProperties::GetMagnificationFilter( ) const	
	{
		return m_MagnificationFilter;
	}

	TextureProperties::eFilter TextureProperties::GetMinificationFilter( ) const
	{
		return m_MinificationFilter;
	}

	TextureProperties::eWarpping TextureProperties::GetWrappingX( ) const
	{
		return m_WrappingX;
	}

	TextureProperties::eWarpping TextureProperties::GetWrappingY( ) const
	{
		return m_WrappingY;
	}

	Bool TextureProperties::GetMipmapping( ) const
	{
		return m_Mipmapping;
	}

	Uint32 TextureProperties::GetAnisotropic( ) const
	{
		return m_AnisotropicLevel;
	}

	Uint8 TextureProperties::GetFlags( ) const
	{
		return m_Flags;
	}

}