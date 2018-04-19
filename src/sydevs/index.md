API Reference Overview {#mainpage}
======================

About SyDEVS
------------

This library provides a framework for implementating complex systems analysis and simulation code in a modular/hierarchical fashion. It was originally developed to serve as a backend for the visual programming interfaces described by [Maleki et al. (2015)](https://www.autodeskresearch.com/publications/designingdevs), but the same functionality can be achieved without a GUI by defining C++ classes that derive from one of the system node base classes ([atomic_node](@ref sydevs::systems::atomic_node), [composite_node](@ref sydevs::systems::composite_node), [collection_node](@ref sydevs::systems::collection_node), [function_node](@ref sydevs::systems::function_node)). The framework combines two programming paradigms: [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming) as exemplified by Autodesk's [Dynamo](http://dynamobim.org/) tool, and the [DEVS](https://en.wikipedia.org/wiki/DEVS) message-passing paradigm implemented in tools such as DesignDEVS (see [software](http://simaud.com/resources.php#software), [conference paper](https://www.autodeskresearch.com/designdevs), [journal paper](https://www.autodeskresearch.com/publications/practical-aspects-designdevs-simulation-environment)). These foundations give the framework the generality neeeded to support essentially any type of simulation, regardless of domain, time scale, or time advancement scheme.

Main Classes
------------

Namespace: [sydevs](@ref sydevs)

- Core Classes ([sydevs](@ref src/sydevs)/[core](@ref src/sydevs/core)) - generic classes for a variety of applications
  * [scale](@ref sydevs::scale) ([scale.h](@ref sydevs/core/scale.h)) - dimensionless power of 1000
    * [number_types.h](@ref sydevs/core/number_types.h) - related header file with number type aliases, pi constant
  * [quantity](@ref sydevs::quantity) ([quantity.h](@ref sydevs/core/quantity.h)) - Standard International (SI) quantity (e.g. mass, acceleration)
    * [units](@ref sydevs::units) ([units.h](@ref sydevs/core/units.h)) - related template for SI units (e.g. grams, meters/second^2)
  * [arraynd](@ref sydevs::arraynd) ([arraynd.h](@ref sydevs/core/arraynd.h)) - multidimensional array
    * [range](@ref sydevs::range) ([range.h](@ref sydevs/core/range.h)) - related class representing range of array indices
  * [core_type](@ref sydevs::core_type) ([core_types.h](@ref sydevs/core/core_types.h)) - traits for types exchanged between system nodes
    * [pointer](@ref sydevs::pointer) ([pointer.h](@ref sydevs/core/pointer.h)) - related class for pointers to any type of data
    * [string_builder](@ref sydevs::string_builder) ([string_builder.h](@ref sydevs/core/string_builder.h)) - related class for value-to-string conversion

- Time Classes ([sydevs](@ref src/sydevs)/[time](@ref src/sydevs/time)) - multiscale time representation
  * [time_point](@ref sydevs::time_point) ([time_point.h](@ref sydevs/time/time_point.h)) - arbitrary-precision point in time
  * [time_sequence](@ref sydevs::time_sequence) ([time_sequence.h](@ref sydevs/time/time_sequence.h)) - sequence of increasing time points
  * [time_queue](@ref sydevs::time_queue) ([time_queue.h](@ref sydevs/time/time_queue.h)) - data structure tracking future event times
  * [time_cache](@ref sydevs::time_cache) ([time_cache.h](@ref sydevs/time/time_cache.h)) - data structure tracking past event times

- Systems Classes ([sydevs](@ref src/sydevs)/[systems](@ref src/sydevs/systems)) - dataflow + message-passing networks
  * [system_node](@ref sydevs::systems::system_node) ([system_node.h](@ref sydevs/systems/system_node.h)) - base class for all nodes
    * [atomic_node](@ref sydevs::systems::atomic_node) ([atomic_node.h](@ref sydevs/systems/atomic_node.h)) - derived from [system_node](@ref sydevs::systems::system_node), supports event handlers
    * [composite_node](@ref sydevs::systems::composite_node) ([composite_node.h](@ref sydevs/systems/composite_node.h)) - derived from [system_node](@ref sydevs::systems::system_node), supports fixed-structure compositions
    * [collection_node](@ref sydevs::systems::collection_node) ([collection_node.h](@ref sydevs/systems/collection_node.h)) - derived from [system_node](@ref sydevs::systems::system_node), supports variable-length collections
    * [function_node](@ref sydevs::systems::function_node) ([function_node.h](@ref sydevs/systems/function_node.h)) - derived from [system_node](@ref sydevs::systems::system_node), supports functions
      * [parameter_node](@ref sydevs::systems::parameter_node) ([parameter_node.h](@ref sydevs/systems/parameter_node.h)) - derived from [function_node](@ref sydevs::systems::function_node), handles parameter values
      * [statistic_node](@ref sydevs::systems::statistic_node) ([statistic_node.h](@ref sydevs/systems/statistic_node.h)) - derived from [function_node](@ref sydevs::systems::function_node), handles statistic values
    * port<[data_mode](@ref sydevs::systems::data_mode), [data_goal](@ref sydevs::systems::data_goal)> ([port.h](@ref sydevs/systems/port.h)) - related classes for node ports
  * [simulation](@ref sydevs::systems::simulation) ([simulation.h](@ref sydevs/systems/simulation.h)) - template for simulations based on a port-free [system_node](@ref sydevs::systems::system_node)
    * [discrete_event_time](@ref sydevs::systems::discrete_event_time) ([discrete_event_time.h](@ref sydevs/systems/discrete_event_time.h)) - related class tracking progress through a simulation
