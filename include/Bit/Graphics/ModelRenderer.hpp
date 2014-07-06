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
	class Shader;
	class ShaderProgram;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model renderer base class.
	///
	/// A great way for creating your own model renderer with your own custom
	/// shader(maybe with custom light sources?) is to inherit this class
	/// and overload any virtual function you would like to replace
	/// and keep the other ones that does the job for you.
	///
	/// \see Model
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ModelRenderer
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		ModelRenderer( const GraphicDevice & p_GraphicDevice );

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~ModelRenderer( );

		////////////////////////////////////////////////////////////////
		/// \brief Load model renderer.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Load( );

		////////////////////////////////////////////////////////////////
		/// \brief Unload model renderer.
		///
		////////////////////////////////////////////////////////////////
		virtual void Unload( );

		////////////////////////////////////////////////////////////////
		/// \brief Function for rendering a model with any animation type.
		///
		////////////////////////////////////////////////////////////////
		virtual void Render( Model & p_Model );

	protected:

		////////////////////////////////////////////////////////////////
		/// \brief	Render vertex animation. Uses the AnimationState class from
		///			the model in order to render the right pose.
		///			Interpolates between key frames.
		///
		////////////////////////////////////////////////////////////////
		virtual void RenderVertexAnimation( Model & p_Model );

		////////////////////////////////////////////////////////////////
		/// \brief	Render skeletal animation. Uses the AnimationState class from
		///			the model in order to render the right pose.
		///
		////////////////////////////////////////////////////////////////
		virtual void RenderSkeletalAnimation( Model & p_Model );

		////////////////////////////////////////////////////////////////
		/// \brief Render the initial pose, or "static" model.
		///
		/// This function could be used for static model rendering, such as props.
		///
		////////////////////////////////////////////////////////////////
		virtual void RenderInitialPose( Model & p_Model );

		// Protected variables
		const GraphicDevice & m_GraphicDevice;

	};

}

#endif
