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

#include <inttypes.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <list>

#include "utils.h"
#include "record.h"

#include "xml/pugixml.hpp"

#define HEADER_SIZE 4
#define PROG_STEP 1

using namespace std;

int stdf2xml( int argc, char* argv[] )
{
	string inputName;
	string outputName;

	switch( argc )
	{
		case 3:
		{
			inputName = string( argv[1] );
			outputName = string( argv[2] );
			break;
		}
		case 0:
		case 1:
		case 2:
		default:
		{
			cout << "Invalid input" << endl;
			return 1;
		}
	}

	ifstream inputFile( inputName.c_str(), ios::binary );
	ofstream outputFile( outputName.c_str() );

	streampos size;
	uint32_t totalSize;
	uint32_t lastProg = 0;
	uint32_t currentProg = 0;
	char* memblock;
	char* bufferPtr;
	uint16_t recordSize;

	STDF_Record* record;
	list<STDF_Record*> recordList;
	pugi::xml_document doc;
	pugi::xml_node root;
	pugi::xml_node node;

	if ( inputFile.is_open() && outputFile.is_open() )
	{
		std::cout << "Loading file..." << endl;
		root = doc.append_child("root");
		size = Utils::getFileSize( inputFile );
		totalSize = size;
		memblock = new char[size];
		bufferPtr = memblock;
		inputFile.seekg( 0, ios::beg );
		inputFile.read( memblock, size );
		inputFile.close();

		outputFile << "<?xml version=\"1.0\"?>" << endl;
		outputFile << "<root>" << endl;

		while (size > 0)
		{
			record = NULL;
			recordSize = static_cast<uint16_t>( *bufferPtr );

			if ( recordSize > 0 )
			{
				record = STDF_Record::getRecordInstance( bufferPtr );
			}

			if ( record )
			{
				node = record->appendNode( root );
				node.print( outputFile );
				root.remove_child( node );
				delete record;
			}

			bufferPtr += HEADER_SIZE + recordSize;
			size -= HEADER_SIZE + recordSize;

			currentProg = static_cast<uint32_t>(100.0f * (totalSize - size) / totalSize);

			if ( ( currentProg - lastProg == PROG_STEP) )
			{
				system( "cls" );
				std::cout << "Converting file..." << endl;
				std::cout << currentProg << "%" << endl;
				lastProg = currentProg;
			}
		}
		delete [] memblock;

		outputFile << "</root>" << endl;
		outputFile.close();
	}
	std::cout << "Finished!" << endl;
	return 0;
}

int main( int argc, char* argv[] )
{
	return stdf2xml( argc, argv );
}
