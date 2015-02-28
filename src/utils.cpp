#include "utils.h"

using namespace std;

uint32_t Utils::getFileSize( std::ifstream& file )
{
	file.seekg( 0, ios::end );
	return static_cast<uint32_t>( file.tellg() );
}

const uint8_t Utils::readU1( char*& bufferPtr )
{
	uint8_t value;
	memcpy( &value, bufferPtr, 1 );
	bufferPtr++;
	return value;
}

const uint16_t Utils::readU2( char*& bufferPtr )
{
	uint16_t value;
	memcpy( &value, bufferPtr, 2 );
	bufferPtr += 2;
	return value;
}

const uint32_t Utils::readU4( char*& bufferPtr )
{
	uint32_t value;
	memcpy( &value, bufferPtr, 4 );
	bufferPtr += 4;
	return value;
}

const char Utils::readC1( char*& bufferPtr )
{
	char value = static_cast<char>( *bufferPtr );
	bufferPtr++;
	return value;
}

const string Utils::readCn( char*& bufferPtr )
{
	uint16_t length = Utils::readU1( bufferPtr );

	if ( length > 0 )
	{
		char* tempBuffer = new char[length];
		memcpy( tempBuffer, bufferPtr, length );
		string returnStr( tempBuffer, length );
		delete[] tempBuffer;
		bufferPtr += length;
		return returnStr;
	}
	return string();
}
