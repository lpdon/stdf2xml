#include "record.h"

#include <stdlib.h>
#include "utils.h"
#include <string.h>

STDF_record* STDF_record::getRecordInstance( char* bufferPtr )
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
				case 20:
					return new MRR( length, bufferPtr );
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
				case 70:
					return new RDR( length, bufferPtr );
				case 80:
					return new SDR( length, bufferPtr );
				default:
					break;
			}
			break;
		case 2:
			switch ( subType )
			{
				case 10:
					return new WIR( length, bufferPtr );
				case 20:
					return new WRR( length, bufferPtr );
				case 30:
					return new WCR( length, bufferPtr );
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
		case 10:
			switch ( subType )
			{
				case 30:
					return new TSR( length, bufferPtr );
				default:
					break;
			}
			break;
		case 15:
			switch ( subType )
			{
				case 10:
					return new PTR( length, bufferPtr );
				case 15:
					return new MPR( length, bufferPtr );
				case 20:
					return new FTR( length, bufferPtr );
				default:
					break;
			}
			break;
		case 20:
			switch ( subType )
			{
				case 10:
					return new BPS( length, bufferPtr );
				case 20:
					return new EPS( length, bufferPtr );
				default:
					break;
			}
			break;
		case 50:
			switch ( subType )
			{
				case 30:
					return new DTR( length, bufferPtr );
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

template<>
void STDF_record::appendChild<uint8_t>( pugi::xml_node& node,
				const string& variableName, uint8_t variable )
{
	pugi::xml_node childNode = node.append_child( variableName.c_str() );
		childNode.append_child( pugi::node_pcdata ).set_value(
				SSTR( static_cast<uint16_t>( variable ) ).c_str() );
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

const uint8_t* STDF_record::readKU1( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*1;
		return Utils::readKU1( bufferPtr, k );
	}
	return NULL;
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

const uint32_t STDF_record::readU4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readU4( bufferPtr );
	}
	return 0;
}

const int8_t STDF_record::readI1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 1;
		return Utils::readI1( bufferPtr );
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

const int32_t STDF_record::readI4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readI4( bufferPtr );
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
		this->bytesLeft -= k*2;
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
	if ( this->bytesLeft > 0 )
	{
		uint16_t fieldLength = Utils::readU1( bufferPtr );
		bufferPtr--;
		this->bytesLeft -= fieldLength + 1;
		return Utils::readBn( bufferPtr );
	}
	return NULL;
}

const float STDF_record::readR4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readR4( bufferPtr );
	}
	return 0;
}

const float* STDF_record::readKR4( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*4;
		return Utils::readKR4( bufferPtr, k );
	}
	return NULL;
}

const uint8_t* STDF_record::readKN1( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*1;
		return Utils::readKN1( bufferPtr, k );
	}
	return NULL;
}

const uint8_t* STDF_record::readDn( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		uint16_t fieldLength = Utils::readU2( bufferPtr );
		bufferPtr-=2;
		this->bytesLeft -= fieldLength + 2;
		return Utils::readDn( bufferPtr );
	}
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
}

void FAR::decodeData()
{
	cpuType = readU1( data );
	stdfVersion = readU1( data );
}

pugi::xml_node FAR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "FAR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "CPU", this->cpuType );
	STDF_record::appendChild( node, "Version", static_cast<uint16_t>( this->stdfVersion ) );

	return node;
}

ATR::ATR( int l, char*& d ) :
		STDF_record( "ATR", l, d ), modTime( 0 )
{
	decodeData();
}

void ATR::decodeData()
{
	modTime = readU4( data );
	cmdLine = readCn( data );
}

pugi::xml_node ATR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "ATR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "ModTime", this->modTime );
	STDF_record::appendChild( node, "CommandLine", this->cmdLine );

	return node;
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

pugi::xml_node MIR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "MIR" );

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

	return node;
}

MRR::MRR( int l, char*& d ) :
		STDF_record( "MRR", l, d ),
		finishTime( 0 ),
		dispositionCode( ' ' )
{
	decodeData();
}

void MRR::decodeData()
{
	finishTime = readU4( data );
	dispositionCode = readC1( data );
	userDesc = readCn( data );
	excDesc = readCn( data );
}

