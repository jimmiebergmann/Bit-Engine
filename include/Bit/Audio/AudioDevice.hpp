#ifndef BIT_AUDIO_DEVICE_HPP
#define BIT_AUDIO_DEVICE_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/System/Vector3.hpp>

namespace Bit
{

	// Forward declarations
	class Sound;
	
	class BIT_API AudioDevice
	{

	public:

		// Constructors/destructors
		virtual ~AudioDevice( ) { }

		// Public general functions
		virtual BIT_UINT32 Open( ) = 0;

		// Create functions for different audio elements
		virtual Sound * CreateSound( ) = 0;

		// Set functions
		virtual void SetGlobalVolume( BIT_FLOAT32 p_Volume ) = 0;
		virtual void SetListenerPosition( Vector3_f32 p_Position ) = 0;
		virtual void SetListenerTarget( Vector3_f32 p_Target ) = 0;
		virtual void SetListenerVelocity( Vector3_f32 p_Velocity ) = 0;

		// Get functions
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }

	protected:

		BIT_BOOL m_Open;

	};

	// Create a cross platform audio device via this function
	BIT_API AudioDevice * CreateAudioDevice( );

}

#endif