/*
 * utils.h
 *
 *  Created on: 15.02.2015
 *      Author: Leandro
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <iostream>
#include <fstream>

//http://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//http://stackoverflow.com/questions/9444988/how-to-print-a-variables-name-in-c
#define VNAME(x) #x
#define VDUMP(x) std::cout << #x << " " << x << std::endl

class Utils
{
	public:
		static uint32_t getFileSize( std::ifstream& file );

};

#endif /* UTILS_H_ */
