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

/*
template <typename T>
inline Vector2<T>::Vector2( )
{
}
*/

template <class classType>
Timestep<classType>::Timestep( classType * p_pClassPointer, ClassFunctionPtr p_pClassFunctionPtr ) :
	m_pClassPointer( p_pClassPointer ),
	m_pFunctionPointer( p_pClassFunctionPtr )
{
}

template <class classType>
Time Timestep<classType>::Execute( const Int32 p_UpdatesPerSecond )
{
	if( p_UpdatesPerSecond == 0 )
	{
		return Microseconds( 0 );
	}

	// Calculate the update time
	const Float64 updateTime = 1.0f / static_cast<Float64>( p_UpdatesPerSecond );

	// Start a timer
	Timer timer;
	timer.Start( );


	// Call the function
	(*m_pClassPointer.*m_pFunctionPointer)();

	// Calculate the sleep time
	timer.Stop( );
	Float64 sleepTime = updateTime - timer.GetTime( ).AsSeconds( );

	// Sleep for some time
	if( sleepTime > 0.0f )
	{
		Sleep( Seconds( sleepTime ) );
	}
	else
	{
		// Return the exceeded time
		return Seconds( std::abs<Float64>( sleepTime ) );
	}
	
	// Succeeded in time.
	return Microseconds( 0 );
}