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
include output/examples/CMakeFiles/sshnetcat.dir/depend.make

# Include the progress variables for this target.
include output/examples/CMakeFiles/sshnetcat.dir/progress.make

# Include the compile flags for this target's objects.
include output/examples/CMakeFiles/sshnetcat.dir/flags.make

output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.o: output/examples/CMakeFiles/sshnetcat.dir/flags.make
output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.o: /home/mo/share/netconf2/lib_src/libssh/examples/sshnetcat.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.o"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sshnetcat.dir/sshnetcat.c.o   -c /home/mo/share/netconf2/lib_src/libssh/examples/sshnetcat.c

output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sshnetcat.dir/sshnetcat.c.i"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mo/share/netconf2/lib_src/libssh/examples/sshnetcat.c > CMakeFiles/sshnetcat.dir/sshnetcat.c.i

output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sshnetcat.dir/sshnetcat.c.s"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mo/share/netconf2/lib_src/libssh/examples/sshnetcat.c -o CMakeFiles/sshnetcat.dir/sshnetcat.c.s

output/examples/CMakeFiles/sshnetcat.dir/authentication.c.o: output/examples/CMakeFiles/sshnetcat.dir/flags.make
output/examples/CMakeFiles/sshnetcat.dir/authentication.c.o: /home/mo/share/netconf2/lib_src/libssh/examples/authentication.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object output/examples/CMakeFiles/sshnetcat.dir/authentication.c.o"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sshnetcat.dir/authentication.c.o   -c /home/mo/share/netconf2/lib_src/libssh/examples/authentication.c

output/examples/CMakeFiles/sshnetcat.dir/authentication.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sshnetcat.dir/authentication.c.i"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mo/share/netconf2/lib_src/libssh/examples/authentication.c > CMakeFiles/sshnetcat.dir/authentication.c.i

output/examples/CMakeFiles/sshnetcat.dir/authentication.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sshnetcat.dir/authentication.c.s"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mo/share/netconf2/lib_src/libssh/examples/authentication.c -o CMakeFiles/sshnetcat.dir/authentication.c.s

output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.o: output/examples/CMakeFiles/sshnetcat.dir/flags.make
output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.o: /home/mo/share/netconf2/lib_src/libssh/examples/knownhosts.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.o"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sshnetcat.dir/knownhosts.c.o   -c /home/mo/share/netconf2/lib_src/libssh/examples/knownhosts.c

output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sshnetcat.dir/knownhosts.c.i"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mo/share/netconf2/lib_src/libssh/examples/knownhosts.c > CMakeFiles/sshnetcat.dir/knownhosts.c.i

output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sshnetcat.dir/knownhosts.c.s"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mo/share/netconf2/lib_src/libssh/examples/knownhosts.c -o CMakeFiles/sshnetcat.dir/knownhosts.c.s

output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.o: output/examples/CMakeFiles/sshnetcat.dir/flags.make
output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.o: /home/mo/share/netconf2/lib_src/libssh/examples/connect_ssh.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.o"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sshnetcat.dir/connect_ssh.c.o   -c /home/mo/share/netconf2/lib_src/libssh/examples/connect_ssh.c

output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sshnetcat.dir/connect_ssh.c.i"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mo/share/netconf2/lib_src/libssh/examples/connect_ssh.c > CMakeFiles/sshnetcat.dir/connect_ssh.c.i

output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sshnetcat.dir/connect_ssh.c.s"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && /home/mo/share/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mo/share/netconf2/lib_src/libssh/examples/connect_ssh.c -o CMakeFiles/sshnetcat.dir/connect_ssh.c.s

# Object files for target sshnetcat
sshnetcat_OBJECTS = \
"CMakeFiles/sshnetcat.dir/sshnetcat.c.o" \
"CMakeFiles/sshnetcat.dir/authentication.c.o" \
"CMakeFiles/sshnetcat.dir/knownhosts.c.o" \
"CMakeFiles/sshnetcat.dir/connect_ssh.c.o"

# External object files for target sshnetcat
sshnetcat_EXTERNAL_OBJECTS =

output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/sshnetcat.c.o
output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/authentication.c.o
output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/knownhosts.c.o
output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/connect_ssh.c.o
output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/build.make
output/examples/sshnetcat: /home/mo/share/netconf2/lib/libssh/../zlib/debug/output/libz.so
output/examples/sshnetcat: lib/libssh.so.4.8.1
output/examples/sshnetcat: /home/mo/share/netconf2/lib/libssh/../zlib/debug/output/libz.so
output/examples/sshnetcat: output/examples/CMakeFiles/sshnetcat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mo/share/netconf2/lib/libssh/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable sshnetcat"
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sshnetcat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
output/examples/CMakeFiles/sshnetcat.dir/build: output/examples/sshnetcat

.PHONY : output/examples/CMakeFiles/sshnetcat.dir/build

output/examples/CMakeFiles/sshnetcat.dir/clean:
	cd /home/mo/share/netconf2/lib/libssh/debug/output/examples && $(CMAKE_COMMAND) -P CMakeFiles/sshnetcat.dir/cmake_clean.cmake
.PHONY : output/examples/CMakeFiles/sshnetcat.dir/clean

output/examples/CMakeFiles/sshnetcat.dir/depend:
	cd /home/mo/share/netconf2/lib/libssh/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mo/share/netconf2/lib/libssh /home/mo/share/netconf2/lib_src/libssh/examples /home/mo/share/netconf2/lib/libssh/debug /home/mo/share/netconf2/lib/libssh/debug/output/examples /home/mo/share/netconf2/lib/libssh/debug/output/examples/CMakeFiles/sshnetcat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : output/examples/CMakeFiles/sshnetcat.dir/depend

