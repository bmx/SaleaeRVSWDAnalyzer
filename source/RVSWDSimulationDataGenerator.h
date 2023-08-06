#ifndef RVSWD_SIMULATION_DATA_GENERATOR
#define RVSWD_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class RVSWDAnalyzerSettings;

class RVSWDSimulationDataGenerator
{
public:
	RVSWDSimulationDataGenerator();
	~RVSWDSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, RVSWDAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	RVSWDAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //RVSWD_SIMULATION_DATA_GENERATOR