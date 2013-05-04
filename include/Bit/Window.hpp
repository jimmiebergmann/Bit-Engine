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

#ifndef __VML_WINDOW_HPP__
#define __VML_WINDOW_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Window/WindowBase.hpp>

namespace Bit
{

	class Window
	{

	public:

		// Constructors/destructors
		Window( );
		~Window( );

		// Public general functions
		BIT_UINT32 Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height, const BIT_UINT32 p_Bits );
		BIT_UINT32 Create( const BIT_UINT32 p_Width, const BIT_UINT32 p_Height, const BIT_UINT32 p_Bits,
			const std::string p_Title );
		BIT_UINT32 Destroy( );
		BIT_UINT32 DoEvents( );
		BIT_BOOL IsCreated( ) const;
		BIT_BOOL PollEvent( Event & p_Event );

		// Get functions
		// ...

		// Set functions
		BIT_UINT32 SetTitle( std::string p_Title );
		
	private:

		// Private variables
		WindowBase * m_pWindowBase;

	};

}


#endif