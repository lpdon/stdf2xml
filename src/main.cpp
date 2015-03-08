#include <iostream>
#include <fstream>
#include <list>

#include "utils.h"
#include "record.h"

#include "xml/pugixml.hpp"

#define HEADER_SIZE 4

using namespace std;

int main( int argc, char* argv[] )
{
	streampos size;
	char* memblock;
	char* bufferPtr;
	uint16_t recordSize;

	STDF_record* record;
	list<STDF_record*> recordList;
	pugi::xml_document doc;
	pugi::xml_node root;

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
		memblock = new char[size];
		bufferPtr = memblock;
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

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
				record->appendNode( root );
				delete record;
			}

			bufferPtr += HEADER_SIZE + recordSize;
			size -= HEADER_SIZE + recordSize;
		}
		delete[] memblock;

		doc.print( std::cout );
	}
	return 0;
}
