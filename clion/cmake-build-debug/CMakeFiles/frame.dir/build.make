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

# Include any dependencies generated for this target.
include CMakeFiles\frame.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\frame.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\frame.dir\flags.make

CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.obj: CMakeFiles\frame.dir\flags.make
CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.obj: C:\Users\Cassim\Documents\projet_c\tests\frame.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/frame.dir/C_/Users/Cassim/Documents/projet_c/tests/frame.c.obj"
	C:\PROGRA~2\MICROS~3\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.obj /FdCMakeFiles\frame.dir\ /FS -c C:\Users\Cassim\Documents\projet_c\tests\frame.c
<<

CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/frame.dir/C_/Users/Cassim/Documents/projet_c/tests/frame.c.i"
	C:\PROGRA~2\MICROS~3\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx64\x64\cl.exe > CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Cassim\Documents\projet_c\tests\frame.c
<<

CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/frame.dir/C_/Users/Cassim/Documents/projet_c/tests/frame.c.s"
	C:\PROGRA~2\MICROS~3\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.s /c C:\Users\Cassim\Documents\projet_c\tests\frame.c
<<

# Object files for target frame
frame_OBJECTS = \
"CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.obj"

# External object files for target frame
frame_EXTERNAL_OBJECTS =

frame.exe: CMakeFiles\frame.dir\C_\Users\Cassim\Documents\projet_c\tests\frame.c.obj
frame.exe: CMakeFiles\frame.dir\build.make
frame.exe: C:\Users\Cassim\Documents\projet_c\_win\eibase64.lib
frame.exe: C:\projetc\SDL2_windows\lib\x64\SDL2.lib
frame.exe: C:\projetc\SDL2_windows\lib\x64\SDL2_gfx.lib
frame.exe: C:\projetc\SDL2_windows\lib\x64\SDL2_ttf.lib
frame.exe: C:\projetc\SDL2_windows\lib\x64\FreeImage.lib
frame.exe: CMakeFiles\frame.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable frame.exe"
	"C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\frame.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x64\mt.exe --manifests -- C:\PROGRA~2\MICROS~3\2017\BUILDT~1\VC\Tools\MSVC\1416~1.270\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\frame.dir\objects1.rsp @<<
 /out:frame.exe /implib:frame.lib /pdb:C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug\frame.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console  ei.lib C:\Users\Cassim\Documents\projet_c\_win\eibase64.lib C:\projetc\SDL2_windows\lib\x64\SDL2.lib C:\projetc\SDL2_windows\lib\x64\SDL2_gfx.lib C:\projetc\SDL2_windows\lib\x64\SDL2_ttf.lib C:\projetc\SDL2_windows\lib\x64\FreeImage.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\frame.dir\build: frame.exe

.PHONY : CMakeFiles\frame.dir\build

CMakeFiles\frame.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\frame.dir\cmake_clean.cmake
.PHONY : CMakeFiles\frame.dir\clean

CMakeFiles\frame.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\Cassim\Documents\projet_c\clion C:\Users\Cassim\Documents\projet_c\clion C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug C:\Users\Cassim\Documents\projet_c\clion\cmake-build-debug\CMakeFiles\frame.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\frame.dir\depend

