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


#include <Bit/Window/VideoMode.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	VideoMode::VideoMode( ) :
		m_Size( 0, 0 ),
		m_BitsPerPixel( 0 )
	{
	}

	VideoMode::VideoMode( const Vector2u32 & p_Size, const Uint32 p_BitsPerPixel ) :
		m_Size( p_Size ),
		m_BitsPerPixel( p_BitsPerPixel )
	{
	}

	void VideoMode::SetSize( const Vector2u32 & p_Size )
	{
		m_Size = p_Size;
	}

	void VideoMode::SetBitsPerPixel( const Uint32 p_BitsPerPixel )
	{
		m_BitsPerPixel = p_BitsPerPixel;
	}

	Vector2u32 VideoMode::GetSize( ) const
	{
		return m_Size;
	}

	Uint32 VideoMode::GetBitsPerPixel( ) const
	{
		return m_BitsPerPixel;
	}

}