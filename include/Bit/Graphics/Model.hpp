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

#ifndef BIT_GRAPHICS_MODEL_HPP
#define BIT_GRAPHICS_MODEL_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/VertexObject.hpp>
#include <Bit/Graphics/Texture.hpp>

namespace Bit
{

	class BIT_API Model
	{

	public:

		// Public enum
		enum eModelType
		{
			Model_None = 1,
			Model_OBJ = 2
		};

		// Destructor
		virtual ~Model( ) { }

		// Virtual public functions
		virtual BIT_UINT32 ReadFile( const char * p_pFilePath ) = 0;
		virtual BIT_UINT32 Load( const BIT_UINT32 m_VertexElementBits, const Texture::eFilter * p_pTextureFilters,
			const BIT_BOOL p_Mipmapping ) = 0;
		virtual void Unload( ) = 0;
		virtual void Render( const VertexObject::eRenderMode p_Mode ) = 0;

		// Virtual get functions
		virtual std::string GetName( ) const = 0;
		virtual BIT_UINT32 GetVertexGroupCount( ) const = 0;
		virtual BIT_UINT32 GetTriangleCount( ) const = 0;
		virtual BIT_UINT32 GetPositionCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetTextureCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetNormalCoordinateCount( ) const = 0;
		virtual BIT_UINT32 GetTextureCount( ) const = 0;
		virtual BIT_UINT32 GetAnimationCount( ) const = 0;
		virtual BIT_BOOL ContainsRagdoll( ) const = 0;

		// Public inline get functions
		BIT_INLINE BIT_BOOL IsLoaded( ) { return m_Loaded; }
		BIT_INLINE eModelType GetType( ) { return m_Type; }

	protected:

		BIT_BOOL m_Loaded;
		eModelType m_Type;

	};

}

#endif
