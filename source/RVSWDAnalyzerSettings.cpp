#include "RVSWDAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

RVSWDAnalyzerSettings::RVSWDAnalyzerSettings()
:	mRVSWDIOChannel( UNDEFINED_CHANNEL ),
	mSWCLKChannel( UNDEFINED_CHANNEL )
{
	mRVSWDIOChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mRVSWDIOChannelInterface->SetTitleAndTooltip( "RVSWDIO", "RVSWDIO" );
	mRVSWDIOChannelInterface->SetChannel( mRVSWDIOChannel );

	mSWCLKChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mSWCLKChannelInterface->SetTitleAndTooltip( "SWCLK", "SWCLK" );
	mSWCLKChannelInterface->SetChannel( mSWCLKChannel );

	AddInterface( mRVSWDIOChannelInterface.get() );
	AddInterface( mSWCLKChannelInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mRVSWDIOChannel, "RVSWDIO", false );
	AddChannel( mSWCLKChannel, "SWCLK", false );
}

RVSWDAnalyzerSettings::~RVSWDAnalyzerSettings()
{
}

bool RVSWDAnalyzerSettings::SetSettingsFromInterfaces()
{
	mRVSWDIOChannel = mRVSWDIOChannelInterface->GetChannel();
	mSWCLKChannel = mSWCLKChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mRVSWDIOChannel, "RVSWDIO", true );
	AddChannel( mSWCLKChannel, "SWCLK", true );

	return true;
}

void RVSWDAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mRVSWDIOChannelInterface->SetChannel( mRVSWDIOChannel );
	mSWCLKChannelInterface->SetChannel( mSWCLKChannel );
}

void RVSWDAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mRVSWDIOChannel;
	text_archive >> mSWCLKChannel;

	ClearChannels();
	AddChannel( mRVSWDIOChannel, "RVSWDIO", true );
	AddChannel( mSWCLKChannel, "SWCLK", true );

	UpdateInterfacesFromSettings();
}

const char* RVSWDAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mRVSWDIOChannel;
	text_archive << mSWCLKChannel;

	return SetReturnString( text_archive.GetString() );
}

