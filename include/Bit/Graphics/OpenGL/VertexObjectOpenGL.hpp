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

#ifndef __BIT_GRAPHICS_VERTEX_OBJECT_OPENGL_HPP__
#define __BIT_GRAPHICS_VERTEX_OBJECT_OPENGL_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/VertexObject.hpp>
#include <Bit/Graphics/OpenGL/OpenGL.hpp>
#include <vector>

namespace Bit
{
	class BIT_API VertexObjectOpenGL : public VertexObject
	{

	public:

		// Making a friend :D
		friend class GraphicDeviceWin32;

		// Destructor
		~VertexObjectOpenGL( );

		// Virtual public functions
		virtual BIT_UINT32 Load( BIT_UINT32 p_PieceCount, BIT_UINT32 p_PieceSize ) ;
		virtual BIT_UINT32 Unload( );
		virtual BIT_UINT32 AddVertexBuffer( void * p_pBuffer, const BIT_UINT32 p_VertexDimensions );
		virtual void Render(eRenderMode p_Mode);

	private:

		// Private constructor. We are just letting the graphic device initialize this class.
		VertexObjectOpenGL( );

		// Private main variables
		GLuint m_VertexArrayObject;
		GLuint * m_pVertexBufferObjects;
		BIT_UINT32 m_VertexBufferObjectCount;

		// Private loading variables
		struct BufferStruct
		{
			void * pBuffer;
			BIT_UINT32 DimensionCount;
		};

		BIT_UINT32 m_TotalPieceSize;	// PieceCount + PieceSize
		std::vector< BufferStruct > m_BufferVector;

		// Static private variables(etg
		static GLenum s_RenderModes[ 3 ];

	};

}

#endif