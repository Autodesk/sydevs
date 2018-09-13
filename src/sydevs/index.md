API Reference Overview {#mainpage}
======================

About SyDEVS
------------

This library provides a framework for implementing complex systems analysis and simulation code in a modular/hierarchical fashion. The framework combines three modeling paradigms: [discrete event simulation](https://en.wikipedia.org/wiki/Discrete_event_simulation), [agent-based modeling](https://en.wikipedia.org/wiki/Agent-based_model), and [dataflow programming](https://en.wikipedia.org/wiki/Dataflow_programming). The discrete event simulation aspect is based on [DEVS](https://en.wikipedia.org/wiki/DEVS), a well-regarded modeling formalism involving two types of models that correspond with the library's atomic and composite nodes. The library also includes collection nodes, which extend DEVS with agent-based modeling capabilities, and function nodes, which simply compute outputs from inputs. The atomic, composite, collection, and function nodes can communicate through flow ports as part of a dataflow programming network. With the exception of function nodes, they can also communicate in a DEVS-like fashion through message ports.

In addition to supporting multiple modeling paradigms, the SyDEVS library provides comprehensive and reusable Modern C++ implementations of multidimensional arrays, Standard International (SI) units, a multiscale time representation, and other technical computing elements.

The main SyDEVS website is at [https://autodesk.github.io/sydevs](https://autodesk.github.io/sydevs).

The SyDEVS repository is at [https://github.com/Autodesk/sydevs](https://github.com/Autodesk/sydevs).

Main Classes
------------

Namespace: [sydevs](@ref sydevs)

- Core Classes ([sydevs](@ref src/sydevs)/[core](@ref src/sydevs/core)) - generic classes for a variety of applications
  * [scale](@ref sydevs::scale) ([scale.h](@ref sydevs/core/scale.h)) - dimensionless power of 1000
    * [number_types.h](@ref sydevs/core/number_types.h) - related header file with number type aliases, pi constant
  * [quantity](@ref sydevs::quantity) ([quantity.h](@ref sydevs/core/quantity.h)) - Standard International (SI) quantity (e.g. mass, acceleration)
    * [units](@ref sydevs::units) ([units.h](@ref sydevs/core/units.h)) - related template for SI units (e.g. grams, meters/second^2)
  * [identity](@ref sydevs::identity) ([identity.h](@ref sydevs/core/identity.h)) - dimensioned integer for identifying items
  * [arraynd](@ref sydevs::arraynd) ([arraynd.h](@ref sydevs/core/arraynd.h)) - multidimensional array
    * [range](@ref sydevs::range) ([range.h](@ref sydevs/core/range.h)) - related class for representing ranges of array indices
  * [qualified_type](@ref sydevs::qualified_type) ([qualified_type.h](@ref sydevs/core/qualified_type.h)) - traits for types exchanged between system nodes
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
    * [interactive_system](@ref sydevs::systems::interactive_system) ([interactive_system.h](@ref sydevs/systems/interactive_system.h)) - derived from [collection_node](@ref sydevs::systems::collection_node), supports real time simulations
    * port<[data_mode](@ref sydevs::systems::data_mode), [data_goal](@ref sydevs::systems::data_goal)> ([port.h](@ref sydevs/systems/port.h)) - related classes for node ports
  * [simulation](@ref sydevs::systems::simulation) ([simulation.h](@ref sydevs/systems/simulation.h)) - template for simulations based on a port-free [system_node](@ref sydevs::systems::system_node)
    * [real_time_simulation](@ref sydevs::systems::real_time_simulation) ([real_time_simulation.h](@ref sydevs/systems/real_time_simulation.h)) - derived from [simulation](@ref sydevs::systems::simulation), supports real time simulations
      * [real_time_buffer](@ref sydevs::systems::real_time_buffer) ([real_time_buffer.h](@ref sydevs/systems/real_time_buffer.h)) - related class for linking simulated time with wall clock time
    * [discrete_event_time](@ref sydevs::systems::discrete_event_time) ([discrete_event_time.h](@ref sydevs/systems/discrete_event_time.h)) - related class for tracking progress through a simulation
