#include "utils.h"

using namespace std;

uint32_t Utils::getFileSize( std::ifstream& file )
{
	file.seekg(0, ios::end);
	return static_cast<uint32_t>( file.tellg() );
}
