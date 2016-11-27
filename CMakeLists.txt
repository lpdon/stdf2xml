cmake_minimum_required (VERSION 2.6)
project (stdf2xml)

include_directories(src/xml src)
add_subdirectory(src)

set (
	stdf2xml_src
	src/record.cpp 
	src/utils.cpp 
	src/main.cpp
)	

add_executable (stdf2xml ${stdf2xml_src})
target_link_libraries(stdf2xml stdf2xml_libs)
