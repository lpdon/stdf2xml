#include "record.h"

#include <stdlib.h>
#include "utils.h"
#include <string.h>

STDF_record* STDF_record::getRecordInstance(char*& bufferPtr)
{
	uint16_t length = Utils::readU2(bufferPtr);
	uint8_t type = Utils::readU1(bufferPtr);
	uint8_t subType = Utils::readU1(bufferPtr);

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
	if( this->bytesLeft > 0)
	{
		this->bytesLeft--;
		return Utils::readU1( bufferPtr );
	}
	return 0;
}

const uint16_t STDF_record::readU2(char*& bufferPtr)
{
	if( this->bytesLeft > 0)
	{
		this->bytesLeft -= 2;
		return Utils::readU2( bufferPtr );
	}
	return 0;
}

const uint32_t STDF_record::readU4( char*& bufferPtr )
{
	if( this->bytesLeft > 0)
	{
		this->bytesLeft -= 4;
		return Utils::readU4( bufferPtr );
	}
	return 0;
}

const char STDF_record::readC1( char*& bufferPtr )
{
	if( this->bytesLeft > 0)
	{
		this->bytesLeft --;
		return Utils::readC1( bufferPtr );
	}
	return ' ';
}

const string STDF_record::readCn( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		uint16_t recordLength = Utils::readU1( bufferPtr );
		bufferPtr --;
		this->bytesLeft -= recordLength + 1;
		return Utils::readCn( bufferPtr );
	}
	return string();
}

STDF_record::STDF_record( string name, uint16_t length, char*& data )
	: name(name), length(length), data(data), bytesLeft(length)
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
	cpuType = readU1(data);
	stdfVersion = readU1(data);
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
	setupTime = readU4(data);
	startTime = readU4(data);
	stationNumber = readU1(data);
	modeCode = readC1(data);
	retestCode = readC1(data);
	protectionCode = readC1(data);
	burnInTime = readU2(data);
	commmandModeCode = readC1(data);
	lotId = readCn(data);
	partType = readCn(data);
	nodeName = readCn(data);
	testerType = readCn(data);
	jobName = readCn(data);
	jobRevision = readCn(data);
	sublotId = readCn(data);
	operatorName = readCn(data);
	execType = readCn(data);
	execVer = readCn(data);
	testCode = readCn(data);
	testTemperature = readCn(data);
	userText = readCn(data);
	auxFile = readCn(data);
	pkgType = readCn(data);
	familyId = readCn(data);
	dateCode = readCn(data);
	facilityId = readCn(data);
	floorId = readCn(data);
	processId = readCn(data);
	operationFrequency = readCn(data);
	specName = readCn(data);
	specVer = readCn(data);
	flowId = readCn(data);
	setupId = readCn(data);
	designRevision = readCn(data);
	engId = readCn(data);
	romCode = readCn(data);
	serialNum = readCn(data);
	supervisiorName = readCn(data);
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
	STDF_record::appendChild(node, "specName", this->specName);
	STDF_record::appendChild(node, "specVer", this->specVer);
	STDF_record::appendChild(node, "flowId", this->flowId);
	STDF_record::appendChild(node, "setupId", this->setupId);
	STDF_record::appendChild(node, "designRevision", this->designRevision);
	STDF_record::appendChild(node, "engId", this->engId);
	STDF_record::appendChild(node, "romCode", this->romCode);
	STDF_record::appendChild(node, "serialNum", this->serialNum);
	STDF_record::appendChild(node, "supervisiorName", this->supervisiorName);
}

