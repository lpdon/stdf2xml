#ifndef RECORD_H_
#define RECORD_H_

#include <string.h>
#include <string>

#include "xml/pugixml.hpp"
#include "utils.h"

#define BUFFER_SIZE 255

using namespace std;

/*Class that represents a generic record*/
class STDF_record
{
	protected:
		const string name;
		const uint16_t length;
		char* data;

		uint16_t bytesLeft;

		STDF_record( string name, uint16_t length, char*& data );
		virtual void decodeData() = 0;

	public:
		void setData( char* d );

		static string getName();
		char* getData();

		virtual pugi::xml_node appendNode( pugi::xml_node& root ) = 0;

		template<typename T>
		static void appendChild( pugi::xml_node& node,
				const string& variableName, T variable );

		static STDF_record* getRecordInstance( char* bufferPtr );

		const uint8_t readU1( char*& bufferPtr );
		const uint16_t readU2( char*& bufferPtr );
		const uint16_t* readKU2( char*& bufferPtr, uint16_t k );
		const uint32_t readU4( char*& bufferPtr );
		const int8_t readI1( char*& bufferPtr );
		const int16_t readI2( char*& bufferPtr );
		const int32_t readI4( char*& bufferPtr );
		const char readC1( char*& bufferPtr );
		const string readCn( char*& bufferPtr );
		const uint8_t readB1( char*& bufferPtr );
		const uint8_t* readBn( char*& bufferPtr );
		const float readR4( char*& bufferPtr );
		const uint8_t* readKN1( char*& bufferPtr, uint16_t k );
		const uint8_t* readDn( char*& bufferPtr );

		virtual ~STDF_record()
		{
		}
		;
};

template<typename T>
void STDF_record::appendChild( pugi::xml_node& node, const string& variableName,
		T variable )
{
	pugi::xml_node childNode = node.append_child( variableName.c_str() );
	childNode.append_child( pugi::node_pcdata ).set_value(
			SSTR( variable ).c_str() );
}

/*File Attributes Record*/
class FAR: public STDF_record
{
	private:
		uint8_t cpuType;
		uint8_t stdfVersion;

		virtual void decodeData();

	public:
		FAR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Audit Trail Record*/
class ATR: public STDF_record
{
	private:
		uint32_t modTime;
		string cmdLine;

		virtual void decodeData();

	public:
		ATR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );

};

/*Master Information Record*/
class MIR: public STDF_record
{
	private:
		uint32_t setupTime;
		uint32_t startTime;
		uint8_t stationNumber;
		char modeCode;
		char retestCode;
		char protectionCode;
		uint16_t burnInTime;
		string commmandModeCode;
		string lotId;
		string partType;
		string nodeName;
		string testerType;
		string jobName;
		string jobRevision;
		string sublotId;
		string operatorName;
		string execType;
		string execVer;
		string testCode;
		string testTemperature;
		string userText;
		string auxFile;
		string pkgType;
		string familyId;
		string dateCode;
		string facilityId;
		string floorId;
		string processId;
		string operationFrequency;
		string specName;
		string specVer;
		string flowId;
		string setupId;
		string designRevision;
		string engId;
		string romCode;
		string serialNum;
		string supervisiorName;

		virtual void decodeData();

	public:
		MIR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Master Results Record*/
class MRR : public STDF_record
{
	private:
		uint32_t finishTime;
		char dispositionCode;
		string userDesc;
		string excDesc;

		virtual void decodeData();

	public:
		MRR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Part Count Record*/
class PCR: public STDF_record
{
	private:
		uint8_t headNumber;
		uint8_t siteNumber;
		uint32_t partCount;
		uint32_t retestCount;
		uint32_t abortCount;
		uint32_t goodCount;
		uint32_t functionalCount;

		virtual void decodeData();

	public:
		PCR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Pin Map Record*/
class PMR : public STDF_record
{
	private:
		uint16_t pinIndex;
		uint16_t channelType;
		string channelName;
		string physicalName;
		string logicalName;
		uint8_t headNumber;
		uint8_t siteNumber;

		virtual void decodeData();

	public:
		PMR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Hardware Bin Record*/
class HBR : public STDF_record
{
	private:
		uint8_t headNumber;
		uint8_t siteNumber;
		uint16_t hBinNumber;
		uint32_t hBinCount;
		char hBinPassFail;
		string hBinName;

		virtual void decodeData();

	public:
		HBR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Software Bin Record*/
class SBR : public STDF_record
{
	private:
		uint8_t headNumber;
		uint8_t siteNumber;
		uint16_t sBinNumber;
		uint32_t sBinCount;
		char sBinPassFail;
		string sBinName;

		virtual void decodeData();

	public:
		SBR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Pin Group Record*/
class PGR : public STDF_record
{
	private:
		uint16_t groupIndex;
		string groupName;
		uint16_t indexCount;
		const uint16_t* pmrIndex;

		virtual void decodeData();

	public:
		PGR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );

		virtual ~PGR();
};

/*Retest Data Record*/
class RDR : public STDF_record
{
	private:
		uint16_t numBins;
		const uint16_t* rtstBin;

		virtual void decodeData();

	public:
		RDR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );

		virtual ~RDR();
};

/*Part Information Record*/
class PIR : public STDF_record
{
	private:
		uint8_t headNumber;
		uint8_t siteNumber;

		virtual void decodeData();

	public:
		PIR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Part Results Record*/
class PRR : public STDF_record
{
	private:
		uint8_t headNumber;
		uint8_t siteNumber;
		uint8_t partFlag;
		uint16_t numTest;
		uint16_t hardBin;
		uint16_t softBin;
		int16_t xCoord;
		int16_t yCoord;
		uint32_t testTime;
		string partId;
		string partText;
		const uint8_t* partFix;

		virtual void decodeData();

	public:
		PRR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );

		virtual ~PRR();
};

/*Parametric Test Record*/
class PTR : public STDF_record
{
	private:
		uint32_t testNumber;
		uint8_t headNumber;
		uint8_t siteNumber;
		uint8_t testFlag;
		uint8_t parmFlag;
		float result;
		string testTxt;
		string alarmId;
		uint8_t optFlag;
		int8_t resScal;
		int8_t llmScal;
		int8_t hlmScal;
		float loLimit;
		float hiLimit;
		string units;
		string cResFmt;
		string cLlmFmt;
		string cHlmFmt;
		float loSpec;
		float hiSpec;

		virtual void decodeData();

	public:
		PTR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );
};

/*Functional Test Record*/
class FTR : public STDF_record
{
	private:
		uint32_t testNumber;
		uint8_t headNumber;
		uint8_t siteNumber;
		uint8_t testFlag;
		uint8_t optFlag;
		uint32_t cycleCount;
		uint32_t relativeVectorAddress;
		uint32_t repeatCount;
		uint32_t numFail;
		int32_t xFailAddress;
		int32_t yFailAddress;
		int16_t vectorOffset;
		uint16_t returnIndexCount;
		uint16_t progIndexCount;
		const uint16_t* returnIndex;
		const uint8_t* returnState;
		const uint16_t* progIndex;
		const uint8_t* progState;
		const uint8_t* failPin;
		string vectorName;
		string timeSet;
		string opCode;
		string testTxt;
		string alarmId;
		string progTxt;
		string resultTxt;
		uint8_t patternGenNumber;
		const uint8_t* spinMap;

		virtual void decodeData();

	public:
		FTR( int l, char*& d );
		virtual pugi::xml_node appendNode( pugi::xml_node& root );

		virtual ~FTR();
};


#endif /* RECORD_H_ */
