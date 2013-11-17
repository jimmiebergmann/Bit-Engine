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

#ifndef BIT_GRAPHICS_POST_PROCESSING_BLOOM_OPENGL_HPP
#define BIT_GRAPHICS_POST_PROCESSING_BLOOM_OPENGL_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/PostProcessingBloom.hpp>
#include <Bit/Graphics/VertexObject.hpp>
#include <Bit/Graphics/OpenGL/ShaderProgramOpenGL.hpp>
#include <Bit/Graphics/OpenGL/ShaderOpenGL.hpp>

namespace Bit
{

	class BIT_API PostProcessingBloomOpenGL : public PostProcessingBloom
	{

	public:

		// Constructor/destructor
		PostProcessingBloomOpenGL( ShaderProgram * p_pShaderProgram, Shader * p_pVertexShader, Shader * p_pFragmentShader,
			VertexObject * p_pVertexObject, Texture * p_pTexture );
		virtual ~PostProcessingBloomOpenGL( );

		// Public general functions
		virtual BIT_UINT32 Load( BIT_FLOAT32 p_Power, BIT_UINT32 p_BlurSize, BIT_FLOAT32 p_PixelSize );
		virtual void Process( );

		// Public set functions
		virtual void SetTexture( Texture * p_pTexture );

		// Public get functions
		virtual Texture * GetTexture( ) const;

	private:

		BIT_BOOL m_Loaded;
		VertexObject * m_pVertexObject;
		ShaderProgram * m_pShaderProgram;
		Shader * m_pVertexShader;
		Shader * m_pFragmentShader;
		Texture * m_pTexture;


	};


}

#endif