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

#ifndef BIT_GRAPHICS_MODEL_RENDERER_HPP
#define BIT_GRAPHICS_MODEL_RENDERER_HPP

#include <Bit/Build.hpp>

namespace Bit
{
	// Forward declaractions
	class Model;
	class GraphicDevice;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model renderer base class.
	///
	/// \see Model
	/// \see OpenGLModelRenderer
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ModelRenderer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor. 
		///
		////////////////////////////////////////////////////////////////
		virtual ~ModelRenderer( );

		////////////////////////////////////////////////////////////////
		/// \brief Load model renderer.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Load( const GraphicDevice & p_GraphicDevice ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Unload model renderer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unload( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Function for rendering a model.
		///
		////////////////////////////////////////////////////////////////
		virtual void Render( Model & p_Model ) = 0;

	protected:

		////////////////////////////////////////////////////////////////
		/// \brief Function for rendering the vertices.
		///	This is a help function for making the rendering procedure easier.
		/// Bind textures and shaders, then use this function.
		///
		////////////////////////////////////////////////////////////////
		virtual void RenderVertices( Model & p_Model );

	};

}

#endif
