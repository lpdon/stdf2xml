#include "record.h"

#include <stdlib.h>
#include "utils.h"
#include <string.h>

STDF_record* STDF_record::getRecordInstance( char*& bufferPtr )
{
	uint16_t length = Utils::readU2( bufferPtr );
	uint8_t type = Utils::readU1( bufferPtr );
	uint8_t subType = Utils::readU1( bufferPtr );

	switch ( type )
	{
		case 0:
			switch ( subType )
			{
				case 10:
					return new FAR( length, bufferPtr );
				case 20:
					return new ATR( length, bufferPtr );
				default:
					break;
			}
			break;
		case 1:
			switch ( subType )
			{
				case 10:
					return new MIR( length, bufferPtr );
				case 30:
					return new PCR( length, bufferPtr );
				case 40:
					return new HBR( length, bufferPtr );
				case 50:
					return new SBR( length, bufferPtr );
				case 60:
					return new PMR( length, bufferPtr );
				case 62:
					return new PGR( length, bufferPtr );
				default:
					break;
			}
			break;
		case 5:
			switch ( subType )
			{
				case 10:
					return new PIR( length, bufferPtr );
				case 20:
					return new PRR( length, bufferPtr );
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

const uint8_t STDF_record::readU1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readU1( bufferPtr );
	}
	return 0;
}

const uint16_t STDF_record::readU2( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 2;
		return Utils::readU2( bufferPtr );
	}
	return 0;
}

const int16_t STDF_record::readI2( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 2;
		return Utils::readI2( bufferPtr );
	}
	return 0;
}

const uint32_t STDF_record::readU4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readU4( bufferPtr );
	}
	return 0;
}

const char STDF_record::readC1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readC1( bufferPtr );
	}
	return ' ';
}

const string STDF_record::readCn( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		uint16_t fieldLength = Utils::readU1( bufferPtr );
		bufferPtr--;
		this->bytesLeft -= fieldLength + 1;
		return Utils::readCn( bufferPtr );
	}
	return string();
}

const uint16_t* STDF_record::readKU2( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		uint16_t fieldLength = Utils::readU2( bufferPtr );
		bufferPtr-=2;
		this->bytesLeft -= fieldLength + 2;
		return Utils::readKU2( bufferPtr, k );
	}
	return NULL;
}

const uint8_t STDF_record::readB1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readB1( bufferPtr );
	}
	return 0;
}

const uint8_t* STDF_record::readBn( char*& bufferPtr )
{
//	if ( this->bytesLeft > 0 )
//	{
//		uint16_t fieldLength = Utils::readU1( bufferPtr );
//		bufferPtr--;
//		this->bytesLeft -= fieldLength + 1;
//
//		for( int i = 0; i < bytesLeft; i++ )
//		{
//
//		}
//		return Utils::readCn( bufferPtr );
//	}
//	return string();
	return NULL;
}

STDF_record::STDF_record( string name, uint16_t length, char*& data ) :
		name( name ), length( length ), data( data ), bytesLeft( length )
{
}

FAR::FAR( int l, char*& d ) :
		STDF_record( "FAR", l, d ), cpuType( 0 ), stdfVersion( 0 )
{
	decodeData();
	d += l;
}

void FAR::decodeData()
{
	cpuType = readU1( data );
	stdfVersion = readU1( data );
}

void FAR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "CPU", static_cast<uint16_t>( this->cpuType ) );
	STDF_record::appendChild( node, "Version", static_cast<uint16_t>( this->stdfVersion ) );
}

ATR::ATR( int l, char*& d ) :
		STDF_record( "ATR", l, d ), modTime( 0 )
{
	decodeData();
	d += l;
}

void ATR::decodeData()
{
	modTime = readU4( data );
	cmdLine = readCn( data );
}

void ATR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "ModTime", this->modTime );
	STDF_record::appendChild( node, "CommandLine", this->cmdLine );
}

MIR::MIR( int l, char*& d ) :
		STDF_record( "MIR", l, d ),
		setupTime( 0 ),
		startTime( 0 ),
		stationNumber( 0 ),
		modeCode( 0 ),
		retestCode( 0 ),
		protectionCode( 0 ),
		burnInTime( 0 )
{
	decodeData();
	d += l;
}

void MIR::decodeData()
{
	setupTime = readU4( data );
	startTime = readU4( data );
	stationNumber = readU1( data );
	modeCode = readC1( data );
	retestCode = readC1( data );
	protectionCode = readC1( data );
	burnInTime = readU2( data );
	commmandModeCode = readC1( data );
	lotId = readCn( data );
	partType = readCn( data );
	nodeName = readCn( data );
	testerType = readCn( data );
	jobName = readCn( data );
	jobRevision = readCn( data );
	sublotId = readCn( data );
	operatorName = readCn( data );
	execType = readCn( data );
	execVer = readCn( data );
	testCode = readCn( data );
	testTemperature = readCn( data );
	userText = readCn( data );
	auxFile = readCn( data );
	pkgType = readCn( data );
	familyId = readCn( data );
	dateCode = readCn( data );
	facilityId = readCn( data );
	floorId = readCn( data );
	processId = readCn( data );
	operationFrequency = readCn( data );
	specName = readCn( data );
	specVer = readCn( data );
	flowId = readCn( data );
	setupId = readCn( data );
	designRevision = readCn( data );
	engId = readCn( data );
	romCode = readCn( data );
	serialNum = readCn( data );
	supervisiorName = readCn( data );
}

