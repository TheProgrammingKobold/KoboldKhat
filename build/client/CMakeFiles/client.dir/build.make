# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/crimson/Public/KoboldKhat/KoboldKhat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/crimson/Public/KoboldKhat/KoboldKhat/build

# Include any dependencies generated for this target.
include client/CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include client/CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include client/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/client.dir/flags.make

client/CMakeFiles/client.dir/main.cpp.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/main.cpp.o: /home/crimson/Public/KoboldKhat/KoboldKhat/client/main.cpp
client/CMakeFiles/client.dir/main.cpp.o: client/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/crimson/Public/KoboldKhat/KoboldKhat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object client/CMakeFiles/client.dir/main.cpp.o"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT client/CMakeFiles/client.dir/main.cpp.o -MF CMakeFiles/client.dir/main.cpp.o.d -o CMakeFiles/client.dir/main.cpp.o -c /home/crimson/Public/KoboldKhat/KoboldKhat/client/main.cpp

client/CMakeFiles/client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/main.cpp.i"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/crimson/Public/KoboldKhat/KoboldKhat/client/main.cpp > CMakeFiles/client.dir/main.cpp.i

client/CMakeFiles/client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/main.cpp.s"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/crimson/Public/KoboldKhat/KoboldKhat/client/main.cpp -o CMakeFiles/client.dir/main.cpp.s

client/CMakeFiles/client.dir/client.cpp.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/client.cpp.o: /home/crimson/Public/KoboldKhat/KoboldKhat/client/client.cpp
client/CMakeFiles/client.dir/client.cpp.o: client/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/crimson/Public/KoboldKhat/KoboldKhat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object client/CMakeFiles/client.dir/client.cpp.o"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT client/CMakeFiles/client.dir/client.cpp.o -MF CMakeFiles/client.dir/client.cpp.o.d -o CMakeFiles/client.dir/client.cpp.o -c /home/crimson/Public/KoboldKhat/KoboldKhat/client/client.cpp

client/CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/crimson/Public/KoboldKhat/KoboldKhat/client/client.cpp > CMakeFiles/client.dir/client.cpp.i

client/CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/crimson/Public/KoboldKhat/KoboldKhat/client/client.cpp -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.cpp.o" \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client/client: client/CMakeFiles/client.dir/main.cpp.o
client/client: client/CMakeFiles/client.dir/client.cpp.o
client/client: client/CMakeFiles/client.dir/build.make
client/client: common/libnetworking.a
client/client: client/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/crimson/Public/KoboldKhat/KoboldKhat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable client"
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/client.dir/build: client/client
.PHONY : client/CMakeFiles/client.dir/build

client/CMakeFiles/client.dir/clean:
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build/client && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/client.dir/clean

client/CMakeFiles/client.dir/depend:
	cd /home/crimson/Public/KoboldKhat/KoboldKhat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/crimson/Public/KoboldKhat/KoboldKhat /home/crimson/Public/KoboldKhat/KoboldKhat/client /home/crimson/Public/KoboldKhat/KoboldKhat/build /home/crimson/Public/KoboldKhat/KoboldKhat/build/client /home/crimson/Public/KoboldKhat/KoboldKhat/build/client/CMakeFiles/client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : client/CMakeFiles/client.dir/depend

