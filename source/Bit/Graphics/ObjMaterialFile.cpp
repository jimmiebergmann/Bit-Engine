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
#include <Bit/System/Log.hpp>
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
											const std::string p_AmbientTexture,
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
		m_AmbientTexture( p_AmbientTexture ),
		m_DiffuseTexture( p_DiffuseTexture ),
		m_SpecularTexture( p_SpecularTexture ),
		m_ShininessTexture( p_ShininessTexture ),
		m_EmissiveTexture( p_EmissiveTexture ),
		m_DisplacementTexture( p_DisplacementTexture ),
		m_BumpTexture( p_BumpTexture )
	{
	}

	void ObjMaterialFile::Material::SetName( const std::string & p_Name )
	{
		m_Name = p_Name;
	}

	void ObjMaterialFile::Material::SetIllum( const Uint8 p_Illum )
	{
		m_Illum = p_Illum;
	}

	void ObjMaterialFile::Material::SetAmbientColor( const Vector3f32 & p_AmbientColor )
	{
		m_AmbientColor = p_AmbientColor;
	}

	void ObjMaterialFile::Material::SetDiffuseColor( const Vector3f32 & p_DiffuseColor )
	{
		m_DiffuseColor = p_DiffuseColor;
	}

	void ObjMaterialFile::Material::SetSpecularColor( const Vector3f32 & p_SpecularColor )
	{
		m_SpecularColor = p_SpecularColor;
	}

	void ObjMaterialFile::Material::SetEmissiveColor( const Vector3f32 & p_EmissiveColor )
	{
		m_EmissiveColor = p_EmissiveColor;
	}
	
	void ObjMaterialFile::Material::SetShininess( const Float32 p_Shininess )
	{
		m_Shininess = p_Shininess;
	}

	void ObjMaterialFile::Material::SetOpticalDensity( const Float32 p_OpticalDensity )
	{
		m_OpticalDensity = p_OpticalDensity;
	}

	void ObjMaterialFile::Material::SetAmbientTexture( const std::string & p_AmbientTexture )
	{
		m_AmbientTexture = p_AmbientTexture;
	}

	void ObjMaterialFile::Material::SetDiffuseTexture( const std::string & p_DiffuseTexture )
	{
		m_DiffuseTexture = p_DiffuseTexture;
	}

	void ObjMaterialFile::Material::SetSpecularTexture( const std::string & p_SpecularTexture )
	{
		m_SpecularTexture = p_SpecularTexture;
	}

	void ObjMaterialFile::Material::SetShininessTexture( const std::string & p_ShininessTexture )
	{
		m_ShininessTexture = p_ShininessTexture;
	}

	void ObjMaterialFile::Material::SetEmissiveTexture( const std::string & p_EmissiveTexture )
	{
		m_EmissiveTexture = p_EmissiveTexture;
	}

	void ObjMaterialFile::Material::SetDisplacementTexture( const std::string & p_DisplacementTexture )
	{
		m_DisplacementTexture = p_DisplacementTexture;
	}

	void ObjMaterialFile::Material::SetBumpTexture( const std::string & p_BumpTexture )
	{
		m_BumpTexture = p_BumpTexture;
	}

	const std::string & ObjMaterialFile::Material::GetName( ) const
	{
		return m_Name;
	}

	Uint8 ObjMaterialFile::Material::GetIllum( const Uint8 ) const
	{
		return m_Illum;
	}

	const Vector3f32 & ObjMaterialFile::Material::GetAmbientColor( ) const
	{
		return m_AmbientColor;
	}

	const Vector3f32 & ObjMaterialFile::Material::GetDiffuseColor( ) const
	{
		return m_DiffuseColor;
	}

	const Vector3f32 & ObjMaterialFile::Material::GetSpecularColor( ) const
	{
		return m_SpecularColor;
	}

	const Vector3f32 & ObjMaterialFile::Material::GetEmissiveColor( ) const
	{
		return m_EmissiveColor;
	}

	Float32 ObjMaterialFile::Material::GetShininess( ) const
	{
		return m_Shininess;
	}

	Float32 ObjMaterialFile::Material::GetOpticalDensity( ) const
	{
		return m_OpticalDensity;
	}

	const std::string & ObjMaterialFile::Material::GetAmbientTexture( ) const
	{
		return m_AmbientTexture;
	}

	const std::string & ObjMaterialFile::Material::GetDiffuseTexture( ) const
	{
		return m_DiffuseTexture;
	}

	const std::string & ObjMaterialFile::Material::GetSpecularTexture( ) const
	{
		return m_SpecularTexture;
	}

	const std::string & ObjMaterialFile::Material::GetShininessTexture( ) const
	{
		return m_ShininessTexture;
	}

	const std::string & ObjMaterialFile::Material::GetEmissiveTexture( ) const
	{
		return m_EmissiveTexture;
	}

	const std::string & ObjMaterialFile::Material::GetDisplacementTexture( ) const
	{
		return m_DisplacementTexture;
	}

	const std::string & ObjMaterialFile::Material::GetBumpTexture( ) const
	{
		return m_BumpTexture;
	}


	// Material file class
	ObjMaterialFile::ObjMaterialFile( )
	{
	}

	ObjMaterialFile::~ObjMaterialFile( )
	{
		Clear( );
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
		Vector3f32 vector3;
		Int32 integer;
		const SizeType maxLineLength = 128;
		char line[ maxLineLength ];
		keyword.reserve( 32 );
		string.reserve( 64 );
		Material * pMaterial = NULL;

		// Keep on reading until we reach the end of the stream.
		while( p_Stream.eof( ) == false )
		{
			// Read the keyword
			p_Stream >> keyword;

			if( p_Stream.eof( ) == true )
			{
				break;
			}

			// Error check the keyword and ignore comments.
			if( keyword.size( ) == 0 )
			{
				continue;
			}

			// ignore comments
			else if( keyword[ 0 ] == '#' )
			{
				std::getline( p_Stream, string );
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

			// Check all the other possible keywords.
			if( keyword == "illium" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%i", &integer ) != 1 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_Illum = integer;
			}
			else if( keyword == "ka" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f %f %f", &vector3.x, &vector3.y, &vector3.z ) != 3 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_AmbientColor = vector3;

			}
			else if( keyword == "kd" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f %f %f", &vector3.x, &vector3.y, &vector3.z ) != 3 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_DiffuseColor = vector3;
			}
			else if( keyword == "ks" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f %f %f", &vector3.x, &vector3.y, &vector3.z ) != 3 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_SpecularColor = vector3;
			}
			else if( keyword == "ke" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f %f %f", &vector3.x, &vector3.y, &vector3.z ) != 3 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_EmissiveColor = vector3;
			}
			else if( keyword == "ns" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f", &vector3.x ) != 1 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_Shininess = vector3.x;
			}
			else if( keyword == "ni" )
			{
				// Get the line and scan the data
				p_Stream.getline( line, maxLineLength );
				if( sscanf( line, "%f", &vector3.x ) != 1 )
				{
					continue;
				}

				// Set the value
				pMaterial->m_OpticalDensity = vector3.x;
			}
			// Texture maps
			else if( keyword == "map_ka" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_AmbientTexture = string;
			}
			else if( keyword == "map_kd" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_DiffuseTexture = string;
			}
			else if( keyword == "map_ks" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_SpecularTexture = string;
			}
			else if( keyword == "map_ns" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_ShininessTexture = string;
			}
			else if( keyword == "map_ke" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_EmissiveTexture = string;
			}
			else if( keyword == "disp" || keyword == "map_disp" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_DisplacementTexture = string;
			}
			else if( keyword == "bump" || keyword == "map_bump" )
			{
				p_Stream.get();
				std::getline(p_Stream, string);
				pMaterial->m_BumpTexture = string;
			}

		}

		// Everything is fine
		return true;
	}

	Bool ObjMaterialFile::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ) );
		if( fin.is_open( ) == false )
		{
			bitLogGraErr(  "Can not open the file." );
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
			bitLogGraErr(  "Can not open the file." );
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