pugi::xml_node MRR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "MRR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "FinishTime", this->finishTime );
	STDF_record::appendChild( node, "DispositionCode", this->dispositionCode );
	STDF_record::appendChild( node, "UserDesc", this->userDesc );
	STDF_record::appendChild( node, "ExcDesc", this->excDesc );

	return node;
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

pugi::xml_node PCR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PCR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "PartCount", this->partCount );
	STDF_record::appendChild( node, "RetestCount", this->retestCount );
	STDF_record::appendChild( node, "AbortCount", this->abortCount );
	STDF_record::appendChild( node, "GoodCount", this->goodCount );
	STDF_record::appendChild( node, "FunctionalCount", this->functionalCount );

	return node;
}

PMR::PMR( int l, char*& d ) :
		STDF_record( "PMR", l, d ),
		pinIndex( 0 ),
		channelType( 0 ),
		headNumber( 0 ),
		siteNumber( 0 )
{
	decodeData();
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

pugi::xml_node PMR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PMR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "PinIndex", this->pinIndex );
	STDF_record::appendChild( node, "ChannelType", this->channelType );
	STDF_record::appendChild( node, "ChannelName", this->channelName );
	STDF_record::appendChild( node, "PhysicalName", this->physicalName );
	STDF_record::appendChild( node, "LogicalName", this->logicalName );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );

	return node;
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

pugi::xml_node HBR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "HBR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "HBinNumber", this->hBinNumber );
	STDF_record::appendChild( node, "HBinCount", this->hBinCount );
	STDF_record::appendChild( node, "HBinPassFail", this->hBinPassFail );
	STDF_record::appendChild( node, "HBinName", this->hBinName );

	return node;
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

pugi::xml_node SBR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "SBR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "SBinNumber", this->sBinNumber );
	STDF_record::appendChild( node, "SBinCount", this->sBinCount );
	STDF_record::appendChild( node, "SBinPassFail", this->sBinPassFail );
	STDF_record::appendChild( node, "SBinName", this->sBinName );

	return node;
}

PGR::PGR( int l, char*& d ) :
		STDF_record( "PGR", l, d ),
		groupIndex( 0 ),
		indexCount( 0 ),
		pmrIndex( NULL )
{
	decodeData();
}

PGR::~PGR()
{
	DELETE_BPTR( pmrIndex );
}

void PGR::decodeData()
{
	groupIndex = readU2( data );
	groupName = readCn( data );
	indexCount = readU2( data );
	pmrIndex = readKU2( data, indexCount );
}

pugi::xml_node PGR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PGR" );

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

	return node;
}

RDR::RDR( int l, char*& d ) :
		STDF_record( "RDR", l, d ),
		numBins( 0 ),
		rtstBin( NULL )
{
	decodeData();
}

RDR::~RDR()
{
	DELETE_BPTR( rtstBin );
}

void RDR::decodeData()
{
	numBins = readU2( data );
	rtstBin = readKU2( data, numBins );
}

pugi::xml_node RDR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "RDR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "NumBins", this->numBins );
	STDF_record::appendChild( node, "RetestBin", this->rtstBin );

	return node;
}

SDR::SDR( int l, char*& d ):
		STDF_record( "SDR", l, d ),
		headNumber( 0 ),
		siteGroup( 0 ),
		siteCount( 0 ),
		siteNumber( NULL )
{
	decodeData();
}

SDR::~SDR()
{
	DELETE_BPTR( siteNumber );
}

void SDR::decodeData()
{
	headNumber = readU1( data );
	siteGroup = readU1( data );
	siteCount = readU1( data );
	siteNumber = readKU1( data, siteCount );
	handType = readCn( data );
	handId = readCn( data );
	cardType = readCn( data );
	cardId = readCn( data );
	loadType = readCn( data );
	loadId = readCn( data );
	dibType = readCn( data );
	dibId = readCn( data );
	cableType = readCn( data );
	cableId = readCn( data );
	contType = readCn( data );
	contId = readCn( data );
	laserType = readCn( data );
	laserId = readCn( data );
	extraType = readCn( data );
	extraId = readCn( data );
}

