# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/mp_exec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mp_exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mp_exec.dir/flags.make

CMakeFiles/mp_exec.dir/commandline_parser.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/commandline_parser.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/commandline_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mp_exec.dir/commandline_parser.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/commandline_parser.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/commandline_parser.cpp"

CMakeFiles/mp_exec.dir/commandline_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/commandline_parser.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/commandline_parser.cpp" > CMakeFiles/mp_exec.dir/commandline_parser.cpp.i

CMakeFiles/mp_exec.dir/commandline_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/commandline_parser.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/commandline_parser.cpp" -o CMakeFiles/mp_exec.dir/commandline_parser.cpp.s

CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.requires

CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.provides: CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.provides

CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.provides.build: CMakeFiles/mp_exec.dir/commandline_parser.cpp.o


CMakeFiles/mp_exec.dir/FileWrap.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/FileWrap.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/FileWrap.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mp_exec.dir/FileWrap.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/FileWrap.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/FileWrap.cpp"

CMakeFiles/mp_exec.dir/FileWrap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/FileWrap.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/FileWrap.cpp" > CMakeFiles/mp_exec.dir/FileWrap.cpp.i

CMakeFiles/mp_exec.dir/FileWrap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/FileWrap.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/FileWrap.cpp" -o CMakeFiles/mp_exec.dir/FileWrap.cpp.s

CMakeFiles/mp_exec.dir/FileWrap.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/FileWrap.cpp.o.requires

CMakeFiles/mp_exec.dir/FileWrap.cpp.o.provides: CMakeFiles/mp_exec.dir/FileWrap.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/FileWrap.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/FileWrap.cpp.o.provides

CMakeFiles/mp_exec.dir/FileWrap.cpp.o.provides.build: CMakeFiles/mp_exec.dir/FileWrap.cpp.o


CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/gif_wrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/gif_wrapper.cpp"

CMakeFiles/mp_exec.dir/gif_wrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/gif_wrapper.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/gif_wrapper.cpp" > CMakeFiles/mp_exec.dir/gif_wrapper.cpp.i

CMakeFiles/mp_exec.dir/gif_wrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/gif_wrapper.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/gif_wrapper.cpp" -o CMakeFiles/mp_exec.dir/gif_wrapper.cpp.s

CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.requires

CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.provides: CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.provides

CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.provides.build: CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o


CMakeFiles/mp_exec.dir/image.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/image.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/mp_exec.dir/image.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/image.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image.cpp"

CMakeFiles/mp_exec.dir/image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/image.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image.cpp" > CMakeFiles/mp_exec.dir/image.cpp.i

CMakeFiles/mp_exec.dir/image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/image.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image.cpp" -o CMakeFiles/mp_exec.dir/image.cpp.s

CMakeFiles/mp_exec.dir/image.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/image.cpp.o.requires

CMakeFiles/mp_exec.dir/image.cpp.o.provides: CMakeFiles/mp_exec.dir/image.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/image.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/image.cpp.o.provides

CMakeFiles/mp_exec.dir/image.cpp.o.provides.build: CMakeFiles/mp_exec.dir/image.cpp.o


CMakeFiles/mp_exec.dir/image_pixel.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/image_pixel.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/image_pixel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/mp_exec.dir/image_pixel.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/image_pixel.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image_pixel.cpp"

CMakeFiles/mp_exec.dir/image_pixel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/image_pixel.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image_pixel.cpp" > CMakeFiles/mp_exec.dir/image_pixel.cpp.i

CMakeFiles/mp_exec.dir/image_pixel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/image_pixel.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/image_pixel.cpp" -o CMakeFiles/mp_exec.dir/image_pixel.cpp.s

CMakeFiles/mp_exec.dir/image_pixel.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/image_pixel.cpp.o.requires

CMakeFiles/mp_exec.dir/image_pixel.cpp.o.provides: CMakeFiles/mp_exec.dir/image_pixel.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/image_pixel.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/image_pixel.cpp.o.provides

CMakeFiles/mp_exec.dir/image_pixel.cpp.o.provides.build: CMakeFiles/mp_exec.dir/image_pixel.cpp.o


