cmake_minimum_required(VERSION 3.14)

project(ssl C)

include(ExternalProject)
ExternalProject_Add(OpenSSL PREFIX openssl
        URL ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/openssl/OpenSSL_1_1_0l.tar.gz
        URL_MD5 46d9a2a92fd39198501503b40954e6f0
        CONFIGURE_COMMAND ./Configure  linux-generic32 no-asm shared no-async --prefix=${CMAKE_CURRENT_SOURCE_DIR}/debug/output/
        BUILD_IN_SOURCE 1)
