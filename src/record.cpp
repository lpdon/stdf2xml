/*The MIT License (MIT)

Copyright (c) 2015 lpdon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "record.h"

#include <stdlib.h>
#include "utils.h"
#include <string.h>

STDF_Record* STDF_Record::getRecordInstance( char* bufferPtr )
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
				case 63:
					return new PLR( length, bufferPtr );
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
void STDF_Record::appendChild<uint8_t>( pugi::xml_node& node,
				const string& variableName, uint8_t variable )
{
	pugi::xml_node childNode = node.append_child( variableName.c_str() );
		childNode.append_child( pugi::node_pcdata ).set_value(
				SSTR( static_cast<uint16_t>( variable ) ).c_str() );
}

template<>
void STDF_Record::appendChild<string>( pugi::xml_node& node,
				const string& variableName, string variable )
{
	const string value = "\"" + variable + "\"";
	pugi::xml_node childNode = node.append_child( variableName.c_str() );
		childNode.append_child( pugi::node_pcdata ).set_value( value.c_str() );
}

const uint8_t STDF_Record::readU1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readU1( bufferPtr );
	}
	return 0;
}

const uint8_t* STDF_Record::readKU1( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*1;
		return Utils::readKU1( bufferPtr, k );
	}
	return NULL;
}

const uint16_t STDF_Record::readU2( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 2;
		return Utils::readU2( bufferPtr );
	}
	return 0;
}

const uint32_t STDF_Record::readU4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readU4( bufferPtr );
	}
	return 0;
}

const int8_t STDF_Record::readI1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 1;
		return Utils::readI1( bufferPtr );
	}
	return 0;
}

const int16_t STDF_Record::readI2( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 2;
		return Utils::readI2( bufferPtr );
	}
	return 0;
}

const int32_t STDF_Record::readI4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readI4( bufferPtr );
	}
	return 0;
}

const char STDF_Record::readC1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readC1( bufferPtr );
	}
	return ' ';
}

const string STDF_Record::readCn( char*& bufferPtr )
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

const string* STDF_Record::readKCn( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		if ( k > 0 )
		{
			string* retPtr = new string[k];

			for( int i = 0; i < k; i++ )
			{
				uint16_t length = Utils::readU1( bufferPtr );
				bufferPtr--;
				bytesLeft -= length + 1;
				retPtr[k] = readCn( bufferPtr );
			}
			return retPtr;
		}
		return NULL;
	}
	return NULL;
}

const uint16_t* STDF_Record::readKU2( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*2;
		return Utils::readKU2( bufferPtr, k );
	}
	return NULL;
}

const uint8_t STDF_Record::readB1( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft--;
		return Utils::readB1( bufferPtr );
	}
	return 0;
}

const uint8_t* STDF_Record::readBn( char*& bufferPtr )
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

const float STDF_Record::readR4( char*& bufferPtr )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= 4;
		return Utils::readR4( bufferPtr );
	}
	return 0;
}

const float* STDF_Record::readKR4( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*4;
		return Utils::readKR4( bufferPtr, k );
	}
	return NULL;
}

const uint8_t* STDF_Record::readKN1( char*& bufferPtr, uint16_t k )
{
	if ( this->bytesLeft > 0 )
	{
		this->bytesLeft -= k*1;
		return Utils::readKN1( bufferPtr, k );
	}
	return NULL;
}

const uint8_t* STDF_Record::readDn( char*& bufferPtr )
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

STDF_Record::STDF_Record( string name, uint16_t length, char*& data ) :
		name( name ), length( length ), data( data ), bytesLeft( length )
{
}

FAR::FAR( int l, char*& d ) :
		STDF_Record( "FAR", l, d ), cpuType( 0 ), stdfVersion( 0 )
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "CPU_TYPE", this->cpuType );
	STDF_Record::appendChild( node, "STDF_VER", this->stdfVersion );

	return node;
}

ATR::ATR( int l, char*& d ) :
		STDF_Record( "ATR", l, d ), modTime( 0 )
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "MOD_TIME", this->modTime );
	STDF_Record::appendChild( node, "CMD_LINE", this->cmdLine );

	return node;
}

MIR::MIR( int l, char*& d ) :
		STDF_Record( "MIR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "SETUP_T", this->setupTime );
	STDF_Record::appendChild( node, "START_T", this->startTime );
	STDF_Record::appendChild( node, "STAT_NUM", this->stationNumber );
	STDF_Record::appendChild( node, "MODE_COD", this->modeCode );
	STDF_Record::appendChild( node, "RTST_COD", this->retestCode );
	STDF_Record::appendChild( node, "PROT_COD", this->protectionCode );
	STDF_Record::appendChild( node, "BURN_TIM", this->burnInTime );
	STDF_Record::appendChild( node, "CMOD_COD", this->commmandModeCode );
	STDF_Record::appendChild( node, "LOT_ID", this->lotId );
	STDF_Record::appendChild( node, "PART_TYP", this->partType );
	STDF_Record::appendChild( node, "NODE_NAM", this->nodeName );
	STDF_Record::appendChild( node, "TSTR_TYP", this->testerType );
	STDF_Record::appendChild( node, "JOB_NAM", this->jobName );
	STDF_Record::appendChild( node, "JOB_REV", this->jobRevision );
	STDF_Record::appendChild( node, "SBLOT_ID", this->sublotId );
	STDF_Record::appendChild( node, "OPER_NAM", this->operatorName );
	STDF_Record::appendChild( node, "EXEC_TYP", this->execType );
	STDF_Record::appendChild( node, "EXEC_VER", this->execVer );
	STDF_Record::appendChild( node, "TEST_COD", this->testCode );
	STDF_Record::appendChild( node, "TST_TEMP", this->testTemperature );
	STDF_Record::appendChild( node, "USER_TXT", this->userText );
	STDF_Record::appendChild( node, "AUX_FILE", this->auxFile );
	STDF_Record::appendChild( node, "PKG_TYP", this->pkgType );
	STDF_Record::appendChild( node, "FAMLY_ID", this->familyId );
	STDF_Record::appendChild( node, "DATE_COD", this->dateCode );
	STDF_Record::appendChild( node, "FACIL_ID", this->facilityId );
	STDF_Record::appendChild( node, "FLOOR_ID", this->floorId );
	STDF_Record::appendChild( node, "PROC_ID", this->processId );
	STDF_Record::appendChild( node, "OPER_FRQ", this->operationFrequency );
	STDF_Record::appendChild( node, "SPEC_NAM", this->specName );
	STDF_Record::appendChild( node, "SPEC_VER", this->specVer );
	STDF_Record::appendChild( node, "FLOW_ID", this->flowId );
	STDF_Record::appendChild( node, "SETUP_ID", this->setupId );
	STDF_Record::appendChild( node, "DSGN_REV", this->designRevision );
	STDF_Record::appendChild( node, "ENG_ID", this->engId );
	STDF_Record::appendChild( node, "ROM_COD", this->romCode );
	STDF_Record::appendChild( node, "SERL_NUM", this->serialNum );
	STDF_Record::appendChild( node, "SUPR_NAM", this->supervisiorName );

	return node;
}

MRR::MRR( int l, char*& d ) :
		STDF_Record( "MRR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "FINISH_T", this->finishTime );
	STDF_Record::appendChild( node, "DISP_COD", this->dispositionCode );
	STDF_Record::appendChild( node, "USR_DESC", this->userDesc );
	STDF_Record::appendChild( node, "EXC_DESC", this->excDesc );

	return node;
}

PCR::PCR( int l, char*& d ) :
		STDF_Record( "PCR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "PART_CNT", this->partCount );
	STDF_Record::appendChild( node, "RTST_CNT", this->retestCount );
	STDF_Record::appendChild( node, "ABRT_CNT", this->abortCount );
	STDF_Record::appendChild( node, "GOOD_CNT", this->goodCount );
	STDF_Record::appendChild( node, "FUNC_CNT", this->functionalCount );

	return node;
}

PMR::PMR( int l, char*& d ) :
		STDF_Record( "PMR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "PMR_INDX", this->pinIndex );
	STDF_Record::appendChild( node, "CHAN_TYP", this->channelType );
	STDF_Record::appendChild( node, "CHAN_NAM", this->channelName );
	STDF_Record::appendChild( node, "PHY_NAM", this->physicalName );
	STDF_Record::appendChild( node, "LOG_NAM", this->logicalName );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );

	return node;
}

HBR::HBR( int l, char*& d ) :
		STDF_Record( "HBR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "HBIN_NUM", this->hBinNumber );
	STDF_Record::appendChild( node, "HBIN_CNT", this->hBinCount );
	STDF_Record::appendChild( node, "HBIN_PF", this->hBinPassFail );
	STDF_Record::appendChild( node, "HBIN_NAM", this->hBinName );

	return node;
}

SBR::SBR( int l, char*& d ) :
		STDF_Record( "SBR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "SBIN_NUM", this->sBinNumber );
	STDF_Record::appendChild( node, "SBIN_CNT", this->sBinCount );
	STDF_Record::appendChild( node, "SBIN_PF", this->sBinPassFail );
	STDF_Record::appendChild( node, "SBIN_NAM", this->sBinName );

	return node;
}

PGR::PGR( int l, char*& d ) :
		STDF_Record( "PGR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "GRP_INDX", this->groupIndex );
	STDF_Record::appendChild( node, "GRP_NAM", this->groupName );
	STDF_Record::appendChild( node, "INDX_CNT", this->indexCount );

	if ( pmrIndex )
	{
		pugi::xml_node indexNode = node.append_child( "PMR_INDX" );
		for( int i = 0; i < indexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Index", this->pmrIndex[i] );
		}
	}

	return node;
}

PLR::PLR( int l, char*& d ) :
		STDF_Record( "PLR", l, d ),
		groupCount( 0 ),
		groupIndex( NULL ),
		groupMode( NULL ),
		groupRadix( NULL ),
		programChar( NULL ),
		returnChar( NULL ),
		programChal( NULL ),
		returnChal( NULL )
{
	decodeData();
}

PLR::~PLR()
{
	DELETE_BPTR( groupIndex );
	DELETE_BPTR( groupMode );
	DELETE_BPTR( groupRadix );
	DELETE_BPTR( programChar );
	DELETE_BPTR( returnChar );
	DELETE_BPTR( programChal );
	DELETE_BPTR( returnChal );
}

void PLR::decodeData()
{
	groupIndex = readKU2( data, groupCount );
	groupMode = readKU2( data, groupCount );
	groupRadix = readKU1( data, groupCount );
	programChar = readKCn( data, groupCount );
	returnChar = readKCn( data, groupCount );
	programChal = readKCn( data, groupCount );
	programChal = readKCn( data, groupCount );
}

pugi::xml_node PLR::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "PGR" );

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "GRP_CNT", this->groupCount );

	if ( groupCount )
	{
		pugi::xml_node groupIndexNode = node.append_child( "GRP_INDX" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( groupIndexNode, "Index", this->groupIndex[i] );
		}

		pugi::xml_node groupModeNode = node.append_child( "GRP_MODE" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( groupModeNode, "Mode", this->groupMode[i] );
		}

		pugi::xml_node groupRadixNode = node.append_child( "GRP_RADX" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( groupRadixNode, "Radix", this->groupRadix[i] );
		}

		pugi::xml_node programCharNode = node.append_child( "PGM_CHAR" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( programCharNode, "PChar", this->programChar[i] );
		}

		pugi::xml_node returnCharNode = node.append_child( "RTN_CHAR" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( returnCharNode, "RChar", this->returnChar[i] );
		}

		pugi::xml_node programChalNode = node.append_child( "PGM_CHAL" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( programChalNode, "PChar", this->programChal[i] );
		}

		pugi::xml_node returnChalNode = node.append_child( "RTN_CHAL" );
		for( int i = 0; i < groupCount; i++ )
		{
			STDF_Record::appendChild( returnChalNode, "RChar", this->returnChal[i] );
		}
	}

	return node;
}

RDR::RDR( int l, char*& d ) :
		STDF_Record( "RDR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "NUM_BINS", this->numBins );
	STDF_Record::appendChild( node, "RTST_BIN", this->rtstBin );

	return node;
}

SDR::SDR( int l, char*& d ):
		STDF_Record( "SDR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_GRP", this->siteGroup );
	STDF_Record::appendChild( node, "SITE_CNT", this->siteCount );

	if ( siteCount )
	{
		pugi::xml_node indexNode = node.append_child( "SITE_NUM" );
		for( int i = 0; i < siteCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Number", this->siteNumber[i] );
		}
	}


	STDF_Record::appendChild( node, "HAND_TYP", this->handType );
	STDF_Record::appendChild( node, "HAND_ID", this->handId );
	STDF_Record::appendChild( node, "CARD_TYP", this->cardType );
	STDF_Record::appendChild( node, "CARD_ID", this->cardId );
	STDF_Record::appendChild( node, "LOAD_TYP", this->loadType );
	STDF_Record::appendChild( node, "LOAD_ID", this->loadId );
	STDF_Record::appendChild( node, "DIB_TYP", this->dibType );
	STDF_Record::appendChild( node, "DIB_ID", this->dibId );
	STDF_Record::appendChild( node, "CABL_TYP", this->cableType );
	STDF_Record::appendChild( node, "CABL_ID", this->cableId );
	STDF_Record::appendChild( node, "CONT_TYP", this->contType );
	STDF_Record::appendChild( node, "CONT_ID", this->contId );
	STDF_Record::appendChild( node, "LASR_TYP", this->laserType );
	STDF_Record::appendChild( node, "LASR_ID", this->laserId );
	STDF_Record::appendChild( node, "EXTR_TYP", this->extraType );
	STDF_Record::appendChild( node, "EXTR_ID", this->extraId );

	return node;
}

WIR::WIR( int l, char*& d ) :
		STDF_Record( "WIR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_GRP", this->siteGroup );
	STDF_Record::appendChild( node, "START_T", this->startTime );
	STDF_Record::appendChild( node, "WAFER_ID", this->waferId );

	return node;
}

WRR::WRR( int l, char*& d) :
		STDF_Record( "WRR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_GRP", this->siteGroup );
	STDF_Record::appendChild( node, "FINISH_T", this->finishTime );
	STDF_Record::appendChild( node, "PART_CNT", this->partCount );
	STDF_Record::appendChild( node, "RTST_CNT", this->retestCount );
	STDF_Record::appendChild( node, "ABRT_CNT", this->abortCount );
	STDF_Record::appendChild( node, "GOOD_CNT", this->goodCount );
	STDF_Record::appendChild( node, "FUNC_CNT", this->funcCount );
	STDF_Record::appendChild( node, "WAFER_ID", this->waferId );
	STDF_Record::appendChild( node, "FABWF_ID", this->fabWaferId );
	STDF_Record::appendChild( node, "FRAME_ID", this->frameId );
	STDF_Record::appendChild( node, "MASK_ID", this->maskId );
	STDF_Record::appendChild( node, "USR_DESC", this->userDescription );
	STDF_Record::appendChild( node, "EXC_DESC", this->execDescription );



	return node;
}

WCR::WCR( int l, char*& d) :
		STDF_Record( "WCR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "WAFR_SIZ", this->waferSize );
	STDF_Record::appendChild( node, "DIE_HT", this->dieHeight );
	STDF_Record::appendChild( node, "DIE_WID", this->dieWidth );
	STDF_Record::appendChild( node, "WF_UNITS", this->waferUnits );
	STDF_Record::appendChild( node, "WF_FLAT", this->waferFlat );
	STDF_Record::appendChild( node, "CENTER_X", this->centerX );
	STDF_Record::appendChild( node, "CENTER_Y", this->centerY );
	STDF_Record::appendChild( node, "POS_X", this->posX );
	STDF_Record::appendChild( node, "POS_Y", this->posY );

	return node;
}

PIR::PIR( int l, char*& d ) :
		STDF_Record( "PIR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );

	return node;
}

PRR::PRR( int l, char*& d ) :
		STDF_Record( "PRR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "PART_FLG", this->partFlag );
	STDF_Record::appendChild( node, "NUM_TEST", this->numTest );
	STDF_Record::appendChild( node, "HARD_BIN", this->hardBin );
	STDF_Record::appendChild( node, "SOFT_BIN", this->softBin );
	STDF_Record::appendChild( node, "X_COORD", this->xCoord );
	STDF_Record::appendChild( node, "Y_COORD", this->yCoord );
	STDF_Record::appendChild( node, "TEST_T", this->testTime );
	STDF_Record::appendChild( node, "PART_ID", this->partId );
	STDF_Record::appendChild( node, "PART_TXT", this->partText );

	if ( partFix )
	{
		pugi::xml_node indexNode = node.append_child( "PART_FIX" );
		while( ++partFix )
		{
			STDF_Record::appendChild( indexNode, "Fix", this->partFix );
		}
	}

	return node;
}

TSR::TSR( int l, char*& d ):
		STDF_Record( "TSR", l, d),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "TEST_TYP", this->testType );
	STDF_Record::appendChild( node, "TEST_NUM", this->testNumber );
	STDF_Record::appendChild( node, "EXEC_CNT", this->execCount );
	STDF_Record::appendChild( node, "FAIL_CNT", this->failCount );
	STDF_Record::appendChild( node, "ALRM_CNT", this->alarmCount );
	STDF_Record::appendChild( node, "TEST_NAM", this->testName );
	STDF_Record::appendChild( node, "SEQ_NAME", this->seqName );
	STDF_Record::appendChild( node, "TEST_LBL", this->testLabel );
	STDF_Record::appendChild( node, "OPT_FLAG", this->optFlag );
	STDF_Record::appendChild( node, "TEST_TIM", this->testTime );
	STDF_Record::appendChild( node, "TEST_MIN", this->testMin );
	STDF_Record::appendChild( node, "TEST_MAX", this->testMax );
	STDF_Record::appendChild( node, "TST_SUMS", this->testSums );
	STDF_Record::appendChild( node, "TST_SQRS", this->testSquares );

	return node;
}

PTR::PTR( int l, char*& d ) :
		STDF_Record( "PTR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "TEST_NUM", this->testNumber );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "TEST_FLG", this->testFlag );
	STDF_Record::appendChild( node, "PARM_FLG", this->parmFlag );
	STDF_Record::appendChild( node, "RESULT", this->result );
	STDF_Record::appendChild( node, "TEST_TXT", this->testTxt );
	STDF_Record::appendChild( node, "ALARM_ID", this->alarmId );
	STDF_Record::appendChild( node, "OPT_FLAG", this->optFlag );
	STDF_Record::appendChild( node, "RES_SCAL", static_cast<uint16_t>( this->resScal ) );
	STDF_Record::appendChild( node, "LLM_SCAL", static_cast<uint16_t>( this->llmScal ) );
	STDF_Record::appendChild( node, "HLM_SCAL", static_cast<uint16_t>( this->hlmScal ) );
	STDF_Record::appendChild( node, "LO_LIMIT", this->loLimit );
	STDF_Record::appendChild( node, "HI_LIMIT", this->hiLimit );
	STDF_Record::appendChild( node, "UNITS", this->units );
	STDF_Record::appendChild( node, "C_RESFMT", this->cResFmt );
	STDF_Record::appendChild( node, "C_LLMFMT", this->cLlmFmt );
	STDF_Record::appendChild( node, "C_HLMFMT", this->cHlmFmt );
	STDF_Record::appendChild( node, "LO_SPEC", this->loSpec );
	STDF_Record::appendChild( node, "HI_SPEC", this->hiSpec );

	return node;
}

MPR::MPR( int l, char*& d ) :
		STDF_Record( "MPR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "TEST_NUM", this->testNumber );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "TEST_FLG", this->testFlag );
	STDF_Record::appendChild( node, "PARM_FLG", this->parmFlag );
	STDF_Record::appendChild( node, "RTN_ICNT", this->returnIndexCount );
	STDF_Record::appendChild( node, "RSLT_CNT", this->resultCount );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "RTN_STAT" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "State", this->returnState[i] );
		}
	}

	if ( resultCount )
	{
		pugi::xml_node indexNode = node.append_child( "RTN_RSLT" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Count", this->returnResult[i] );
		}
	}

	STDF_Record::appendChild( node, "TEST_TXT", this->testText );
	STDF_Record::appendChild( node, "ALARM_ID", this->alarmId );
	STDF_Record::appendChild( node, "OPT_FLAG", this->optFlag );
	STDF_Record::appendChild( node, "RES_SCAL", static_cast<uint16_t>( this->resScal ) );
	STDF_Record::appendChild( node, "LLM_SCAL", static_cast<uint16_t>( this->llmScal ) );
	STDF_Record::appendChild( node, "HLM_SCAL", static_cast<uint16_t>( this->hlmScal ) );
	STDF_Record::appendChild( node, "LO_LIMIT", this->loLimit );
	STDF_Record::appendChild( node, "HI_LIMIT", this->hiLimit );
	STDF_Record::appendChild( node, "START_IN", this->hiLimit );
	STDF_Record::appendChild( node, "INCR_IN", this->hiLimit );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "RTN_INDX" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Index", this->returnIndex[i] );
		}
	}

	STDF_Record::appendChild( node, "UNITS", this->units );
	STDF_Record::appendChild( node, "UNITS_IN", this->unitsInput );
	STDF_Record::appendChild( node, "C_RESFMT", this->cResFmt );
	STDF_Record::appendChild( node, "C_LLMFMT", this->cLlmFmt );
	STDF_Record::appendChild( node, "C_HLMFMT", this->cHlmFmt );
	STDF_Record::appendChild( node, "LO_SPEC", this->loSpec );
	STDF_Record::appendChild( node, "HI_SPEC", this->hiSpec );

	return node;
}

FTR::FTR( int l, char*& d ) :
		STDF_Record( "FTR", l, d ),
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "TEST_NUM", this->testNumber );
	STDF_Record::appendChild( node, "HEAD_NUM", this->headNumber );
	STDF_Record::appendChild( node, "SITE_NUM", this->siteNumber );
	STDF_Record::appendChild( node, "TEST_FLG", this->testFlag );
	STDF_Record::appendChild( node, "OPT_FLAG", this->optFlag );
	STDF_Record::appendChild( node, "CYCL_CNT", this->cycleCount );
	STDF_Record::appendChild( node, "REL_VADR", this->relativeVectorAddress );
	STDF_Record::appendChild( node, "REPT_CNT", this->repeatCount );
	STDF_Record::appendChild( node, "NUM_FAIL", this->numFail );
	STDF_Record::appendChild( node, "XFAIL_AD", this->xFailAddress );
	STDF_Record::appendChild( node, "YFAIL_AD", this->yFailAddress );
	STDF_Record::appendChild( node, "VECT_OFF", this->vectorOffset );
	STDF_Record::appendChild( node, "RTN_ICNT", this->returnIndexCount );
	STDF_Record::appendChild( node, "PGM_ICNT", this->progIndexCount );

	if ( returnIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "RTN" );
		for( int i = 0; i < returnIndexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Index", this->returnIndex[i] );
			STDF_Record::appendChild( indexNode, "State", this->returnState[i] );
		}
	}

	if ( progIndexCount )
	{
		pugi::xml_node indexNode = node.append_child( "PGM" );
		for( int i = 0; i < progIndexCount; i++ )
		{
			STDF_Record::appendChild( indexNode, "Index", this->progIndex[i] );
			STDF_Record::appendChild( indexNode, "State", this->progState[i] );
		}
	}

	if ( failPin )
	{
		pugi::xml_node indexNode = node.append_child( "FAIL_PIN" );
		while( ++failPin )
		{
			STDF_Record::appendChild( indexNode, "Pin", this->failPin );
		}
	}

	STDF_Record::appendChild( node, "VECT_NAM", this->vectorName );
	STDF_Record::appendChild( node, "TIME_SET", this->timeSet );
	STDF_Record::appendChild( node, "OP_CODE", this->opCode );
	STDF_Record::appendChild( node, "TEST_TXT", this->testTxt );
	STDF_Record::appendChild( node, "ALARM_ID", this->alarmId );
	STDF_Record::appendChild( node, "PROG_TXT", this->progTxt );
	STDF_Record::appendChild( node, "RSLT_TXT", this->resultTxt );
	STDF_Record::appendChild( node, "PATG_NUM", this->patternGenNumber );

	if ( spinMap )
	{
		pugi::xml_node indexNode = node.append_child( "SPIN_MAP" );
		while( ++spinMap )
		{
			STDF_Record::appendChild( indexNode, "Spin", this->spinMap );
		}
	}

	return node;
}

BPS::BPS( int l, char*& d) :
		STDF_Record( "BPS", l, d )
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "SEQ_NAME", this->seqName );

	return node;
}

EPS::EPS( int l, char*& d) :
		STDF_Record( "EPS", l, d )
{
	decodeData();
}

void EPS::decodeData()
{

}

pugi::xml_node EPS::appendNode( pugi::xml_node& root )
{
	pugi::xml_node node = root.append_child( "EPS" );

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );

	return node;
}

DTR::DTR( int l, char*& d) :
		STDF_Record( "DTR", l, d )
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

	STDF_Record::appendChild( node, "NAME", this->name );
	STDF_Record::appendChild( node, "REC_LEN", this->length );
	STDF_Record::appendChild( node, "TEXT_DAT", this->textData );

	return node;
}
