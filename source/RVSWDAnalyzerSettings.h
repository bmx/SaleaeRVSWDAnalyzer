#ifndef RVSWD_ANALYZER_SETTINGS
#define RVSWD_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class RVSWDAnalyzerSettings : public AnalyzerSettings
{
public:
	RVSWDAnalyzerSettings();
	virtual ~RVSWDAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mRVSWDIOChannel;
	Channel mSWCLKChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRVSWDIOChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mSWCLKChannelInterface;
};

#endif //RVSWD_ANALYZER_SETTINGS
