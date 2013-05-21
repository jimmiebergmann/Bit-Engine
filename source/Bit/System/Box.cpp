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

#include <Bit/System/Box.hpp>
#include <Bit/System/Line3.hpp>
#include <Bit/System/Sphere.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Box::Box( )
	{
	}

	Box::Box( Vector3_f32 p_Size ) :
		Positsion( 0.0f, 0.0f, 0.0f ),
		Size( p_Size )
	{
	}

	Box::Box( Vector3_f32 p_Positsion, Vector3_f32 p_Size ) :
		Positsion( p_Positsion ),
		Size( p_Size )
	{
	}

	// Public functions
	BIT_FLOAT32 Box::GetArea( ) const
	{
		// Calculate the face areas
		BIT_FLOAT32 A = Size.x * Size.y;
		BIT_FLOAT32 B = Size.z * Size.y;
		BIT_FLOAT32 C = Size.x * Size.z;

		return ( 2.0f * A ) + ( 2.0f * B ) + ( 2.0f * C );
	}

	BIT_FLOAT32 Box::GetDiagonal( ) const
	{
		BIT_FLOAT32 Diagonal = sqrt( ( Size.x * Size.x ) + ( Size.y * Size.y ) );
		return sqrt( ( Diagonal * Diagonal ) + ( Size.z * Size.z ) );
	}

	BIT_FLOAT32 Box::GetVolume( ) const
	{
		return Size.x * Size.y * Size.z;
	}

	// Intersection functions
	BIT_BOOL Box::Intersection( Vector3_f32 p_Point )
	{
		return BIT_FALSE;
	}

	BIT_BOOL Box::Intersection( Line3 p_Line )
	{
		return BIT_FALSE;
	}

	BIT_BOOL Box::Intersection( Box p_Box )
	{
		return BIT_FALSE;
	}
	
}