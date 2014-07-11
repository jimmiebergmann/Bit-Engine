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
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Material file static variables
	const ObjMaterialFile::Material ObjMaterialFile::s_DefaultMaterial;
	const ObjMaterialFile::Material::Property ObjMaterialFile::Material::s_DefaulPropery;

	// Property class
	ObjMaterialFile::Material::Property::Property( ) :
		Name( "" )
	{
	}

	// Material class
	ObjMaterialFile::Material::Material( ) :
		m_Name( "" )
	{
	}

	void ObjMaterialFile::Material::AddProperty( const Property & p_Property )
	{
		m_Properties.push_back( p_Property );
	}

	void ObjMaterialFile::Material::DeletedProperty( const SizeType p_Index )
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Properties.size( ) ) )
		{
			return;
		}

		// Erase the property.
		m_Properties.erase( m_Properties.begin( ) + p_Index );
	}

	const ObjMaterialFile::Material::Property & ObjMaterialFile::Material::GetProperty( const SizeType p_Index ) const
	{
		// Error check the index.
		if( p_Index >= static_cast<SizeType>( m_Properties.size( ) ) )
		{
			return s_DefaulPropery;
		}

		// Return the property.
		return m_Properties[ p_Index ];
	}

	SizeType ObjMaterialFile::Material::GetPropertyCount( ) const
	{
		return static_cast<SizeType>( m_Properties.size( ) );
	}

	// Material file class
	ObjMaterialFile::ObjMaterialFile( )
	{
	}

	Bool ObjMaterialFile::LoadFromMemory( const std::string & p_Memory )
	{
		return false;
	}

	Bool ObjMaterialFile::LoadFromStream( std::istream & p_Stream )
	{
		return false;
	}

	Bool ObjMaterialFile::LoadFromFile( const std::string & p_Filename )
	{
		return false;
	}

	Bool ObjMaterialFile::SaveToMemory( std::string & p_Memory )
	{
		return false;
	}

	Bool ObjMaterialFile::SaveToStream( std::ostream & p_Stream )
	{
		return false;
	}

	Bool ObjMaterialFile::SaveToFile( const std::string & p_Filename )
	{
		return false;
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
		Material * pMaterial = new Material;
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