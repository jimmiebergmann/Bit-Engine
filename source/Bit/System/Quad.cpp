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

#include <Bit/System/Quad.hpp>
#include <Bit/System/Line2.hpp>
#include <Bit/System/Circle.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Construcotrs
	Quad::Quad( )
	{
	}

	Quad::Quad( Vector2_f32 p_Size ) :
		Size( p_Size ),
		Positsion( 0.0f, 0.0f )
	{
	}

	Quad::Quad( Vector2_f32 p_Size, Vector2_f32 p_Positsion ) :
		Size( p_Size ),
		Positsion( p_Positsion )
	{
	}

	// Public functions
	BIT_FLOAT32 Quad::GetArea( ) const
	{
		return Size.x * Size.y;
	}

	BIT_FLOAT32 Quad::GetDiagonal( ) const
	{
		return sqrt( ( Size.x * Size.x ) + ( Size.y * Size.y ) );
	}

	Vector2_f32 Quad::GetLowCoords( ) const
	{
		return Vector2_f32( Positsion.x - ( Size.x / 2.0f ),
							Positsion.y - ( Size.y / 2.0f ) );
	}

	Vector2_f32 Quad::GetHighCoords( ) const
	{
		return Vector2_f32( Positsion.x + ( Size.x / 2.0f ),
							Positsion.y + ( Size.y / 2.0f ) );
	}

	// Intersection functions
	BIT_BOOL Quad::Intersection( Vector2_f32 p_Point )
	{
		return IntersectionPoint2Quad( p_Point, *this );
	}

	BIT_BOOL Quad::Intersection( Line2 p_Line )
	{
		return IntersectionLine2Quad( p_Line, *this );
	}

	BIT_BOOL Quad::Intersection( Circle p_Circle )
	{
		return IntersectionCircleQuad( p_Circle, *this );
	}
	
	BIT_BOOL Quad::Intersection( Quad p_Quad )
	{
		return IntersectionQuadQuad( *this, p_Quad);
	}
	
}