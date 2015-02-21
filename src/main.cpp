//============================================================================
// Name        : STDF2XML.cpp
// Author      : pidgey
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <list>

#include "utils.h"
#include "record.h"

#include "xml/pugixml.hpp"

#define HEADER_SIZE 4

using namespace std;

int main(int argc, char* argv[]) {
	streampos size;
	string filePath = "D:/Coisas TI/full code/full code/C-2409102-11-E4.DLOG.STD";
	char* memblock;
	char* bufferPtr;
	uint16_t recordSize;

	STDF_record* record;
	list<STDF_record*> recordList;
    pugi::xml_document doc;

	ifstream file( filePath.c_str(), ios::binary );

	if( file.is_open() )
	{
		size = Utils::getFileSize( file );
		memblock = new char [size];
		bufferPtr = memblock;
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();

		while ( size > 0 )
		{
			recordSize = static_cast<uint16_t>( *bufferPtr );

			if ( recordSize > 0)
			{
				record = STDF_record::getRecordInstance( bufferPtr );
			}

			if ( record )
			{
				record->appendNode( doc );
				delete record;
			}

			size -= HEADER_SIZE + recordSize;
		}
		delete [] memblock;

	    doc.print(std::cout);
	}
	return 0;
}
