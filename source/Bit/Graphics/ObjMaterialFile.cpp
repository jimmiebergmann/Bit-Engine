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

#include <Bit/Graphics/ObjMaterialFile.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Material file static variables
	const ObjMaterialFile::Material ObjMaterialFile::s_DefaultMaterial;

	// Material class
	ObjMaterialFile::Material::Material(	const std::string p_Name,
											const Uint8 p_Illum,
											const Vector3f32 p_AmbientColor,
											const Vector3f32 p_DiffuseColor,
											const Vector3f32 p_SpecularColor,
											const Vector3f32 p_EmissiveColor,
											const Float32 p_Shininess,
											const Float32 p_OpticalDensity,
											const std::string p_AmbienTexture,
											const std::string p_DiffuseTexture,
											const std::string p_SpecularTexture,
											const std::string p_ShininessTexture,
											const std::string p_EmissiveTexture,
											const std::string p_DisplacementTexture,
											const std::string p_BumpTexture ) :
		m_Name( p_Name ),
		m_Illum( p_Illum ),
		m_AmbientColor( p_AmbientColor ),
		m_DiffuseColor( p_DiffuseColor ),
		m_SpecularColor( p_SpecularColor ),
		m_EmissiveColor( p_EmissiveColor ),
		m_Shininess( p_Shininess ),
		m_OpticalDensity( p_OpticalDensity ),
		m_AmbienTexture( p_AmbienTexture ),
		m_DiffuseTexture( p_DiffuseTexture ),
		m_SpecularTexture( p_SpecularTexture ),
		m_ShininessTexture( p_ShininessTexture ),
		m_EmissiveTexture( p_EmissiveTexture ),
		m_DisplacementTexture( p_DisplacementTexture ),
		m_BumpTexture( p_BumpTexture )
	{
	}

	// Material file class
	ObjMaterialFile::ObjMaterialFile( )
	{
	}

	Bool ObjMaterialFile::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.rdbuf( )->pubsetbuf( const_cast<char*>( p_Memory.c_str( ) ), p_Memory.size( ) );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool ObjMaterialFile::LoadFromStream( std::istream & p_Stream )
	{
		std::string keyword;
		std::string string;
		keyword.reserve( 32 );
		string.reserve( 64 );
		Material * pMaterial = NULL;

		// Keep on reading until we reach the end of the stream.
		while( p_Stream.eof( ) == false )
		{
			// Read the keyword
			p_Stream >> keyword;

			// Error check the keyword and ignore comments.
			if( keyword.size( ) == 0 || keyword[ 0 ] == '#' )
			{
				continue;
			}

			// Turn the keyword into lower case letters.
			std::transform( keyword.begin( ), keyword.end( ), keyword.begin( ), ::tolower );

			// Check if this is a new material.
			if( keyword == "newmtl" )
			{
				// Get the name of the material and error check it.
				p_Stream >> string;
				if( string.size( ) == 0 )
				{
					continue;
				}

				// Create and add the new material.
				pMaterial = new Material( string );
				m_Materials.push_back( pMaterial );
				continue;
			}

			// Check if we've allocated any material yet.
			if( pMaterial == NULL )
			{
				continue;
			}




		}




		return false;
	}

	Bool ObjMaterialFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ) );
		if( fin.is_open( ) == false )
		{
			std::cout << "[ObjMaterialFile::LoadFromFile] Can not open the file. " << std::endl;
			return false;
		}

		Bool status = LoadFromStream( fin );
		fin.close( );


		// Return the status
		return status;
	}

	Bool ObjMaterialFile::SaveToMemory( std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;

		// Save the stream
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Get the string
		p_Memory = ss.str( );

		// Succeeded
		return true;
	}

	Bool ObjMaterialFile::SaveToStream( std::ostream & p_Stream )
	{
		return false;
	}

	Bool ObjMaterialFile::SaveToFile( const std::string & p_Filename )
	{
		// Load a string stream.
		std::stringstream ss;

		// Save the stream.
		if( SaveToStream( ss ) == false )
		{
			return false;
		}

		// Open the file.
		std::ofstream fout( p_Filename.c_str( ), std::fstream::binary );
		if( fout.is_open( ) == false )
		{
			std::cout << "[ObjMaterialFile::SaveToFile] Can not open the file. " << std::endl;
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

		// Succeeded.
		return true;
	}

	void ObjMaterialFile::Clear( )
	{
		// Go throguh the materials
		for( MaterialVector::size_type i = 0; i < m_Materials.size( ); i++ )
		{
			delete m_Materials[ i ];
		}

		// Clear the materials
		m_Materials.clear( );
	}

	void ObjMaterialFile::AddMaterial( const Material & p_Material )
	{
		// Create the new material.
		Material * pMaterial = new Material( "" );
		*pMaterial = p_Material;

		// Add the new material
		m_Materials.push_back( pMaterial );
	}

	void ObjMaterialFile::DeletedMaterial( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Materials.size( ) ) )
		{
			return;
		}

		// Erase the property.
		delete m_Materials[ p_Index ];
		m_Materials.erase( m_Materials.begin( ) + p_Index );
	}

	const ObjMaterialFile::Material & ObjMaterialFile::GetMaterial( const SizeType p_Index ) const
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Materials.size( ) ) )
		{
			return s_DefaultMaterial;
		}

		// Return the property.
		return *m_Materials[ p_Index ];
	}

	SizeType ObjMaterialFile::GetMaterialCount( ) const
	{
		return static_cast<SizeType>( m_Materials.size( ) );
	}

}