pugi::xml_node SDR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "SDR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteGroup", this->siteGroup );
	STDF_record::appendChild( node, "SiteCount", this->siteCount );

	if ( siteCount )
	{
		pugi::xml_node indexNode = node.append_child( "SiteNumber" );
		for( int i = 0; i < siteCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Number", this->siteNumber[i] );
		}
	}


	STDF_record::appendChild( node, "HandType", this->handType );
	STDF_record::appendChild( node, "HandId", this->handId );
	STDF_record::appendChild( node, "CardType", this->cardType );
	STDF_record::appendChild( node, "CardId", this->cardId );
	STDF_record::appendChild( node, "LoadType", this->loadType );
	STDF_record::appendChild( node, "LoadId", this->loadId );
	STDF_record::appendChild( node, "DibType", this->dibType );
	STDF_record::appendChild( node, "DibId", this->dibId );
	STDF_record::appendChild( node, "CableType", this->cableType );
	STDF_record::appendChild( node, "CableId", this->cableId );
	STDF_record::appendChild( node, "ContType", this->contType );
	STDF_record::appendChild( node, "ContId", this->contId );
	STDF_record::appendChild( node, "LaserType", this->laserType );
	STDF_record::appendChild( node, "LaserId", this->laserId );
	STDF_record::appendChild( node, "ExtraType", this->extraType );
	STDF_record::appendChild( node, "ExtraId", this->extraId );

	return node;
}

WIR::WIR( int l, char*& d ) :
		STDF_record( "WIR", l, d ),
		headNumber( 0 ),
		siteGroup( 0 ),
		startTime( 0 )
{
	decodeData();
}

void WIR::decodeData()
{
	headNumber = readU1( data );
	siteGroup = readU1( data );
	startTime = readU4( data );
	waferId = readCn( data );
}

pugi::xml_node WIR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "WIR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteGroup", this->siteGroup );
	STDF_record::appendChild( node, "StartTime", this->startTime );
	STDF_record::appendChild( node, "WaferId", this->waferId );

	return node;
}

WRR::WRR( int l, char*& d) :
		STDF_record( "WRR", l, d ),
		headNumber( 0 ),
		siteGroup( 0 ),
		finishTime( 0 ),
		partCount( 0 ),
		retestCount( ' ' ),
		abortCount( 0 ),
		goodCount( 0 ),
		funcCount( 0 )
{
	decodeData();
}

void WRR::decodeData()
{
	headNumber = readU1( data );
	siteGroup = readU1( data );
	finishTime = readU4( data );
	partCount = readU4( data );
	retestCount = readU4( data );
	abortCount = readU4( data );
	goodCount = readU4( data );
	funcCount = readU4( data );
	waferId = readCn( data );
	fabWaferId = readCn( data );
	frameId = readCn( data );
	maskId = readCn( data );
	userDescription = readCn( data );
	execDescription = readCn( data );
}

pugi::xml_node WRR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "WRR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteGroup", this->siteGroup );
	STDF_record::appendChild( node, "FinishTime", this->finishTime );
	STDF_record::appendChild( node, "PartCount", this->partCount );
	STDF_record::appendChild( node, "RetestCount", this->retestCount );
	STDF_record::appendChild( node, "AbortCount", this->abortCount );
	STDF_record::appendChild( node, "GoodCount", this->goodCount );
	STDF_record::appendChild( node, "FuncCount", this->funcCount );
	STDF_record::appendChild( node, "WaferId", this->waferId );
	STDF_record::appendChild( node, "FabWaferId", this->fabWaferId );
	STDF_record::appendChild( node, "FrameId", this->frameId );
	STDF_record::appendChild( node, "MaskId", this->maskId );
	STDF_record::appendChild( node, "UserDescription", this->userDescription );
	STDF_record::appendChild( node, "ExecDescription", this->execDescription );



	return node;
}

WCR::WCR( int l, char*& d) :
		STDF_record( "WCR", l, d ),
		waferSize( 0 ),
		dieHeight( 0 ),
		dieWidth( 0 ),
		waferUnits( 0 ),
		waferFlat( ' ' ),
		centerX( 0 ),
		centerY( 0 ),
		posX( ' ' ),
		posY( ' ' )
{
	decodeData();
}

