Getting Started {#getting_started}
===============

This tutorial will guide you through the steps of creating a simulation project using the SyDEVS library.

You will need a C++ compiler that implements the C++14 standard. SyDEVS has been tested successfully with Visual Studio 2015, Visual Studio 2017, and recent versions of GCC and Clang.

- @subpage getting_started_part01
- @subpage getting_started_part02

@page getting_started_part01 Part 1: Setting up your Simulation Project

First, create a folder for your simulation project. Our examples will use the folder name (`my_simulation_project`), but choose whatever name you want.

Next, make several new folders within `my_simulation_project` to produce the following directory structure.
```
my-sydevs-project/
    bin/
    external/
    src/
        nodes/
        simulations/
            part01/
```

(If you prefer, use the folder name `build` instead of `bin`.)

Now download and extract a SyDEVS release from [github.com/Autodesk/sydevs/releases](https://github.com/Autodesk/sydevs/releases). Shorten the name of the extracted folder so that it contains only the word `sydevs` followed by the version number. Then place the folder into the `external` folder of your project.

The resulting directory structure should be as follows, with the exception of the version number in the `sydevs-v0.4` folder name. The `core`, `systems` and `time` folders should each contain a number of header files, and the `lib` folder should contain several pre-compiled libraries.
```
my-sydevs-project/
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
        nodes/
        simulations/
            part01/
```

The next step is to add code specific your project, which goes somewhere within `src`.

In `src/simulations/part01/`, create a file named `main.cpp`. Copy the C++ code below into the file and save.

```cpp
#include <sydevs/systems/simulation.h>
#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "SyDEVS Getting Started - Part 1 Complete" << std::endl;
    return 0;
}
```

Next, create a text file named `CMakeLists.txt` in the project folder (`my-sydevs-project`). Copy the CMake code below into the file and save.

```CMake
cmake_minimum_required(VERSION 3.4)
project(cinder-sydevs)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if(MSVC)
    add_definitions("/wd4244")
    message(STATUS "Visual Studio")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -std=c++14 ${WARNING_FLAGS}")
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	endif()
    add_compile_options(-std=c++14)
    message(STATUS "Unix Build")
endif(MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SYDEVS_DIR external/sydevs-v0.4)
include_directories(${SYDEVS_DIR}/include)
link_directories(${SYDEVS_DIR}/lib)
 
set(NODES_DIR src/nodes)
include_directories(${NODES_DIR})
aux_source_directory(${NODES_DIR} NODES_SRCS)

set(SIMULATIONS_DIR src/simulations)

set(PART01_DIR ${SIMULATIONS_DIR}/part01)
file (GLOB PART01_HDRS "${PART01_DIR}/*.h")
aux_source_directory(${PART01_DIR} PART01_SRCS)
add_executable(part01 ${PART01_SRCS} ${PART01_HDRS})
target_link_libraries(part01 SyDEVS-static)
```

Now it's time to build and test the project to make sure everything is set up properly.

1. If you do not already have CMake, [download](http://www.cmake.org/) and install it.
2. Open a Command Prompt and navigate into the `my-sydevs-project/bin` folder.
3. Run CMake; for example...
  - `cmake ..`
  - `cmake -G "Visual Studio 15 2017 Win64" ..`
  - `cmake -G "Visual Studio 14 2015 Win64" ..`
  - `cmake -G "Xcode" ..`
4. Build your project (e.g. using Visual Studio or Xcode)
5. Find the `part01` executable (e.g. in `bin/Debug/` or `bin/Release/`)
6. Run `part01` (e.g. `part01.exe`)

The `part01` executable should produce the following output.`

```
SyDEVS Getting Started - Part 1 Complete
```

That's it for Part 1! The program you just created doesn't do anything, but at least you've got the project set up and ready to support simulation code.

In [Part 2](getting_started_part02.html), you will create your first SyDEVS node and simulation.

@page getting_started_part02 Part 2: Creating your First Simulation

Coming soon...
