#ifndef RVSWD_ANALYZER_H
#define RVSWD_ANALYZER_H

#include <Analyzer.h>
#include <iostream>
#include "RVSWDAnalyzerResults.h"
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
	std::auto_ptr< RVSWDAnalyzerSettings > mSettings;
	std::auto_ptr< RVSWDAnalyzerResults > mResults;
	AnalyzerChannelData* mRVSWDIO;
	AnalyzerChannelData* mSWCLK;

	RVSWDSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitialized;
	
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //RVSWD_ANALYZER_H
