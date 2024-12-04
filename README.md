# SyDEVS

The SyDEVS C++ library provides a framework for implementing complex systems simulation code in a modular and hierarchical fashion.

The framework combines three modeling paradigms: [discrete event simulation](https://en.wikipedia.org/wiki/Discrete_event_simulation), [agent-based modeling](https://en.wikipedia.org/wiki/Agent-based_model), and [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming). The discrete event simulation aspect is based on [DEVS](https://en.wikipedia.org/wiki/DEVS), a well-regarded modeling formalism involving two types of models that correspond with the ***atomic*** and ***composite*** nodes of SyDEVS. SyDEVS also includes ***collection*** nodes, which extend DEVS with agent-based modeling capabilities, and ***function*** nodes, for dataflow programming. Atomic, composite, collection, and function nodes communicate through flow ports as part of a dataflow programming network. Atomic, composite, and collection nodes also communicate in a DEVS-like fashion through message ports.

In addition to supporting multiple modeling paradigms, the SyDEVS library provides comprehensive and reusable Modern C++ implementations of multidimensional arrays, Standard International (SI) units, a multiscale time representation, and other technical computing elements.

The main SyDEVS website is at [https://autodesk.github.io/sydevs](https://autodesk.github.io/sydevs).

The SyDEVS API reference is at [https://autodesk.github.io/sydevs/doc/html](https://autodesk.github.io/sydevs/doc/html).

## Instructions

### Using SyDEVS

The recommended way to use SyDEVS in a simulation project is to include the [latest release](https://github.com/Autodesk/sydevs/releases) as an external library, as described in the [Getting Started](https://autodesk.github.io/sydevs/getting_started/) tutorial.

### Building SyDEVS using MSVC on Windows

To build the SyDEVS examples and tests using [CMake](http://www.cmake.org/) and [Visual Studio 2022](https://www.visualstudio.com), follow the instructions below. 

* Open a command prompt and navigate to the `sydevs` project directory.
* `mkdir build`
* `cd build/`
* `cmake -G "Visual Studio 17 2022" -A x64 ..`
* Open `build/SyDEVS.sln` in Visual Studio
* Recommendation: In `Tools -> Options -> Text Editor -> C/C++ -> Tabs`, select `Insert spaces` with a `Tab size` and `Indent size` of `4`

### Building SyDEVS using GCC on Windows

To build the SyDEVS examples and tests using [CMake](http://www.cmake.org/) and [TDM-GCC](https://jmeubank.github.io/tdm-gcc/), follow the instructions below. 

* Install the 64+32-bit MinGW-w64 edition of TDM-GCC.
* Open a command prompt and navigate to the `sydevs` project directory.
* `mkdir bin`
* `cd bin/`
* `cmake -G "MinGW Makefiles" ..`
* `mingw32-make`

### Testing SyDEVS

Unit and regression tests are run automatically during the build process.

The unit testing framework used in SyDEVS is [Catch2](https://github.com/catchorg/Catch2). See the [documentation](https://github.com/catchorg/Catch2/tree/master/docs) for a tutorial and reference material. Catch2 is released under the Boost Software License 1.0.

Note that the complete set of regression tests will only run if MSVC is used. If another compiler is used, the regression tests involving random distributions will be skipped, since different compilers are expected to generate different random samples.

### Generating the API documentation

To build or update the documentation files with [Doxygen](https://www.doxygen.nl/), make sure you are in the top level directory (where the `doxygen.config` is) and execute the following command:

- `doxygen doxygen.config`

This will build the documentation in the [`doc`](doc) directory. To open the documentation:

- `cd docs/html`
- Open `index.html` in your web browser.

You should update the documentation on a regular basis to keep it in sync with the code.

## Documentation

### Website

The [Main SyDEVS Website](https://autodesk.github.io/sydevs) includes an [Overview](https://autodesk.github.io/sydevs/overview.html), a [Getting Started](https://autodesk.github.io/sydevs/getting_started/) tutorial, a [User Manual](https://autodesk.github.io/sydevs/user_manual/), and the [API Reference](https://autodesk.github.io/sydevs/doc/html/) for the latest release.

### PowerPoint

There are three PowerPoint documents located in the [`doc`](doc) folder:

- [`SyDEVS_Introduction.pptx`](doc/SyDEVS_Introduction.pptx): An introduction to the theory, paradigm, and code associated with SyDEVS.
- [`SyDEVS_Framework_Overview.pptx`](doc/SyDEVS_Framework_Overview.pptx): An overview of the systems modeling framework and related snippets of code.
- [`SyDEVS_Building7m_Tutorial.pptx`](doc/SyDEVS_Building7m_Tutorial.pptx): A tutorial that challenges developers to enhance the [`building7m`](src/examples/demo/building7m) example.

### Examples

Examples of SyDEVS-based simulations are found in the [`src/examples`](src/examples) directory.

The best examples for learning the library are those in [`demo`](src/examples/demo).

In particular, the [`queueing`](src/examples/demo/queueing) project features one well-commented example of each of the four main types of nodes:

- [`queueing_node.h`](src/examples/demo/queueing/queueing_node.h) (atomic)
- [`two_stage_queueing_node.h`](src/examples/demo/queueing/two_stage_queueing_node.h) (composite)
- [`parallel_queueing_node.h`](src/examples/demo/queueing/parallel_queueing_node.h) (collection)
- [`plus_node.h`](src/examples/demo/queueing/plus_node.h) (function)

## Contributing

Review the [contributing guidelines](CONTRIBUTING.md) before you consider working on SyDEVS and proposing contributions.

## License

SPDX-License-Identifier: [Apache-2.0](LICENSE.md)
