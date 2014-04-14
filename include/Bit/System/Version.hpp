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

#ifndef BIT_SYSTEM_VERSION_HPP
#define BIT_SYSTEM_VERSION_HPP

#include <Bit/Build.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Version class.
	///
	/// Example: 1.2.3, where 1 is the major version, 2 is the minor version
	///	and 3 is the revision number of the current version.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Version
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		////////////////////////////////////////////////////////////////
		Version(	const Uint32 p_Major = 0,
					const Uint32 p_Minor = 0,
					const Uint32 p_Revision = 0 );

		////////////////////////////////////////////////////////////////
		/// \brief Get the major version.
		///
		////////////////////////////////////////////////////////////////
		Uint32 GetMajor( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the minor version.
		///
		////////////////////////////////////////////////////////////////
		Uint32 GetMinor( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the revision number.
		///
		////////////////////////////////////////////////////////////////
		Uint32 GetRevision( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Set the major version.
		///
		////////////////////////////////////////////////////////////////
		void SetMajor( const Uint32 p_Major );

		////////////////////////////////////////////////////////////////
		/// \brief Set the minor version.
		///
		////////////////////////////////////////////////////////////////
		void SetMinor( const Uint32 p_Minor );

		////////////////////////////////////////////////////////////////
		/// \brief Set the revision number.
		///
		////////////////////////////////////////////////////////////////
		void SetRevision( const Uint32 p_Revision );

		////////////////////////////////////////////////////////////////
		/// \brief Equal to operator
		///
		////////////////////////////////////////////////////////////////
		Bool operator == ( const Version & p_Version ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Not equal to operator
		///
		////////////////////////////////////////////////////////////////
		Bool operator != ( const Version & p_Version ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Static default version type
		///
		////////////////////////////////////////////////////////////////
		static const Version Default;

	private:

		Uint32 m_Major;		///< Major version number
		Uint32 m_Minor;		///< Minor version number
		Uint32 m_Revision;	///< Revision number of the current version

	};

}

#endif