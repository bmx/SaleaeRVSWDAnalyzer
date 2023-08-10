/*
	The RVSWD protocol is used in WCH riscV cores and has no public
	documentation yet.

	It seems to be based on I2C and a mix of swd.

	START - the io line fall while clk is high

		_____
	  IO     \______________________
		____________________
	 CLK           S        \_______


	ONE - io is high when clock rise and fall

		   ______________________
	 IO __/                      \__
			 __________________
	CLK ____/         1        \____

	ZERO - io is low where clock rise and fall


	 IO _____________________________
			 __________________
	CLK ____/         0        \_____


	STOP - io goes hi while clk is high

					________________
	  IO___________/
		____________________
	 CLK           K        \_______


	Copyright (C) 2023 bmx.  All rights reserved.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License version 2.1, as
	published by the Free Software Foundation.  This program is
	distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
	License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program; if not, see <http://www.gnu.org/licenses/>.
*/
#define LOGIC2
#include "RVSWDAnalyzer.h"
#include "RVSWDAnalyzerSettings.h"
#include "RVSWDAnalyzerResults.h"
#include <AnalyzerResults.h>

#include <AnalyzerChannelData.h>
#include <cstdio>

RVSWDAnalyzer::RVSWDAnalyzer()
	: Analyzer2(),
	  mSettings(new RVSWDAnalyzerSettings()),
	  mSimulationInitialized(false)
{
	SetAnalyzerSettings(mSettings.get());
	UseFrameV2();
}

RVSWDAnalyzer::~RVSWDAnalyzer()
{
	KillThread();
}

