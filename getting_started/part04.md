| [Prev](part03.html) | [Getting Started -- Table of Contents](index.html) | [Next](part05.html) |
# Part 4: Adding Ports

In [Part 3](getting_started_part03.html) we improved the `square_wave_closed_system` node by replacing `std::cout` with `print` as a means of outputting information. Yet there's an even better way to let a node interact with its surroundings. In general, a node should have ***ports*** through which it ***communicates*** with other nodes. So let's add ports to the node.

First make a copy of the `square_wave_closed_system.h` file, and name the new file `square_wave_generator_node.h`.

In the new version of the node, replace both instances of...

```cpp
SYDEVS_EXAMPLES_SQUARE_WAVE_CLOSED_SYSTEM_H_
```

...with...

```cpp
SYDEVS_EXAMPLES_SQUARE_WAVE_GENERATOR_NODE_H_
```

...and then use a Find & Replace tool to replace all instances of...

```cpp
square_wave_closed_system
```

...with the following.

```cpp
square_wave_generator_node
```

You now have a node that's identical to the one you previously simulated, except with a new name. Now let's add ports.

Find the comment `// Ports:` and define three ports underneath, as follows.

```cpp
    // Ports:
    port<flow, input, duration> period_dt_input;  // input waveform period
    port<flow, input, float64> duty_cycle_input;  // input duty cycle
    port<message, output, float64> y_output;      // output signal level
```

The first two ports are flow input ports, otherwise known as "parameters". They allow the period and duty cycle of the square wave to be configured when the node is first initialized. The message output port is used to update the output signal level whenever it changes.

Ports must be initialized at the top of the constructor, so find the following line of code...

```cpp
    : atomic_node(node_name, external_context)
```

...and directly underneath it add the three instructions below.

```cpp
    , period_dt_input("period_dt_input", external_interface())
    , duty_cycle_input("duty_cycle_input", external_interface())
    , y_output("y_output", external_interface())
```

Now that the ports have been defined and initialized, the next step is to make use of them. Go to the `initialization_event` member function and replace the lines that initialize the `period_dt` and `duty_cycle` state variables with the following.

```cpp
    period_dt = period_dt_input.value().fixed_at(time_precision());
    duty_cycle = duty_cycle_input.value();
```

The above lines extract data from the flow input ports. To make use of the message output port, delete the `print` statement in the `planned_event` function and replace it with the instruction below.

```cpp
   y_output.send(float64(phase));
```

Save the file.

The node is now complete and needs to be tested. However, because the node has ports, it cannot be simulated directly. To enable simulation, we need to define an encompassing "closed system" node with no ports.

Create a text file named `square_wave_integration_closed_system.h` and save it with the following code.

```cpp
#pragma once
#ifndef SYDEVS_EXAMPLES_SQUARE_WAVE_INTEGRATION_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_SQUARE_WAVE_INTEGRATION_CLOSED_SYSTEM_H_

#include <examples/getting_started/waveform/square_wave_generator_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class square_wave_integration_closed_system : public composite_node
{
public:
    // Constructor/Destructor:
    square_wave_integration_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~square_wave_integration_closed_system() = default;

    // Ports:

    // Components:
    parameter_node<duration> period_dt;
    parameter_node<float64> duty_ratio;
    square_wave_generator_node generator;
};


square_wave_integration_closed_system::square_wave_integration_closed_system(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , period_dt("period_dt", internal_context())
    , duty_ratio("duty_ratio", internal_context(), 0.5)
    , generator("generator", internal_context())
{
    // Initialization Links:
    inner_link(period_dt.parameter, generator.period_dt_input);
    inner_link(duty_ratio.parameter, generator.duty_cycle_input);

    // Simulation Links:

    // Finalization Links:
}


}  // namespace

#endif
```

The node in this file inherits from `composite_node`, a base class for defining networks of nodes. In this case the network consists of two parameter nodes (`period_dt` and `duty_ratio`) connected to a simulation node (`generator`). In the next part of this tutorial, we will expand this example with an additional simulation node that numerically integrates the square wave.

Now that the `square_wave_generator_node` is incorporated by another node with no ports, we can prepare a simulation. Edit the `square_wave.h` file by inserting the following `#include` line.

```cpp
#include <examples/getting_started/waveform/square_wave_integration_closed_system.h>
```

Then add the `simulate_square_wave_integration_closed_system` function, as defined below, directly below the similar function that already exists in the file.

```cpp
void simulate_square_wave_integration_closed_system()
{
    try {
        simulation<square_wave_integration_closed_system> sim(1_min, 0, std::cout);
        sim.top.period_dt.set_value(10_s);
        sim.top.duty_ratio.set_value(0.3);
        sim.top.generator.y_output.print_on_use();
        sim.process_remaining_events();
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}
```

In the above function, observe the lines that provide values for the `period_dt` and `duty_ratio` parameters. If the `period_dt` assignment were omitted, the simulation would fail due to the missing input. But if the `duty_ratio` assignment were omitted, the simulation would proceed with the 50% default duty ratio specified in the composite node. Also observe in the above function the instruction that prints every output of the `generator` node's `y_output` port. This is how we will view the simulation results.

Save the file.

In the `src/simulations` folder, make a new folder called `simulation_with_ports`. In the new folder, save a `main.cpp` file with the following code.

```cpp
#include <examples/getting_started/waveform/square_wave.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::simulate_square_wave_integration_closed_system();
    return 0;
}
```

Finally, append the following instructions onto the bottom of `CMakeLists.txt`.

```cmake
set(SIMULATION_WITH_PORTS_DIR ${SIMULATIONS_DIR}/simulation_with_ports)
aux_source_directory(${SIMULATION_WITH_PORTS_DIR} SIMULATION_WITH_PORTS_SRCS)
add_executable(simulation_with_ports ${SIMULATION_WITH_PORTS_SRCS} ${WAVEFORM_HDRS})
target_link_libraries(simulation_with_ports debug SyDEVS-static-debug optimized SyDEVS-static)
```

Build and run the `simulation_with_ports` executable. You should get the results below, where the signal level is printed along with the node and port on which the output values are made available.

```
0|0|$time:time_point()
0|5|top.generator#y_output:0
1|0|$time:time_point() + 7_s
1|1|top.generator#y_output:1
2|0|$time:time_point() + 10_s
2|1|top.generator#y_output:0
3|0|$time:time_point() + 17_s
3|1|top.generator#y_output:1
4|0|$time:time_point() + 20_s
4|1|top.generator#y_output:0
5|0|$time:time_point() + 27_s
5|1|top.generator#y_output:1
6|0|$time:time_point() + 30_s
6|1|top.generator#y_output:0
7|0|$time:time_point() + 37_s
7|1|top.generator#y_output:1
8|0|$time:time_point() + 40_s
8|1|top.generator#y_output:0
9|0|$time:time_point() + 47_s
9|1|top.generator#y_output:1
10|0|$time:time_point() + 50_s
10|1|top.generator#y_output:0
11|0|$time:time_point() + 57_s
11|1|top.generator#y_output:1
```

| [***Continue to Part 5***](part05.html) |
