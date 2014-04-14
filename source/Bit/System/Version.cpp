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

#include <Bit/System/Version.hpp>

namespace Bit
{

	// Static version variables
	const Version Version::Default = Version( 0, 0, 0 );

	// Version class
	Version::Version(	const Uint32 p_Major,
						const Uint32 p_Minor,
						const Uint32 p_Revision ) :
		m_Major( p_Major ),
		m_Minor( p_Minor ),
		m_Revision( p_Revision )
	{
	}

	Uint32 Version::GetMajor( ) const
	{
		return m_Major;
	}

	Uint32 Version::GetMinor( ) const
	{
		return m_Minor;
	}

	Uint32 Version::GetRevision( ) const
	{
		return m_Revision;
	}

	void Version::SetMajor( const Uint32 p_Major )
	{
		m_Major = p_Major;
	}

	void Version::SetMinor( const Uint32 p_Minor )
	{
		m_Minor = p_Minor;
	}

	void Version::SetRevision( const Uint32 p_Revision )
	{
		m_Revision = p_Revision;
	}

	Bool Version::operator == ( const Version & p_Version ) const
	{
		return	m_Major == p_Version.m_Major &&
				m_Minor == p_Version.m_Minor &&
				m_Revision == p_Version.m_Revision;
	}

	Bool Version::operator != ( const Version & p_Version ) const
	{
		return	m_Major != p_Version.m_Major ||
				m_Minor != p_Version.m_Minor ||
				m_Revision != p_Version.m_Revision;
	}

}