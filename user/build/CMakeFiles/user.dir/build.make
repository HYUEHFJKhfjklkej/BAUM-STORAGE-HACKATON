# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/trol53/hakaton/baum-may-akacuki/user

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/trol53/hakaton/baum-may-akacuki/user/build

# Include any dependencies generated for this target.
include CMakeFiles/user.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/user.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/user.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/user.dir/flags.make

CMakeFiles/user.dir/user.cc.o: CMakeFiles/user.dir/flags.make
CMakeFiles/user.dir/user.cc.o: ../user.cc
CMakeFiles/user.dir/user.cc.o: CMakeFiles/user.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/trol53/hakaton/baum-may-akacuki/user/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/user.dir/user.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/user.dir/user.cc.o -MF CMakeFiles/user.dir/user.cc.o.d -o CMakeFiles/user.dir/user.cc.o -c /home/trol53/hakaton/baum-may-akacuki/user/user.cc

CMakeFiles/user.dir/user.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/user.dir/user.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/trol53/hakaton/baum-may-akacuki/user/user.cc > CMakeFiles/user.dir/user.cc.i

CMakeFiles/user.dir/user.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/user.dir/user.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/trol53/hakaton/baum-may-akacuki/user/user.cc -o CMakeFiles/user.dir/user.cc.s

# Object files for target user
user_OBJECTS = \
"CMakeFiles/user.dir/user.cc.o"

# External object files for target user
user_EXTERNAL_OBJECTS =

user: CMakeFiles/user.dir/user.cc.o
user: CMakeFiles/user.dir/build.make
user: /usr/lib/x86_64-linux-gnu/libboost_date_time.a
user: /usr/lib/x86_64-linux-gnu/libboost_system.a
user: CMakeFiles/user.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/trol53/hakaton/baum-may-akacuki/user/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable user"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/user.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/user.dir/build: user
.PHONY : CMakeFiles/user.dir/build

CMakeFiles/user.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/user.dir/cmake_clean.cmake
.PHONY : CMakeFiles/user.dir/clean

CMakeFiles/user.dir/depend:
	cd /home/trol53/hakaton/baum-may-akacuki/user/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/trol53/hakaton/baum-may-akacuki/user /home/trol53/hakaton/baum-may-akacuki/user /home/trol53/hakaton/baum-may-akacuki/user/build /home/trol53/hakaton/baum-may-akacuki/user/build /home/trol53/hakaton/baum-may-akacuki/user/build/CMakeFiles/user.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/user.dir/depend

