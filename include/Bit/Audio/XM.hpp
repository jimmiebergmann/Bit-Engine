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


#ifndef __BIT_AUDIO_XM_HPP__
#define __BIT_AUDIO_XM_HPP__

#include <Bit/DataTypes.hpp>
#include <vector>

namespace Bit
{
	
	class BIT_API XM
	{

	public:

		// Public structures
		struct HeaderStruct
		{
			BIT_BYTE ID[ 17 ];
			BIT_BYTE ModuleName[ 20 ];
			BIT_BYTE UnknownData;		// Should be 1A(26) ?
			BIT_BYTE TrackerName[ 20 ];
			BIT_UINT16 Version;			// Should be $0104(260)
			BIT_UINT32 HeaderSize;
			BIT_UINT16 SongLength;
			BIT_UINT16 RestartPosition;
			BIT_UINT16 ChannelCount;	// (2,4,6,8,10,...,32)
			BIT_UINT16 PatternCount;	// Max 256
			BIT_UINT16 InstrumentCount;	// Max 128
			BIT_UINT16 Frags;	// Bit 0 = Amiga freq table, Bit 1 = linear freq table
			BIT_UINT16 DefaultTempo;
			BIT_UINT16 DefaultBMP;
			BIT_BYTE PatternOrderTable[ 256 ];
		};

		struct PatternHeaderStruct
		{
			BIT_UINT32 HeaderSize;
			BIT_BYTE PackingType; // Always 0
			BIT_UINT16 RowCount; // 1...256
			BIT_UINT16 DataSize; // Equals to 0 if no pattern data
		};

		struct InstrumentHeaderStruct
		{
			BIT_UINT32 HeaderSize;
			BIT_BYTE Name[ 22 ];
			BIT_BYTE Type; // Always 0 ?
			BIT_UINT16 SampleCount;
		};

		struct ExtraInstrumentHeaderStruct
		{
			BIT_UINT32 HeaderSize;
			BIT_BYTE SampleNoteNumbers[ 96 ];
			BIT_BYTE VolumePoints[ 48 ];
			BIT_BYTE PanningPoints[ 48 ];
			BIT_BYTE VolumePointCount;
			BIT_BYTE PanningPointCount;
			BIT_BYTE VolumeSustainPoint;
			BIT_BYTE VolumeLoopStartPoint;
			BIT_BYTE VolumeLoopEndPoint;
			BIT_BYTE PanningSustainPoint;
			BIT_BYTE PanningLoopStartPoint;
			BIT_BYTE PanningLoopEndPoint;
			BIT_BYTE VolumeType; // 0 - On, 1 - Sustain, 2 - Loop
			BIT_BYTE PanningType; // 0 - On, 1 - Sustain, 2 - Loop
			BIT_BYTE VibratoType;
			BIT_BYTE VibratoSweep;
			BIT_BYTE VibratoDepth;
			BIT_BYTE VibratoRate;
			BIT_UINT16 VolumeFadeout;
			BIT_UINT16 Reserved;
		};

		struct SampleHeaderStruct
		{
			BIT_UINT32 Length;
			BIT_UINT32 LoopStart;
			BIT_UINT32 LoopLength;
			BIT_BYTE Volume;
			BIT_BYTE Finetune;
			BIT_BYTE Type; //  0 = No loop, 1 = Forward loop, 2 = Ping pong loop, 3 = ?
			BIT_BYTE Panning; // ( 0 - 255 )
			BIT_BYTE RelativeNoteNumber;
			BIT_BYTE Reserved;
			BIT_BYTE Name[ 22 ];
		};

		struct PatternStruct
		{
			PatternStruct( ) { pData = BIT_NULL; }
			PatternHeaderStruct Header;
			BIT_BYTE * pData;
		};

		struct SampleStruct
		{
			SampleStruct( ) { pData = BIT_NULL; }
			SampleHeaderStruct Header;
			BIT_BYTE * pData;
		};

		struct InstrumentStruct
		{
			InstrumentStruct( ) { }
			InstrumentHeaderStruct Header;
			ExtraInstrumentHeaderStruct ExtraHeader;
			std::vector< SampleStruct > m_Samples;
		};

		// Constructor/destructor
		XM( );
		~XM( );

		// Public functions
		BIT_UINT32 Read( char * p_pFileName );

		// Get functions	
		BIT_INLINE BIT_UINT16 GetPatternCount( ) const { return m_Header.PatternCount; }
		BIT_INLINE BIT_UINT16 GetInstrumentCount( ) const { return m_Header.InstrumentCount; }
		BIT_INLINE HeaderStruct GetHeader( ) const { return m_Header; }
		PatternStruct GetPattern( BIT_UINT32 p_Index ) const;
		InstrumentStruct GetInstrument( BIT_UINT32 p_Index ) const;
		SampleStruct GetSample( BIT_UINT32 p_InstrumentIndex, BIT_UINT32 p_SampleIndex ) const;
		BIT_BYTE * GetWavData( );
		BIT_INLINE BIT_BOOL FileIsRead( ) const { return m_ReadFile; }

	private:

		// Private functions
		void Clear( );
		void ClearPatterns( );
		void ClearInstruments( );
		void ClearSample( SampleStruct & p_Sample );

		void PrintHeader( HeaderStruct & p_Header );
		void PrintPatternHeader( PatternHeaderStruct & p_Header );
		void PrintInstrumentHeader( InstrumentHeaderStruct & p_Header );
		void PrintExtraInstrumentHeader( ExtraInstrumentHeaderStruct & p_Header );
		void PrintSampleHeader( SampleHeaderStruct & p_Header );

		// Private variables
		BIT_BOOL m_ReadFile;
		HeaderStruct m_Header;
		std::vector< PatternStruct > m_Patterns;
		std::vector< InstrumentStruct > m_Instruments;

	};

}

#endif