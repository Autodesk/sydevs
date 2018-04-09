# SyDEVS

Multiscale Simulation and Systems Modeling Library

## About

This library provides a framework for implementating complex systems analysis and simulation code in a modular/hierarchical fashion. It was originally developed to serve as a backend for the visual programming interfaces described by [Maleki et al. (2015)](https://www.autodeskresearch.com/publications/designingdevs), but the same functionality can be achieved without a GUI by defining C++ classes that derive from one of the system node base classes (`atomic_node`, `composite_node`, `collection_node`, `function_node`). The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) as exemplified by Autodesk's [Dynamo](http://dynamobim.org/) tool, and the [DEVS](https://en.wikipedia.org/wiki/DEVS) message-passing paradigm implemented in tools such as DesignDEVS (see [software](http://simaud.com/resources.php#software), [conference paper](https://www.autodeskresearch.com/designdevs), [journal paper](https://www.autodeskresearch.com/publications/practical-aspects-designdevs-simulation-environment)). These foundations give the framework the generality neeeded to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

## Documentation

- [API Reference](doc/html/index.html): Doxygen generated documentation files.
- [SyDEVS Framework Overview](doc/SyDEVS_Framework.pdf): An overview of the systems modeling framework and related snippets of code.
- [SyDEVS Tutorial](doc/SyDEVS_Tutorial.pdf): A tutorial that challenges developers to enhance the building7m example.


## License

[Apache-2.0](LICENSE.md)