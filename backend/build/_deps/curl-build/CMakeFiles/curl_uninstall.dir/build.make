# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /home/adri/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/adri/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/adri/Documents/openUItest/backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adri/Documents/openUItest/backend/build

# Utility rule file for curl_uninstall.

# Include any custom commands dependencies for this target.
include _deps/curl-build/CMakeFiles/curl_uninstall.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/curl-build/CMakeFiles/curl_uninstall.dir/progress.make

_deps/curl-build/CMakeFiles/curl_uninstall:
	cd /home/adri/Documents/openUItest/backend/build/_deps/curl-build && /home/adri/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -P /home/adri/Documents/openUItest/backend/build/_deps/curl-build/CMake/cmake_uninstall.cmake

curl_uninstall: _deps/curl-build/CMakeFiles/curl_uninstall
curl_uninstall: _deps/curl-build/CMakeFiles/curl_uninstall.dir/build.make
.PHONY : curl_uninstall

# Rule to build all files generated by this target.
_deps/curl-build/CMakeFiles/curl_uninstall.dir/build: curl_uninstall
.PHONY : _deps/curl-build/CMakeFiles/curl_uninstall.dir/build

_deps/curl-build/CMakeFiles/curl_uninstall.dir/clean:
	cd /home/adri/Documents/openUItest/backend/build/_deps/curl-build && $(CMAKE_COMMAND) -P CMakeFiles/curl_uninstall.dir/cmake_clean.cmake
.PHONY : _deps/curl-build/CMakeFiles/curl_uninstall.dir/clean

_deps/curl-build/CMakeFiles/curl_uninstall.dir/depend:
	cd /home/adri/Documents/openUItest/backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adri/Documents/openUItest/backend /home/adri/Documents/openUItest/backend/build/_deps/curl-src /home/adri/Documents/openUItest/backend/build /home/adri/Documents/openUItest/backend/build/_deps/curl-build /home/adri/Documents/openUItest/backend/build/_deps/curl-build/CMakeFiles/curl_uninstall.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/curl-build/CMakeFiles/curl_uninstall.dir/depend

