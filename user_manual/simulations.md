| [Prev](collection_nodes.html) | [User Manual -- Table of Contents](index.html) | Next |
# Simulations

The ***simulation*** class template performs a simulation using a SyDEVS node with no ports. This SyDEVS node is specified as the template parameter. The simulation class template can be found in [simulation.h](https://github.com/Autodesk/sydevs/blob/master/src/sydevs/systems/simulation.h), which must be included in order to run simulations.

```cpp
#include <sydevs/systems/simulation.h>
```

## Sample Simulations

Several examples of simulations can be found in [queueing.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing.h). Below is one of the examples.

```cpp
    // Perform a simulation using the "parallel queueing" collection node.
    try {
        std::cout << "Testing parallel_queueing_node" << std::endl;
        simulation<parallel_queueing_closed_system> sim(2_min, 0, std::cout);
        sim.top.job_gen_dt.set_value(5_s);
        sim.top.serv_dt.set_value(14_s);
        sim.top.max_n.set_value(3);
        sim.top.queue.prototype.print_on_event();
        sim.top.queue.prototype.job_id_input.print_on_use();
        sim.top.queue.prototype.job_id_output.print_on_use();
        sim.process_remaining_events();
        std::cout << "Idle time: " << sim.top.idle_dt.value().rescaled(unit) << std::endl;
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
```

## Simulation Instantiation

Simulations are instantiated with three arguments: the duration of the simulation, the random seed, and the output stream. In the example below, the simulation is to be run for 2 minutes of simulated time, the random seed is 0, and debug info is directed to the standard output stream.

```cpp
        simulation<parallel_queueing_closed_system> sim(2_min, 0, std::cout);
```

It is possible to supply specific start and end times instead of a total simulation duration. See the [simulation constructor reference](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1systems_1_1simulation.html#acca4a2e7f23f9ef5426e230393f07471).

Note that there is no danger in using a random seed of 0. The seed is transformed via a series of operations into a sequence of random numbers that are used to initialize the Mersenne Twister 19937 generator. The seeding algorithm can be found in [node_context.cpp](https://github.com/Autodesk/sydevs/blob/master/src/sydevs/systems/node_context.cpp).


## Simulation Configuration

Configuring a simulation involves supplying values to the parameter nodes, and setting print flags that determine what information is directed to the output stream. An example is below.

```cpp
        sim.top.job_gen_dt.set_value(5_s);
        sim.top.serv_dt.set_value(14_s);
        sim.top.max_n.set_value(3);
        sim.top.queue.prototype.print_on_event();
        sim.top.queue.prototype.job_id_input.print_on_use();
        sim.top.queue.prototype.job_id_output.print_on_use();
```

Parameter values are typically only required in cases where the topmost node in the simulation model hierarchy is a composite node. This topmost node is accessible via the public simulation member variable `top`. Because components are declared public, the parameter nodes should be accessible as member variables of `top`. Values are supplied to a parameter node using its `set_value` member function. Some parameter nodes have default values set within the composite node; these parameters do not need to be assigned externally.

To indicate what information is directed to the output stream, one accesses a node or port within the model hierarchy, and invokes a member function. For nodes within the hierarchy, the member functions include the following: `print_on_event`, which indicates the type of event being processed; `print_elapsed_duration`, which provides the elapsed duration since the previous event of any type; and `print_planned_duration`, which provides the planned duration until the next planned event. For ports, one may invoke `print_on_use`, which provides the value that is either input or output from the port.

If any type of print flag is set within a prototype node of a collection node, the corresponding flag is set for all of the collection node's agents.

## Simulation Execution

The simplest way to execute a simulation is to call `process_remaining_events`, which runs the simulation until it terminates.

```cpp
        sim.process_remaining_events();
```

It is also possible to run the simulation one event at a time, or in batches of events. See the [simulation class reference](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1systems_1_1simulation.html) for details.

## Simulation Analysis

After a simulation completes, one may extract values from the statistic nodes by invoking the member function `value`. Below is an example.

```cpp
        std::cout << "Idle time: " << sim.top.idle_dt.value().rescaled(unit) << std::endl;
```

## Simulation Error Handling

It is recommended that at least two types of exceptions be caught. The first type is `system_node::error`, which is primarily intended to capture exceptions caused by modeling mistakes. For example, if someone implements a node with an illegal operation in one of the event handlers, it should be caught by this exception type. The other exception type is the standard `std::exception`, which should catch all other exceptions.

```cpp
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
```
