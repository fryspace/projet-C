# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Cassim\Documents\projet_c\clion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug

# Utility rule file for doc.

# Include the progress variables for this target.
include CMakeFiles\doc.dir\progress.make

CMakeFiles\doc:
	cd C:\Users\Cassim\Documents\projet_c
	doxygen C:/Users/Cassim/Documents/projet_c/docs/doxygen.cfg
	cd C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug

doc: CMakeFiles\doc
doc: CMakeFiles\doc.dir\build.make

.PHONY : doc

# Rule to build all files generated by this target.
CMakeFiles\doc.dir\build: doc

.PHONY : CMakeFiles\doc.dir\build

CMakeFiles\doc.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\doc.dir\cmake_clean.cmake
.PHONY : CMakeFiles\doc.dir\clean

CMakeFiles\doc.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\Cassim\Documents\projet_c\clion C:\Users\Cassim\Documents\projet_c\clion C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug\CMakeFiles\doc.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\doc.dir\depend

