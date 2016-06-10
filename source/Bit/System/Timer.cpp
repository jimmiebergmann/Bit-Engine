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

#include <Bit/System/Timer.hpp>
#include <ctime>
#include <Bit/System/MemoryLeak.hpp>

////////////////////////////////////////////////////////////////
// Platform dependent includes
////////////////////////////////////////////////////////////////
#ifdef BIT_PLATFORM_WINDOWS
	#include <windows.h>
#elif defined( BIT_PLATFORM_LINUX )
	#include <sys/time.h>
#endif

namespace Bit
{

	Timer::Timer( ) :
		m_StartTime( 0 ),
		m_Time( 0 )
	{
	}

	void Timer::Start( )
	{
		m_StartTime = GetSystemTime( );
	}

	void Timer::Stop( )
	{
		Uint64 CurrentTime = GetSystemTime( );

		m_Time = ( CurrentTime - m_StartTime );
	}

	Time Timer::GetTime( )
	{
		return Microseconds( m_Time );
	}

	Time Timer::GetLapsedTime( )
	{
		Stop( );
		return Microseconds( m_Time );
	}

	Uint64 Timer::GetSystemTime( )
	{
		// Windows implementation.
		#ifdef BIT_PLATFORM_WINDOWS

			Int64 counter = 0;
			Int64 frequency = 0;

			QueryPerformanceCounter( (LARGE_INTEGER*)&counter );
			QueryPerformanceFrequency( (LARGE_INTEGER*)&frequency );

			Float64 timeFloat = (static_cast<Float64>(counter) * 1000000.0f ) / static_cast<Float64>(frequency);
			return static_cast<Uint64>(timeFloat);

		// Linux implementation.
		#elif defined( BIT_PLATFORM_LINUX )

			timeval Time;
			gettimeofday( &Time, 0 );

			return	static_cast< Int64 >( Time.tv_sec ) * 1000000 +
					static_cast< Int64 >( Time.tv_usec );

		#endif
	}

}

