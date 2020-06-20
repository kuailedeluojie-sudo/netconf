# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if("/home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz" STREQUAL "")
  message(FATAL_ERROR "LOCAL can't be empty")
endif()

if(NOT EXISTS "/home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz")
  message(FATAL_ERROR "File not found: /home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz")
endif()

if("MD5" STREQUAL "")
  message(WARNING "File will not be verified since no URL_HASH specified")
  return()
endif()

if("46d9a2a92fd39198501503b40954e6f0" STREQUAL "")
  message(FATAL_ERROR "EXPECT_VALUE can't be empty")
endif()

message(STATUS "verifying file...
     file='/home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz'")

file("MD5" "/home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz" actual_value)

if(NOT "${actual_value}" STREQUAL "46d9a2a92fd39198501503b40954e6f0")
  message(FATAL_ERROR "error: MD5 hash of
  /home/mo/share/netconf2/lib/openssl/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz
does not match expected value
  expected: '46d9a2a92fd39198501503b40954e6f0'
    actual: '${actual_value}'
")
endif()

message(STATUS "verifying file... done")