void WCR::decodeData()
{
	waferSize = readR4( data );
	dieHeight = readR4( data );
	dieWidth = readR4( data );
	waferUnits = readU1( data );
	waferFlat = readC1( data );
	centerX = readI2( data );
	centerY = readI2( data );
	posX = readC1( data );
	posY = readC1( data );
}

pugi::xml_node WCR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "WCR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "WaferSize", this->waferSize );
	STDF_record::appendChild( node, "DieHeight", this->dieHeight );
	STDF_record::appendChild( node, "DieWidth", this->dieWidth );
	STDF_record::appendChild( node, "WaferUnits", this->waferUnits );
	STDF_record::appendChild( node, "WaferFlat", this->waferFlat );
	STDF_record::appendChild( node, "CenterX", this->centerX );
	STDF_record::appendChild( node, "CenterY", this->centerY );
	STDF_record::appendChild( node, "PosX", this->posX );
	STDF_record::appendChild( node, "PosY", this->posY );

	return node;
}

PIR::PIR( int l, char*& d ) :
		STDF_record( "PIR", l, d ),
		headNumber( 0 ),
		siteNumber( 0 )
{
	decodeData();
}

void PIR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
}

pugi::xml_node PIR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PIR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );

	return node;
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
		testTime( 0 ),
		partFix( NULL )
{
	decodeData();
}

PRR::~PRR()
{
	DELETE_BPTR( partFix );
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
	partFix = readBn( data );
}

pugi::xml_node PRR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PRR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "PartFlag", this->partFlag );
	STDF_record::appendChild( node, "NumTest", this->numTest );
	STDF_record::appendChild( node, "HardBin", this->hardBin );
	STDF_record::appendChild( node, "SoftBin", this->softBin );
	STDF_record::appendChild( node, "XCoord", this->xCoord );
	STDF_record::appendChild( node, "YCoord", this->yCoord );
	STDF_record::appendChild( node, "TestTime", this->testTime );
	STDF_record::appendChild( node, "PartId", this->partId );
	STDF_record::appendChild( node, "PartText", this->partText );

	if ( partFix )
	{
		pugi::xml_node indexNode = node.append_child( "PartFix" );
		while( ++partFix )
		{
			STDF_record::appendChild( indexNode, "Fix", this->partFix );
		}
	}

	return node;
}

TSR::TSR( int l, char*& d ):
		STDF_record( "TSR", l, d),
		headNumber( 0 ),
		siteNumber( 0 ),
		testType( 0 ),
		testNumber( 0 ),
		execCount( 0 ),
		failCount( 0 ),
		alarmCount( 0 ),
		optFlag( 0 ),
		testTime( 0 ),
		testMin( 0 ),
		testMax( 0 ),
		testSums( 0 ),
		testSquares( 0 )
{
	decodeData();
}

void TSR::decodeData()
{
	headNumber = readU1( data );
	siteNumber = readU1( data );
	testType = readC1( data );
	testNumber = readU4( data );
	execCount = readU4( data );
	failCount = readU4( data );
	alarmCount = readU4( data );
	testName = readCn( data );
	seqName = readCn( data );
	testLabel = readCn( data );
	optFlag = readB1( data );
	testTime = readR4( data );
	testMin = readR4( data );
	testMax = readR4( data );
	testSums = readR4( data );
	testSquares = readR4( data );
}

pugi::xml_node TSR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "TSR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "TestType", this->testType );
	STDF_record::appendChild( node, "TestNumber", this->testNumber );
	STDF_record::appendChild( node, "ExecCount", this->execCount );
	STDF_record::appendChild( node, "FailCount", this->failCount );
	STDF_record::appendChild( node, "AlarmCount", this->alarmCount );
	STDF_record::appendChild( node, "SeqName", this->seqName );
	STDF_record::appendChild( node, "TestLabel", this->testLabel );
	STDF_record::appendChild( node, "OptFlag", this->optFlag );
	STDF_record::appendChild( node, "TestTime", this->testTime );
	STDF_record::appendChild( node, "TestMin", this->testMin );
	STDF_record::appendChild( node, "TestMax", this->testMax );
	STDF_record::appendChild( node, "TestSums", this->testSums );
	STDF_record::appendChild( node, "TestSquares", this->testSquares );

	return node;
}

