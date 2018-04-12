# SyDEVS

Multiscale Simulation and Systems Modeling Library

## About

The SyDEVS open source C++ library provides a framework for implementing complex systems analysis and simulation code in a modular, hierarchical fashion.

**In a nutshell, it will help make your simulation code scale.**

The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) and [DEVS](https://en.wikipedia.org/wiki/DEVS). These foundations give the framework the generality needed to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

## Documentation

- [Main Documentation](doc/html/index.html): Includes both the [Getting Started](doc/html/getting_started.html) tutorial and the API Reference.
- SyDEVS Framework Overview ([PPTX](doc/downloads/SyDEVS_Framework_Overview.pptx), [PDF](doc/downloads/SyDEVS_Framework_Overview.pdf)): Illustrates the main concepts & relates them to code.
- SyDEVS Building7m Tutorial ([PPTX](doc/downloads/SyDEVS_Building7m_Tutorial.pptx), [PDF](doc/downloads/SyDEVS_Building7m_Tutorial.pdf)): Challenges users to learn by enhancing an example.

## Download

Download the latest SyDEVS release [here](https://github.com/Autodesk/sydevs/releases) (**first release coming soon!**), or fork, clone, and build the [repo](https://github.com/Autodesk/sydevs).

You will need a C++ compiler that implements the C++14 standard. SyDEVS has been tested successfully with Visual Studio 2015, Visual Studio 2017, and recent versions of GCC and Clang.

SyDEVS is released under the [Apache License 2.0](https://github.com/Autodesk/sydevs/blob/master/LICENSE.md), a permissive open source license.

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

The complete implementation of the node can be found [here](https://github.com/Autodesk/sydevs/blob/master/src/example/demo/queueing/queueing_node.h).

## Notable Features

- [Multidimensional Arrays](doc/html/classsydevs_1_1arraynd.html#details): The SyDEVS library includes a generic multidimensional array class template supporting a range of numerical and slicing operations in C++, similar to [NumPy](http://www.numpy.org/) arrays in Python.
- [Standard International (SI) Units](doc/html/classsydevs_1_1quantity.html#details): Quantities can be declared with SI units such as "kilograms" for mass and "meters per second squared" for acceleration. Operations and assignments involving these quantities are checked at compile-time to ensure the units match.
- Multiscale Time: The following classes support the recording of past events and scheduling of future events involving any combination of time scales, yet store most event times internally using 64-bit numbers.
  - [time_point](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__point.html#details)
  - [time_sequence](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__sequence.html#details)
  - [time_queue](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__queue.html#details)
  - [time_cache](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1time__cache.html#details)

## Related Articles

- [Designing DEVS Visual Interfaces for End-User Programmers](https://autodeskresearch.com/publications/designingdevs) (Maleki et al., 2015): SyDEVS is largely based on the visual programming interfaces proposed in this paper. Although there is currently no graphical modeling tool accompanying the framework, the same node-based models presented visually in the paper can be implemented directly in C++ using the SyDEVS library. Also, the manner in which DEVS is expressed in SyDEVS is largely based on the conventions and terminology proposed in the paper.
- [Multiscale Representation of Simulated Time](https://autodeskresearch.com/publications/multiscale-representation-simulated-time) (Goldstein et al., 2017): Standard representations of time based on integers or floating-point numbers have serious drawbacks that limit the ability of most frameworks to handle models with dramatically different timescales. The multiscale time representation proposed in this paper is implemented in SyDEVS, allowing models with varying levels of time precision to coexist while keeping rounding errors bounded.
- [The Future of Designing Systems](https://autodeskresearch.com/blog/future-designing-systems) (Goldstein, 2016): This blog post describes the motivation behind frameworks like SyDEVS, focusing on the design of buildings as systems involving people, controls, and environmental conditions.

## Other DEVS Frameworks

- [Adevs](https://web.ornl.gov/~nutarojj/adevs/) (C++) developed at Oak Ridge National Laboratory
- [CD++](http://cell-devs.sce.carleton.ca/mediawiki/index.php/Main_Page) (C++) by the [ARSLab](http://cell-devs.sce.carleton.ca/ars/) at Carleton University
- [CoSMoS](https://acims.asu.edu/software/cosmos/) (Java) by the [ACIMS](https://acims.asu.edu/) at Arizona State University
- [DesignDEVS](http://simaud.org/resources.php#software) (Lua) by [Autodesk Research](https://autodeskresearch.com/)
- [DEVS-Ruby](https://github.com/devs-ruby/devs) (Ruby) developed at the University of Corsica
- [James II](http://jamesii.informatik.uni-rostock.de/jamesii.org/) (Java) by the [Modeling and Simulation Group](https://mosi.informatik.uni-rostock.de/) at the University of Rostock
- [MS4 Me](http://www.ms4systems.com/pages/ms4me.php) (Java) by [MS4 Systems](http://www.ms4systems.com/pages/main.php)
- [PowerDEVS](https://sourceforge.net/projects/powerdevs/) (C++) developed at the National University of Rosario
- [PythonPDEVS](http://msdl.cs.mcgill.ca/projects/DEVS/PythonPDEVS) (Python) by the [MSDL](http://msdl.cs.mcgill.ca/) (McGill University) and [AnSyMo](https://www.uantwerpen.be/en/research-groups/nexor/team/ansymo/) (University of Antwerp)
- [Virtual Laboratory Environment](http://www.vle-project.org/) (C++)
- [XSY](https://code.google.com/archive/p/x-s-y/) (Python)

## Contact

Inquires about SyDEVS can be directed to [Rhys Goldstein](https://autodeskresearch.com/people/rhys-goldstein).
