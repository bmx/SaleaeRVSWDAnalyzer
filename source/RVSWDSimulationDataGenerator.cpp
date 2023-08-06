#include "RVSWDSimulationDataGenerator.h"
#include "RVSWDAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

RVSWDSimulationDataGenerator::RVSWDSimulationDataGenerator()
:	mSerialText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
{
}

RVSWDSimulationDataGenerator::~RVSWDSimulationDataGenerator()
{
}

void RVSWDSimulationDataGenerator::Initialize( U32 simulation_sample_rate, RVSWDAnalyzerSettings* settings )
{
}

U32 RVSWDSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	return 1;
}

void RVSWDSimulationDataGenerator::CreateSerialByte()
{
}