void MIR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "setupTime", this->setupTime );
	STDF_record::appendChild( node, "startTime", this->startTime );
	STDF_record::appendChild( node, "stationNumber", this->stationNumber );
	STDF_record::appendChild( node, "modeCode", this->modeCode );
	STDF_record::appendChild( node, "retestCode", this->retestCode );
	STDF_record::appendChild( node, "protectionCode", this->protectionCode );
	STDF_record::appendChild( node, "burnInTime", this->burnInTime );
	STDF_record::appendChild( node, "commmandModeCode", this->commmandModeCode );
	STDF_record::appendChild( node, "lotId", this->lotId );
	STDF_record::appendChild( node, "partType", this->partType );
	STDF_record::appendChild( node, "nodeName", this->nodeName );
	STDF_record::appendChild( node, "testerType", this->testerType );
	STDF_record::appendChild( node, "jobName", this->jobName );
	STDF_record::appendChild( node, "jobRevision", this->jobRevision );
	STDF_record::appendChild( node, "sublotId", this->sublotId );
	STDF_record::appendChild( node, "operatorName", this->operatorName );
	STDF_record::appendChild( node, "execType", this->execType );
	STDF_record::appendChild( node, "execVer", this->execVer );
	STDF_record::appendChild( node, "testCode", this->testCode );
	STDF_record::appendChild( node, "testTemperature", this->testTemperature );
	STDF_record::appendChild( node, "auxFile", this->auxFile );
	STDF_record::appendChild( node, "pkgType", this->pkgType );
	STDF_record::appendChild( node, "familyId", this->familyId );
	STDF_record::appendChild( node, "dateCode", this->dateCode );
	STDF_record::appendChild( node, "facilityId", this->facilityId );
	STDF_record::appendChild( node, "floorId", this->floorId );
	STDF_record::appendChild( node, "processId", this->processId );
	STDF_record::appendChild( node, "operationFrequency", this->operationFrequency );
	STDF_record::appendChild( node, "specName", this->specName );
	STDF_record::appendChild( node, "specVer", this->specVer );
	STDF_record::appendChild( node, "flowId", this->flowId );
	STDF_record::appendChild( node, "setupId", this->setupId );
	STDF_record::appendChild( node, "designRevision", this->designRevision );
	STDF_record::appendChild( node, "engId", this->engId );
	STDF_record::appendChild( node, "romCode", this->romCode );
	STDF_record::appendChild( node, "serialNum", this->serialNum );
	STDF_record::appendChild( node, "supervisiorName", this->supervisiorName );
}

PCR::PCR( int l, char*& d ) :
		STDF_record( "PCR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 ),
		partCount( 0 ),
		retestCount( 0 ),
		abortCount( 0 ),
		goodCount( 0 ),
		functionalCount( 0 )
{
	decodeData();
	d += l;
}

void PCR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
	partCount = readU4( data );
	retestCount = readU4( data );
	abortCount = readU4( data );
	goodCount = readU4( data );
	functionalCount = readU4( data );
}

void PCR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "PartCount", this->partCount );
	STDF_record::appendChild( node, "RetestCount", this->retestCount );
	STDF_record::appendChild( node, "AbortCount", this->abortCount );
	STDF_record::appendChild( node, "GoodCount", this->goodCount );
	STDF_record::appendChild( node, "FunctionalCount", this->functionalCount );
}

PMR::PMR( int l, char*& d ) :
		STDF_record( "PCR", l, d ),
		pinIndex( 0 ),
		channelType( 0 ),
		headNumber( 0 ),
		siteNumber( 0 )
{
	decodeData();
	d += l;
}

void PMR::decodeData()
{
	pinIndex = readU2( data );
	channelType = readU2( data );
	channelName = readCn( data );
	physicalName = readCn( data );
	logicalName = readCn( data );
	headNumber = readU1( data );
	siteNumber = readU1( data );
}

void PMR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "PinIndex", this->pinIndex );
	STDF_record::appendChild( node, "ChannelType", this->channelType );
	STDF_record::appendChild( node, "ChannelName", this->channelName );
	STDF_record::appendChild( node, "PhysicalName", this->physicalName );
	STDF_record::appendChild( node, "LogicalName", this->logicalName );
	STDF_record::appendChild( node, "HeadNumber", static_cast<uint16_t>( this->headNumber ) );
	STDF_record::appendChild( node, "SiteNumber", static_cast<uint16_t>( this->siteNumber ) );
}

