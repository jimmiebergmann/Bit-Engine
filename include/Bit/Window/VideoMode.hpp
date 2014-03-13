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

#ifndef BIT_WINDOW_VIDEO_MODE_HPP
#define BIT_WINDOW_VIDEO_MODE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Vector2.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Video mode class for the window class
	///
	////////////////////////////////////////////////////////////////
	class VideoMode
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		VideoMode( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		VideoMode( const Vector2u32 & p_Size, const Uint32 p_BitsPerPixel = 32 );

		////////////////////////////////////////////////////////////////
		/// \brief Set the size
		///
		////////////////////////////////////////////////////////////////
		void SetSize( const Vector2u32 & p_Size );

		////////////////////////////////////////////////////////////////
		/// \brief Set the bits per pixel value
		///
		////////////////////////////////////////////////////////////////
		void SetBitsPerPixel( const Uint32 p_BitsPerPixel );

		////////////////////////////////////////////////////////////////
		/// \brief Get the size
		///
		////////////////////////////////////////////////////////////////
		Vector2u32 GetSize( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the bits per pixel value
		///
		////////////////////////////////////////////////////////////////
		Uint32 GetBitsPerPixel( ) const;


	private:

		Vector2u32	m_Size;
		Uint32		m_BitsPerPixel;

	};

}

#endif