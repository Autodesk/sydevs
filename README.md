# SyDEVS

Multiscale Simulation and Systems Modeling Library

| Windows | Linux/OSX |
|---------|-----------|
| [![Build status](https://ci.appveyor.com/api/projects/status/afe2i9b8h9nxfta4?svg=true)](https://ci.appveyor.com/project/sbreslav/sydevs) | [![Build status](https://travis-ci.org/Autodesk/sydevs.svg?branch=master)](https://travis-ci.org/Autodesk/sydevs) |

## About

This library provides a framework for implementating complex systems analysis and simulation code in a modular/hierarchical fashion. It was originally developed to serve as a backend for the visual programming interfaces described by [Maleki et al. (2015)](https://www.autodeskresearch.com/publications/designingdevs), but the same functionality can be achieved without a GUI by defining C++ classes that derive from one of the system node base classes ([`atomic_node`](src/sydevs/systems/atomic_node.h), [`composite_node`](src/sydevs/systems/composite_node.h), [`collection_node`](src/sydevs/systems/collection_node.h), [`function_node`](src/sydevs/systems/function_node.h)). The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) as exemplified by Autodesk's [Dynamo](http://dynamobim.org/) tool, and the [DEVS](https://en.wikipedia.org/wiki/DEVS) message-passing paradigm implemented in tools such as DesignDEVS (see [software](http://simaud.com/resources.php#software), [conference paper](https://www.autodeskresearch.com/designdevs), [journal paper](https://www.autodeskresearch.com/publications/practical-aspects-designdevs-simulation-environment)). These foundations give the framework the generality neeeded to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

Webpage: [https://autodesk.github.io/sydevs](https://autodesk.github.io/sydevs)

## Main Classes

The library includes C++11/14 classes grouped into three main folders:

#### [`src/sydevs/core`](src/sydevs/core)

This folder contains generic classes that may be useful for a variety of applications.

- [`scale`](src/sydevs/core/scale.h): Represents the general concept of scale as a dimensionless power of 1000.
  - [`number_types.h`](src/sydevs/core/number_types.h) (related header file): Defines the constant `pi` and type aliases for integers and floating-point numbers.
- [`quantity`](src/sydevs/core/quantity.h): A data type template which represents a Standard International (SI) quantity (e.g. mass, distance, duration, acceleration) as a multiple of a base-1000 precision level.
  - [`units`](src/sydevs/core/units.h) (related struct template): Represents SI units including the base units such as grams, meters, and seconds, and derived units such as meters-per-second-squared.
- [`arraynd`](src/sydevs/core/arraynd.h): A multidimensional array with features similar to those of the [NumPy Python Library](http://www.numpy.org/).
  - [`range`](src/sydevs/core/range.h) (related class): Represents a range of array indices along a single dimension.
- [`core_types`](src/sydevs/core/core_types.h): Traits for the set of data types that can be passed among system nodes.
  - [`pointer`](src/sydevs/core/pointer.h) (related class): Points to any type of data.
  - [`string_builder`](src/sydevs/core/string_builder.h) (related class): Faciliates value-to-string conversions.

#### [`src/sydevs/time`](src/sydevs/time)

The classes in this folder implement the multiscale time representation proposed by [Goldstein et al. (2017)](https://www.autodeskresearch.com/publications/multiscale-representation-simulated-time) that supports the recording and scheduling of events over any combination of short and long time scales. The classes below use 64-bit operations where possible despite accommodating extremely disparate scales.

- [`time_point`](src/sydevs/time/time_point.h): Represents a point in time as an arbitrary-precision multiple of its shortest time precision.
- [`time_sequence`](src/sydevs/time/time_sequence.h): Represents a sequence of increasing time points.
- [`time_queue`](src/sydevs/time/time_queue.h): Supports the scheduling of future events.
- [`time_cache`](src/sydevs/time/time_cache.h): Provides durations elapsed since past events.

#### [`src/sydevs/systems`](src/sydevs/systems)

This folder contains the elements from which dataflow + message-passing networks are constructed.

- [`system_node`](src/sydevs/systems/system_node.h): A base class for all nodes.
  - [`atomic_node`](src/sydevs/systems/atomic_node.h) (derived from [`system_node`](src/sydevs/systems/system_node.h)): A base class for indivisible nodes in which simulation behavior is procedurally encoded.
  - [`composite_node`](src/sydevs/systems/composite_node.h) (derived from [`system_node`](src/sydevs/systems/system_node.h)): A base class for all nodes defined as fixed-structure compositions of different types of nodes.
  - [`collection_node`](src/sydevs/systems/collection_node.h) (derived from [`system_node`](src/sydevs/systems/system_node.h)): A base class for all nodes defined as variable-length collections of nodes of a single type.
  - [`function_node`](src/sydevs/systems/function_node.h) (derived from [`system_node`](src/sydevs/systems/system_node.h)): A base class for indivisible nodes in which function behavior is procedurally encoded.
    - [`parameter_node`](src/sydevs/systems/parameter_node.h) (derived from [`function_node`](src/sydevs/systems/function_node.h)): A function node template for supplying parameters.
    - [`statistic_node`](src/sydevs/systems/statistic_node.h) (derived from [`function_node`](src/sydevs/systems/function_node.h)): A function node template for retrieving statistics.
  - [`port`](src/sydevs/systems/port.h) (related class): A base class for the four types of ports (flow input, message input, message output, flow output).
- [`simulation`](src/sydevs/systems/simulation.h): Class template for simulations performed using a top-level (port-free) system node.
  - [`discrete_event_time`](src/sydevs/systems/discrete_event_time.h) (related class): Represents progress through a simulation, encapsulating both simulated time and a counter of events within a single point in simulated time.

## Building

#### Building on Windows (Visual Studio)
* You will need [CMake](http://www.cmake.org/)
* Make sure you have [Visual Studio 2015](https://www.visualstudio.com) or [Visual Studio 2017](https://www.visualstudio.com)
* `mkdir bin`
* `cd bin/`
* `cmake -G "Visual Studio 14 2015 Win64" ..` or `cmake -G "Visual Studio 15 2017 Win64" ..`
* Open 'bin/SyDEVS.sln' in Visual Studio
* Recommendation: In `Tools->Options->Text Editor->C/C++->Tabs`, select `Insert spaces` with a `Tab size` and `Indent size` of `4`

#### Building on Mac OS
* You will need [CMake](http://www.cmake.org/)
* `mkdir bin`
* `cd bin/`
* `cmake -G "Xcode" ..`
* `xcodebuild -project SyDEVS.xcodeproj` or open the `.xcodeproj` file.

#### Testing

Unit and regression tests are run automatically during the build process.

The unit testing framework used in SyDEVS is [Catch2](https://github.com/catchorg/Catch2). See the [documentation](https://github.com/catchorg/Catch2/tree/master/docs) for a tutorial and reference material. Catch2 is released under the Boost Software License 1.0.

## Documentation

#### Webpage

Main SyDEVS webpage:

- [https://autodesk.github.io/sydevs](https://autodesk.github.io/sydevs).

#### PowerPoint

There are two PowerPoint documents located in the [`doc`](doc) folder:

- [`SyDEVS_Framework_Overview.pptx`](doc/SyDEVS_Framework_Overview.pptx): An overview of the systems modeling framework and related snippets of code.
- [`SyDEVS_Building7m_Tutorial.pptx`](doc/SyDEVS_Building7m_Tutorial.pptx): A tutorial that challenges developers to enhance the [`building7m`](src/examples/demo/building7m) example.

#### HTML

To build or update the documentation files with [Doxygen](http://www.stack.nl/~dimitri/doxygen) make sure you are in the top level directory (where the `doxygen.config` is) and execute the following command:

- `doxygen doxygen.config`

This will build the documentation in the [`doc`](doc) directory. To open the documentation:

- `cd docs/html`
- Open `index.html` in your web browser.

You should update the documentation on a regular basis to keep it in sync with the code.

Latest published version of the HTML documentation:

- [https://autodesk.github.io/sydevs/doc/html/index.html](https://autodesk.github.io/sydevs/doc/html/index.html).

#### Examples

Examples of SyDEVS-based simulations are found in the [`src/examples`](src/examples) directory.

The best examples for learning the library are those in [`demo`](src/examples/demo).

In particular, the [`queueing`](src/examples/demo/queueing) project features one well-commented example of each of the four types of nodes:

- [`queueing_node.h`](src/examples/demo/queueing/queueing_node.h) (atomic)
- [`two_stage_queueing_node.h`](src/examples/demo/queueing/two_stage_queueing_node.h) (composite)
- [`parallel_queueing_node.h`](src/examples/demo/queueing/parallel_queueing_node.h) (collection)
- [`plus_node.h`](src/examples/demo/queueing/plus_node.h) (function)

## Contributing

Review the [contributing guidelines](CONTRIBUTING.md) before you consider working on SyDEVS and proposing contributions.

## License

SPDX-License-Identifier: [Apache-2.0](LICENSE.md)
