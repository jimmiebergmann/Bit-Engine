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

#ifndef BIT_GRAPHICS_MODEL_MATERIAL_HPP
#define BIT_GRAPHICS_MODEL_MATERIAL_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <string>

namespace Bit
{

	// Forward declaractions
	class GraphicDevice;
	class VertexArray;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief 3D model material class.
	///
	/// Default properties:
	///		- Color			( RGB : 0.0f - 1.0f each channel )
	///		- Opacity		( 0.0f - 1.0f )
	///		- Shininess		( 0.0f - 1.0f )
	///		- ColorMap		( Color texture )
	///		- NormalMap		( Normal Texture )
	///		- SpecularMap	( Specular Texture );
	///
	/// \see Model
	///
	////////////////////////////////////////////////////////////////
	class BIT_API ModelMaterial/* : public NonCopyable*/
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default properties enumerator.
		///
		////////////////////////////////////////////////////////////////
		/*enum eDefaultProperties
		{
			Color
		};*/

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		ModelMaterial( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////////
		~ModelMaterial( );



	private:

		// Private variables
		//Uint8

	};

}

#endif
