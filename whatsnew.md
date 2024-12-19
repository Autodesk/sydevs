# What's New

Latest Developments

## 2024 December 19 -- SyDEVS 0.7 Released

SyDEVS v0.7 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.7).

This release is a major update and cleanup.

## 2023 June 23 -- Links Updated

These documentation pages have been updated to fix broken links and highlight new online resources, including a talk titled [Making Simulation Code Scale with SyDEVS](https://sites.google.com/modelingtalks.org/entry/making-simulation-code-scale-with-sydevs).

## 2021 January 27 -- SyDEVS 0.6.8 Released

SyDEVS v0.6.8 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.8).

This release is a minor update.

## 2020 April 15 -- SyDEVS 0.6.7 Released

SyDEVS v0.6.7 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.7).

This release is a minor update.

## 2019 November 26 -- SyDEVS 0.6.6 Released

SyDEVS v0.6.6 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.6).

This release is a minor update and cleanup, but the [Getting Started](getting_started/) tutorial has been significantly revised. It now recommends that the SyDEVS source code be directly incorporated into new SyDEVS projects, and provides step-by-step instructions on how to do this. This revision will eliminate potential frustrations caused by platform-specific SyDEVS binaries, and make it easier to debug simulation code.

## 2019 July 4 -- SyDEVS 0.6.5 Released

SyDEVS v0.6.5 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.5).

This release enhances SyDEVS with software profiling capabilities. After a simulation is run, it is now possible to see how much time was spent processing events in each node. To see how it works, check out the main simulation function of the [workers](https://github.com/Autodesk/sydevs/tree/master/src/examples/demo/workers/workers.cpp) and [building7m_advanced](https://github.com/Autodesk/sydevs/tree/master/src/examples/research/building7m_advanced/building7m.h) examples.

## 2019 April 29 -- Symmetric DEVS Presented at SpringSim 

A tutorial called *Agent-Based Modeling with Symmetric DEVS* ([Download PPTX](doc/downloads/AgentBasedSyDEVS_Tutorial.pptx)) was presented by Rhys Goldstein (Autodesk Research) at the [2019 Spring Simulation Conference](http://scs.org/springsim/) in Tucson, Arisona. The tutorial features an example of an agent-based model involving workers in a workplace. The complete code for that example can be found [here](https://github.com/Autodesk/sydevs/tree/master/src/examples/demo/workers).

## 2019 April 26 -- SyDEVS 0.6.4 Released

SyDEVS v0.6.4 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.4).

The [building7m_advanced](https://github.com/Autodesk/sydevs/tree/master/src/examples/research/building7m_advanced) acoustics simulation model has been improved, and a new demo example of [workers](https://github.com/Autodesk/sydevs/tree/master/src/examples/demo/workers) in a building has been added.

## 2019 April 8 -- SyDEVS-SteerSuite Integration Presented at SimAUD

The presented paper ([Download PDF](doc/downloads/Schaumann_TowardPlatformOccSim.pdf)) describes early results of a collaboration between Rutgers University, York University, and Autodesk Research. The collaboration aims to establish a platform for simulating activities in buildings by integrating models involving different domains and paradigms. The project combines the SteerSuite crowd simulation tool with a SyDEVS model providing higher-level occupant planning and a simple heat transfer algorithm. David Schaumann (Rutgers) gave the talk at the [SimAUD 2019](http://simaud.com/2019/) symposium in Atlanta, GA.

## 2019 February 25 -- SyDEVS 0.6.3 Released

SyDEVS v0.6.3 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.3).

The [building7m_advanced](https://github.com/Autodesk/sydevs/tree/master/src/examples/research/building7m_advanced) example now includes an acoustics simulation model.

## 2018 December 17 -- SyDEVS 0.6.2 Released

SyDEVS v0.6.2 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.2).

Several compile warnings have been resolved, and several error messages have been improved.

## 2018 December 10 -- Symmetric DEVS Presented at WinterSim

Symmetric DEVS ([Download PDF](doc/downloads/Goldstein__Symmetric_Formalism__2018-08-03_1100.pdf)), the formalism on which SyDEVS is based, was presented for the first time by Rhys Goldstein (Autodesk Research) at the [2018 Winter Simulation Conference](http://meetings2.informs.org/wordpress/wsc2018/) in Gothenburg, Sweden. 

## 2018 November 8 -- SyDEVS 0.6.1 Released

SyDEVS v0.6.1 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6.1).

A new example is included, [building7m_advanced](https://github.com/Autodesk/sydevs/tree/master/src/examples/research/building7m_advanced), a building with multiple occupants who affect the surrounding indoor air temperature.

## 2018 September 14 -- New Website Content

There is now a [User Manual](user_manual/) that provides a detailed descriptions of the main elements of SyDEVS: atomic nodes, function nodes, composite nodes, collection nodes, and simulations.

## 2018 September 13 -- SyDEVS v0.6 Released

SyDEVS v0.6 is available [here](https://github.com/Autodesk/sydevs/releases/tag/v0.6).

With this release, it is possible to turn any C++ type into a "qualified type" that can be passed between SyDEVS nodes. The original C++ types does not have to be modified.

Other recent features include classes to help run SyDEVS simulation in real time, and the ability to handle agents of various subtypes in a single collection node.



