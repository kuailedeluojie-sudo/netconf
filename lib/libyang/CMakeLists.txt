cmake_minimum_required(VERSION 3.14)

project(fustl C)

set(PCRE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libpcre/_install/include)
set(PCRE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libpcre/_install/lib)
#add_library(${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libpcre/_install/lib/libpcre.a STATIC IMPORTED)
#set(PCRE_LIBRARY /home/mo/share/netconf2/lib_src/libpcre/_install/lib)
#set(PCRE_INCLUDE_DIR /home/mo/share/netconf2/lib_src/libpcre/_install/include)
find_package(PCRE  REQUIRED)
#/home/mo/share/netconf2/lib_src/libpcre/_install/lib/libpcre.a

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libyang
  #output
)




#set(PCRE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../pcre/debug/output)
#set(PCRE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../pcre/debug/output)
#add_library(${CMAKE_CURRENT_SOURCE_DIR}/../pcre/debug/output/libpcre.a STATIC IMPORTED)
