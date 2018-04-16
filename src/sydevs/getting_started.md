Getting Started {#getting_started}
===============

This tutorial will guide you through the steps of creating a simulation project using the SyDEVS library.

You will need a C++ compiler that implements the C++14 standard. If you're relatively new to C++, a list of free compilers and a tutorial are available on the [Standard C++ Foundation's getting started page](https://isocpp.org/get-started). SyDEVS has been tested successfully with Visual C++ 2015 and 2017, the Gnu Compiler Collection, and Clang.

- @subpage getting_started_part01
- @subpage getting_started_part02





@page getting_started_part01 Part 1: Setting up your Simulation Project

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

This is the project we will use for the Getting Started tutorial. When you begin other projects, choose different names for the project folder `sydevs_examples` as well as the `examples` folder within `src`. In other projects, your first simulation will not be called `part01`, so change this folder name as well. Other names can also be changed. For example, you might prefer your compiled data to go in a folder named `build` instead of `bin`.

Now download and extract a SyDEVS release from [github.com/Autodesk/sydevs/releases](https://github.com/Autodesk/sydevs/releases). Shorten the name of the extracted folder so that it contains only the word `sydevs` followed by the version number. Then place the `SyDEVS-v???` folder into the `external` folder of your project.

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

The next step is to add code specific your project. In the `src/simulations/part01` folder, create a file named `main.cpp`. Copy the C++ code below into the file and save.

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

When preparing a `CMakeLists.txt` file for other projects, remember to change the name of the project, reference the correct version of SyDEVS, replace `Examples` and the `examples` folder with terms that describe your application, and replace `PART01_DIR` and `part01` to properly identify your simulation.

Now it's time to build and test the project to make sure everything is set up properly.

1. If you do not already have CMake, [download](http://www.cmake.org/) and install it.
2. Open a Command Prompt and navigate into the `sydevs_examples/bin` folder.
3. Run CMake; for example...
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

That's it for Part 1! The program you just created doesn't do anything, but at least you've got the project set up and ready to support simulation code. In [Part 2](getting_started_part02.html), you will create your first SyDEVS node and simulation.

([***Continue to Part 2***](getting_started_part02.html))





@page getting_started_part02 Part 2: Creating your First Simulation

Let's start by creating a few new folders within the `sydevs_examples` project.

1. In `sydevs_examples/src/examples`, create a folder named `getting_started`.
2. In the new `getting_started` folder, create a folder named `waveform`. This is where your first SyDEVS node will be located.
3. In `sydevs_examples/src/simulations` make a folder named `part02`. The code here will invoke the simulation code in `sydevs_examples/src/examples/getting_started/waveform`. 

The overall directory structure should now be as follows.

```
sydevs_examples/
    bin/
        ...
    external/
        ...
    src/
        examples/
            getting_started/
                waveform/
        simulations/
            part01/
            part02/
```

The `CMakeLists.txt` file will have to be updated, so let's get that out of the way. Add the following instructions to the `Examples` section. This prepares a list of the header (.h) files we will later create in the `waveform` folder.

```CMake
set(WAVEFORM_DIR ${EXAMPLES_DIR}/getting_started/waveform)
file(GLOB WAVEFORM_HDRS "${WAVEFORM_DIR}/*.h")
```

Also add the following instructions to the `Simulations` section. This creates an executable for Part 2 of the tutorial. Observe that the executable references the `waveform` header files.

```CMake
set(PART02_DIR ${SIMULATIONS_DIR}/part02)
aux_source_directory(${PART02_DIR} PART02_SRCS)
add_executable(part02 ${PART02_SRCS} ${WAVEFORM_HDRS})
target_link_libraries(part02 debug SyDEVS-static-debug optimized SyDEVS-static)
```

The bottom of the `CMakeLists.txt` file should now appear as follows.

```CMake
# ------------------------------------------------------------------------------
#
#   Examples
#
# ------------------------------------------------------------------------------
set(EXAMPLES_DIR src/examples)

set(WAVEFORM_DIR ${EXAMPLES_DIR}/getting_started/waveform)
file(GLOB WAVEFORM_HDRS "${WAVEFORM_DIR}/*.h")

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

set(PART02_DIR ${SIMULATIONS_DIR}/part02)
aux_source_directory(${PART02_DIR} PART02_SRCS)
add_executable(part02 ${PART02_SRCS} ${WAVEFORM_HDRS})
target_link_libraries(part02 debug SyDEVS-static-debug optimized SyDEVS-static)
```

It's time now to add a SyDEVS node to the project. In the `sydevs_examples/src/examples/getting_started/waveform` folder, create a file named `square_wave_closed_system.h` and save it with the following code.

```cpp
#pragma once
#ifndef SYDEVS_EXAMPLES_SQUARE_WAVE_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_SQUARE_WAVE_CLOSED_SYSTEM_H_

#include <sydevs/systems/atomic_node.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class square_wave_closed_system : public atomic_node
{
public:
    // Constructor/Destructor:
    square_wave_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~square_wave_closed_system() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:

protected:
    // State Variables:
    duration period_dt;  // duration of one period of the sqaure wave
    float64 duty_cycle;  // fraction (0 to 1) of each period spent in the "on" phase
    int64 phase;         // binary signal phase (0 => off, 1 => on)

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline square_wave_closed_system::square_wave_closed_system(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
{
}


inline duration square_wave_closed_system::initialization_event()
{
    period_dt = (10_s).fixed_at(time_precision());
    duty_cycle = 0.3;
    phase = 0;
    std::cout << "phase = " << phase << std::endl;
    return period_dt*(1.0 - duty_cycle);
}


inline duration square_wave_closed_system::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration square_wave_closed_system::planned_event(duration elapsed_dt)
{
    duration planned_dt;
    if (phase == 0) {
        phase = 1;
        planned_dt = period_dt*duty_cycle;
    }
    else if (phase == 1) {
        phase = 0;
        planned_dt = period_dt*(1.0 - duty_cycle);
    }
    std::cout << "phase = " << phase << std::endl;
    return planned_dt;
}


inline void square_wave_closed_system::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
```

Let's make a few observations about this node.

- The node is in the namespace `sydevs_examples`. When you develop your own nodes, you'll want to place them in a different namespace that's specific to your project.
- The name of the node ends with `_closed_system`, reflecting the fact the node has no ports. By convention, nodes that do have ports end with `_node`.
- The node inherits from `atomic_node`. The other node base classes include `composite_node`, `collection_node`, and `function_node`.
- When simulated, the `initialization_event` will be invoked once at the beginning, the `planned_event` will be invoked repeatedly as simulated time advances, and the `finalization_event` will be invoked once at the end. The `unplanned_event` will never be invoked because the node has no message input ports.
- The node represents a square wave cycling between `phase = 0` (off) and `phase = 1` (on). A full cycle is 10 seconds (`period_dt`). The "on" phase lasts for 30% of this time (`duty_cycle`). Observe that the planned duration (`planned_dt`) is either 3 seconds (`period_dt*duty_cycle`) or 7 seconds (`period_dt*(1.0 - duty_cycle)`) depending on the phase. Whenever the planned duration elapses, the `planned_event` is invoked.

Also in the `waveform` folder, create a file named `square_wave.h` and save it with the code below.

```cpp
#pragma once
#ifndef SYDEVS_EXAMPLES_SQUARE_WAVE_H_
#define SYDEVS_EXAMPLES_SQUARE_WAVE_H_

#include <examples/getting_started/waveform/square_wave_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void square_wave()
{
    try {
        simulation<square_wave_closed_system> sim(1_min, 0, std::cout);
        sim.process_remaining_events();
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


}  // namespace

#endif
```

Finally, in `sydevs_examples/src/simulations/part02`, save the following code as `main.cpp`.

```cpp
#include <examples/getting_started/waveform/square_wave.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::square_wave();
    return 0;
}
```

Build the project and run the new executable `part02.exe`. It should produce the following output.

```
phase = 0
phase = 1
phase = 0
phase = 1
phase = 0
phase = 1
phase = 0
phase = 1
phase = 0
phase = 1
phase = 0
phase = 1
```

