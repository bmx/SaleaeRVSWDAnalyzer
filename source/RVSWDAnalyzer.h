#ifndef RVSWD_ANALYZER_H
#define RVSWD_ANALYZER_H

#include <Analyzer.h>
#include "RVSWDAnalyzerResults.h"
#include <iostream>
#include "RVSWDSimulationDataGenerator.h"
	
class RVSWDAnalyzerSettings;
class ANALYZER_EXPORT RVSWDAnalyzer : public Analyzer2
{
	struct bit {
		U64 start;
		U64 end;
		U16 pos;
	};
public:
	RVSWDAnalyzer();
	virtual ~RVSWDAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();
	virtual void advance_io();
	virtual void advance_clk();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();
	U32 bit2num(std::string);
	void parse(std::string, std::vector<bit>);

protected: //vars
	std::unique_ptr< RVSWDAnalyzerSettings > mSettings;
	std::unique_ptr< RVSWDAnalyzerResults > mResults;
	AnalyzerChannelData* mRVSWDIO;
	AnalyzerChannelData* mSWCLK;

	RVSWDSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitialized;
private:
	const char *REG2[0x80] =
	{
		"", "", "", "",													// 00-03
		"DMDATA0",														// 04
		"DMDATA1",														// 05
		"", "", "", "", "", "", "", "", "", "",							// 06-0F
		"DMCONTROL",													// 10
		"DMSTATUS",														// 11
		"DMHARTINFO",													// 12
		"", "", "",														// 13-15
		"DMABSTRACTCS",													// 16
		"DMCOMMAND",													// 17
		"DMABSTRACTAUTO",												// 18
		"", "", "", "", "", "", "",										// 19-1F
		"DMPROGBUF0",													// 20
		"DMPROGBUF1",													// 21
		"DMPROGBUF2",													// 22
		"DMPROGBUF3",													// 23
		"DMPROGBUF4",													// 24
		"DMPROGBUF5",													// 25
		"DMPROGBUF6",													// 26
		"DMPROGBUF7",													// 27
		"", "", "", "", "", "", "", "",									// 28-2F
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 30-3F
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 40-4F
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 50-5F
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 60-6F
		"", "", "", "", "", "", "", "", "", "", "", "",					// 70-7B
		"DMCPBR",														// 7C
		"DMCFGR",														// 7D
		"DMSHDWCFGR",													// 7E
		""																// 7F
	};
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //RVSWD_ANALYZER_H
