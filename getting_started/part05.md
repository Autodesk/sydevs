| [Prev](part04.html) | [Getting Started -- Table of Contents](index.html) | [Next](part06.html) |
# Part 5: Linking Simulation Nodes

Here we will finally end up with an example that demonstrates the main purpose of SyDEVS: to support models with ***multiple*** simulation nodes that ***communicate*** as simulated time advances. 

The example will still be very simple, featuring one-way communication between two simulation nodes. We will take the `square_wave_generator_node` you prepared in [Part 4](part04.html), and direct its output messages into a new `waveform_integrator_node` that performs numerical integration on the piecewise constant signal. 

In the `waveform` example folder, save a file named `waveform_integrator_node.h` with the following code.

```cpp
#pragma once
#ifndef SYDEVS_EXAMPLES_WAVEFORM_INTEGRATOR_NODE_H_
#define SYDEVS_EXAMPLES_WAVEFORM_INTEGRATOR_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class waveform_integrator_node : public atomic_node
{
public:
    // Constructor/Destructor:
    waveform_integrator_node(const std::string& node_name, const node_context& external_context);
    virtual ~waveform_integrator_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> step_dt_input;   // time step for message output
    port<message, input, float64> y_input;       // input signal level
    port<message, output, float64> Y_output;     // integrated signal level
    port<flow, output, float64> Y_final_output;  // final integrated signal level

protected:
    // State Variables:
    duration step_dt;     // duration between successive output messages
    float64 y;            // current signal level
    float64 Y;            // current integrated level
    duration planned_dt;  // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline waveform_integrator_node::waveform_integrator_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , step_dt_input("step_dt_input", external_interface())
    , y_input("y_input", external_interface())
    , Y_output("Y_output", external_interface())
    , Y_final_output("Y_final_output", external_interface())
{
}


inline duration waveform_integrator_node::initialization_event()
{
    step_dt = step_dt_input.value().fixed_at(time_precision());
    y = 0.0;
    Y = 0.0;
    planned_dt = 0_s;
    return planned_dt;
}


inline duration waveform_integrator_node::unplanned_event(duration elapsed_dt)
{
    Y += y*(elapsed_dt/1_s);
    if (y_input.received()) {
        y = y_input.value();
    }
    planned_dt -= elapsed_dt;
    return planned_dt;
}


inline duration waveform_integrator_node::planned_event(duration elapsed_dt)
{
    Y += y*(elapsed_dt/1_s);
    Y_output.send(Y);
    planned_dt = step_dt;
    return planned_dt;
}


inline void waveform_integrator_node::finalization_event(duration elapsed_dt)
{
    Y += y*(elapsed_dt/1_s);
    Y_final_output.assign(Y);
}


}  // namespace

#endif
```

The `waveform_integrator_node` is an example of node that can accept an input message at any time, but only outputs messages at multiples of a fixed interval. Whenever there are inputs that do not affect the timing of outputs, it is typical to see the following line of code in the `unplanned_event` function.

```cpp
    planned_dt -= elapsed_dt;
```

This line basically states that the duration remaining until the next event (`planned_event`) is whatever it was at the previous event, minus the time elapsed since that past event (`elapsed_dt`). In otherwords, the absolute time of the next scheduled event remains unchanged.

The actual numerical integration is performed by the line...

```cpp
    Y += y*(elapsed_dt/1_s);
```

...which appears 3 times in the class: once for every event type with an elapsed time variable (`elapsed_dt`).

Notice that the `waveform_integrator_node` has one port of each of the four types. We previously saw that flow input ports provide a node with parameter values, and that message output ports send messages over the course of a simulation. A message input port, such as `y_input` in this example, receives messages over the course of a simulation. A flow output port, such as `Y_final_output`, can be regarded as a `statistic` that provides information when the simulation is over or when the node is terminated.

Now we need to make use of the new node. Open `square_wave_integration_closed_system.h`. Add additional `#include` statements. You should end up with the following.