PTR::PTR( int l, char*& d ) :
		STDF_record( "PTR", l, d ),
		testNumber( 0 ),
		headNumber( 0 ),
		siteNumber( 0 ),
		testFlag( 0 ),
		parmFlag( 0 ),
		result( 0 ),
		optFlag( 0 ),
		resScal( 0 ),
		llmScal( 0 ),
		hlmScal( 0 ),
		loLimit( 0 ),
		hiLimit( 0 ),
		loSpec( 0 ),
		hiSpec( 0 )
{
	decodeData();
}

void PTR::decodeData()
{
	testNumber = readU4( data );
	headNumber = readU1( data );
	siteNumber = readU1( data );
	testFlag = readB1( data );
	parmFlag = readB1( data );
	result = readR4( data );
	testTxt = readCn( data );
	alarmId = readCn( data );
	optFlag = readB1( data );
	resScal = readI1( data );
	llmScal = readI1( data );
	hlmScal = readI1( data );
	loLimit = readR4( data );
	hiLimit = readR4( data );
	units = readCn( data );
	cResFmt = readCn( data );
	cLlmFmt = readCn( data );
	cHlmFmt = readCn( data );
	loSpec = readR4( data );
	hiSpec = readR4( data );
}

pugi::xml_node PTR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PTR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "TestNumber", this->testNumber );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "TestFlag", this->testFlag );
	STDF_record::appendChild( node, "ParmFlag", this->parmFlag );
	STDF_record::appendChild( node, "Result", this->result );
	STDF_record::appendChild( node, "TestTxt", this->testTxt );
	STDF_record::appendChild( node, "AlarmId", this->alarmId );
	STDF_record::appendChild( node, "OptFlag", this->optFlag );
	STDF_record::appendChild( node, "ResScal", this->resScal );
	STDF_record::appendChild( node, "LlmScal", this->llmScal );
	STDF_record::appendChild( node, "HlmScal", this->hlmScal );
	STDF_record::appendChild( node, "loLimit", this->loLimit );
	STDF_record::appendChild( node, "hiLimit", this->hiLimit );
	STDF_record::appendChild( node, "units", this->units );
	STDF_record::appendChild( node, "cResFmt", this->cResFmt );
	STDF_record::appendChild( node, "cLlmFmt", this->cLlmFmt );
	STDF_record::appendChild( node, "cHlmFmt", this->cHlmFmt );
	STDF_record::appendChild( node, "loSpec", this->loSpec );
	STDF_record::appendChild( node, "hiSpec", this->hiSpec );

	return node;
}

MPR::MPR( int l, char*& d ) :
		STDF_record( "MPR", l, d ),
		testNumber( 0 ),
		headNumber( 0 ),
		siteNumber( 0 ),
		testFlag( 0 ),
		parmFlag( 0 ),
		returnIndexCount( 0 ),
		resultCount( 0 ),
		returnState( NULL ),
		returnResult( NULL ),
		optFlag( 0 ),
		resScal( 0 ),
		llmScal( 0 ),
		hlmScal( 0 ),
		loLimit( 0 ),
		hiLimit( 0 ),
		startInput( 0 ),
		incrementInput( 0 ),
		returnIndex( NULL ),
		loSpec( 0 ),
		hiSpec( 0 )
{
	decodeData();
}

MPR::~MPR()
{
	DELETE_BPTR( returnState );
	DELETE_BPTR( returnResult );
	DELETE_BPTR( returnIndex );
}