void RVSWDAnalyzer::SetupResults()
{
	mResults.reset(new RVSWDAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());
	mResults->AddChannelBubblesWillAppearOn(mSettings->mSWCLKChannel);
}
void RVSWDAnalyzer::advance_io()
{
	mRVSWDIO->AdvanceToNextEdge();
}
void RVSWDAnalyzer::advance_clk()
{
	mSWCLK->AdvanceToNextEdge();
}
void RVSWDAnalyzer::WorkerThread()
{
	mRVSWDIO = GetAnalyzerChannelData(mSettings->mRVSWDIOChannel);
	mSWCLK = GetAnalyzerChannelData(mSettings->mSWCLKChannel);
	bool in_frame = false;
	U64 start_clk_raise, end_clk_fall, start_io_raise;
	enum BitState io_state, clk_state;
	U64 clk_position, io_position, next_clk_edge, next_io_edge;
	std::string word;
	U16 position = 0;

	std::vector<bit> bitarray;

	for (;;)
	{
		if (!in_frame)
		{
			io_state = mRVSWDIO->GetBitState();
			clk_state = mSWCLK->GetBitState();
			clk_position = mSWCLK->GetSampleNumber();
			io_position = mRVSWDIO->GetSampleNumber();
			next_clk_edge = mSWCLK->GetSampleOfNextEdge();
			next_io_edge = mRVSWDIO->GetSampleOfNextEdge();

			if (io_state == BIT_HIGH && clk_state == BIT_HIGH)
			{
				if (mRVSWDIO->WouldAdvancingToAbsPositionCauseTransition(next_clk_edge))
				{
					// START
					word = "";
					position = 0;
					bitarray.clear();
					in_frame = true;
					mResults->AddMarker(next_io_edge, AnalyzerResults::Start, mSettings->mRVSWDIOChannel);
				}
				advance_clk();
				mRVSWDIO->AdvanceToAbsPosition(mSWCLK->GetSampleNumber());
			}
			else
			{
				advance_clk();
			}
		}
		else
		{
			advance_clk();
			clk_position = mSWCLK->GetSampleNumber();
			mRVSWDIO->AdvanceToAbsPosition(clk_position);
			io_state = mRVSWDIO->GetBitState();
			next_clk_edge = mSWCLK->GetSampleOfNextEdge();

			if (io_state == BIT_HIGH)
			{
				// ONE
				word = word + "1";
				bitarray.push_back({clk_position, next_clk_edge, position++});
				mResults->AddMarker(clk_position, AnalyzerResults::One, mSettings->mRVSWDIOChannel);
				advance_clk();
			}
			else
			{
				if (io_state == BIT_LOW && mRVSWDIO->WouldAdvancingToAbsPositionCauseTransition(next_clk_edge - 3))
				{
					// STOP
					mResults->AddMarker(clk_position, AnalyzerResults::Stop, mSettings->mRVSWDIOChannel);
					in_frame = false;
					advance_io();
					io_position = mRVSWDIO->GetSampleNumber();
					mSWCLK->AdvanceToAbsPosition(io_position);
					parse(word, bitarray);
				}
				else
				{
					// ZERO
					word = word + "0";
					bitarray.push_back({clk_position, next_clk_edge, position++});
					mResults->AddMarker(clk_position, AnalyzerResults::Zero, mSettings->mRVSWDIOChannel);
					advance_clk();
				}
			}
		}
		ReportProgress(mSWCLK->GetSampleNumber());
		mResults->CommitResults();
	}
}
U32 RVSWDAnalyzer::bit2num(std::string b)
{
	U32 val = 0;
	U32 len = b.length();
	for (int i = 1; i <= len; i++)
	{
		if (b[i - 1] == '1')
			val = val | (1 << len - i);
	}
	return val;
}
void RVSWDAnalyzer::parse(std::string s, std::vector<bit> array)
{

	Frame frame;
	FrameV2 frame_v2;
	U32 len = s.length();
	U8 host_address;
	U32 host_data;
	U8 operation;
	U8 host_parity;
	U8 target_address;
	U32 target_data;
	U8 target_parity;
	U8 status;
	U32 extra;

	if (len == 52)
	{
		host_address = bit2num(s.substr(0, 7));
		operation = bit2num(s.substr(7, 1));
		host_parity = bit2num(s.substr(8, 1));
		target_data = bit2num(s.substr(14, 32));
		target_parity = bit2num(s.substr(46, 1));
		extra = bit2num(s.substr(47, 51));

		frame_v2.AddString("host_reg", REG2[host_address]);
		frame_v2.AddBoolean("host_parity", host_parity);
		frame_v2.AddByte("operation", operation);
		frame_v2.AddInteger("target_data", target_data);
		frame_v2.AddBoolean("target_parity", target_parity);
		frame_v2.AddByte("extra", extra);

		U64 total1 = bit2num(s.substr(0, 32));
		U64 total2 = bit2num(s.substr(32, 20));
		Frame Total;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = array[0].start;
		frame.mEndingSampleInclusive = array[51].end;
		frame.mData1 = (total1 << 20) | total2;
		frame.mType = RV_Short;
		mResults->AddFrame(frame);
		mResults->AddFrameV2(frame_v2, "detail", frame.mStartingSampleInclusive, frame.mEndingSampleInclusive);
		mResults->CommitResults();
	}
	else if (len == 84)
	{
		host_address = bit2num(s.substr(0, 7));
		host_data = bit2num(s.substr(7, 32));
		operation = bit2num(s.substr(39, 2));
		host_parity = bit2num(s.substr(41, 1));
		target_address = bit2num(s.substr(42, 7));
		target_data = bit2num(s.substr(49, 32));
		status = bit2num(s.substr(81, 2));
		target_parity = bit2num(s.substr(83, 1));

		U64 total1 = bit2num(s.substr(0, 32));
		U64 total2 = bit2num(s.substr(32, 32));
		U64 total3 = bit2num(s.substr(64, 20));

		frame_v2.AddString("host_reg", REG2[host_address]);
		frame_v2.AddInteger("host_data", host_data);
		frame_v2.AddBoolean("host_parity", host_parity);
		frame_v2.AddByte("operation", operation);
		frame_v2.AddString("target_reg", REG2[target_address]);
		frame_v2.AddInteger("target_data", target_data);
		frame_v2.AddBoolean("target_parity", target_parity);
		frame_v2.AddByte("status", status);

		frame.mFlags = 0;
		frame.mStartingSampleInclusive = array[0].start;
		frame.mEndingSampleInclusive = array[83].end;
		frame.mData1 = (total1 << 32) + total2;
		frame.mData2 = total3;
		frame.mType = RV_Long;
		mResults->AddFrame(frame);
		mResults->AddFrameV2(frame_v2, "detail", frame.mStartingSampleInclusive, frame.mEndingSampleInclusive);
		mResults->CommitResults();
	}
}

bool RVSWDAnalyzer::NeedsRerun()
{
	return false;
}

U32 RVSWDAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
	if (mSimulationInitialized == false)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitialized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 RVSWDAnalyzer::GetMinimumSampleRateHz()
{
	return 0;
}

const char *RVSWDAnalyzer::GetAnalyzerName() const
{
	return "RVSWD";
}

const char *GetAnalyzerName()
{
	return "RVSWD";
}

Analyzer *CreateAnalyzer()
{
	return new RVSWDAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
	delete analyzer;
}