```cpp
#include <examples/getting_started/waveform/square_wave_generator_node.h>
#include <examples/getting_started/waveform/waveform_integrator_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>
```

Add to the component declarations. There should be six in total.

```cpp
    // Components:
    parameter_node<duration> period_dt;
    parameter_node<float64> duty_ratio;
    parameter_node<duration> integrator_step_dt;
    square_wave_generator_node generator;
    waveform_integrator_node integrator;
    statistic_node<float64> Y_final;
```

Expand the constructor as follows, then save.

```cpp
square_wave_integration_closed_system::square_wave_integration_closed_system(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , period_dt("period_dt", internal_context(), 10_s)
    , duty_ratio("duty_ratio", internal_context(), 0.5)
    , integrator_step_dt("integrator_step_dt", internal_context())
    , generator("generator", internal_context())
    , integrator("integrator", internal_context())
    , Y_final("Y_final", internal_context())
{
    // Initialization Links:
    inner_link(period_dt.parameter, generator.period_dt_input);
    inner_link(duty_ratio.parameter, generator.duty_cycle_input);
    inner_link(integrator_step_dt.parameter, integrator.step_dt_input);

    // Simulation Links:
    inner_link(generator.y_output, integrator.y_input);

    // Finalization Links:
    inner_link(integrator.Y_final_output, Y_final.statistic);
}
```

Observe the one simulation link that connects the `generator` to the `integrator`. This is the only link that is activated repeatly as the simulation is underway (i.e. as simulated time is advancing). Complex models may have many more simulation links.

Also observe the new initialization link, which supplies a value from the new parameter node `integrator_step_dt`. In addition, there is now a finalization link that delivers a value to the new statistic node `Y_final`.

Note that initialization and finalization links, which connect flow ports, must form a directed acyclic graph. You must not have any cycles where, for example, Node `A` supplies information to Node `B`, Node `B` supplies information to Node `C`, and Node `C` supplies information to Node `A`. Simulation links, which connect message ports, are permitted to form cycles.

To complete the example, open `square_wave.h` and replace the instructions in the `try` block of the `simulate_square_wave_integration_closed_system` function. The new code sets the time step of the integrator output, and also obtains the final value of the integrated signal (the accumulated area under the square wave).

```cpp
        simulation<square_wave_integration_closed_system> sim(1_min, 0, std::cout);
        sim.top.period_dt.set_value(10_s);
        sim.top.duty_ratio.set_value(0.3);
        sim.top.integrator_step_dt.set_value(1500_ms);
        sim.top.generator.y_output.print_on_use();
        sim.top.integrator.Y_output.print_on_use();
        sim.process_remaining_events();
        float64 Y_final = sim.top.Y_final.value();
        std::cout << "Y_final = " << Y_final << std::endl;
```

Save the file.

After rebuilding and running the `simulation_with_ports` executable, you should see the following results.

