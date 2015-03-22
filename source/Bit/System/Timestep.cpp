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

#include <Bit/System/Timestep.hpp>
#include <Bit/System/Timer.hpp>
#include <Bit/System/Sleep.hpp>
#include <complex>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	Timestep::Timestep(  )
	{
	}

	Time Timestep::Execute( const Time & p_UpdateTime, Function p_Function )
	{
		// Start a timer
		Timer timer;
		timer.Start( );

		// Call the function
		p_Function( );

		// Calculate the sleep time
		timer.Stop( );
		Time sleepTime = p_UpdateTime - timer.GetTime( );
		Float64 exceeded = std::abs<Float64>( p_UpdateTime.AsSeconds( ) - timer.GetTime( ).AsSeconds( ) );

		// Sleep for some time
		if( exceeded > 0.0f )
		{
			Sleep( sleepTime );
		}
		else
		{
			// Return the exceeded time
			return Seconds( exceeded );
		}
	
		// Succeeded in time.
		return Microseconds( 0 );
	}

}