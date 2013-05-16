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

#ifndef __BIT_GRAPHICS_IMAGE_HPP__
#define __BIT_GRAPHICS_IMAGE_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
#include <string>

namespace Bit
{

	// Pixel class
	class BIT_API Pixel
	{

	public:

		// Constructor
		Pixel( );
		Pixel( BIT_BYTE p_R, BIT_BYTE p_G, BIT_BYTE p_B );
		Pixel( BIT_BYTE p_R, BIT_BYTE p_G, BIT_BYTE p_B, BIT_BYTE p_A );
		Pixel( Vector3_byte p_RGB, BIT_BYTE p_A );

		// Public members
		BIT_BYTE m_R;
		BIT_BYTE m_G;
		BIT_BYTE m_B;
		BIT_BYTE m_A;
		
	};


	// Image class
	class BIT_API Image
	{

	public:

		// Construcotr/destructor
		Image( );
		~Image( );

		// Public general functions
		BIT_UINT32 ReadFile( const std::string p_pFilePath ); // Read data from a file.
		BIT_UINT32 ReadTGA( const std::string p_pFilePath );
		void DeallocateData( );
		BIT_BOOL ContainsData( ) const; // Does the Image class have any data?

		// Set functions
		BIT_UINT32 SetData( BIT_BYTE * p_pData, const Vector2_ui32 p_Size, const BIT_UINT32 p_Depth );
		void SetPixel( const BIT_UINT32 p_Index, Pixel p_Pixel );
		Pixel GetPixel( const BIT_UINT32 p_Index );

		// Get functions
		BIT_BYTE * GetData( ) const;
		Vector2_ui32 GetSize( ) const;
		BIT_UINT32 GetDataSize( ) const;
		BIT_UINT32 GetDepth( ) const;
		

	private:

		// Conversion functions
		void BGR_To_RGB( );
		void BGRA_To_RGBA( );

		// Private variables
		BIT_BYTE * m_pData;
		Vector2_ui32 m_Size;
		BIT_UINT32 m_Depth;

	};

}

#endif