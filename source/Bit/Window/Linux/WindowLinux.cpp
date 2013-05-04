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

#include <Bit/Window/Linux/WindowLinux.hpp>
#include <Bit/System/Debugger.hpp>

namespace Bit
{

	// Constructors/destructors
	WindowLinux::WindowLinux( )
	{
	}

	// Public functions
	BIT_UINT32 WindowLinux::Create( const Vector2_ui32 p_Size, const BIT_UINT32 p_Bits,
		const std::string p_Title )
	{

		return BIT_ERROR;
	}

	BIT_UINT32 WindowLinux::Destroy( )
	{

		return BIT_ERROR;
	}

	BIT_UINT32 WindowLinux::DoEvents( )
	{

		return BIT_ERROR;
	}

	// Set functions
	BIT_UINT32 WindowLinux::SetTitle( std::string p_Title )
	{

		return BIT_ERROR;
	}


	// Private functions



}