HBR::HBR( int l, char*& d ) :
		STDF_record( "HBR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 ),
		hBinNumber( 0 ),
		hBinCount( 0 ),
		hBinPassFail( ' ' )
{
	decodeData();
	d += l;
}

void HBR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
	hBinNumber = readU2( data );
	hBinCount = readU4( data );
	hBinPassFail = readC1( data );
	hBinName = readCn( data );
}

void HBR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", static_cast<uint16_t>( this->headNumber ) );
	STDF_record::appendChild( node, "SiteNumber", static_cast<uint16_t>( this->siteNumber ) );
	STDF_record::appendChild( node, "HBinNumber", this->hBinNumber );
	STDF_record::appendChild( node, "HBinCount", this->hBinCount );
	STDF_record::appendChild( node, "HBinPassFail", this->hBinPassFail );
	STDF_record::appendChild( node, "HBinName", this->hBinName );
}

SBR::SBR( int l, char*& d ) :
		STDF_record( "SBR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 ),
		sBinNumber( 0 ),
		sBinCount( 0 ),
		sBinPassFail( ' ' )
{
	decodeData();
	d += l;
}

void SBR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
	sBinNumber = readU2( data );
	sBinCount = readU4( data );
	sBinPassFail = readC1( data );
	sBinName = readCn( data );
}

void SBR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", static_cast<uint16_t>( this->headNumber ) );
	STDF_record::appendChild( node, "SiteNumber", static_cast<uint16_t>( this->siteNumber ) );
	STDF_record::appendChild( node, "SBinNumber", this->sBinNumber );
	STDF_record::appendChild( node, "SBinCount", this->sBinCount );
	STDF_record::appendChild( node, "SBinPassFail", this->sBinPassFail );
	STDF_record::appendChild( node, "SBinName", this->sBinName );
}

PGR::PGR( int l, char*& d ) :
		STDF_record( "PGR", l, d ),
		groupIndex( 0 ),
		indexCount( 0 ),
		pmrIndex( NULL )
{
	decodeData();
	d += l;
}

PGR::~PGR()
{
	if ( pmrIndex )
	{
		delete [] pmrIndex;
	}
}

void PGR::decodeData()
{
	groupIndex = readU2( data );
	groupName = readCn( data );
	indexCount = readU2( data );
	pmrIndex = const_cast<uint16_t*>( readKU2( data, indexCount ) );
}

void PGR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "GroupIndex", this->groupIndex );
	STDF_record::appendChild( node, "GroupName", this->groupName );
	STDF_record::appendChild( node, "IndexCount", this->indexCount );

	if ( pmrIndex )
	{
		pugi::xml_node indexNode = node.append_child( "PmrIndexes" );
		for( int i = 0; i < indexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Index", this->pmrIndex[i] );
		}
	}
}

PIR::PIR( int l, char*& d ) :
		STDF_record( "PIR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 )
{
	decodeData();
	d += l;
}

void PIR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
}

void PIR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", static_cast<uint16_t>( this->headNumber ) );
	STDF_record::appendChild( node, "SiteNumber", static_cast<uint16_t>( this->siteNumber ) );
}

PRR::PRR( int l, char*& d ) :
		STDF_record( "PRR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 ),
		partFlag( 0 ),
		numTest( 0 ),
		hardBin( 0 ),
		softBin( 0 ),
		xCoord( 0 ),
		yCoord( 0 ),
		testTime( 0 )
{
	memset(partFix, 0, 255);
	decodeData();
	d += l;
}

void PRR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
	partFlag = readB1( data );
	numTest = readU2( data );
	hardBin = readU2( data );
	softBin = readU2( data );
	xCoord = readI2( data );
	yCoord = readI2( data );
	testTime = readU4( data );
	partId = readCn( data );
	partText = readCn( data );
	//partFix = readBn( data );
}

void PRR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "record" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", static_cast<uint16_t>( this->headNumber ) );
	STDF_record::appendChild( node, "SiteNumber", static_cast<uint16_t>( this->siteNumber ) );
	STDF_record::appendChild( node, "PartFlag", this->partFlag );
	STDF_record::appendChild( node, "NumTest", this->numTest );
	STDF_record::appendChild( node, "HardBin", this->hardBin );
	STDF_record::appendChild( node, "SoftBin", this->softBin );
	STDF_record::appendChild( node, "XCoord", this->xCoord );
	STDF_record::appendChild( node, "YCoord", this->yCoord );
	STDF_record::appendChild( node, "TestTime", this->testTime );
	STDF_record::appendChild( node, "PartId", this->partId );
	STDF_record::appendChild( node, "PartText", this->partText );
	//STDF_record::appendChild( node, "PartFix", this->partFix );
}
