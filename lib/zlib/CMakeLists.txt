cmake_minimum_required(VERSION 3.14)

project(fustl C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/zlib-1.2.11)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_src/zlib-1.2.11
  output
)
