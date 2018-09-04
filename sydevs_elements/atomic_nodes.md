# Atomic Nodes

The ***atomic node*** provides a general structure for representing behavior that unfolds over simulated time. The modeler must implement four event handers that are invoked by the simulator. The initialization event hander is invoked first. The unplanned and planned event handlers are then called any number of times over the course of the simulation in response to incoming messages (unplanned events) or internally scheduled events (planned events). The finalization event handler is invoked at the end.

![Atomic Node](../doc/images/sydevs_atomic_node.png "SyDEVS atomic node")

The atomic node source code can be found in [atomic_node.h](https://github.com/Autodesk/sydevs/blob/master/src/sydevs/systems/atomic_node.h).

## Example

An example of an atomic node is the `queueing_node` class found in [queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h).

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

For the complete implementation of this atomic node, see [queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h).

## Constructor/Destructor

The following constructor and destructor declarations should appear for all atomic nodes, except with `queueing_node` replaced with a different node name.

```cpp
    queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~queueing_node() = default;
```

## Attributes

Atomic nodes must override the pure virtual member function `time_precision()`, as in the `queueing_node` example:

```cpp
    virtual scale time_precision() const { return micro; }
```

The function should return one of the following `scale` values:

```cpp
yocto  // scale(-8);
zepto  // scale(-7);
atto   // scale(-6);
femto  // scale(-5);
pico   // scale(-4);
nano   // scale(-3);
micro  // scale(-2);
milli  // scale(-1);
unit   // scale(0);
kilo   // scale(1);
mega   // scale(2);
giga   // scale(3);
tera   // scale(4);
peta   // scale(5);
exa    // scale(6);
zetta  // scale(7);
yotta  // scale(8);
```

This is the precision level to which all elapsed durations will be rounded, and to which all planned duration must be rounded. For example, if an atomic node has a time precision of microseconds (`micro`), and if any of the atomic node's event handlers returns a more fine-grained planned duration of 17 nanoseconds,(`17_ns`), an exception will be thrown.

## Ports

Ports are the primary means by which a SyDEVS node communicates with external entities. There are four types of ports: flow input, message input, message output, and flow output. The [queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h) example has one port of each type.

```cpp
    port<flow, input, duration> serv_dt_input;    // service duration
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration
```

It is possible for a node to have more than one port of the same type, and no ports of a different type. For example, the [building_dynamics_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/building_dynamics_node.h) has 5 flow input ports, 2 message output ports, and no ports of the other two types.

```cpp
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, float64> wall_resistance_input;
    port<flow, input, array1d<int64>> initial_position_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<message, output, array2d<thermodynamic_temperature>> temperature_field_output;
    port<message, output, array1d<int64>> occupant_position_output;
```

Note that every port has a type, which must be one of the following core types:

```cpp
bool         // a boolean, `true` or `false`
int64        // a 64-bit signed integer
float64      // a 64-bit floating-point number (`double`)
std::string  // the standard string type

quantity<U>        // a base-1000 amount of any SI unit `U`
arraynd<T, ndims>  // a multi-dimensional array of type `T` and rank `ndims`

std::pair<T1, T2>  // a standard pair of values of types `T1` and `T2`
std::tuple<...>    // a standard tuple of values of the specified types
std::vector<T>     // a standard vector of type `T`
std::set<T>        // a standard set of type `T`
std::map<T>        // a standard map of type `T`

std::shared_ptr<T>  // a standard shared pointer of type `T`
pointer             // an alias for `std::shared_ptr<void>`
```

The first 4 core types are primitive/standard C++ types.

More information on the SyDEVS `quantity` class can be found [here](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1quantity.html) (see Detailed Description).

More information on the SyDEVS `arraynd` class can be found [here](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1arraynd.html) (see Detailed Description).

Some of the core types include a template parameter specifying another type (`T`, `T1`, `T2`). These types must also be core types, with one exception. The exception is `std::shared_ptr<T>`, for which `T` can be any C++ type. Thus one can use `std::shared_ptr<T>` to pass user-defined classes or any type not in the list. However, there is a cost to this in terms of safety, since the same data may end up being referenced by multiple nodes.

## State Variables

Atomic node state variables are just regular C++ member variables. Whenever appropriate, the SyDEVS convention is to assign these variables an initial value in the `initialization_event` event handler instead of the constructor.

## Event Handles

Atomcic nodes must override the four pure virtual member functions representing each of the four types of event handlers. These functions are declared as follows:

```cpp
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
```