void MPR::decodeData()
{
	testNumber = readU4( data );
	headNumber = readU1( data );
	siteNumber = readU1( data );
	testFlag = readB1( data );
	parmFlag = readB1( data );
	returnIndexCount = readU2( data );
	resultCount = readU2( data );
	returnState = readKN1( data, returnIndexCount );
	returnResult = readKR4( data, resultCount );
	testText = readCn( data );
	alarmId = readCn( data );
	optFlag = readB1( data );
	resScal = readI1( data );
	llmScal = readI1( data );
	hlmScal = readI1( data );
	loLimit = readR4( data );
	hiLimit = readR4( data );
	startInput = readR4( data );
	incrementInput = readR4( data );
	returnIndex = readKU2( data, returnIndexCount );
	units = readCn( data );
	unitsInput = readCn( data );
	cResFmt = readCn( data );
	cLlmFmt = readCn( data );
	cHlmFmt = readCn( data );
	loSpec = readR4( data );
	hiSpec = readR4( data );
}

pugi::xml_node MPR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "MPR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "TestNumber", this->testNumber );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "TestFlag", this->testFlag );
	STDF_record::appendChild( node, "ParmFlag", this->parmFlag );
	STDF_record::appendChild( node, "ReturnIndexCount", this->returnIndexCount );
	STDF_record::appendChild( node, "ResultCount", this->resultCount );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "ReturnState" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "State", this->returnState[i] );
		}
	}

	if ( resultCount )
	{
		pugi::xml_node indexNode = node.append_child( "ResultCount" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Count", this->returnResult[i] );
		}
	}

	STDF_record::appendChild( node, "TestText", this->testText );
	STDF_record::appendChild( node, "AlarmId", this->alarmId );
	STDF_record::appendChild( node, "OptFlag", this->optFlag );
	STDF_record::appendChild( node, "ResScal", this->resScal );
	STDF_record::appendChild( node, "LlmScal", this->llmScal );
	STDF_record::appendChild( node, "HlmScal", this->hlmScal );
	STDF_record::appendChild( node, "loLimit", this->loLimit );
	STDF_record::appendChild( node, "hiLimit", this->hiLimit );
	STDF_record::appendChild( node, "startInput", this->hiLimit );
	STDF_record::appendChild( node, "incrementInput", this->hiLimit );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "ReturnIndex" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Index", this->returnIndex[i] );
		}
	}

	STDF_record::appendChild( node, "units", this->units );
	STDF_record::appendChild( node, "unitsInput", this->unitsInput );
	STDF_record::appendChild( node, "cResFmt", this->cResFmt );
	STDF_record::appendChild( node, "cLlmFmt", this->cLlmFmt );
	STDF_record::appendChild( node, "cHlmFmt", this->cHlmFmt );
	STDF_record::appendChild( node, "loSpec", this->loSpec );
	STDF_record::appendChild( node, "hiSpec", this->hiSpec );

	return node;
}

FTR::FTR( int l, char*& d ) :
		STDF_record( "FTR", l, d ),
		testNumber( 0 ),
		headNumber( 0 ),
		siteNumber( 0 ),
		testFlag( 0 ),
		optFlag( 0 ),
		cycleCount( 0 ),
		relativeVectorAddress( 0 ),
		repeatCount( 0 ),
		numFail( 0 ),
		xFailAddress( 0 ),
		yFailAddress( 0 ),
		vectorOffset( 0 ),
		returnIndexCount( 0 ),
		progIndexCount( 0 ),
		returnIndex( NULL ),
		returnState( NULL ),
		progIndex( NULL ),
		progState( NULL ),
		failPin( NULL ),
		patternGenNumber( 0 ),
		spinMap( NULL )
{
	decodeData();
}

FTR::~FTR()
{
	DELETE_BPTR( returnIndex );
	DELETE_BPTR( returnState );
	DELETE_BPTR( progIndex );
	DELETE_BPTR( progState );
	DELETE_BPTR( failPin );
	DELETE_BPTR( spinMap );
}

void FTR::decodeData()
{
	testNumber = readU4( data );
	headNumber = readU1( data );
	siteNumber = readU1( data );
	testFlag = readB1( data );
	optFlag = readB1( data );
	cycleCount = readU4( data );
	relativeVectorAddress = readU4( data );
	repeatCount = readU4( data );
	numFail = readU4( data );
	xFailAddress = readI4( data );
	yFailAddress = readI4( data );
	vectorOffset = readI2( data );
	returnIndexCount = readU2( data );
	progIndexCount = readU2( data );
	returnIndex = readKU2( data, returnIndexCount );
	returnState = readKN1( data, returnIndexCount );
	progIndex = readKU2( data, progIndexCount );
	progState = readKN1( data, progIndexCount );
	failPin = readDn( data );
	vectorName = readCn( data );
	timeSet = readCn( data );
	opCode = readCn( data );
	testTxt = readCn( data );
	alarmId = readCn( data );
	progTxt = readCn( data );
	resultTxt = readCn( data );
	patternGenNumber = readU1( data );
	spinMap = readDn( data );
}

