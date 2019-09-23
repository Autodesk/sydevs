| [Prev](part01.html) | [Getting Started -- Table of Contents](index.html) | [Next](part03.html) |
# Part 2: Creating your First Simulation

Let's start by adding a few new folders to your `sydevs-examples` project.

1. In `sydevs-examples/src/examples`, create a folder named `getting_started`.
2. In the new `getting_started` folder, create a folder named `waveform`. This is where your first SyDEVS node will be located.
3. In `sydevs-examples/src/simulations` make a folder named `first_simulation`. The code here will invoke the simulation code in `examples/getting_started/waveform`. 

The overall directory structure should now be as follows.

```
sydevs-examples/
    bin/
        ...
    external/
        ...
    src/
        examples/
            getting_started/
                waveform/
        simulations/
            first_simulation/
            setting_up/
```

The `CMakeLists.txt` file will have to be updated, so let's get that out of the way. Add the following instructions to the `Examples` section. These instructions prepare a list of the header (.h) files you will later create in the `waveform` folder.

```cmake
set(WAVEFORM_DIR ${EXAMPLES_DIR}/getting_started/waveform)
file(GLOB WAVEFORM_HDRS "${WAVEFORM_DIR}/*.h")
```

Also add the following instructions to the `Simulations` section. These instructions create an executable for Part 2 of the tutorial. Observe that the executable references the `waveform` header files.

```cmake
set(FIRST_SIMULATION_DIR ${SIMULATIONS_DIR}/first_simulation)
aux_source_directory(${FIRST_SIMULATION_DIR} FIRST_SIMULATION_SRCS)
add_executable(first_simulation ${FIRST_SIMULATION_SRCS} ${WAVEFORM_HDRS})
target_link_libraries(first_simulation debug SyDEVS-static-debug optimized SyDEVS-static)
```

The bottom of the `CMakeLists.txt` file should now appear as follows.

```cmake
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

set(SETTING_UP_DIR ${SIMULATIONS_DIR}/setting_up)
aux_source_directory(${SETTING_UP_DIR} SETTING_UP_SRCS)
add_executable(setting_up ${SETTING_UP_SRCS})
target_link_libraries(setting_up debug SyDEVS-static-debug optimized SyDEVS-static)

set(FIRST_SIMULATION_DIR ${SIMULATIONS_DIR}/first_simulation)
aux_source_directory(${FIRST_SIMULATION_DIR} FIRST_SIMULATION_SRCS)
add_executable(first_simulation ${FIRST_SIMULATION_SRCS} ${WAVEFORM_HDRS})
target_link_libraries(first_simulation debug SyDEVS-static-debug optimized SyDEVS-static)
```

Save the file.

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
    duration period_dt;  // duration of one period of the square wave
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
    phase = 1;
    return 0_s;
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
    std::cout << "y = " << float64(phase) << std::endl;
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
- The node represents a square wave cycling between `y = 0` (off) and `y = 1` (on). A full cycle is 10 seconds (`period_dt`). The "on" phase lasts for 30% of this time (`duty_cycle`). Observe that the planned duration (`planned_dt`) is either 3 seconds (`period_dt*duty_cycle`) or 7 seconds (`period_dt*(1.0 - duty_cycle)`) depending on the phase. Whenever the planned duration elapses, the `planned_event` is invoked.

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


void simulate_square_wave_closed_system()
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

This file contains a function (`simulate_square_wave_closed_system`) which constructs a simulation (`sim`) configured using three arguments. The first argument with value `1_min` (1 minute) is the duration of the simulation in simulated time. The second argument with value 0 is an integer that seeds the random number generator common to all SyDEVS nodes. For stochastic simulations, the same seed on the same platform should give the same results. Changing the seed will change all randomly generated numbers. The third argument indicates that output data is sent to `std::cout`. The simulation is executed by invoking the `sim` object's `process_remaining_events` member function.

Finally, in `simulations/first_simulation`, save the following code as `main.cpp`. This main program simply calls the function described above.

```cpp
#include <examples/getting_started/waveform/square_wave.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::simulate_square_wave_closed_system();
    return 0;
}
```

Build the project and run the new `first_simulation` executable. It should produce the following output.

```
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
```

The square wave signal alternatives between off and on.

You've now completed Part 2 of the Getting Started tutorial, and have successfully run a simulation developed using SyDEVS. In [Part 3](part03.html), you will modify this example to output more information about the simulation run.

| [***Continue to Part 3***](part03.html) |
