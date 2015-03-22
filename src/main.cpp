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

int main( int argc, char* argv[] )
{
	streampos size;
	uint32_t totalSize;
	uint32_t lastProg = 0;
	uint32_t currentProg = 0;
	char* memblock;
	char* bufferPtr;
	uint16_t recordSize;

	STDF_record* record;
	list<STDF_record*> recordList;
	pugi::xml_document doc;
	pugi::xml_node root;
	pugi::xml_node node;

	if ( argc < 2 )
	{
		cout << "File not found" << endl;
		return 1;
	}

	ifstream file( argv[1], ios::binary );

	if ( file.is_open() )
	{
		root = doc.append_child("root");
		size = Utils::getFileSize( file );
		totalSize = size;
		memblock = new char[size];
		bufferPtr = memblock;
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

//		std::cout << "<?xml version=\"1.0\"?>" << endl;
//		std::cout << "<root>" << endl;

		while (size > 0)
		{
			record = NULL;
			recordSize = static_cast<uint16_t>( *bufferPtr );

			if ( recordSize > 0 )
			{
				record = STDF_record::getRecordInstance( bufferPtr );
			}

			if ( record )
			{
				node = record->appendNode( root );
//				node.print( std::cout );
//				root.remove_child(node);
				delete record;
			}

			bufferPtr += HEADER_SIZE + recordSize;
			size -= HEADER_SIZE + recordSize;

			currentProg = static_cast<uint32_t>(100.0f * (totalSize - size) / totalSize);

			if ( ( currentProg - lastProg == PROG_STEP) )
			{
				system( "cls" );
				std::cout << currentProg << "%" << endl;
				lastProg = currentProg;
			}
		}
		delete[] memblock;

//		std::cout << "</xml>" << endl;
//		std::cout << "</root>" << endl;
		//doc.print( std::cout );
		doc.save_file( "test.xml" );
	}
	return 0;
}
