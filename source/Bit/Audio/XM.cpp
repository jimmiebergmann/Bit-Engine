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


#include <Bit/Audio/XM.hpp>
#include <fstream>
#include <iostream>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Constructor/destructor
	XM::XM( ) :
		m_ReadFile( BIT_FALSE )
	{
	}

	XM::~XM( )
	{
		Clear( );
	}

	// Public functions
	BIT_UINT32 XM::Read( char * p_pFileName )
	{
		// Clear all the data
		Clear( );

		// Open the file
		std::ifstream fin( p_pFileName, std::ios_base::binary );
		if( !fin.is_open( ) )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Can not open the file.\n" );
			return BIT_ERROR_OPEN_FILE;
		}

		// Get the file size
		fin.seekg( 0, std::ios_base::end );
		BIT_UINT32 FileSize = fin.tellg( );
		fin.seekg( 0, std::ios_base::beg );

		// Create a file position variable
		BIT_UINT32 FilePos = 0;

		// Error check the file size
		if( FileSize < 80 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Wrong file size.\n" );
			fin.close( );
			return BIT_ERROR;
		}

		// Read the main header
		fin.read( (char*)m_Header.ID, sizeof( BIT_BYTE ) * 17 );
		fin.read( (char*)m_Header.ModuleName, sizeof( BIT_BYTE ) * 20 );
		fin.read( (char*)&m_Header.UnknownData, sizeof( BIT_BYTE ) );
		fin.read( (char*)m_Header.TrackerName, sizeof( BIT_BYTE ) * 20 );
		fin.read( (char*)&m_Header.Version, sizeof( BIT_UINT16 ) );
		// Get the position size we have to seek out of
		// the header when we are done reading the header data.
		FilePos = fin.tellg( );
		fin.read( (char*)&m_Header.HeaderSize, sizeof( BIT_UINT32 ) );
		fin.read( (char*)&m_Header.SongLength, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.RestartPosition, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.ChannelCount, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.PatternCount, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.InstrumentCount, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.Frags, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.DefaultTempo, sizeof( BIT_UINT16 ) );
		fin.read( (char*)&m_Header.DefaultBMP, sizeof( BIT_UINT16 ) );
		fin.read( (char*)m_Header.PatternOrderTable, sizeof( BIT_BYTE ) * 256 );
		fin.seekg( FilePos + m_Header.HeaderSize, std::ios_base::beg );

		// Error check the header
		if( m_Header.Version != 260 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Header error: Wrong version numer: %i.\n", m_Header.Version );
			fin.close( );
			return BIT_ERROR;
		}
		if( m_Header.UnknownData != 26 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Header error: Unknown data isn't 26.\n" );
			fin.close( );
			return BIT_ERROR;
		}
		/*
		// Error check channel count
		// ......
		*/
		if( m_Header.PatternCount > 256 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Header error: Too many patterns: %i.\n", m_Header.PatternCount );
			fin.close( );
			return BIT_ERROR;
		}
		if( m_Header.InstrumentCount > 128 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Header error: Too many instruments: %i.\n", m_Header.InstrumentCount );
			fin.close( );
			return BIT_ERROR;
		}
		if( m_Header.Frags != 1 )
		{
			bitTrace( BIT_NULL, "[Bit::XM::ReadFile] <error> "
				"Header error: Flags, not supporting amiga freq patterns: %i.\n" );
			fin.close( );
			return BIT_ERROR;
		}
