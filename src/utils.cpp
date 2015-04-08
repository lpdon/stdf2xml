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

const uint8_t* Utils::readKU1( char*& bufferPtr, uint16_t k )
{
	if ( k > 0 )
	{
		uint8_t* retPtr = new uint8_t[k];

		for( int i = 0; i < k; i++ )
		{
			memcpy( &retPtr[i], bufferPtr, 1 );
			bufferPtr += 1;
		}
		return retPtr;
	}
	return NULL;
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

const int8_t Utils::readI1( char*& bufferPtr )
{
	int8_t value;
	memcpy( &value, bufferPtr, 1 );
	bufferPtr++;
	return value;
}

const int16_t Utils::readI2( char*& bufferPtr )
{
	int16_t value;
	memcpy( &value, bufferPtr, 2 );
	bufferPtr += 2;
	return value;
}

const int32_t Utils::readI4( char*& bufferPtr )
{
	int32_t value;
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
		delete [] tempBuffer;
		bufferPtr += length;
		return returnStr;
	}
	return string();
}

const uint16_t* Utils::readKU2( char*& bufferPtr, uint16_t k )
{
	if ( k > 0 )
	{
		uint16_t* retPtr = new uint16_t[k];

		for( int i = 0; i < k; i++ )
		{
			memcpy( &retPtr[i], bufferPtr, 2 );
			bufferPtr += 2;
		}
		return retPtr;
	}
	return NULL;
}

const uint8_t Utils::readB1( char*& bufferPtr )
{
	return Utils::readU1( bufferPtr );
}

const uint8_t* Utils::readBn( char*& bufferPtr )
{
	uint16_t length = Utils::readU1( bufferPtr );

	if ( length > 0 )
	{
		uint8_t* tempBuffer = new uint8_t[length + 1];
		memcpy( tempBuffer, bufferPtr, length );
		tempBuffer[length + 1] = 0;
		bufferPtr += length;
		return tempBuffer;
	}
	return NULL;
}

const float Utils::readR4( char*& bufferPtr )
{
	float value;
	memcpy( &value, bufferPtr, 4 );
	bufferPtr += 4;
	return value;
}

const float* Utils::readKR4( char*& bufferPtr, uint16_t k )
{
	if ( k > 0 )
	{
		float* retPtr = new float[k];

		for( int i = 0; i < k; i++ )
		{
			memcpy( &retPtr[i], bufferPtr, 4 );
			bufferPtr += 4;
		}
		return retPtr;
	}
	return NULL;
}

const uint8_t* Utils::readKN1( char*& bufferPtr, uint16_t k )
{
	if ( k > 0 )
	{
		uint8_t* retPtr = new uint8_t[k];

		for( int i = 0; i < k; i++ )
		{
			memcpy( &retPtr[i], bufferPtr, 1 );
			bufferPtr ++;
		}
		return retPtr;
	}
	else
	{
		return NULL;
	}
}

const uint8_t* Utils::readDn( char*& bufferPtr )
{
	uint16_t length = Utils::readU2( bufferPtr );

	if ( length > 0 )
	{
		uint8_t* tempBuffer = new uint8_t[length + 1];
		memcpy( tempBuffer, bufferPtr, length );
		tempBuffer[length + 1] = 0;
		bufferPtr += length;
		return tempBuffer;
	}
	return NULL;
}
