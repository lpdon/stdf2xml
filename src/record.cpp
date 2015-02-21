#include "record.h"

#include <stdlib.h>
#include "utils.h"
#include <string.h>

STDF_record* STDF_record::getRecordInstance(char*& bufferPtr)
{
	uint16_t length = STDF_record::readU2(bufferPtr);
	uint8_t type = STDF_record::readU1(bufferPtr);
	uint8_t subType = STDF_record::readU1(bufferPtr);

	switch( type )
	{
		case 0:
			switch ( subType )
			{
				case 10:
					return new FAR(length, bufferPtr);
				default:
					break;
			}
			break;
		case 1:
			switch ( subType )
			{
				case 10:
					return new MIR(length, bufferPtr);
				default:
					break;
			}
			break;
		default:
			break;
	}
	bufferPtr += length;
	return NULL;
}

const uint8_t STDF_record::readU1(char*& bufferPtr)
{
	uint8_t value;
	memcpy(&value, bufferPtr, 1);
	bufferPtr++;
	return value;
}

const uint16_t STDF_record::readU2(char*& bufferPtr)
{
	uint16_t value;
	memcpy(&value, bufferPtr, 2);
	bufferPtr+=2;
	return value;
}

const uint32_t STDF_record::readU4( char*& bufferPtr )
{
	uint32_t value;
	memcpy(&value, bufferPtr, 4);
	bufferPtr += 4;
	return value;
}

const char STDF_record::readC1( char*& bufferPtr )
{
	char value = static_cast<char>( *bufferPtr );
	bufferPtr ++;
	return value;
}

const string STDF_record::readCn( char*& bufferPtr )
{
	uint16_t length = STDF_record::readU1(bufferPtr);

	if ( length > 0)
	{
		char* tempBuffer = new char[length];
		memcpy(tempBuffer, bufferPtr, length);
		string returnStr(tempBuffer, length);
		delete [] tempBuffer;
		bufferPtr += length;
		return returnStr;
	}
	return string();
}

STDF_record::STDF_record( string name, uint16_t length, char*& data )
	: name(name), length(length), data(data)
{}

FAR::FAR( int l, char*& d )
	: STDF_record("FAR", l, d),
	  cpuType(0),
	  stdfVersion(0)
{
	decodeData();
	d = data;
}

void FAR::decodeData()
{
	cpuType = STDF_record::readU1(data);
	stdfVersion = STDF_record::readU1(data);
}

void FAR::appendNode(pugi::xml_document& doc)
{
	pugi::xml_node node = doc.append_child("record");

	STDF_record::appendChild(node, "name", this->name);
	STDF_record::appendChild(node, "length", this->length);
	STDF_record::appendChild(node, "CPU", this->cpuType);
	STDF_record::appendChild(node, "Version", this->stdfVersion);
}

MIR::MIR( int l, char*& d )
	: STDF_record("MIR", l, d),
	  setupTime(0),
	  startTime(0),
	  stationNumber(0),
	  modeCode(0),
	  retestCode(0),
	  protectionCode(0),
	  burnInTime(0)
{
	decodeData();
	d = data;
}

void MIR::decodeData()
{
	setupTime = STDF_record::readU4(data);
	startTime = STDF_record::readU4(data);
	stationNumber = STDF_record::readU1(data);
	modeCode = STDF_record::readC1(data);
	retestCode = STDF_record::readC1(data);
	protectionCode = STDF_record::readC1(data);
	burnInTime = STDF_record::readU2(data);
	commmandModeCode = STDF_record::readC1(data);
	lotId = STDF_record::readCn(data);
	partType = STDF_record::readCn(data);
	nodeName = STDF_record::readCn(data);
	testerType = STDF_record::readCn(data);
	jobName = STDF_record::readCn(data);
	jobRevision = STDF_record::readCn(data);
	sublotId = STDF_record::readCn(data);
	operatorName = STDF_record::readCn(data);
	execType = STDF_record::readCn(data);
	execVer = STDF_record::readCn(data);
	testCode = STDF_record::readCn(data);
	testTemperature = STDF_record::readCn(data);
	userText = STDF_record::readCn(data);
	auxFile = STDF_record::readCn(data);
	pkgType = STDF_record::readCn(data);
	familyId = STDF_record::readCn(data);
	dateCode = STDF_record::readCn(data);
	facilityId = STDF_record::readCn(data);
	floorId = STDF_record::readCn(data);
	processId = STDF_record::readCn(data);
	operationFrequency = STDF_record::readCn(data);
}

void MIR::appendNode( pugi::xml_document& doc )
{
	pugi::xml_node node = doc.append_child("record");

	STDF_record::appendChild(node, "name", this->name);
	STDF_record::appendChild(node, "length", this->length);
	STDF_record::appendChild(node, "setupTime", this->setupTime);
	STDF_record::appendChild(node, "startTime", this->startTime);
	STDF_record::appendChild(node, "stationNumber", this->stationNumber);
	STDF_record::appendChild(node, "modeCode", this->modeCode);
	STDF_record::appendChild(node, "retestCode", this->retestCode);
	STDF_record::appendChild(node, "protectionCode", this->protectionCode);
	STDF_record::appendChild(node, "burnInTime", this->burnInTime);
	STDF_record::appendChild(node, "commmandModeCode", this->commmandModeCode);
	STDF_record::appendChild(node, "lotId", this->lotId);
	STDF_record::appendChild(node, "partType", this->partType);
	STDF_record::appendChild(node, "nodeName", this->nodeName);
	STDF_record::appendChild(node, "testerType", this->testerType);
	STDF_record::appendChild(node, "jobName", this->jobName);
	STDF_record::appendChild(node, "jobRevision", this->jobRevision);
	STDF_record::appendChild(node, "sublotId", this->sublotId);
	STDF_record::appendChild(node, "operatorName", this->operatorName);
	STDF_record::appendChild(node, "execType", this->execType);
	STDF_record::appendChild(node, "execVer", this->execVer);
	STDF_record::appendChild(node, "testCode", this->testCode);
	STDF_record::appendChild(node, "testTemperature", this->testTemperature);
	STDF_record::appendChild(node, "auxFile", this->auxFile);
	STDF_record::appendChild(node, "pkgType", this->pkgType);
	STDF_record::appendChild(node, "familyId", this->familyId);
	STDF_record::appendChild(node, "dateCode", this->dateCode);
	STDF_record::appendChild(node, "facilityId", this->facilityId);
	STDF_record::appendChild(node, "floorId", this->floorId);
	STDF_record::appendChild(node, "processId", this->processId);
	STDF_record::appendChild(node, "operationFrequency", this->operationFrequency);
}

