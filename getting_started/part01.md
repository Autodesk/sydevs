| [Prev](index.html) | [Getting Started - Table of Contents](index.html) | [Next](part02.html) |
# Part 1:  Setting up your Simulation Project

First, create a folder named `sydevs_examples` for your simulation project. Next make several new folders within `sydevs_examples` to produce the following directory structure.

```
sydevs_examples/
    bin/
    external/
    src/
        examples/
        simulations/
            part01/
```

This is the project we will use for the Getting Started tutorial. When you begin other projects, choose different names for the project folder (`sydevs_examples`) and the folder that will contain reusable code (`examples`). In other projects, your first simulation will not be called `part01`, so change this folder name as well. Other names can also be changed. For example, you might prefer your compiled data to go in a folder named `build` instead of `bin`.

Now download and extract a SyDEVS release from [github.com/Autodesk/sydevs/releases](https://github.com/Autodesk/sydevs/releases). Shorten the name of the extracted folder so that it contains only the word `sydevs` followed by the version number. Then place the `SyDEVS-v[...]` folder into the `external` folder of your project.

The resulting directory structure should be as follows (except possibly with a different version number in the `sydevs-v0.4` folder name). The `core`, `systems` and `time` folders should each contain a number of header files, and the `lib` folder should contain several pre-compiled libraries.
```
sydevs_examples/
    bin/
    external/
        sydevs-v0.4/
            include/
                sydevs/
                    core/
                    systems/
                    time/
            lib/
    src/
        examples/
        simulations/
            part01/
```

The next step is to add code specific your project. In the `src/simulations/part01` folder, create a text file named `main.cpp`. Copy the C++ code below into the file and save.

```cpp
#include <sydevs/systems/simulation.h>
#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "SyDEVS Getting Started - Part 1 Complete" << std::endl;
    return 0;
}
```

Next, create a text file named `CMakeLists.txt` in the project folder (`sydevs_examples`). Copy the CMake code below into the file, change the SyDEVS version number if needed, and save.

```CMake
################################################################################
# CMake File for SyDEVS Examples
################################################################################
cmake_minimum_required(VERSION 3.4)
project(SyDEVS_Examples)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if(MSVC)
    add_definitions("/wd4244")
    message(STATUS "Visual Studio")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -std=c++14 ${WARNING_FLAGS}")
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	endif()
    add_compile_options(-std=c++14)
    message(STATUS "Unix Build")
endif(MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(src)

# ------------------------------------------------------------------------------
#
#   SyDEVS
#
# ------------------------------------------------------------------------------
set(SYDEVS_DIR external/sydevs-v0.4)
include_directories(${SYDEVS_DIR}/include)
link_directories(${SYDEVS_DIR}/lib)

# ------------------------------------------------------------------------------
#
#   Examples
#
# ------------------------------------------------------------------------------
set(EXAMPLES_DIR src/examples)

# ------------------------------------------------------------------------------
#
#   Simulations
#
# ------------------------------------------------------------------------------
set(SIMULATIONS_DIR src/simulations)

set(PART01_DIR ${SIMULATIONS_DIR}/part01)
aux_source_directory(${PART01_DIR} PART01_SRCS)
add_executable(part01 ${PART01_SRCS})
target_link_libraries(part01 debug SyDEVS-static-debug optimized SyDEVS-static)
```

When preparing a `CMakeLists.txt` file for other projects, remember to change the name of the project, reference the correct version of SyDEVS, replace the `Examples` heading (and the `examples` folder) with terms that describe your application, and replace the `PART01_DIR` variable (and `part01` executable) to properly identify your simulation.

Now it's time to build and test the project to make sure everything is properly set up.

1. If you do not already have CMake, [download](http://www.cmake.org/) and install it.
2. Open a Command Prompt and navigate into the `sydevs_examples/bin` folder.
3. Run CMake using the appropriate command, possibly one of the following:
  - `cmake ..`
  - `cmake -G "Visual Studio 15 2017 Win64" ..`
  - `cmake -G "Visual Studio 14 2015 Win64" ..`
  - `cmake -G "Xcode" ..`
4. Build your project (e.g. using Visual Studio or Xcode)
5. Find the `part01` executable (e.g. in `bin/Debug/` or `bin/Release/`)
6. Run `part01` (e.g. `part01.exe`)

The `part01` executable should produce the following output.

```
SyDEVS Getting Started - Part 1 Complete
```

That's it for Part 1! The program you just made doesn't do anything, but at least you've got the project set up and ready to support simulation code. In [Part 2](part02.html), you will create your first SyDEVS node and simulation.

([***Continue to Part 2***](part02.html))