```
0|0|$time:time_point()
0|7|top.generator#y_output:0
0|11|top.integrator#Y_output:0
1|0|$time:time_point() + 1_s + 500_ms
1|1|top.integrator#Y_output:0
2|0|$time:time_point() + 3_s
2|1|top.integrator#Y_output:0
3|0|$time:time_point() + 4_s + 500_ms
3|1|top.integrator#Y_output:0
4|0|$time:time_point() + 6_s
4|1|top.integrator#Y_output:0
5|0|$time:time_point() + 7_s
5|1|top.generator#y_output:1
6|0|$time:time_point() + 7_s + 500_ms
6|1|top.integrator#Y_output:0.5
7|0|$time:time_point() + 9_s
7|1|top.integrator#Y_output:2
8|0|$time:time_point() + 10_s
8|1|top.generator#y_output:0
9|0|$time:time_point() + 10_s + 500_ms
9|1|top.integrator#Y_output:3
10|0|$time:time_point() + 12_s
10|1|top.integrator#Y_output:3
11|0|$time:time_point() + 13_s + 500_ms
11|1|top.integrator#Y_output:3
12|0|$time:time_point() + 15_s
12|1|top.integrator#Y_output:3
13|0|$time:time_point() + 16_s + 500_ms
13|1|top.integrator#Y_output:3
14|0|$time:time_point() + 17_s
14|1|top.generator#y_output:1
15|0|$time:time_point() + 18_s
15|1|top.integrator#Y_output:4
16|0|$time:time_point() + 19_s + 500_ms
16|1|top.integrator#Y_output:5.5
17|0|$time:time_point() + 20_s
17|1|top.generator#y_output:0
18|0|$time:time_point() + 21_s
18|1|top.integrator#Y_output:6
19|0|$time:time_point() + 22_s + 500_ms
19|1|top.integrator#Y_output:6
20|0|$time:time_point() + 24_s
20|1|top.integrator#Y_output:6
21|0|$time:time_point() + 25_s + 500_ms
21|1|top.integrator#Y_output:6
22|0|$time:time_point() + 27_s
22|1|top.generator#y_output:1
22|5|top.integrator#Y_output:6
23|0|$time:time_point() + 28_s + 500_ms
23|1|top.integrator#Y_output:7.5
24|0|$time:time_point() + 30_s
24|1|top.generator#y_output:0
24|5|top.integrator#Y_output:9
25|0|$time:time_point() + 31_s + 500_ms
25|1|top.integrator#Y_output:9
26|0|$time:time_point() + 33_s
26|1|top.integrator#Y_output:9
27|0|$time:time_point() + 34_s + 500_ms
27|1|top.integrator#Y_output:9
28|0|$time:time_point() + 36_s
28|1|top.integrator#Y_output:9
29|0|$time:time_point() + 37_s
29|1|top.generator#y_output:1
30|0|$time:time_point() + 37_s + 500_ms
30|1|top.integrator#Y_output:9.5
31|0|$time:time_point() + 39_s
31|1|top.integrator#Y_output:11
32|0|$time:time_point() + 40_s
32|1|top.generator#y_output:0
33|0|$time:time_point() + 40_s + 500_ms
33|1|top.integrator#Y_output:12
34|0|$time:time_point() + 42_s
34|1|top.integrator#Y_output:12
35|0|$time:time_point() + 43_s + 500_ms
35|1|top.integrator#Y_output:12
36|0|$time:time_point() + 45_s
36|1|top.integrator#Y_output:12
37|0|$time:time_point() + 46_s + 500_ms
37|1|top.integrator#Y_output:12
38|0|$time:time_point() + 47_s
38|1|top.generator#y_output:1
39|0|$time:time_point() + 48_s
39|1|top.integrator#Y_output:13
40|0|$time:time_point() + 49_s + 500_ms
40|1|top.integrator#Y_output:14.5
41|0|$time:time_point() + 50_s
41|1|top.generator#y_output:0
42|0|$time:time_point() + 51_s
42|1|top.integrator#Y_output:15
43|0|$time:time_point() + 52_s + 500_ms
43|1|top.integrator#Y_output:15
44|0|$time:time_point() + 54_s
44|1|top.integrator#Y_output:15
45|0|$time:time_point() + 55_s + 500_ms
45|1|top.integrator#Y_output:15
46|0|$time:time_point() + 57_s
46|1|top.generator#y_output:1
46|5|top.integrator#Y_output:15
47|0|$time:time_point() + 58_s + 500_ms
47|1|top.integrator#Y_output:16.5
Y_final = 18
```

The integrated waveform (`top.integrator#Y_output`) increases whenever the square wave (`top.generator#y_output`) is in the "on" phase of the cycle. In the end, the area under the square wave is 18 (as expected, since there are 3 seconds per cycle in the "on" phase, times 6 ten-second cycles in the one-minute simulation).

We're now done with the `waveform` example. In [Part 6](part06.html), you'll download and incorporate two examples that demonstrate other types of nodes and SyDEVS features.

| [***Continue to Part 6***](part06.html) |
