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

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/libssh
  output
)
