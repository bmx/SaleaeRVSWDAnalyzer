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

static void build_string ( char *number_str, Frame *frame )
{
	const char *op_names[4] =
	{
		"WriteDP",
		"ReadDP",
		"WriteAP",
		"ReadAP",
	};
	const char *reg_names[2][4][2] =
	{
		{
			/* SW-DP registers */
			{ "IDCODE", "ABORT" },
			{ "CTRL/STAT", "CTRL/STAT" },
			{ "RESEND", "SELECT" },
			{ "RDBUFF", "TARGETSEL" },
		},
		{
			/* AHB-AP registers */
			{ "CSW", "CSW" },
			{ "TAR", "TAR" },
			{ "N/A", "N/A" },
			{ "DRW", "DRW" },
		},
	};
	unsigned op_code, reg_addr, ack_code;
	const char *op_name, *reg_name;

	op_code  = (frame->mData1 & 0x0C) >> 2;
	reg_addr = (frame->mData1 & 0x03);
	ack_code = (frame->mData1 & 0x70) >> 4;

	op_name = op_names[op_code];
	reg_name = reg_names[(op_code & 2) ? 1 : 0][reg_addr][(op_code & 1) ? 0 : 1];

	if ( (0 == op_code) && (3 == reg_addr) ) ack_code = 8; /* writes to TARGETSEL don't get a response */

	switch (ack_code)
	{
	case 8: /* special case of TARGETSEL */
	case 0x1: /* OK */
		sprintf(number_str, "%s[%u=%s] %08x",   op_name, reg_addr, reg_name, frame->mData2);
		break;
	case 0x2: /* WAIT */
	case 0x4: /* FAULT */
		sprintf(number_str, "%s[%u=%s] %s",     op_name, reg_addr, reg_name, (0x2==ack_code) ? "WAIT" : "FAULT");
		break;
	default: /* unknown */
		sprintf(number_str, "%s[%u=%s] ACK=%x", op_name, reg_addr, reg_name, ack_code);
		break;
	}
}

void RVSWDAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128];
	
	build_string(number_str, &frame);

	//AddResultString( number_str );
}

void RVSWDAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
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
}

void RVSWDAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	build_string(number_str, &frame);
	AddTabularText( number_str );
}

void RVSWDAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported
}

void RVSWDAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}