pugi::xml_node FTR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "FTR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "TestNumber", this->testNumber );
	STDF_record::appendChild( node, "HeadNumber", this->headNumber );
	STDF_record::appendChild( node, "SiteNumber", this->siteNumber );
	STDF_record::appendChild( node, "TestFlag", this->testFlag );
	STDF_record::appendChild( node, "OptFlag", this->optFlag );
	STDF_record::appendChild( node, "CycleCount", this->cycleCount );
	STDF_record::appendChild( node, "RelativeVectorAddress", this->relativeVectorAddress );
	STDF_record::appendChild( node, "RepeatCount", this->repeatCount );
	STDF_record::appendChild( node, "NumFail", this->numFail );
	STDF_record::appendChild( node, "XFailAddress", this->xFailAddress );
	STDF_record::appendChild( node, "YFailAddress", this->yFailAddress );
	STDF_record::appendChild( node, "VectorOffset", this->vectorOffset );
	STDF_record::appendChild( node, "ReturnIndexCount", this->returnIndexCount );
	STDF_record::appendChild( node, "ProgIndexCount", this->progIndexCount );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "Return" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Index", this->returnIndex[i] );
			STDF_record::appendChild( indexNode, "State", static_cast<uint16_t>( this->returnState[i] ) );
		}
	}

	if ( progIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "ProgIndexes" );
		for( int i = 0; i < progIndexCount; i++ )
		{
			STDF_record::appendChild( indexNode, "Index", this->progIndex[i] );
			STDF_record::appendChild( indexNode, "State", static_cast<uint16_t>( this->progState[i] ) );
		}
	}

	if ( failPin )
	{
		pugi::xml_node indexNode = node.append_child( "FailPin" );
		while( ++failPin )
		{
			STDF_record::appendChild( indexNode, "Pin", this->failPin );
		}
	}

	STDF_record::appendChild( node, "VectorName", this->vectorName );
	STDF_record::appendChild( node, "TimeSet", this->timeSet );
	STDF_record::appendChild( node, "OpCode", this->opCode );
	STDF_record::appendChild( node, "TestTxt", this->testTxt );
	STDF_record::appendChild( node, "AlarmId", this->alarmId );
	STDF_record::appendChild( node, "ProgTxt", this->progTxt );
	STDF_record::appendChild( node, "ResultTxt", this->resultTxt );
	STDF_record::appendChild( node, "PatternGenNumber", this->patternGenNumber );

	if ( spinMap )
	{
		pugi::xml_node indexNode = node.append_child( "SpinMap" );
		while( ++spinMap )
		{
			STDF_record::appendChild( indexNode, "Spin", this->spinMap );
		}
	}

	return node;
}

BPS::BPS( int l, char*& d) :
		STDF_record( "BPS", l, d )
{
	decodeData();
}

void BPS::decodeData()
{
	seqName = readCn( data );
}

pugi::xml_node BPS::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "BPS" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "SeqName", this->seqName );

	return node;
}

EPS::EPS( int l, char*& d) :
		STDF_record( "EPS", l, d )
{
	decodeData();
}

void EPS::decodeData()
{

}

pugi::xml_node EPS::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "EPS" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );

	return node;
}

DTR::DTR( int l, char*& d) :
		STDF_record( "DTR", l, d )
{
	decodeData();
}

void DTR::decodeData()
{
	textData = readCn( data );
}

pugi::xml_node DTR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "DTR" );

	STDF_record::appendChild( node, "name", this->name );
	STDF_record::appendChild( node, "length", this->length );
	STDF_record::appendChild( node, "TextData", this->textData );

	return node;
}
