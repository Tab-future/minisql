# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build"

# Include any dependencies generated for this target.
include CMakeFiles/MiniSQL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MiniSQL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MiniSQL.dir/flags.make

CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj: CMakeFiles/MiniSQL.dir/flags.make
CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj: CMakeFiles/MiniSQL.dir/includes_CXX.rsp
CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj: ../src/BufferMange.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MiniSQL.dir\src\BufferMange.cpp.obj -c "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\BufferMange.cpp"

CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.i"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\BufferMange.cpp" > CMakeFiles\MiniSQL.dir\src\BufferMange.cpp.i

CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.s"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\BufferMange.cpp" -o CMakeFiles\MiniSQL.dir\src\BufferMange.cpp.s

CMakeFiles/MiniSQL.dir/src/Record.cpp.obj: CMakeFiles/MiniSQL.dir/flags.make
CMakeFiles/MiniSQL.dir/src/Record.cpp.obj: CMakeFiles/MiniSQL.dir/includes_CXX.rsp
CMakeFiles/MiniSQL.dir/src/Record.cpp.obj: ../src/Record.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MiniSQL.dir/src/Record.cpp.obj"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MiniSQL.dir\src\Record.cpp.obj -c "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\Record.cpp"

CMakeFiles/MiniSQL.dir/src/Record.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MiniSQL.dir/src/Record.cpp.i"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\Record.cpp" > CMakeFiles\MiniSQL.dir\src\Record.cpp.i

CMakeFiles/MiniSQL.dir/src/Record.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MiniSQL.dir/src/Record.cpp.s"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\Record.cpp" -o CMakeFiles\MiniSQL.dir\src\Record.cpp.s

CMakeFiles/MiniSQL.dir/src/test.cpp.obj: CMakeFiles/MiniSQL.dir/flags.make
CMakeFiles/MiniSQL.dir/src/test.cpp.obj: CMakeFiles/MiniSQL.dir/includes_CXX.rsp
CMakeFiles/MiniSQL.dir/src/test.cpp.obj: ../src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MiniSQL.dir/src/test.cpp.obj"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MiniSQL.dir\src\test.cpp.obj -c "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\test.cpp"

CMakeFiles/MiniSQL.dir/src/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MiniSQL.dir/src/test.cpp.i"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\test.cpp" > CMakeFiles\MiniSQL.dir\src\test.cpp.i

CMakeFiles/MiniSQL.dir/src/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MiniSQL.dir/src/test.cpp.s"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\src\test.cpp" -o CMakeFiles\MiniSQL.dir\src\test.cpp.s

# Object files for target MiniSQL
MiniSQL_OBJECTS = \
"CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj" \
"CMakeFiles/MiniSQL.dir/src/Record.cpp.obj" \
"CMakeFiles/MiniSQL.dir/src/test.cpp.obj"

# External object files for target MiniSQL
MiniSQL_EXTERNAL_OBJECTS =

MiniSQL.exe: CMakeFiles/MiniSQL.dir/src/BufferMange.cpp.obj
MiniSQL.exe: CMakeFiles/MiniSQL.dir/src/Record.cpp.obj
MiniSQL.exe: CMakeFiles/MiniSQL.dir/src/test.cpp.obj
MiniSQL.exe: CMakeFiles/MiniSQL.dir/build.make
MiniSQL.exe: CMakeFiles/MiniSQL.dir/linklibs.rsp
MiniSQL.exe: CMakeFiles/MiniSQL.dir/objects1.rsp
MiniSQL.exe: CMakeFiles/MiniSQL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable MiniSQL.exe"
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MiniSQL.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MiniSQL.dir/build: MiniSQL.exe

.PHONY : CMakeFiles/MiniSQL.dir/build

CMakeFiles/MiniSQL.dir/clean:
	cd /d C:\Users\KseeleK\Desktop\2017-2~1\DB\MiniSQL\build && $(CMAKE_COMMAND) -P CMakeFiles\MiniSQL.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MiniSQL.dir/clean

CMakeFiles/MiniSQL.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL" "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL" "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build" "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build" "C:\Users\KseeleK\Desktop\2017-2018 The Second Semester\DB\MiniSQL\build\CMakeFiles\MiniSQL.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/MiniSQL.dir/depend

