# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mo/share/netconf2/lib/libssh

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mo/share/netconf2/lib/libssh/debug

# Include any dependencies generated for this target.
include output/examples/CMakeFiles/libsshpp.dir/depend.make

# Include the progress variables for this target.
include output/examples/CMakeFiles/libsshpp.dir/progress.make

# Include the compile flags for this target's objects.
include output/examples/CMakeFiles/libsshpp.dir/flags.make

output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.o: output/examples/CMakeFiles/libsshpp.dir/flags.make
output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.o: /home/mo/share/netconf2/lib_src/libssh/examples/libsshpp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.o"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libsshpp.dir/libsshpp.cpp.o -c /home/mo/share/netconf2/lib_src/libssh/examples/libsshpp.cpp

output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libsshpp.dir/libsshpp.cpp.i"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mo/share/netconf2/lib_src/libssh/examples/libsshpp.cpp > CMakeFiles/libsshpp.dir/libsshpp.cpp.i

output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libsshpp.dir/libsshpp.cpp.s"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mo/share/netconf2/lib_src/libssh/examples/libsshpp.cpp -o CMakeFiles/libsshpp.dir/libsshpp.cpp.s

# Object files for target libsshpp
libsshpp_OBJECTS = \
"CMakeFiles/libsshpp.dir/libsshpp.cpp.o"

# External object files for target libsshpp
libsshpp_EXTERNAL_OBJECTS =

output/examples/libsshpp: output/examples/CMakeFiles/libsshpp.dir/libsshpp.cpp.o
output/examples/libsshpp: output/examples/CMakeFiles/libsshpp.dir/build.make
output/examples/libsshpp: /home/mo/share/netconf2/lib/libssh/../zlib/debug/output/libz.so
output/examples/libsshpp: lib/libssh.so.4.8.1
output/examples/libsshpp: /home/mo/share/netconf2/lib/libssh/../zlib/debug/output/libz.so
output/examples/libsshpp: output/examples/CMakeFiles/libsshpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable libsshpp"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libsshpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
output/examples/CMakeFiles/libsshpp.dir/build: output/examples/libsshpp

.PHONY : output/examples/CMakeFiles/libsshpp.dir/build

output/examples/CMakeFiles/libsshpp.dir/clean:
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && $(CMAKE_COMMAND) -P CMakeFiles/libsshpp.dir/cmake_clean.cmake
.PHONY : output/examples/CMakeFiles/libsshpp.dir/clean

output/examples/CMakeFiles/libsshpp.dir/depend:
	cd /home/mo/share/netconf2/lib/libssh/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mo/share/netconf2/lib/libssh /home/mo/share/netconf2/lib_src/libssh/examples /home/mo/share/netconf2/lib/libssh/debug /home/mo/share/netconf2/lib/libssh/debug/output/examples /home/mo/share/netconf2/lib/libssh/debug/output/examples/CMakeFiles/libsshpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : output/examples/CMakeFiles/libsshpp.dir/depend

