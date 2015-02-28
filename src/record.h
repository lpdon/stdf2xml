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

		virtual void appendNode( pugi::xml_node& root ) = 0;

		template<typename T>
		static void appendChild( pugi::xml_node& node,
				const string& variableName, T variable );

		static STDF_record* getRecordInstance( char*& bufferPtr );

		const uint8_t readU1( char*& bufferPtr );
		const uint16_t readU2( char*& bufferPtr );
		const uint32_t readU4( char*& bufferPtr );
		const char readC1( char*& bufferPtr );
		const string readCn( char*& bufferPtr );

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
		virtual void appendNode( pugi::xml_node& root );
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
		virtual void appendNode( pugi::xml_node& root );

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
		virtual void appendNode( pugi::xml_node& root );
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
		virtual void appendNode( pugi::xml_node& root );
};

/*Pin Map Record*/
class PMR: public STDF_record
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
		virtual void appendNode( pugi::xml_node& root );
};

#endif /* RECORD_H_ */
