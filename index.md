# SyDEVS

Multiscale Simulation and Systems Modeling Library

## About

The SyDEVS C++ library provides a framework for implementating complex systems analysis and simulation code in a modular, hierarchical fashion.

**In a nutshell, it will help make your simulation code scale.**

The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) and [DEVS](https://en.wikipedia.org/wiki/DEVS). These foundations give the framework the generality neeeded to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

## Documentation

- [Main Documentation](doc/html/index.html): Includes both the [Getting Started](doc/html/getting_started.html) tutorial and the API Reference.
- SyDEVS Framework Overview ([PPTX](doc/downloads/SyDEVS_Framework.pptx), [PDF](doc/downloads/SyDEVS_Framework.pdf)): Illustrates the main concepts & relates them to code.
- SyDEVS Building7m Tutorial ([PPTX](doc/downloads/SyDEVS_Tutorial.pptx), [PDF](doc/downloads/SyDEVS_Tutorial.pdf)): Challenges users to learn by enhancing an example.

## Concept

Using SyDEVS, simulation code is organized into nodes, which can be linked together to form dataflow and simulation networks. The four main types of nodes are illustrated below.

![SyDEVS Nodes](doc/images/sydevs_nodes.png "The four main types of SyDEVS nodes")

## Example

Here's an example of a SyDEVS atomic node declared in C++. Observe that the four types of ports and four types of event handlers match the atomic node diagram above. The code is taken from the [queueing](https://github.com/Autodesk/sydevs/tree/master/src/example/demo/queueing) demonstration project, which features an example of each of the four main types of nodes.

```cpp
/**
 * This node processes jobs one at a time. The time required to process a job
 * is determined by the service duration flow input. Any job that arrives while
 * another is being processed is placed in a queue and processed later. The
 * total time spend in an idle state, with no jobs to be processed, is tracked
 * and reported as a flow output.
 */
class queueing_node : public atomic_node
{
public:
    // Constructor/Destructor:
    queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~queueing_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> serv_dt_input;    // service duration
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration

protected:
    // State Variables:
    duration serv_dt;      // service duration (constant)
    std::vector<int64> Q;  // queue of IDs of jobs waiting to be processed
    duration idle_dt;      // idle duration (accumulating)
    duration planned_dt;   // planned duration

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};
```

The complete implementation is [here](https://github.com/Autodesk/sydevs/blob/master/src/example/demo/queueing/queueing_node.h).

## Notable Features

- [Multidimensional Arrays](doc/html/classsydevs_1_1arraynd.html#details): The SyDEVS library includes a generic multidimensional array class template supporting a range of numerical and slicing operations in C++, similar to [NumPy](http://www.numpy.org/) arrays in Python.
- [Standard International (SI) Units](doc/html/classsydevs_1_1quantity.html#details): Quantities can be declared with SI units such as "kilograms" for mass and "meters per second squared" for acceleration. Operations and assignments involving these quantities are checked at compile-time to ensure the units match.
- Multiscale Time: The following classes support the recording of past events and scheduling of future events involving any combination of time scales, yet store most event times internally using 64-bit numbers.
  - [time_point](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__point.html#details)
  - [time_sequence](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__sequence.html#details)
  - [time_queue](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__queue.html#details)
  - [time_cache](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__cache.html#details)

## Open Source License

SyDEVS is released under the [Apache License 2.0](https://github.com/Autodesk/sydevs/blob/master/LICENSE.md), a permissive open source license. The code adheres to the C++14 standard.

## Other DEVS Frameworks

- [Adevs](https://web.ornl.gov/~nutarojj/adevs/) (C++)
- [CD++](http://cell-devs.sce.carleton.ca/mediawiki/index.php/Main_Page) (C++)
- [CoSMoS](https://acims.asu.edu/software/cosmos/) (Java)
- [DesignDEVS](http://simaud.org/resources.php#software) (Lua)
- [DEVS-Ruby](https://github.com/devs-ruby/devs) (Ruby)
- [James II](http://jamesii.informatik.uni-rostock.de/jamesii.org/) (Java)
- [MS4 Me](http://www.ms4systems.com/pages/ms4me.php) (Java)
- [PowerDEVS](https://sourceforge.net/projects/powerdevs/) (C++)
- [PythonPDEVS](http://msdl.cs.mcgill.ca/projects/DEVS/PythonPDEVS) (Python)
- [Virtual Laboratory Environment](http://www.vle-project.org/) (C++)
- [x-s-y](https://code.google.com/archive/p/x-s-y/) (Python)

