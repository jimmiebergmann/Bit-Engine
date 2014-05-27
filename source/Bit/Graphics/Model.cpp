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
#include <Bit/Graphics/GraphicDevice.hpp>
#include <iostream>
#include <algorithm>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	Model::Model( const GraphicDevice & p_GraphicDevice ) :
		m_GraphicDevice( p_GraphicDevice )
	{
	}

	Model::~Model( )
	{
	}

	Bool  Model::LoadFromFile( const std::string & p_Filename )
	{
		// Get the file's extension
		std::string fileExtension = "";
		for( SizeType i = p_Filename.size( ) - 2; i >= 0; i-- )
		{
			// Look for '.'
			if( p_Filename[ i ] == '.' )
			{
				fileExtension = p_Filename.substr( i + 1, p_Filename.size( ) - i - 1 );
				break;
			}
		}

		// Make all the characters in the file extension to upper case letters
		std::transform( fileExtension.begin( ), fileExtension.end( ), fileExtension.begin( ), ::toupper );

		// Load the right format.
		if( fileExtension == "Obj" )
		{
			return LoadFromObjFile( p_Filename );
		}
		else if( fileExtension == "MD2" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD2 models yet.\n";
			return false;
		}
		else if( fileExtension == "MD3" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD3 models yet.\n";
			return false;
		}
		else if( fileExtension == "MD5" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MD5 models yet.\n";
			return false;
		}
		else if( fileExtension == "MDL" )
		{
			std::cout << "[Model::LoadFromFile] Not supporting MDL models yet.\n";
			return false;
		}

		// Unknown extension
		std::cout << "[Model::LoadFromFile] Unknow extension: " <<  fileExtension.c_str( ) << std::endl;
		return false;
	}

	Bool  Model::LoadFromObjFile( const std::string & p_Filename )
	{
		return false;
	}

	Skeleton & Model::GetSkeleton( )
	{
		return m_Skeleton;
	}

}