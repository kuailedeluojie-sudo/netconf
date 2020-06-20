#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ssh" for configuration ""
set_property(TARGET ssh APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ssh PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libssh.so.4.8.1"
  IMPORTED_SONAME_NOCONFIG "libssh.so.4"
  )

list(APPEND _IMPORT_CHECK_TARGETS ssh )
list(APPEND _IMPORT_CHECK_FILES_FOR_ssh "${_IMPORT_PREFIX}/lib/libssh.so.4.8.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
