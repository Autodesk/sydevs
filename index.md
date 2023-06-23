The SyDEVS open source C++ library provides a framework for modeling and simulating complex systems. 

**In a nutshell, it will help make your simulation code scale.**

SyDEVS allows physics solvers, state machines, and other types of simulation models to be implemented as independent nodes and later integrated. Even nodes that use different time steps (or variable time steps) can be linked together and allowed to interact.

Below is an example of a multi-solver simulation developed by Rutgers University, York University, and Autodesk Research using SyDEVS as the underlying framework. Click the image to see the animated results on YouTube.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=UvEn9yLMi9Y
" target="_blank"><img src="http://img.youtube.com/vi/UvEn9yLMi9Y/0.jpg" 
alt="SyDEVS in Action" width="480" height="360" border="10" /></a>

Also see [Making Simulation Code Scale with SyDEVS](https://sites.google.com/modelingtalks.org/entry/making-simulation-code-scale-with-sydevs), a tutorial presented by [Autodesk Research](http://autodeskresearch.com/) and hosted by [Alphabet](https://abc.xyz/) as part of their [Modeling Talk Series](https://sites.google.com/modelingtalks.org/entry/home).

The SyDEVS framework is based on the [Symmetric DEVS formalism](https://www.research.autodesk.com/publications/a-symmetric-formalism-for-discrete-event-simulation-with-agents) ([PDF](https://autodesk.github.io/sydevs/doc/downloads/Goldstein__Symmetric_Formalism__2018-08-03_1100.pdf)), a set of conventions combining three modeling paradigms: [discrete event simulation](https://en.wikipedia.org/wiki/Discrete_event_simulation), [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) and [agent-based modeling](https://en.wikipedia.org/wiki/Agent-based_model). These foundations give SyDEVS the generality needed to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

In addition to supporting multiple modeling paradigms, the SyDEVS library provides comprehensive and reusable Modern C++ implementations of multidimensional arrays, Standard International (SI) units, and the multiscale representation of simulated time described in [this freely available paper](http://journals.sagepub.com/eprint/mIKXDU2UtbJUjPZ8kupv/full) from *Simulation*.

## Documentation

- [Overview](overview.html): Briefly introduces SyDEVS.
- [Getting Started](getting_started/): Explains how to use SyDEVS, step by step.
- [User Manual](user_manual/): Describes the main elements of SyDEVS in detail, with examples.
- [API Reference](doc/html/index.html): Documents the C++ classes that make up the library.
- SyDEVS Introduction ([PPTX](doc/downloads/SyDEVS_Introduction.pptx), [PDF](doc/downloads/SyDEVS_Introduction.pdf)): Provides background information and introduces SyDEVS.
- SyDEVS Framework Overview ([PPTX](doc/downloads/SyDEVS_Framework_Overview.pptx), [PDF](doc/downloads/SyDEVS_Framework_Overview.pdf)): Illustrates the main concepts & relates them to code.
- SyDEVS Building7m Tutorial ([PPTX](doc/downloads/SyDEVS_Building7m_Tutorial.pptx), [PDF](doc/downloads/SyDEVS_Building7m_Tutorial.pdf)): Challenges users to learn by enhancing an example.
- Symmetric DEVS Tutorial ([PPTX](doc/downloads/AgentBasedSyDEVS_Tutorial.pptx)): Explains the theory underlying the SyDEVS framework.

## Download

Download the latest SyDEVS release [here](https://github.com/Autodesk/sydevs/releases), or fork, clone, and build the [repo](https://github.com/Autodesk/sydevs).

You will need a C++ compiler that implements the C++14 standard. SyDEVS has been tested successfully with Visual Studio 2015, Visual Studio 2017, and recent versions of GCC and Clang.

SyDEVS is released under the [Apache License 2.0](https://github.com/Autodesk/sydevs/blob/master/LICENSE.md), a permissive open source license.

## Contact

Inquires about SyDEVS can be directed to [Rhys Goldstein](https://research.autodesk.com/people/rhys-goldstein).
