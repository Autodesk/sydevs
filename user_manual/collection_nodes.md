| [Prev](composite_nodes.html) | [User Manual -- Table of Contents](index.html) | [Next](simulations.html) |
# Collection Nodes

The ***collection node*** contains an arbitrary number of agents, which are instances of the same type of node. This contrasts with the composite node, which contains a fixed number of instances of different types of nodes. To define a collection node, the modeler must specify the agent type and implement handlers for five types of events. Four of these event types are similar to those of the atomic node, but are prefixed with "macro": the macro initialization event, the macro unplanned event, the macro planned event, and the macro finalization event. The fifth type of event is the micro planned event. The collection node has a variety of uses, but its primary purpose is to support agent-based modeling.

![Collection Node](../doc/images/sydevs_collection_node.png "SyDEVS collection node")

The collection node base class is defined in [collection_node.h](https://github.com/Autodesk/sydevs/blob/master/src/sydevs/systems/collection_node.h), which must be included by all collection nodes.

```cpp
#include <sydevs/systems/collection_node.h>
```

## Sample Declarations

An example of a collection node is the `parallel_queueing_node` class found in [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h), which is part of the [queueing](https://github.com/Autodesk/sydevs/tree/master/src/examples/demo/queueing) demonstration project. Below is the class declaration, which inherits from `collection_node`.

```cpp
/**
 * This node initially directs incoming jobs to a single queue, but more queues
 * are added in parallel when needed. Normally, a job will go into the first
 * queue with fewer than the maximum number of jobs. But if a job is received
 * while the current queues are all full, then a new queue is created. The
 * variable number of queues are modeled as agents in a collection. The idle
 * duration of this collection node is the sum of the idle durations of all
 * queueing node agents.
 */
class parallel_queueing_node : public collection_node<int64, queueing_node>
{
public:
    // Constructor/Destructor:
    parallel_queueing_node(const std::string& node_name, const node_context& external_context);
    virtual ~parallel_queueing_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> serv_dt_input;    // service duration
    port<flow, input, int64> max_n_input;         // maximum number of jobs in a queue
    port<message, input, int64> job_id_input;     // job ID (input)
    port<message, output, int64> job_id_output;   // job ID (output)
    port<flow, output, duration> idle_dt_output;  // idle duration

protected:
    // State Variables:
    duration serv_dt;      // service duration (constant)
    int64 max_n;           // maximum number of jobs in a queue (constant)
    std::vector<int64> N;  // number of jobs in each queue

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};
```

## Inheritance Declarations

Unlike atomic, function, and composite nodes, which inherit directly from their respective base classes, collection nodes must provide types for the template parameters `AgentID` and `Node`. The `AgentID` template parameter is the type used internally to identify each agent. The `Node` template parameter is the type of each agent, or the type that each agent class must inherit from.

Below is the inheritance declaration from [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h). Agents are identified by 64-bit integers. Each agent is an instance of `queueing_node`. In this case the agent type is an atomic node, but agents can also be composite nodes or other collection nodes.

```cpp
class parallel_queueing_node : public collection_node<int64, queueing_node>
```

Agents can also be function nodes, in which case the collection node works differently from how it is described in this page (see the [collection node reference](https://autodesk.github.io/sydevs/doc/html/classsydevs_1_1systems_1_1collection__node.html#details) and the example in [lower_cellular_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/test_systems/cellular/lower_cellular_node.h)). For all intents and purposes, such a collection node will behave like a function node.

## Constructor/Destructor Declarations

Constructors and destructor declarations are the same for all types of nodes (see [Atomic Nodes -- Constructor/Destructor Declarations](atomic_nodes.html#constructordestructor-declarations)).

## Ports

Ports are the same as for atomic and composite nodes (see [Atomic Nodes -- Ports](atomic_nodes.html#ports)).

## State Variables

State variables are the same as for atomic nodes (see [Atomic Nodes -- State Variables](atomic_nodes.html#state-variables)).


## Constructor Definitions

Similar to the atomic node (see [Atomic Nodes -- Constructor Definitions](atomic_nodes.html#constructor-definitions)), the constructor of a collection node is generally used only to initialize the base class (`collection_node`) and the ports.

Below is the constructor definition for [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h).

```cpp
inline parallel_queueing_node::parallel_queueing_node(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, queueing_node>(node_name, external_context)
    , serv_dt_input("serv_dt_input", external_interface())
    , max_n_input("max_n_input", external_interface())
    , job_id_input("job_id_input", external_interface())
    , job_id_output("job_id_output", external_interface())
    , idle_dt_output("idle_dt_output", external_interface())
{
}

```

## Event Handlers

Collection nodes must override the five pure virtual member functions representing each of the five types of event handlers. These functions are declared as follows:

```cpp
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
```

### Macro Initialization Events

The `macro_initialization_event` function is invoked once, before any other event handling function. The main purpose of the function is to take parameter values from the flow input ports, and use the information to initialize the state variables and create agents. The return value is the planned duration until the next planned event.

Below is the macro initialization event handler from [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h). Observe the creation of an agent, an instance of `queueing_node`, with an ID of 0.

```cpp
inline duration parallel_queueing_node::macro_initialization_event()
{
    // Initialize the state variables.
    serv_dt = serv_dt_input.value().fixed_at(time_precision());  
    max_n = max_n_input.value();
    N = std::vector<int64>();

    // Add the first queue.
    get(prototype.serv_dt_input) = serv_dt;  // Set the queueing node flow input value.
    create_agent(0);                            // Create the queueing node agent.
    N.push_back(0);                             // Record that the queueing node has no jobs.

    // Wait indefinitely.
    return duration::inf();
}
```

Creating an agent involves two steps. First, the flow input ports are supplied to a pre-defined instance named `prototype`, which can be regarded as a proxy agent that doesn't directly take part in the simulation. The flow input values are supplied by applying the `get` function to the prototype's ports, and assigning directly to the result. The second step is to call `create_agent` with the ID of the new agent as an argument. An agent with the specified ID is then created using the values assigned to the prototype's flow input ports. This triggers an initialization event in the new agent node.

It is possible to create multiple agents in this fashion. In that case, each of the prototype's flow input ports can be left unchanged if the agents have the same parameter value, or the flow input ports can be assigned new values if the agents have different parameters. It is also possible to send messages to agents or remove agents during the macro initialization event. These operations are demonstrated below for other types of event handlers.

### Macro Unplanned Events

The `macro_unplanned_event` function is invoked every time a message is received. The simulator provides the elapsed duration `elapsed_dt` since the previous event of any type. The return value is the planned duration until the next planned event.

Inside the function, there is usually an `if`...`else if` structure that checks which message input port is the one that received the message. The message value is then extracted from that port. These operations make use of the bool-valued port member function `received`, and the port member function `value`.

Below is the unplanned event handler from [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h). Observe that in certain cases, a new agent is created using the `create_agent` function. Also observe that near the bottom of the event handler, a message is sent to an agent.

```cpp
inline duration parallel_queueing_node::macro_unplanned_event(duration elapsed_dt)
{
    // Handle the message input.
    if (job_id_input.received()) {
        // Get the received job ID
        int64 job_id = job_id_input.value();

        // Find the ID of the first queue with space for the job.
        int64 agent_id = 0;
        auto found = false;
        while (!found && agent_id < int64(N.size())) {
            if (N[agent_id] < max_n) {
                found = true;
            }
            else {
                ++agent_id;
            }
        }

        // Add a new queue if needed and update the number of jobs in each queue.
        if (agent_id == int64(N.size())) {
            get(prototype.serv_dt_input) = serv_dt;  // Set the queueing node flow input value.
            create_agent(agent_id);                     // Create the queueing node agent.
            N.push_back(1);                             // Record that the queueing node has one job.
        }
        else {
            ++N[agent_id];  // Record that the selected queueing node has one additional job.
        }

        // Send the received job ID to the selected queueing node.
        get(prototype.job_id_input) = job_id;
        affect_agent(agent_id);
    }

    // Wait indefinitely.
    return duration::inf();
}
```

Sending a message to an agent involves two steps. First, the message input ports are supplied to the prototype using the `get` function. Only a single message input port should be assigned in this manner. The second step is to call `affect_agent` with the ID as an argument. The message supplied to the prototype is then transferred to the identified agent, triggering an unplanned event in that agent. The prototype's message input port is then reset, allowing new messages to be sent using either the same port or a different message input port.

As with all event handlers, it is possible create, send messages to, or remove agents, as many times as desired. Agent removal is demonstrated in the example of the macro finalization event handler.

In the event that a macro unplanned event is scheduled to occur at the same time as a micro or macro planned event, the macro unplanned event takes priority.

### Micro Planned Events

The `micro_planned_event` function is triggered by a message output value sent by one of the collection node's agents. It is the only event triggered by an agent ("micro" refers to the agent level; "macro" refers to the level of the encompassing collection node). It is invoked automatically with the ID of the agent that sent the message (`agent_id`), and the elapsed duration since the previous event. The micro planned event handler can send messages out of the collection node's message output ports. It returns the planned duration until the next planned event.

Similar to macro unplanned events, there is usually an `if`...`else if` structure that checks which message input port is the one that received the message. The message value is then extracted from the corresponding port of the prototype instance. These operations make use of the bool-valued function `transmitted`, and the function `get`.

Below is the micro planned event handler from [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h). Observe the use of the port member function `send` to send a message out of the collection node.

```cpp
inline duration parallel_queueing_node::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    // Handle the message output transmitted from the queueing node.
    if (transmitted(prototype.job_id_output)) {
        // Get the transmitted job ID.
        int64 job_id = get(prototype.job_id_output);

        // Send the job ID as a message output.
        job_id_output.send(job_id);

        // Record that the transmitting queueing node has one fewer job.
        --N[agent_id];
    }

    // Wait indefinitely.
    return duration::inf();
}
```

Micro planned events can create, send messages to, or remove agents.

### Macro Planned Events

The `macro_planned_event` function is invoked whenever the planned duration elapses without being interrupted by either a macro unplanned event or a micro planned event. Like its unplanned counterpart, the macro planned event handler is invoked with the elapsed duration `elapsed_dt`. The macro planned event handler can send messages out of the collection node's message output ports. It returns the planned duration until the next planned event.

Below is the macro planned event handler from [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h). In this example, the function will never be invoked because the planned durations returned by other event handlers are all infinity. Therefore, the function simply returns an invalid duration.

```cpp
inline duration parallel_queueing_node::macro_planned_event(duration elapsed_dt)
{
    // This function should never be called, so trigger an error by returning
    // an invalid planned duration.
    return duration();
}
```

Macro planned events can create, send messages to, or remove agents.

### Macro Finalization Events

The `macro_finalization_event` function is invoked once, after all other events. The main purpose of the function is to assign statistic values to all the flow output ports. These statistic values sometimes depend on the elapsed duration, which is available as an argument. The values may also depend on the statistics of the agents, which can be obtained by removing the agents.

Below is the finalization event handler from [queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h). Observe that all agents are removed in this example.

```cpp
inline void parallel_queueing_node::macro_finalization_event(duration elapsed_dt)
{
    // Add the idle times of all queueing nodes, and assign the result to the
    // flow output port.
    auto idle_dt = 0_s;
    while (agent_count() > 0) {
        int64 agent_id = *agent_begin();
        remove_agent(agent_id);
        idle_dt += get(prototype.idle_dt_output);
    }
    idle_dt_output.assign(idle_dt);
}
```

Removing an agent in a macro finalization event is only necessary if the agent's flow output port values need to be accessed. All unremoved agent node instances will be destructed regardless soon after the finalization event completes. Agents are removed by applying the `remove_agent` function to the ID of the agent. It is subsequently possible to obtain the removed agent's flow output values by applying the `get` function to each of the flow output ports on the prototype.

Macro finalization events can create and send messages to agents, if needed.

## Agent Subtypes

When creating an agent, it is possible to supply a template type to the `create_agent` function. The supplied type must be a node that inherits from the agent type associated with the collection node.

This approach is called agent subtyping, and it allows a single collection node to support a heterogeneous set of agents. The restriction is that all agent types must inherit from the same agent base class.

An example of a collection node with agent subtyping can be found in [subtyping_closed_system.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/research/subtyping/subtyping_closed_system.h). Below is the macro initialization event where `create_agent` is invoked with different derived types.

```cpp
inline duration subtyping_closed_system::macro_initialization_event()
{
    int64 agent_id = 0;
    for (float64 y0 = 0.0; y0 < 5.0; ++y0) {
        get(prototype.y0_input) = y0;
        create_agent<agent_derived_A_node>(agent_id);
        ++agent_id;
        create_agent<agent_derived_B_node>(agent_id);
        ++agent_id;
    }
    return 5_s;
}
```

| [***Continue to Simulations***](simulations.html) |