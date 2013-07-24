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

#ifdef BIT_PLATFORM_WIN32
	#include <windows.h>
#elif defined( BIT_PLATFORM_LINUX )
	#include <sys/time.h>
#endif

#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>


namespace Bit
{

	Timer::Timer( ) :
		m_StartTime( 0.0f ),
		m_Time( 0.0f )
	{
	}

	void Timer::Start( )
	{
		m_StartTime = GetSystemTime( );
	}

	void Timer::Stop( )
	{
		BIT_FLOAT64 CurrentTime = GetSystemTime( );
		m_Time = ( CurrentTime - m_StartTime );
	}

	BIT_FLOAT64 Timer::GetTime( )
	{
		return m_Time;
	}

	BIT_FLOAT64 Timer::GetLapsedTime( )
	{
		Stop( );
		return m_Time;
	}

	BIT_FLOAT64 Timer::GetSystemTime( )
	{
		#ifdef BIT_PLATFORM_WIN32

			static BIT_SINT64 Counter = 0;
			static BIT_SINT64 Frequency = 0;

			QueryPerformanceCounter( (LARGE_INTEGER*)&Counter );
			QueryPerformanceFrequency( (LARGE_INTEGER*)&Frequency );

			return ( static_cast< BIT_FLOAT64 >( Counter ) /
				static_cast< BIT_FLOAT64 >( Frequency ) );

		#elif defined( BIT_PLATFORM_LINUX )

			timeval Time;
			gettimeofday( &Time, 0 );

			return  static_cast< BIT_FLOAT64 >( Time.tv_sec ) +
					static_cast< BIT_FLOAT64 >( Time.tv_usec ) * 0.000001f;

		#endif
	}

}

