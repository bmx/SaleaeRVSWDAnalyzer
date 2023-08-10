#include "RVSWDAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "RVSWDAnalyzer.h"
#include "RVSWDAnalyzerSettings.h"
#include <iostream>
#include <fstream>

RVSWDAnalyzerResults::RVSWDAnalyzerResults( RVSWDAnalyzer* analyzer, RVSWDAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

RVSWDAnalyzerResults::~RVSWDAnalyzerResults()
{
}

void RVSWDAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );
	char number_str[128] = "Error";
	
	if (frame.mType == RV_Long) { // long packet (84bits)
		U64 n1 = frame.mData1;
		U64 n2 = frame.mData2;
		U8 host_address = (U8)((n1 >> 57) & 0x7f);
		U32 host_data = (U32)(n1 >> 25) & 0xffffffff;
		U8 operation = (U8)(n1 >> 23) & 0x3;
		bool host_parity = (U8)(n1 >> 21) & 1;
		U8 target_address = (U8)(n1 >> 15) & 0x7f;
		U32 target_data = ((n1 & 0x7fff)<<17) | (((n2>>3) & 0x1ffff));
		U8 status = (n2 >> 1) & 0x7;
		bool target_parity = n2 & 1;

		sprintf(number_str, "%s %08x %x %d - %s %08x %d - %d", 
			REG[host_address], host_data, operation, host_parity, 
			REG[target_address], target_data, status, target_parity
			);
	} else if (frame.mType == RV_Short) { // short packet (54bits)

		U64 n1 = frame.mData1;
		U8 host_address = (U8)((n1 >> 45) & 0x7f);
		U8 operation = (U8)(n1 >> 44) & 1;
		bool host_parity = (U8)(n1 >> 43) & 1;
		U32 target_data = (n1 >> 6) & 0xffffffff;
		bool target_parity = (U8)(n1 >> 5) & 1;
		U32 extra = n1 & 0x7f;
		sprintf(number_str, "%s(%x) -- %x %x - %08x %x (%x)", 
			REG[host_address], host_address, operation, host_parity,	
			target_data, target_parity, extra
			);
	}
	AddResultString( number_str );
}

void RVSWDAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	/*
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		build_string(number_str, &frame);

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
	*/
}

void RVSWDAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
	/*
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	build_string(number_str, &frame);
	AddTabularText( number_str );
	*/
}

void RVSWDAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported
}

void RVSWDAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}
