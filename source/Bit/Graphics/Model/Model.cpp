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

#include <Bit/Graphics/Model.hpp>
#include <Bit/System.hpp>
#include <Bit/Graphics/Model/ModelOBJ.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Model creating functions
	/*BIT_API StaticModel * CreateStaticModel( const char * p_pFilePath )
	{
		// Get the file's extension
		std::string Extension = GetFileExtension( p_pFilePath );

		// Compare the extensions
		if( Extension == "OBJ" )
		{
			return new ModelOBJ( );
		}

		return BIT_NULL;
	}

	BIT_API DynamicModel * CreateDynamicModel( const char * p_pFilePath )
	{
		// Get the file's extension
		std::string Extension = GetFileExtension( p_pFilePath );

		// Compare the extensions
		if( Extension == "" )
		{
			return BIT_NULL;
		}

		return BIT_NULL;
	}*/

}