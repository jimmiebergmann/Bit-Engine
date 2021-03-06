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

#ifndef BIT_NON_COPYABLE_HPP
#define BIT_NON_COPYABLE_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Base
	/// \brief Obj model file class.
	///
	/// Publicly inherit by any class in order to make it non copyable.
	/// Example:
	///		class ClassA : public NonCopyable { };
	///
	////////////////////////////////////////////////////////////////
	class BIT_API NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		NonCopyable( )
		{
		}

	private:

		////////////////////////////////////////////////////////////////
		/// \brief Making the copy constructor private.
		///
		////////////////////////////////////////////////////////////////
		NonCopyable( const NonCopyable & p_Copy );

		////////////////////////////////////////////////////////////////
		/// \brief Making the assignment constructor private.
		///
		////////////////////////////////////////////////////////////////
		NonCopyable & operator = ( const NonCopyable & p_Copy );

	};

}

#endif