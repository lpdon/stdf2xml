#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

//http://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//http://stackoverflow.com/questions/9444988/how-to-print-a-variables-name-in-c
#define VNAME(x) #x
#define VDUMP(x) std::cout << #x << " " << x << std::endl

#define DELETE_PTR( x ) if( x ) delete x
#define DELETE_BPTR( x ) if( x ) delete [] x

class Utils
{
	public:
		static uint32_t getFileSize( std::ifstream& file );
		static const uint8_t readU1( char*& bufferPtr );
		static const uint8_t* readKU1( char*& bufferPtr, uint16_t k );
		static const uint16_t readU2( char*& bufferPtr );
		static const uint16_t* readKU2( char*& bufferPtr, uint16_t k );
		static const uint32_t readU4( char*& bufferPtr );
		static const int8_t readI1( char*& bufferPtr );
		static const int16_t readI2( char*& bufferPtr );
		static const int32_t readI4( char*& bufferPtr );
		static const char readC1( char*& bufferPtr );
		static const string readCn( char*& bufferPtr );
		static const uint8_t readB1( char*& bufferPtr );
		static const uint8_t* readBn( char*& bufferPtr );
		static const float readR4( char*& bufferPtr );
		static const float* readKR4( char*& bufferPtr, uint16_t k );
		static const uint8_t* readKN1( char*& bufferPtr, uint16_t k );
		static const uint8_t* readDn( char*& bufferPtr );
};

#endif /* UTILS_H_ */
