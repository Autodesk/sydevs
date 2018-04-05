---
title: SyDEVS
---

# SyDEVS

Multiscale Simulation and Systems Modeling Library

## About

This library provides a framework for implementating complex systems analysis and simulation code in a modular/hierarchical fashion. It was originally developed to serve as a backend for the visual programming interfaces described by [Maleki et al. (2015)](https://www.autodeskresearch.com/publications/designingdevs), but the same functionality can be achieved without a GUI by defining C++ classes that derive from one of the system node base classes (`atomic_node`, `composite_node`, `collection_node`, `function_node`). The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) as exemplified by Autodesk's [Dynamo](http://dynamobim.org/) tool, and the [DEVS](https://en.wikipedia.org/wiki/DEVS) message-passing paradigm implemented in tools such as DesignDEVS (see [software](http://simaud.com/resources.php#software), [conference paper](https://www.autodeskresearch.com/designdevs), [journal paper](https://www.autodeskresearch.com/publications/practical-aspects-designdevs-simulation-environment)). These foundations give the framework the generality neeeded to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

## Building

#### Building on Windows (Visual Studio)
* You will need [CMake](http://www.cmake.org/)
* Make sure you have [Visual Studio 2015](https://www.visualstudio.com) or [Visual Studio 2017](https://www.visualstudio.com)
* `git clone https://github.com/Autodesk/sydevs sydevs`
* `cd sydevs`
* `mkdir bin`
* `cd bin/`
* `cmake -G "Visual Studio 14 2015 Win64" ..` or `cmake -G "Visual Studio 15 2017 Win64" ..`
* Open `bin/SyDEVS.sln` in Visual Studio
* Recommendation: In `Tools->Options->Text Editor->C/C++->Tabs`, select `Insert spaces` with a `Tab size` and `Indent size` of `4`

#### Building on Mac OS
* You will need [CMake](http://www.cmake.org/)
* `git clone https://github.com/Autodesk/sydevs sydevs`
* `cd sydevs`
* `mkdir bin`
* `cd bin/`
* `cmake -G "Xcode" ..`
* `xcodebuild -project SyDEVS.xcodeproj` or open the `.xcodeproj` file.

#### Testing

Unit and regression tests are run automatically during the build process.

The unit testing framework used in SyDEVS is [Catch2](https://github.com/catchorg/Catch2). See the [documentation](https://github.com/catchorg/Catch2/tree/master/docs) for a tutorial and reference material. Catch2 is released under the Boost Software License 1.0.

## Documentation

#### PowerPoint

There are two PowerPoint documents located in the `doc` folder:

- [SyDEVS_Framework.pptx](doc/SyDEVS_Framework.pptx): An overview of the systems modeling framework and related snippets of code.
- [SyDEVS_Tutorial.pptx](doc/SyDEVS_Tutorial.pptx): A tutorial that challenges developers to enhance the building7m example.


## License

[Apache-2.0](LICENSE.md)