/*
		//Print the header
		std::cout << "==========================================================" << std::endl;
		std::cout << "XM Header" << std::endl;
		std::cout << "==========================================================" << std::endl;
		PrintHeader( m_Header );
		std::cout << std::endl << std::endl;
		std::cout << "==========================================================" << std::endl;
		std::cout << "Pattern Header" << std::endl;
		std::cout << "==========================================================" << std::endl;
*/
		// Reserve the patterns
		m_Patterns.reserve( m_Header.PatternCount );

		// Read the patterns
		for( BIT_MEMSIZE i = 0; i < m_Header.PatternCount; i++ )
		{
			PatternStruct Pattern;
			FilePos = fin.tellg( );
			fin.read( (char*)&Pattern.Header.HeaderSize, sizeof( BIT_UINT32 ) );
			fin.read( (char*)&Pattern.Header.PackingType, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Pattern.Header.RowCount, sizeof( BIT_UINT16 ) );
			fin.read( (char*)&Pattern.Header.DataSize, sizeof( BIT_UINT16 ) );
/*
			// Print the pattern headers
			PrintPatternHeader( Pattern.Header );
			std::cout << std::endl;
*/
			// Seek to the pattern data / the next pattern header
			fin.seekg( FilePos + Pattern.Header.HeaderSize, std::ios_base::beg );
			
			// Read the packed pattern data if there's any.
			if( Pattern.Header.DataSize > 0 )
			{
				Pattern.pData = new BIT_BYTE[ Pattern.Header.DataSize ];
				fin.read( (char*)Pattern.pData, sizeof( BIT_BYTE ) * Pattern.Header.DataSize );	
			}

			// Push the pattern to the pattern vector
			m_Patterns.push_back( Pattern );
		}
/*
		std::cout << "==========================================================" << std::endl;
		std::cout << "Instrument Header" << std::endl;
		std::cout << "==========================================================" << std::endl;
*/
		// Reserve the instruments
		m_Instruments.reserve( m_Header.InstrumentCount );
		
		// Read instruments
		for( BIT_MEMSIZE i = 0; i < m_Header.InstrumentCount; i++ )
		{
			// Create an instrument struct
			InstrumentStruct Instrument;

			// Read the Instrument header
			FilePos = fin.tellg( );
			fin.read( (char*)&Instrument.Header.HeaderSize, sizeof( BIT_UINT32 ) );
			fin.read( (char*)Instrument.Header.Name, sizeof( BIT_BYTE ) * 22 );
			fin.read( (char*)&Instrument.Header.Type, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.Header.SampleCount, sizeof( BIT_UINT16 ) );
/*
			// Print the instrument header
			PrintInstrumentHeader( Instrument.Header );
			std::cout << "-----------------------" << std::endl;
*/

			// Read the extra instrument header
			fin.read( (char*)&Instrument.ExtraHeader.HeaderSize, sizeof( BIT_UINT32 ) );
			fin.read( (char*)Instrument.ExtraHeader.SampleNoteNumbers, sizeof( BIT_BYTE ) * 96 );
			fin.read( (char*)Instrument.ExtraHeader.VolumePoints, sizeof( BIT_BYTE ) * 48 );
			fin.read( (char*)Instrument.ExtraHeader.PanningPoints, sizeof( BIT_BYTE ) * 48 );
			fin.read( (char*)&Instrument.ExtraHeader.VolumePointCount, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.PanningPointCount, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VolumeSustainPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VolumeLoopStartPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VolumeLoopEndPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.PanningSustainPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.PanningSustainPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.PanningLoopEndPoint, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VolumeType, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.PanningType, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VibratoType, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VibratoSweep, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VibratoDepth, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VibratoRate, sizeof( BIT_BYTE ) );
			fin.read( (char*)&Instrument.ExtraHeader.VolumeFadeout, sizeof( BIT_UINT16 ) );
			fin.read( (char*)&Instrument.ExtraHeader.Reserved, sizeof( BIT_UINT16 ) );
			
			// Seek to the sample headers
			fin.seekg( FilePos + Instrument.Header.HeaderSize, std::ios_base::beg );
/*			
			// Print the extra instrument header
			PrintExtraInstrumentHeader( Instrument.ExtraHeader );
			std::cout << "-----------------------" << std::endl;
*/		

			// Reserve the samples for the current instrument
			Instrument.m_Samples.reserve( Instrument.Header.SampleCount );

			// Read the instrument data
			if( Instrument.Header.SampleCount > 0 )
			{
				// Read all the sample headers
				for( BIT_MEMSIZE j = 0; j < Instrument.Header.SampleCount; j++ )
				{
					SampleStruct Sample;
					fin.read( (char*)&Sample.Header.Length, sizeof( BIT_UINT32 ) );
					fin.read( (char*)&Sample.Header.LoopStart, sizeof( BIT_UINT32 ) );
					fin.read( (char*)&Sample.Header.LoopLength, sizeof( BIT_UINT32 ) );
					fin.read( (char*)&Sample.Header.Volume, sizeof( BIT_BYTE ) );
					fin.read( (char*)&Sample.Header.Finetune, sizeof( BIT_BYTE ) );
					fin.read( (char*)&Sample.Header.Type, sizeof( BIT_BYTE ) );
					fin.read( (char*)&Sample.Header.Panning, sizeof( BIT_BYTE ) );
					fin.read( (char*)&Sample.Header.RelativeNoteNumber, sizeof( BIT_BYTE ) );
					fin.read( (char*)&Sample.Header.Reserved, sizeof( BIT_BYTE ) );
					fin.read( (char*)Sample.Header.Name, sizeof( BIT_BYTE ) * 22 );
/*
					std::cout << "Instrument(" << i << " " << Instrument.Header.Name << ") Sample( " << j << ") Header: " << std::endl;
					PrintSampleHeader( Sample.Header );
					std::cout << "-----------------------" << std::endl;
*/				
		
					// Push the sample to the vector
					Instrument.m_Samples.push_back( Sample );

				}

				// Loop through all our samples in the current instrument
				// and read the sample data
				for( BIT_MEMSIZE j = 0; j < Instrument.m_Samples.size( ); j++ )
				{
					const BIT_UINT32 Length = Instrument.m_Samples[ j ].Header.Length;

					Instrument.m_Samples[ j ].pData = new BIT_BYTE[ Length ];
					fin.read( (char*)Instrument.m_Samples[ j ].pData, sizeof( BIT_BYTE ) * Length );	

					// The sampled data is store stored as delta values
					// We have to convert the data
					BIT_BYTE Old = 0;
					BIT_BYTE New = 0;
					for( BIT_MEMSIZE k = 0; k < Length; k++ )
					{
						New = Instrument.m_Samples[ j ].pData[ k ] + Old;
						Instrument.m_Samples[ j ].pData[ k ] = New;
						Old = New;
					}
				}

			}

			// Push the instrument to the vector
			m_Instruments.push_back( Instrument );


		}
