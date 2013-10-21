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

#ifndef __BIT_GRAPHICS_TEXTURE_HPP__
#define __BIT_GRAPHICS_TEXTURE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/Image.hpp>

namespace Bit
{
	class BIT_API Texture
	{

	public:

		// Public enum
		enum eFilter
		{
			Filter_None = 0,

			Filter_Mag = 1,
			Filter_Min = 2,
			Filter_Wrap_X = 3,
			Filter_Wrap_Y = 4,

			Filter_Nearest = 5,
			Filter_Linear = 6,
			Filter_Nearest_Mipmap = 7,
			Filter_Linear_Mipmap = 8,
			Filter_Repeat = 9,
			Filter_Clamp = 10
		};

		// Destructor
		virtual ~Texture( ) { }

		virtual BIT_UINT32 Load( const Image & p_Image, const BIT_BOOL p_Mipmapping ) = 0;
		virtual BIT_UINT32 Load( Vector2_ui32 p_Size, const eColorComponent p_Format,
			const eColorComponent p_InternalFormat, const eDataType p_FormatType, void * p_Data ) = 0;
		virtual void Bind( BIT_UINT32 p_Index ) = 0;
		virtual void Unbind( ) = 0;

		// Set functions
		virtual BIT_UINT32 SetFilters( const eFilter * p_pFilters ) = 0;

		// Get functions
		BIT_INLINE Vector2_ui32 GetSize( ) const { return m_Size; };
		BIT_INLINE BIT_UINT32 GetFormat( ) const { return m_Format; };
		BIT_INLINE BIT_BOOL IsLoaded( ) const { return m_Loaded; };

	protected:

		// Protexted variables
		BIT_BOOL m_Loaded;
		Vector2_ui32 m_Size;
		BIT_UINT32 m_Format;

	};
}



#endif
