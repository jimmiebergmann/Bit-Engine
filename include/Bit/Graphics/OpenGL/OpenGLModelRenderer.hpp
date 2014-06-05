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

#ifndef BIT_GRAPHICS_OPENGL_MODEL_RENDERER_HPP
#define BIT_GRAPHICS_OPENGL_MODEL_RENDERER_HPP

#include <Bit/Build.hpp>
#include <Bit/Graphics/ModelRenderer.hpp>

namespace Bit
{

	// Forward declarations
	class GraphicDevice;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model renderer base class.
	///
	/// \see Model
	/// \see OpenGLModelRenderer
	///
	////////////////////////////////////////////////////////////////
	class BIT_API OpenGLModelRenderer : public ModelRenderer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor. 
		///
		////////////////////////////////////////////////////////////////
		OpenGLModelRenderer( const GraphicDevice & p_GraphicDevice );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor. 
		///
		////////////////////////////////////////////////////////////////
		~OpenGLModelRenderer( );

		////////////////////////////////////////////////////////////////
		/// \brief Function for rendering a model.
		///
		////////////////////////////////////////////////////////////////
		virtual void Render( const Model & p_Model );
	
	private:

		const GraphicDevice & m_GraphicDevice;
	};

}

#endif