/*
		std::cout << "File size / position: " << FileSize << "  " << fin.tellg( ) << std::endl;
*/

		
		// Close the file.
		fin.close( );

		m_ReadFile = BIT_TRUE;
		return BIT_OK;
	}

	BIT_BYTE * XM::GetWavData( )
	{
		

		return BIT_NULL;
	}

	// Get functions
	XM::PatternStruct XM::GetPattern( BIT_UINT32 p_Index ) const
	{
		// Make sure the index is in range
		if( p_Index < m_Patterns.size( ) )
		{
			return m_Patterns[ p_Index ];
		}

		// The index wasn't in range, create a temporary empty struct to return.
		PatternStruct EmptyStruct;
		return EmptyStruct;
	}
	XM::InstrumentStruct XM::GetInstrument( BIT_UINT32 p_Index ) const
	{
		// Make sure the index is in range
		if( p_Index < m_Instruments.size( ) )
		{
			return m_Instruments[ p_Index ];
		}

		// The index wasn't in range, create a temporary empty struct to return.
		InstrumentStruct EmptyStruct;
		return EmptyStruct;
	}
	XM::SampleStruct XM::GetSample( BIT_UINT32 p_InstrumentIndex, BIT_UINT32 p_SampleIndex ) const
	{
		// Make sure the index is in range
		if( p_InstrumentIndex < m_Instruments.size( ) )
		{
			// Make sure the second index is in range
			if( p_SampleIndex < m_Instruments[ p_InstrumentIndex ].m_Samples.size( ) )
			{
				return m_Instruments[ p_InstrumentIndex ].m_Samples[ p_SampleIndex ];
			}
		}

		// The index wasn't in range, create a temporary empty struct to return.
		SampleStruct EmptyStruct;
		return EmptyStruct;
	}

	// Private functions
	void XM::Clear( )
	{
		ClearPatterns( );
		ClearInstruments( );
		m_ReadFile = BIT_FALSE;
	}

	void XM::ClearPatterns( )
	{
		// Loop through all the patterns
		for( BIT_MEMSIZE i = 0; i < m_Patterns.size( ); i++ )
		{
			if( m_Patterns[ i ].pData )
			{
				delete [ ] m_Patterns[ i ].pData;
			}
		}

		// Clear the pattern vector
		m_Patterns.clear( );
	}

	void XM::ClearInstruments( )
	{
		// Loop through all the instruments
		for( BIT_MEMSIZE i = 0; i < m_Instruments.size( ); i++ )
		{
			// Loop through all the instrument's samples
			for( BIT_MEMSIZE j = 0; j < m_Instruments[ i ].m_Samples.size( ); j++ )
			{
				ClearSample( m_Instruments[ i ].m_Samples[ j ] );
			}

		}
		// Clear the instrument vector
		m_Instruments.clear( );
	}

	void XM::ClearSample( SampleStruct & p_Sample )
	{
		if( p_Sample.pData )
		{
			delete [ ] p_Sample.pData;
		}
	}

	void XM::PrintHeader( HeaderStruct & p_Header )
	{
		std::cout << "ID: ";
		for( BIT_MEMSIZE i = 0; i < 17; i++ )
		{
			std::cout << p_Header.ID[ i ];
		}
		std::cout << std::endl;
		std::cout << "Module Name: ";
		for( BIT_MEMSIZE i = 0; i < 20; i++ )
		{
			std::cout << p_Header.ModuleName[ i ];
		}
		std::cout << std::endl;

		std::cout << "Unknown Data: " << (int)p_Header.UnknownData << std::endl;
		std::cout << "Tracker Name: ";
		for( BIT_MEMSIZE i = 0; i < 20; i++ )
		{
			std::cout << p_Header.TrackerName[ i ];
		}
		std::cout << std::endl;
		std::cout << "Version: "			<< p_Header.Version << std::endl;
		std::cout << "Header Size: "		<< p_Header.HeaderSize << std::endl;
		std::cout << "Song Length: "		<< p_Header.SongLength << std::endl;
		std::cout << "Restart Position: "	<< p_Header.RestartPosition << std::endl;
		std::cout << "Channel Count: "		<< p_Header.ChannelCount << std::endl;
		std::cout << "Pattern Count: "		<< p_Header.PatternCount << std::endl;
		std::cout << "Instrument Count: "	<< p_Header.InstrumentCount << std::endl;
		std::cout << "Frags: "				<< p_Header.Frags << std::endl;
		std::cout << "Default Tempo: "		<< p_Header.DefaultTempo << std::endl;
		std::cout << "Default BMP: "		<< p_Header.DefaultBMP << std::endl;
		std::cout << "Pattern Table Order: ";
		for( BIT_MEMSIZE i = 0; i < p_Header.PatternCount; i++ )
		{
			std::cout << (int)p_Header.PatternOrderTable[ i ] << " ";
		}
		std::cout << std::endl;
	}

	void XM::PrintPatternHeader( PatternHeaderStruct & p_Header )
	{
		std::cout << "Header Size: "	<< p_Header.HeaderSize << std::endl;
		std::cout << "Packing Type: "	<< p_Header.PackingType << std::endl;
		std::cout << "Row Count: "		<< p_Header.RowCount << std::endl;
		std::cout << "Data Size: "		<< p_Header.DataSize << std::endl;
	}

	void XM::PrintInstrumentHeader( InstrumentHeaderStruct & p_Header )
	{
		std::cout << "Header Size: " << p_Header.HeaderSize << std::endl;
		std::cout << "Name: ";
		for( BIT_MEMSIZE i = 0; i < 22; i++ )
		{
			std::cout << p_Header.Name[ i ];
		}
		std::cout << std::endl;
		std::cout << "Type: " << (int)p_Header.Type << std::endl;
		std::cout << "Sample Count: " << p_Header.SampleCount << std::endl;
	}

	void XM::PrintExtraInstrumentHeader( ExtraInstrumentHeaderStruct & p_Header )
	{
		std::cout << "Header Size: "	<< p_Header.HeaderSize << std::endl;
		std::cout << "Sample Note Numbers: " << std::endl;
		for( BIT_MEMSIZE i = 0; i < 96; i++ )
		{
			std::cout << (int)p_Header.SampleNoteNumbers[ i ] << " ";
		}
		std::cout << std::endl;
		std::cout << "Volume Points: " << std::endl;
		for( BIT_MEMSIZE i = 0; i < 48; i++ )
		{
			std::cout << (int)p_Header.VolumePoints[ i ] << " ";
		}
		std::cout << std::endl;
		std::cout << "Panning Points: " << std::endl;
		for( BIT_MEMSIZE i = 0; i < 48; i++ )
		{
			std::cout << (int)p_Header.PanningPoints[ i ] << " ";
		}
		std::cout << std::endl;
		std::cout << "Volume Point Count: " << (int)p_Header.VolumePointCount << std::endl;
		std::cout << "Panning Point Count: " << (int)p_Header.PanningPointCount << std::endl;
		std::cout << "Volume Sustain Count: " << (int)p_Header.VolumeSustainPoint << std::endl;
		std::cout << "Volume Loop Start Point: " << (int)p_Header.VolumeLoopStartPoint << std::endl;
		std::cout << "Volume Loop End Point: " << (int)p_Header.VolumeLoopEndPoint << std::endl;
		std::cout << "Panning Sustain Count: " << (int)p_Header.PanningSustainPoint << std::endl;
		std::cout << "Panning Loop Start Point: " << (int)p_Header.PanningLoopStartPoint << std::endl;
		std::cout << "Panning Loop End Point: " << (int)p_Header.PanningLoopEndPoint << std::endl;
		std::cout << "Volume Type: " << (int)p_Header.VolumeType << std::endl;
		std::cout << "Panning Type: " << (int)p_Header.PanningType << std::endl;
		std::cout << "Vibrato Type: " << (int)p_Header.VibratoType << std::endl;
		std::cout << "Vibrato Sweep: " << (int)p_Header.VibratoSweep << std::endl;
		std::cout << "Vibrato Depth: " << (int)p_Header.VibratoDepth << std::endl;
		std::cout << "Vibrato Rate: " << (int)p_Header.VibratoRate << std::endl;
		std::cout << "Volume Fadeout: " << (int)p_Header.VolumeFadeout << std::endl;
		std::cout << "Reserved: " << (int)p_Header.Reserved << std::endl;
	}

	void XM::PrintSampleHeader( SampleHeaderStruct & p_Header )
	{
		std::cout << "Length: " << (int)p_Header.Length << std::endl;
		std::cout << "Loop Start: " << (int)p_Header.LoopStart << std::endl;
		std::cout << "Loop Length: " << (int)p_Header.LoopLength << std::endl;
		std::cout << "Volume: " << (int)p_Header.Volume << std::endl;
		std::cout << "Finetune: " << (int)p_Header.Finetune << std::endl;
		std::cout << "Type: " << (int)p_Header.Type << std::endl;
		std::cout << "Panning: " << (int)p_Header.Panning << std::endl;
		std::cout << "Relative Note Number: " << (int)p_Header.RelativeNoteNumber << std::endl;
		std::cout << "Reserved: " << (int)p_Header.Reserved << std::endl;
		std::cout << "Name: ";
		for( BIT_MEMSIZE i = 0; i < 22; i++ )
		{
			std::cout << p_Header.Name[ i ];
		}
		std::cout << std::endl;
	}



}