CMakeFiles/mp_exec.dir/main.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/main.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/mp_exec.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/main.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/main.cpp"

CMakeFiles/mp_exec.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/main.cpp" > CMakeFiles/mp_exec.dir/main.cpp.i

CMakeFiles/mp_exec.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/main.cpp" -o CMakeFiles/mp_exec.dir/main.cpp.s

CMakeFiles/mp_exec.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/main.cpp.o.requires

CMakeFiles/mp_exec.dir/main.cpp.o.provides: CMakeFiles/mp_exec.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/main.cpp.o.provides

CMakeFiles/mp_exec.dir/main.cpp.o.provides.build: CMakeFiles/mp_exec.dir/main.cpp.o


CMakeFiles/mp_exec.dir/text_color.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/text_color.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/text_color.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/mp_exec.dir/text_color.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/text_color.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/text_color.cpp"

CMakeFiles/mp_exec.dir/text_color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/text_color.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/text_color.cpp" > CMakeFiles/mp_exec.dir/text_color.cpp.i

CMakeFiles/mp_exec.dir/text_color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/text_color.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/text_color.cpp" -o CMakeFiles/mp_exec.dir/text_color.cpp.s

CMakeFiles/mp_exec.dir/text_color.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/text_color.cpp.o.requires

CMakeFiles/mp_exec.dir/text_color.cpp.o.provides: CMakeFiles/mp_exec.dir/text_color.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/text_color.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/text_color.cpp.o.provides

CMakeFiles/mp_exec.dir/text_color.cpp.o.provides.build: CMakeFiles/mp_exec.dir/text_color.cpp.o


CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o: CMakeFiles/mp_exec.dir/flags.make
CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o: /Users/cjh/Documents/UIUC/Fall\ 2017/cs440_ai/cs440_fall2017/MP2/src/breakthrough_test_games.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o -c "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/breakthrough_test_games.cpp"

CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/breakthrough_test_games.cpp" > CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.i

CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src/breakthrough_test_games.cpp" -o CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.s

CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.requires:

.PHONY : CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.requires

CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.provides: CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.requires
	$(MAKE) -f CMakeFiles/mp_exec.dir/build.make CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.provides.build
.PHONY : CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.provides

CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.provides.build: CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o


# Object files for target mp_exec
mp_exec_OBJECTS = \
"CMakeFiles/mp_exec.dir/commandline_parser.cpp.o" \
"CMakeFiles/mp_exec.dir/FileWrap.cpp.o" \
"CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o" \
"CMakeFiles/mp_exec.dir/image.cpp.o" \
"CMakeFiles/mp_exec.dir/image_pixel.cpp.o" \
"CMakeFiles/mp_exec.dir/main.cpp.o" \
"CMakeFiles/mp_exec.dir/text_color.cpp.o" \
"CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o"

# External object files for target mp_exec
mp_exec_EXTERNAL_OBJECTS =

mp_exec: CMakeFiles/mp_exec.dir/commandline_parser.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/FileWrap.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/image.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/image_pixel.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/main.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/text_color.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o
mp_exec: CMakeFiles/mp_exec.dir/build.make
mp_exec: /usr/local/lib/libpng.a
mp_exec: /usr/lib/libz.dylib
mp_exec: CMakeFiles/mp_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable mp_exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mp_exec.dir/build: mp_exec

.PHONY : CMakeFiles/mp_exec.dir/build

CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/commandline_parser.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/FileWrap.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/gif_wrapper.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/image.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/image_pixel.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/main.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/text_color.cpp.o.requires
CMakeFiles/mp_exec.dir/requires: CMakeFiles/mp_exec.dir/breakthrough_test_games.cpp.o.requires

.PHONY : CMakeFiles/mp_exec.dir/requires

CMakeFiles/mp_exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mp_exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mp_exec.dir/clean

CMakeFiles/mp_exec.dir/depend:
	cd "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src" "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/src" "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug" "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug" "/Users/cjh/Documents/UIUC/Fall 2017/cs440_ai/cs440_fall2017/MP2/bin/cmake-build-debug/CMakeFiles/mp_exec.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/mp_exec.dir/depend

