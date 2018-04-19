| [Prev](part01.html) | [Getting Started - Table of Contents](index.html) | [Next](part03.html) |
# Part 2: Creating your First Simulation

Let's start by adding a few new folders to your `sydevs_examples` project.

1. In `sydevs_examples/src/examples`, create a folder named `getting_started`.
2. In the new `getting_started` folder, create a folder named `waveform`. This is where your first SyDEVS node will be located.
3. In `sydevs_examples/src/simulations` make a folder named `part02`. The code here will invoke the simulation code in `examples/getting_started/waveform`. 

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

The `CMakeLists.txt` file will have to be updated, so let's get that out of the way. Add the following instructions to the `Examples` section. These instructions prepare a list of the header (.h) files we will later create in the `waveform` folder.

```CMake
set(WAVEFORM_DIR ${EXAMPLES_DIR}/getting_started/waveform)
file(GLOB WAVEFORM_HDRS "${WAVEFORM_DIR}/*.h")
```

Also add the following instructions to the `Simulations` section. These instructions create an executable for Part 2 of the tutorial. Observe that the executable references the `waveform` header files.

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

It's time now to add a SyDEVS node to the project. In the `examples/getting_started/waveform` folder, create a text file named `square_wave_closed_system.h` and save it with the following code.

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

Also in the `waveform` folder, create a text file named `square_wave.h` and save it with the code below.

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

This file contains a function (`square_wave`) which creates a simulation (`sim`) that will last for 1 minute of simulated time. The simulation has a random seed of 0 and sends output data to `std::cout`. The simulation is executed by invoking the `sim` object's `process_remaining_events` member function.

Finally, in `simulations/part02`, save the following code as `main.cpp`. This main program simply calls the function described above.

```cpp
#include <examples/getting_started/waveform/square_wave.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::square_wave();
    return 0;
}
```

Build the project and run the new `part02` executable. It should produce the following output.

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

The phase of the square wave alternatives between off and on.

You've now completed Part 2 of the Getting Started tutorial, and have successfully run a simulation developed using SyDEVS. In [Part 3](part03.html), you will modify this example to output more information about the simulation run.

([***Continue to Part 3***](part03.html))
