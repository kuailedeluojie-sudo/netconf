cmake_minimum_required(VERSION 3.14)

project(fustl C)

set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../zlib/debug/output)

set(OPENSSL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/debug/output)
set(OPENSSL_LIB  ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/debug/output/lib/libssl.so)
set(OPENSSLCRYPTO_LIB  ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/debug/output/lib/libcrypto.so)
find_package(OpenSSL REQUIRED)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../openssl/debug/output/lib/)

LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/../zlib/debug/output/libz.so)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/zlib-1.2.11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/zlib-1.2.11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../zlib/debug/output)

#2020.6.24设置变量然后编译
set(OPENSSL_LIBRARIES $(CMAKE_CURRENT_SOURCE_DIR)/../openssl/debug/output/lib/)

set(OPENSSL_INCLUDE_DIR $(CMAKE_CURRENT_SOURCE_DIR)/../openssl/debug/output/include)

set(LIBSSH_INCLUDE_DIRS $(CMAKE_CURRENT_SOURCE_DIR)/../libssh/_install/include)

set(LIBSSH_LIBRARIES $(CMAKE_CURRENT_SOURCE_DIR)/../libssh/_install/lib/)

set(LIBYANG_INCLUDE_DIRS $(CMAKE_CURRENT_SOURCE_DIR)/../libyang/include/)

set(LIBYANG_LIBRARIES $(CMAKE_CURRENT_SOURCE_DIR)/../libyang/lib)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libnetconf2
  output
)
#/home/mo/share/netconf2/lib/openssl/debug/output
