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

#include <Bit/System/Path.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	std::string Path::GetDirectory(const std::string & p_Filepath)
	{
		// Find the last "/" or "\"
		Bool foundSlash = false;
		SizeType slashPosition = 0;
		for (Int32 i = p_Filepath.size() - 1; i >= 0; i--)
		{
			if (p_Filepath[i] == '/' || p_Filepath[i] == '\\')
			{
				foundSlash = true;
				slashPosition = static_cast<SizeType>(i);
				break;
			}
		}

		// Return empty string if no directory were found.
		if (foundSlash == false)
		{
			return "";
		}

		// Get the directory
		return p_Filepath.substr(0, slashPosition);
	}

}