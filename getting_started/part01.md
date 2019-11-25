| [Prev](index.html) | [Getting Started -- Table of Contents](index.html) | [Next](part02.html) |
# Part 1:  Setting up your Simulation Project

Here we will set up the C++ project and make sure the build tools are working.

First, create a folder named **sydevs-tutorial** for your simulation project and create the following directory structure:

```
sydevs-tutorial/
    build/
    external/
    src/
        nodes/
        simulations/
            setting_up/
```

This is the project you will use for the Getting Started tutorial.

Now download and extract a **Source code** archive from a recent SyDEVS release at [github.com/Autodesk/sydevs/releases](https://github.com/Autodesk/sydevs/releases).

In the extracted package should be folder with a name similar to **sydevs-0.6.6** (the version number may be different). Copy this folder into **external** folder of your project. Except for the version number, the resulting directory structure should be as follows.
```
sydevs-tutorial/
    build/
    external/
        sydevs-0.6.6/
            doc/
            scripts/
            src/
            test/
    src/
        nodes/
        simulations/
            setting_up/
```

The next step is to add code specific your project. In the **src/simulations/setting_up** folder, create a text file named **main.cpp**. Copy the C++ code below into the file and save.

```cpp
#include <sydevs/systems/simulation.h>
#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "SyDEVS Getting Started - Part 1 Complete" << std::endl;
    return 0;
}
```

Now we need to specify the rules for building the project. In the project folder (**sydevs-tutorial**), create a text file named **CMakeLists.txt**. Copy the CMake code below into the file, ***changing the SyDEVS version number on line 3 if needed***. Save the file.

```cmake
################################################################################
# CMake File for SyDEVS Tutorial
################################################################################
cmake_minimum_required(VERSION 3.4)
project(SyDEVS-Tutorial)
set(SYDEVS_VERSION 0.6.6)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    add_definitions("/wd4244") # Hide type conversion warnings
    add_compile_options(/MP)
    add_compile_options(/std=c++14)
    string(REGEX REPLACE "/Z[iI7]" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /bigobj")
else()
    set(WARNING_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-missing-braces -Wignored-qualifiers -Woverloaded-virtual -Winline")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -std=c++14 ${WARNING_FLAGS}")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	    # Replace g++ standard library with Clang standard library
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
    add_compile_options(-std=c++14)
endif(MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(src)

# ------------------------------------------------------------------------------
#
#   SyDEVS Library
#
# ------------------------------------------------------------------------------
set(SYDEVS_DIR external/sydevs-${SYDEVS_VERSION})
include_directories(${SYDEVS_DIR}/src)

set(CORE_DIR ${SYDEVS_DIR}/src/sydevs/core)
file(GLOB CORE_HDRS "${CORE_DIR}/*.h")
aux_source_directory(${CORE_DIR} CORE_SRCS)
source_group("core-include" FILES ${CORE_HDRS})
source_group("core-src" FILES ${CORE_SRCS})

set(TIME_DIR ${SYDEVS_DIR}/src/sydevs/time)
file(GLOB TIME_HDRS "${TIME_DIR}/*.h")
aux_source_directory(${TIME_DIR} TIME_SRCS)
source_group("time-include" FILES ${TIME_HDRS})
source_group("time-src" FILES ${TIME_SRCS})

set(SYSTEMS_DIR ${SYDEVS_DIR}/src/sydevs/systems)
file(GLOB SYSTEMS_HDRS "${SYSTEMS_DIR}/*.h")
aux_source_directory(${SYSTEMS_DIR} SYSTEMS_SRCS)
source_group("systems-include" FILES ${SYSTEMS_HDRS})
source_group("systems-src" FILES ${SYSTEMS_SRCS})

add_library(SyDEVS-static STATIC ${CORE_SRCS} ${TIME_SRCS} ${SYSTEMS_SRCS} ${CORE_HDRS} ${TIME_HDRS} ${SYSTEMS_HDRS})

# ------------------------------------------------------------------------------
#
#   SyDEVS Nodes
#
# ------------------------------------------------------------------------------
set(NODES_DIR src/nodes)

# ------------------------------------------------------------------------------
#
#   SyDEVS Simulations
#
# ------------------------------------------------------------------------------
set(SIMULATIONS_DIR src/simulations)

set(SETTING_UP_DIR ${SIMULATIONS_DIR}/setting_up)
aux_source_directory(${SETTING_UP_DIR} SETTING_UP_SRCS)
add_executable(setting_up ${SETTING_UP_SRCS})
target_link_libraries(setting_up SyDEVS-static)
```

Now it's time to build and test the project to make sure everything is set up properly.

1. If you do not already have CMake, [download](http://www.cmake.org/) and install it.
2. Open a Command Prompt and navigate into the **sydevs-tutorial/build** folder.
3. Run CMake specifying the path `..`, which indicates that **CMakeLists.txt** is one level above the **build** folder. Use the appropriate command, possibly one of the following (on Windows, be sure to specify "Win64"):
  - `cmake -G "Visual Studio 15 2017 Win64" ..`
  - `cmake -G "Visual Studio 14 2015 Win64" ..`
  - `cmake -G "Xcode" ..`
  - `cmake ..` (on Mac/Unix, to produce a Makefile)
4. Build your project (e.g. open the project or solution file using [Visual Studio](https://www.visualstudio.com/vs/), [MSBuild](https://msdn.microsoft.com/en-us/library/dd393574.aspx), or [Xcode](https://developer.apple.com/xcode/), or run the command `make` to use the Makefile).
5. Find and run the **setting_up** executable (e.g. in the **build**, **build/Debug** or **build/Release** folder).

You should see the following output:

```
SyDEVS Getting Started - Part 1 Complete
```

That's it for Part 1! The program you just made doesn't do anything, but at least you've got the project set up and ready to support simulation code. In [Part 2](part02.html), you will create your first SyDEVS node and simulation.

| [***Continue to Part 2***](part02.html) |
