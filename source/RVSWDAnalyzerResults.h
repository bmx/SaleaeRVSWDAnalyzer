#ifndef RVSWD_ANALYZER_RESULTS
#define RVSWD_ANALYZER_RESULTS

#include <AnalyzerResults.h>
enum RVFRameTypes {
	RV_Long,
	RV_Short
};



class RVSWDAnalyzer;
class RVSWDAnalyzerSettings;

class RVSWDAnalyzerResults : public AnalyzerResults
{
public:
	RVSWDAnalyzerResults( RVSWDAnalyzer* analyzer, RVSWDAnalyzerSettings* settings );
	virtual ~RVSWDAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	RVSWDAnalyzerSettings* mSettings;
	RVSWDAnalyzer* mAnalyzer;
private:
const char *REG[0x80] =
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

#endif //RVSWD_ANALYZER_RESULTS
