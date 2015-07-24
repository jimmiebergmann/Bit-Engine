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

#ifndef BIT_GRAPHICS_RENDERER_HPP
#define BIT_GRAPHICS_RENDERER_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>

namespace Bit
{

	// Forward declarations
	class Drawable;
	class GraphicDevice;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Renderer class. 
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Renderer : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		Renderer(GraphicDevice * p_pGraphicDevice);

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~Renderer();

		////////////////////////////////////////////////////////////////
		/// \brief Draw any drawable object.
		///
		////////////////////////////////////////////////////////////////
		void Draw(Drawable & p_Drawable);

		////////////////////////////////////////////////////////////////
		/// \brief Draw any drawable object.
		///
		////////////////////////////////////////////////////////////////
		GraphicDevice * GetGraphicDevice() const;

	private:

		// Private variables
		GraphicDevice *  m_pGraphicDevice;

	};

}

#endif