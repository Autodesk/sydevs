#pragma once
#ifndef SYDEVS_SYSTEMS_COLLECTION_NODE_H_
#define SYDEVS_SYSTEMS_COLLECTION_NODE_H_

#include <sydevs/systems/collection_node_base.h>
#include <sydevs/time/time_queue.h>
#include <sydevs/time/time_cache.h>

namespace sydevs {
namespace systems {


// collection node declaration

/**
 * @brief A base class template for all collection nodes.
 *
 * @details
 * The `collection_node` base class template is inherited by classes that
 * represent system behavior using a variable-length collection of nodes of the
 * same type. Collection nodes feature procedurally defined behavior, similar
 * to an `atomic_node`, but can also have component nodes, similar to a
 * `composite_node`. A collection node's component nodes are referred to as
 * agents. Collection nodes thus support agent-based modeling. 
 *
 * The procedurally defined behavior is specified by overriding the following
 * pure virtual member functions:
 *
 * - `macro_initialization_event`, which is invoked during initialization and
 *   returns a planned duration;
 * - `macro_unplanned_event`, which is invoked when a message input is received
 *   and returns a planned duration.
 * - `micro_planned_event`, which is invoked when an agent's previous planned
 *   duration elapses and returns a planned duration;
 * - `macro_planned_event`, which is invoked when the previous planned duration
 *   elapses and returns a planned duration;
 * - `macro_finalization`, which is invoked during finalization.
 *
 * Data is exchanged between the collection node and the agents using a proxy
 * agent node called `prototype` of type `Node`.
 *
 * If agents are message nodes (`prototype.node_dmode() == message`), then
 * interactions with agents are supported by the following member functions:
 *
 * - `create_agent`, which creates and initializes a new agent;
 * - `affect_agent`, which sends a message to an agent;
 * - `remove_agent`, which finalizes and removes an agent.
 *
 * If agents are flow nodes (`prototype.node_dmode() == flow`), then
 * interactions with agents are supported by the following member function:
 *
 * - `invoke_agent`, which creates, invokes and removes a new agent;
 */
template<typename AgentID, typename Node>
class collection_node : public collection_node_base
{
public:
    template<typename T>
    class flow_port_proxy;

    template<typename T>
    class message_port_proxy;

    class const_iterator;

    /**
     * @brief Constructs a `collection_node`.
     * 
     * @details
     * The collection node is created along with its associated
     * `node_interface` object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     *
     * @tparam AgentID The type representing all agent IDs.
     * @tparam Node The type of node representing all agents.
     */
    collection_node(const std::string& node_name, const node_context& external_context);

    virtual ~collection_node() = default;  ///< Destructor

    void print_on_elapsed_duration(bool flag = true) const;  ///< If `flag` is `true`, all elapsed durations are printed for this node.
    void print_on_planned_duration(bool flag = true) const;  ///< If `flag` is `true`, all planned durations are printed for this node.

    const Node prototype;  ///< A node that serves as a proxy for all agent nodes.

    duration handle_initialization_event();                ///< Invoked at the beginning of a simulation; calls `macro_initialization_event`.
    duration handle_unplanned_event(duration elapsed_dt);  ///< Invoked whenever a message is received; calls `macro_unplanned_event`.
    duration handle_planned_event(duration elapsed_dt);    ///< Invoked when the planned duration elapses for either an agent or the overall collection; calls `micro_planned_event` or `macro_planned_event`.
    void handle_finalization_event(duration elapsed_dt);   ///< Invoked at the end of a simulation; calls `macro_finalization_event`.

protected:
    static const int64 macro_event_id = std::numeric_limits<int64>::max();  ///< The index used to schedule macro events; it must not conflict with any of the agent indices.

    template<typename T>
    bool transmitted(const port<message, output, T>& prototype_port);  ///< Returns `true` if a message was transmitted by an agent on the port corresponding to `prototype_port`.

    template<typename T>
    flow_port_proxy<T> access(const port<flow, input, T>& prototype_port);   ///< Returns a proxy of the flow input port `prototype_port`, allowing its value to be modified.
    
    template<typename T>
    message_port_proxy<T> access(const port<message, input, T>& prototype_port);   ///< Returns a proxy of the message input port `prototype_port`, allowing its value to be modified.
    
    template<typename T>
    const T& access(const port<message, output, T>& prototype_port);  ///< Returns the value transmitted by an agent on the message output port corresponding to `prototype_port`.

    template<typename T>
    const T& access(const port<flow, output, T>& prototype_port);  ///< Return the value output by an agent on the flow output port corresponding to `prototype_port`.

    /**
     * @brief Creates a new agent of type AgentNode.
     * 
     * @details
     * The collection node must be a collection of message node agents, not 
     * function node agents. In other words, `prototype.node_dmode()` must be
     * `message`. The created agent is initialized with the flow input values
     * found on the prototype. The prototype's flow input values are left 
     * unchanged.
     *
     * The created agent will be of type AgentNode, which must be the same
     * type as Node or a type derived from Node.
     *
     * @param agent_id The ID of the agent to be created.
     */
    template<typename AgentNode>
    void create_agent(const AgentID& agent_id);

    /**
     * @brief Creates a new agent.
     * 
     * @details
     * The collection node must be a collection of message node agents, not 
     * function node agents. In other words, `prototype.node_dmode()` must be
     * `message`. The created agent is initialized with the flow input values
     * found on the prototype. The prototype's flow input values are left 
     * unchanged.
     *
     * @param agent_id The ID of the agent to be created.
     */
    void create_agent(const AgentID& agent_id);

    /**
     * @brief Sends a message to an agent.
     * 
     * @details
     * The collection node must be a collection of message node agents, not 
     * function node agents. In other words, `prototype.node_dmode()` must be
     * `message`. The message sent to the indicated agent is created by
     * accessing a message input port on the prototype. The prototype's message
     * input port is then reset.
     *
     * @param agent_id The ID of the agent to receive a message.
     */
    void affect_agent(const AgentID& agent_id);

    /**
     * @brief Finalizes and removes an agent.
     * 
     * @details
     * The collection node must be a collection of message node agents, not 
     * function node agents. In other words, `prototype.node_dmode()` must be
     * `message`. The flow output values of the removed agent are copied to the
     * prototype.
     *
     * @param agent_id The ID of the agent to be removed.
     */
    void remove_agent(const AgentID& agent_id);

    /**
     * @brief Creates, invokes, and removes an agent.
     * 
     * @details
     * The collection node must be a collection of function node agents, not 
     * message node agents. In other words, `prototype.node_dmode()` must be
     * `flow`. Before invocation, flow input values are copied from the 
     * prototype to the agent. After invocation, flow output values are copied
     * agent to the prototype.
     *
     * @param agent_id The ID of the agent to be created, invoked, and removed.
     */
    void invoke_agent(const AgentID& agent_id);

    bool agent_exists(const AgentID& agent_id);  ///< Returns `true` if an agent with ID `agent_id` currently exists.
    int64 agent_count();                         ///< Returns the current number of agents
    const_iterator agent_begin();                ///< Returns an iterator pointing to the oldest agent.
    const_iterator agent_end();                  ///< Returns an iterator pointing beyond the newest agent.

private:
    node_structure& internal_structure();
    node_interface& prototype_IO() const;
    timer& prototype_ET() const;
    node_interface& agent_IO(const system_node& agent) const;
    discrete_event_time& event_time();

    template<data_mode dmode, data_goal dgoal, typename T>
    void validate_prototype_port(const port<dmode, dgoal, T>& prototype_port);

    Node& handle_agent_planned_event(int64 agent_index);
    void erase_removed_agents();

    template<typename T>
    std::string agent_name_from_id(qualified_type<T> agent_id_type, const AgentID& agent_id);

    std::string agent_name_from_id(qualified_type<std::string> agent_id_type, const AgentID& agent_id);

    void adopt_component_print_flags(const system_node& node) const;

    virtual duration macro_initialization_event() = 0;
    virtual duration macro_unplanned_event(duration elapsed_dt) = 0;
    virtual duration micro_planned_event(const AgentID& agent_id, duration elapsed_dt) = 0;
    virtual duration macro_planned_event(duration elapsed_dt) = 0;
    virtual void macro_finalization_event(duration elapsed_dt) = 0;

    node_context internal_context_;
    std::map<AgentID, int64> agent_indices_;         // Includes indices for all agents except those marked as removed
    std::map<int64, AgentID> agent_ids_;             // Includes IDs for all agents including those marked as removed
    std::map<int64, std::unique_ptr<Node>> agents_;  // Includes all agents including those marked as removed
    std::set<int64> removed_indices_;                // Includes indices only for agents marked as removed
    time_queue t_queue_;
    time_cache t_cache_;
    bool initialized_;
    bool finalized_;
};


// flow port proxy declaration

template<typename AgentID, typename Node>
template<typename T>
class collection_node<AgentID, Node>::flow_port_proxy
{
friend class collection_node<AgentID, Node>;
public:
    flow_port_proxy(const flow_port_proxy&)       = delete;   ///< No copy constructor
    flow_port_proxy(flow_port_proxy&&)            = default;  ///< Move constructor
    flow_port_proxy& operator=(flow_port_proxy&&) = default;  ///< Move assignment
    virtual ~flow_port_proxy()                    = default;  ///< Destructor

    flow_port_proxy& operator=(const T& rhs);
    flow_port_proxy& operator=(const flow_port_proxy& rhs);  ///< Custom copy assignment
    
private: 
    flow_port_proxy(const port<flow, input, T>& input_port);

    node_interface& external_interface_;
    const int64 port_index_;
};


// message port proxy declaration

template<typename AgentID, typename Node>
template<typename T>
class collection_node<AgentID, Node>::message_port_proxy
{
friend class collection_node<AgentID, Node>;
public:
    message_port_proxy(const message_port_proxy&)       = delete;   ///< No copy constructor
    message_port_proxy(message_port_proxy&&)            = default;  ///< Move constructor
    message_port_proxy& operator=(message_port_proxy&&) = default;  ///< Move assignment
    virtual ~message_port_proxy()                       = default;  ///< Destructor

    message_port_proxy& operator=(const T& rhs);
    message_port_proxy& operator=(const message_port_proxy& rhs);  ///< Custom copy assignment
    
private: 
    message_port_proxy(const port<message, input, T>& input_port);

    node_interface& external_interface_;
    const int64 port_index_;
};


// const iterator declaration

template<typename AgentID, typename Node>
class collection_node<AgentID, Node>::const_iterator : public std::iterator<std::bidirectional_iterator_tag, AgentID>
{
friend class collection_node<AgentID, Node>;
public:
    const AgentID& operator*() const;
    const AgentID* operator->() const;
    
    const_iterator& operator++();
    const_iterator operator++(int);
    const_iterator& operator--();
    const_iterator operator--(int);

    bool operator==(const const_iterator& rhs) const;
    bool operator!=(const const_iterator& rhs) const;
 
private:
    const_iterator(const typename std::map<AgentID, int64>::const_iterator& iter);

    typename std::map<AgentID, int64>::const_iterator iter_;
};


// collection node members

template<typename AgentID, typename Node>
inline collection_node<AgentID, Node>::collection_node(const std::string& node_name, const node_context& external_context)
    : collection_node_base(node_name, external_context)
    , prototype("prototype", prototype_context())
    , internal_context_(&const_cast<node_interface&>(external_interface()),
                        const_cast<node_context&>(external_context))
    , agent_indices_()
    , agent_ids_()
    , agents_()
    , removed_indices_()
    , t_queue_()
    , t_cache_()
    , initialized_(false)
    , finalized_(false)
{
    static_assert(qualified_type<AgentID>::valid_and_sortable, "typename AgentID must be a sortable qualified type");
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::print_on_elapsed_duration(bool flag) const
{
    external_IO().print_on_elapsed_duration(flag);
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::print_on_planned_duration(bool flag) const
{
    external_IO().print_on_planned_duration(flag);
}


template<typename AgentID, typename Node>
inline duration collection_node<AgentID, Node>::handle_initialization_event()
{
    if (initialized_) throw std::logic_error("Attempt to initialize collection node (" + full_name() + ") more than once");
    auto& current_t = event_time().t();
    t_queue_ = time_queue(current_t);
    t_cache_ = time_cache(current_t);
    external_IO().print_event("macro-initialization");
    ET().start();
    auto dt = macro_initialization_event();
    ET().stop();
    prototype_IO().clear_flow_inputs();
    prototype_IO().clear_message_input();
    prototype_IO().clear_flow_outputs();
    auto macro_planned_dt = scale_planned_dt(dt);
    if (macro_planned_dt.finite()) {
        t_queue_.plan_event(macro_event_id, macro_planned_dt);
    }
    if (time_precision() != no_scale) {
        t_cache_.retain_event(macro_event_id, time_precision());
    }
    auto planned_dt = t_queue_.imminent_duration();
    external_IO().print_planned_duration(planned_dt);
    initialized_ = true;
    erase_removed_agents();
    return planned_dt;
}


template<typename AgentID, typename Node>
inline duration collection_node<AgentID, Node>::handle_unplanned_event(duration elapsed_dt)
{
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    external_IO().print_event("macro-unplanned");
    external_IO().print_elapsed_duration(elapsed_dt);
    ET().start();
    auto dt = macro_unplanned_event(elapsed_dt);
    ET().stop();
    prototype_IO().clear_flow_inputs();
    prototype_IO().clear_message_input();
    prototype_IO().clear_flow_outputs();
    auto macro_planned_dt = scale_planned_dt(dt);
    if (macro_planned_dt.finite()) {
        t_queue_.plan_event(macro_event_id, macro_planned_dt);
    }
    else {
        t_queue_.cancel_event(macro_event_id);
    }
    if (time_precision() != no_scale) {
        t_cache_.retain_event(macro_event_id, time_precision());
    }
    auto planned_dt = t_queue_.imminent_duration();
    external_IO().print_planned_duration(planned_dt);
    erase_removed_agents();
    return planned_dt;
}


template<typename AgentID, typename Node>
inline duration collection_node<AgentID, Node>::handle_planned_event(duration elapsed_dt)
{
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    if (t_queue_.imminent_duration() != 0_s) throw std::logic_error("Unexpected error while advancing time to that of next planned event");
    int64 agent_index = *std::begin(t_queue_.imminent_event_ids());
    if (agent_index < macro_event_id) {
        // The planned event is triggered by an agent.
        // Execute an agent planned event followed by a micro planned event.
        auto& agent = handle_agent_planned_event(agent_index);
        int64 list_size = agent_IO(agent).message_output_list_size();
        const auto& agent_id = agent_ids_[agent_index];
        auto micro_elapsed_dt = elapsed_dt;
        for (int64 list_index = 0; list_index < list_size; ++list_index) {
            event_time().advance();
            int64 port_index = agent_IO(agent).message_output_index(list_index);
            const auto& val = agent_IO(agent).message_output_value(list_index);
            external_IO().print_event("micro-planned");
            external_IO().print_elapsed_duration(micro_elapsed_dt);
            prototype_IO().append_message_output(port_index, val);
            ET().start();
            auto dt = micro_planned_event(agent_id, micro_elapsed_dt);
            ET().stop();
            prototype_IO().clear_flow_inputs();
            prototype_IO().clear_message_input();
            prototype_IO().clear_message_outputs();
            prototype_IO().clear_flow_outputs();
            auto macro_planned_dt = scale_planned_dt(dt);
            external_IO().print_planned_duration(macro_planned_dt);
            micro_elapsed_dt -= micro_elapsed_dt;
            if (macro_planned_dt.finite()) {
                t_queue_.plan_event(macro_event_id, macro_planned_dt);
            }
            else {
                t_queue_.cancel_event(macro_event_id);
            }
            if (time_precision() != no_scale) {
                t_cache_.retain_event(macro_event_id, time_precision());
            }
        }
        agent_IO(agent).clear_message_outputs();
    }
    else {
        // The planned event is triggered by the collection.
        // Execute a macro planned event.
        external_IO().print_event("macro-planned");
        external_IO().print_elapsed_duration(elapsed_dt);
        ET().start();
        auto dt = macro_planned_event(elapsed_dt);
        ET().stop();
        prototype_IO().clear_flow_inputs();
        prototype_IO().clear_message_input();
        prototype_IO().clear_flow_outputs();
        auto macro_planned_dt = scale_planned_dt(dt);
        external_IO().print_planned_duration(macro_planned_dt);
        if (macro_planned_dt.finite()) {
            t_queue_.plan_event(macro_event_id, macro_planned_dt);
        }
        else {
            t_queue_.pop_imminent_event(macro_event_id);
        }
        if (time_precision() != no_scale) {
            t_cache_.retain_event(macro_event_id, time_precision());
        }
    }
    auto planned_dt = t_queue_.imminent_duration();
    erase_removed_agents();
    return planned_dt;
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::handle_finalization_event(duration elapsed_dt)
{
    if (finalized_) throw std::logic_error("Attempt to finalize collection node (" + full_name() + ") more than once");
    auto& current_t = event_time().t();
    t_queue_.advance_time(current_t);
    t_cache_.advance_time(current_t);
    external_IO().print_event("macro-finalization");
    external_IO().print_elapsed_duration(elapsed_dt);
    ET().start();
    macro_finalization_event(elapsed_dt);
    while (agent_count() > 0) {
        auto agent_id = *agent_begin();
        remove_agent(agent_id);
    }
    ET().stop();
    erase_removed_agents();
    finalized_ = true;
}


template<typename AgentID, typename Node>
template<typename T>
bool collection_node<AgentID, Node>::transmitted(const port<message, output, T>& prototype_port)
{
    validate_prototype_port(prototype_port);
    return prototype_port.port_index() == prototype_IO().message_output_index(0);
}


template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    typename collection_node<AgentID, Node>::flow_port_proxy<T> collection_node<AgentID, Node>::access(const port<flow, input, T>& prototype_port)
#else
    typename collection_node<AgentID, Node>::template flow_port_proxy<T> collection_node<AgentID, Node>::access(const port<flow, input, T>& prototype_port)
#endif
{
    validate_prototype_port(prototype_port);
    return flow_port_proxy<T>(const_cast<port<flow, input, T>&>(prototype_port));
}

    
template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    typename collection_node<AgentID, Node>::message_port_proxy<T> collection_node<AgentID, Node>::access(const port<message, input, T>& prototype_port)
#else
    typename collection_node<AgentID, Node>::template message_port_proxy<T> collection_node<AgentID, Node>::access(const port<message, input, T>& prototype_port)
#endif
{
    validate_prototype_port(prototype_port);
    if (prototype_IO().message_input_port_index() != -1) {
        throw std::logic_error("Attempt to access message input port (" + prototype_port.port_name() + "), " + 
                               "but another message input port of prototype agent (" + prototype.full_name() + ") " +
                               "has already been accessed");
    }
    else {
        prototype_IO().set_message_input(prototype_port.port_index(), pointer());
    }
    return message_port_proxy<T>(const_cast<port<message, input, T>&>(prototype_port));
}

    
template<typename AgentID, typename Node>
template<typename T>
const T& collection_node<AgentID, Node>::access(const port<message, output, T>& prototype_port)
{
    validate_prototype_port(prototype_port);
    if (prototype_port.port_index() != prototype_IO().message_output_index(0)) {
        throw std::logic_error("Attempt to access message output port (" + prototype_port.port_name() + "), " + 
                               "which is not the port of prototype agent (" + prototype.full_name() + ") " +
                               "on which the current message is being transmitted");
    }
    auto& val = prototype_IO().message_output_value(0);
    return const_cast<const T&>(val.template dereference<T>());
}


template<typename AgentID, typename Node>
template<typename T>
const T& collection_node<AgentID, Node>::access(const port<flow, output, T>& prototype_port)
{
    validate_prototype_port(prototype_port);
    auto& val = prototype_IO().flow_output_port_value(prototype_port.port_index());
    return const_cast<const T&>(val.template dereference<T>());
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::create_agent(const AgentID& agent_id)
{
    create_agent<Node>(agent_id);
}


template<typename AgentID, typename Node>
template<typename AgentNode>
inline void collection_node<AgentID, Node>::create_agent(const AgentID& agent_id)
{
    static_assert(std::is_base_of<Node, AgentNode>::value, "AgentNode must inherit from Node");
    if (prototype.node_dmode() == flow) {
        throw std::logic_error("Attempt to use \"create_agent\" to create a flow node agent of a collection node (" + full_name() + "); use \"invoke_agent\" instead");
    }
    event_time().advance();
    auto agent_name = agent_name_from_id(qualified_type<AgentID>(), agent_id);
    std::unique_ptr<Node> agent_ptr(new AgentNode(agent_name, internal_context_));
    auto& agent = *agent_ptr;
    agent.adopt_print_flags(prototype);
    if (agent_exists(agent_id)) {
        throw std::logic_error("Created agent (" + agent.full_name() + ") already exists.");
    }
    int64 agent_index = agent.node_index();
    agent_indices_[agent_id] = agent_index;
    agent_ids_[agent_index] = agent_id;
    agents_[agent_index] = std::move(agent_ptr);
    agent_IO(agent).print_event("initialization");
    int64 missing_input = prototype_IO().missing_flow_input();
    if (missing_input != -1) {
        throw std::logic_error("Flow input port (" + agent_IO(agent).flow_input_port_name(missing_input) + ") " + 
                               "of created agent (" + agent.full_name() + ") has no value");
    }
    auto input_port_count = prototype_IO().flow_input_port_count();
    for (int64 port_index = 0; port_index < input_port_count; ++port_index) {
        const auto& flow_input_val = prototype_IO().flow_input_port_value(port_index);
        agent_IO(agent).assign_flow_input(port_index, flow_input_val);
        agent_IO(agent).print_flow_input(port_index);
    }
    agent_IO(agent).activate(flow, input);
    auto planned_dt = duration();
    try {
        planned_dt = agent.process_initialization_event();
    }
    catch (const std::exception& e) {
        prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
        throw e;
    }
    agent_IO(agent).deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(agent_index, planned_dt);
    }
    if (agent.time_precision() != no_scale) {
        t_cache_.retain_event(agent_index, agent.time_precision());
    }
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::affect_agent(const AgentID& agent_id)
{
    if (prototype.node_dmode() == flow) {
        throw std::logic_error("Attempt to use \"affect_agent\" to affect a flow node agent of a collection node (" + full_name() + "); use \"invoke_agent\" instead");
    }
    event_time().advance();
    auto agent_iter = agent_indices_.find(agent_id);
    if (agent_iter == std::end(agent_indices_)) {
        throw std::logic_error("Attempt to affect agent (" + full_name() + "." + tostring(agent_id) + ") that does not exist");
    }
    int64 agent_index = agent_iter->second;
    auto& agent = *dynamic_cast<Node*>(&internal_structure().node(agent_index));
    int64 port_index = prototype_IO().message_input_port_index();
    if (port_index == -1) {
        throw std::logic_error("Attempt to affect agent (" + agent.full_name() + "), " +
            "but none of the prototype's message input ports have been accessed");
    }
    auto val = prototype_IO().message_input_port_value(port_index);
    if (!val) {
        throw std::logic_error("Attempt to affect agent (" + agent.full_name() + "), " +
            "but none of the prototype's message input ports have been assigned a value");
    }
    agent_IO(agent).print_event("unplanned");
    agent_IO(agent).set_message_input(port_index, val);
    agent_IO(agent).print_message_input(port_index);
    auto elapsed_dt = duration();
    if (agent.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(agent_index).fixed_at(agent.time_precision());
    }
    agent_IO(agent).activate(message, input);
    auto planned_dt = duration();
    try {
        planned_dt = agent.process_unplanned_event(elapsed_dt);
    }
    catch (const std::exception& e) {
        prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
        throw e;
    }
    agent_IO(agent).deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(agent_index, planned_dt);
    }
    else {
        t_queue_.cancel_event(agent_index);
    }
    if (agent.time_precision() != no_scale) {
        t_cache_.retain_event(agent_index, agent.time_precision());
    }
    agent_IO(agent).clear_message_input();
    prototype_IO().clear_message_input();
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::remove_agent(const AgentID& agent_id)
{
    if (prototype.node_dmode() == flow) {
        throw std::logic_error("Attempt to use \"remove_agent\" to remove a flow node agent of a collection node (" + full_name() + "); use \"invoke_agent\" instead");
    }
    event_time().advance();
    auto agent_iter = agent_indices_.find(agent_id);
    if (agent_iter == std::end(agent_indices_)) {
        throw std::logic_error("Attempt to remove agent (" + full_name() + "." + tostring(agent_id) + ") that does not exist");
    }
    int64 agent_index = agent_iter->second;
    auto& agent = *dynamic_cast<Node*>(&internal_structure().node(agent_index));
    agent_IO(agent).print_event("finalization");
    auto elapsed_dt = duration();
    if (agent.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(agent_index).fixed_at(agent.time_precision());
    }
    agent_IO(agent).activate(flow, output);
    try {
        agent.process_finalization_event(elapsed_dt);
    }
    catch (const std::exception& e) {
        prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
        throw e;
    }
    prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
    agent_IO(agent).deactivate();
    int64 missing_output = agent_IO(agent).missing_flow_output();
    if (missing_output != -1) {
        throw std::logic_error("Flow output port (" + agent_IO(agent).flow_output_port_name(missing_output) + ") " +
                               "of removed agent (" + agent.full_name() + ") not assigned");
    }
    int64 output_port_count = agent_IO(agent).flow_output_port_count();
    for (int64 port_index = 0; port_index < output_port_count; ++port_index) {
        const auto& flow_output_val = agent_IO(agent).flow_output_port_value(port_index);
        agent_IO(agent).print_flow_output(port_index);
        prototype_IO().assign_flow_output(port_index, flow_output_val);
    }
    agent_indices_.erase(agent_iter);
    removed_indices_.insert(agent_index);
    t_queue_.cancel_event(agent_index);
    t_cache_.release_event(agent_index);
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::invoke_agent(const AgentID& agent_id)
{
    if (prototype.node_dmode() == message) {
        throw std::logic_error(std::string("Attempt to use \"invoke_agent\" to invoke a message node agent of a collection node (" + full_name() + "); ") +
                                           "use \"create_agent\", \"affect_agent\", and \"remove_agent\" instead");
    }
    event_time().advance();
    std::string agent_name = agent_name_from_id(qualified_type<AgentID>(), agent_id);
    auto agent_ptr = std::make_unique<Node>(agent_name, internal_context_);
    auto& agent = *agent_ptr;
    int64 agent_index = agent.node_index();
    agent.adopt_print_flags(prototype);
    agent_IO(agent).print_event("flow");
    int64 missing_input = prototype_IO().missing_flow_input();
    if (missing_input != -1) {
        throw std::logic_error("Flow input port (" + agent_IO(agent).flow_input_port_name(missing_input) + ") " + 
                               "of invoked agent (" + agent.full_name() + ") has no value");
    }
    auto input_port_count = prototype_IO().flow_input_port_count();
    for (int64 port_index = 0; port_index < input_port_count; ++port_index) {
        const auto& flow_input_val = prototype_IO().flow_input_port_value(port_index);
        if (!flow_input_val) throw std::logic_error("Flow input ports of created agent (" + agent.full_name() + ") not assigned");
        agent_IO(agent).assign_flow_input(port_index, flow_input_val);
        agent_IO(agent).print_flow_input(port_index);
    }
    agent_IO(agent).activate(flow, input);   
    try {
        agent.process_initialization_event();
    }
    catch (const std::exception& e) {
        prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
        throw e;
    }
    agent_IO(agent).deactivate();
    int64 missing_output = agent_IO(agent).missing_flow_output();
    if (missing_output != -1) {
        throw std::logic_error("Flow output port (" + agent_IO(agent).flow_output_port_name(missing_output) + ") " +
                               "of invoked agent (" + agent.full_name() + ") not assigned");
    }
    int64 output_port_count = agent_IO(agent).flow_output_port_count();
    for (int64 port_index = 0; port_index < output_port_count; ++port_index) {
        agent_IO(agent).print_flow_output(port_index);
        const auto& flow_output_val = agent_IO(agent).flow_output_port_value(port_index);
        prototype_IO().assign_flow_output(port_index, flow_output_val);
    }
    internal_structure().erase_node(agent_index);
}


template<typename AgentID, typename Node>
inline bool collection_node<AgentID, Node>::agent_exists(const AgentID& agent_id)
{
    return agent_indices_.find(agent_id) != std::end(agent_indices_);
}


template<typename AgentID, typename Node>
inline int64 collection_node<AgentID, Node>::agent_count()
{
    return int64(agent_indices_.size());
}


template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator collection_node<AgentID, Node>::agent_begin()
{
    return const_iterator(std::begin(agent_indices_));
}


template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator collection_node<AgentID, Node>::agent_end()
{
    return const_iterator(std::end(agent_indices_));
}

    
template<typename AgentID, typename Node>
inline node_structure& collection_node<AgentID, Node>::internal_structure()
{
    return internal_context_.internal_structure();
}


template<typename AgentID, typename Node>
inline node_interface& collection_node<AgentID, Node>::prototype_IO() const
{
    return const_cast<node_interface&>(const_cast<Node&>(prototype).external_interface());
}


template<typename AgentID, typename Node>
inline timer& collection_node<AgentID, Node>::prototype_ET() const
{
    return const_cast<timer&>(prototype.event_timer());
}


template<typename AgentID, typename Node>
inline node_interface& collection_node<AgentID, Node>::agent_IO(const system_node& agent) const
{
    return const_cast<node_interface&>(const_cast<system_node&>(agent).external_interface());
}


template<typename AgentID, typename Node>
inline discrete_event_time& collection_node<AgentID, Node>::event_time()
{
    return external_IO().external_context().event_time();
}


template<typename AgentID, typename Node>
template<data_mode dmode, data_goal dgoal, typename T>
inline void collection_node<AgentID, Node>::validate_prototype_port(const port<dmode, dgoal, T>& prototype_port)
{
    if (&prototype_port.external_interface() != &prototype_IO()) {
        throw std::logic_error("Attempt to access " + string_from_data_goal(dgoal) + " " + string_from_data_mode(dmode) + " " +
                               "port (" + prototype_port.port_name() + "), " +
                               "which is not a port of the prototype agent (" + prototype.full_name() + ")");
    }
    if (!external_interface().active()) {
        throw std::logic_error("Attempt to access " + string_from_data_goal(dgoal) + " " + string_from_data_mode(dmode) + " " +
                               "port (" + prototype_port.port_name() + "), " + 
                               "which is a prototype port of inactive node (" + full_name() + ")");
    }
}

    
template<typename AgentID, typename Node>
inline Node& collection_node<AgentID, Node>::handle_agent_planned_event(int64 agent_index)
{
    event_time().advance();
    auto& agent = *dynamic_cast<Node*>(&internal_structure().node(agent_index));
    agent_IO(agent).print_event("planned");
    auto elapsed_dt = duration();
    if (agent.time_precision() != no_scale) {
        elapsed_dt = t_cache_.duration_since(agent_index).fixed_at(agent.time_precision());
    }
    agent_IO(agent).activate(message, output);
    auto planned_dt = duration();
    try {
        ET().start();
        planned_dt = agent.process_planned_event(elapsed_dt);
        ET().stop();
    }
    catch (const std::exception& e) {
        if (ET().timing()) {
            ET().stop();
        }
        prototype_ET() = timer(prototype_ET().cumulative_duration() + agent.event_timer().cumulative_duration());
        throw e;
    }
    agent_IO(agent).deactivate();
    if (planned_dt.finite()) {
        t_queue_.plan_event(agent_index, planned_dt);
    }
    else {
        t_queue_.pop_imminent_event(agent_index);
    }
    if (agent.time_precision() != no_scale) {
        t_cache_.retain_event(agent_index, agent.time_precision());
    }
    int64 list_size = agent_IO(agent).message_output_list_size();
    for (int64 list_index = 0; list_index < list_size; ++list_index) {
        int64 port_index = agent_IO(agent).message_output_index(list_index);
        agent_IO(agent).print_message_output(list_index, port_index);
    }
    return agent;
}


template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::erase_removed_agents()
{
    for (int64 agent_index : removed_indices_) {
        internal_structure().erase_node(agent_index);
        agent_ids_.erase(agent_index);
        agents_.erase(agent_index);
    }
    removed_indices_.clear();
}


template<typename AgentID, typename Node>
template<typename T>
inline std::string collection_node<AgentID, Node>::agent_name_from_id(qualified_type<T> agent_id_type, const AgentID& agent_id)
{
    return tostring(agent_id);
}


template<typename AgentID, typename Node>
inline std::string collection_node<AgentID, Node>::agent_name_from_id(qualified_type<std::string> agent_id_type, const AgentID& agent_id)
{
    return agent_id;
}

    
template<typename AgentID, typename Node>
inline void collection_node<AgentID, Node>::adopt_component_print_flags(const system_node& node) const
{
    const auto node_ptr = dynamic_cast<const collection_node*>(&node);
    if (!node_ptr) {
        throw std::logic_error("Attempt to transfer print flags from node (" + node.full_name() + ")" +
                               "to node (" + full_name() + "), but the nodes are of different types");
    }
    auto& other_node = const_cast<collection_node&>(*node_ptr);
    prototype.adopt_print_flags(other_node.prototype);
}


// flow port proxy members

template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    inline typename collection_node<AgentID, Node>::flow_port_proxy<T>& collection_node<AgentID, Node>::flow_port_proxy<T>::operator=(const T& rhs)
#else
    inline typename collection_node<AgentID, Node>::template flow_port_proxy<T>& collection_node<AgentID, Node>::flow_port_proxy<T>::operator=(const T& rhs)
#endif
{
    external_interface_.assign_flow_input(port_index_, qualified_type<T>::copy(rhs));
    return *this;
}


template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    inline typename collection_node<AgentID, Node>::flow_port_proxy<T>& collection_node<AgentID, Node>::flow_port_proxy<T>::operator=(const flow_port_proxy& rhs)
#else
    inline typename collection_node<AgentID, Node>::template flow_port_proxy<T>& collection_node<AgentID, Node>::flow_port_proxy<T>::operator=(const flow_port_proxy& rhs)
#endif
{
    const pointer& val = rhs.external_interface_.flow_input_port_value(rhs.port_index_);
    external_interface_.assign_flow_input(port_index_, val);
    return *this;
}


template<typename AgentID, typename Node>
template<typename T>
inline collection_node<AgentID, Node>::flow_port_proxy<T>::flow_port_proxy(const port<flow, input, T>& input_port)
    : external_interface_(const_cast<node_interface&>(input_port.external_interface()))
    , port_index_(input_port.port_index())
{
}


// message port proxy members

template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    inline typename collection_node<AgentID, Node>::message_port_proxy<T>& collection_node<AgentID, Node>::message_port_proxy<T>::operator=(const T& rhs)
#else
    inline typename collection_node<AgentID, Node>::template message_port_proxy<T>& collection_node<AgentID, Node>::message_port_proxy<T>::operator=(const T& rhs)
#endif
{
    external_interface_.set_message_input(port_index_, qualified_type<T>::copy(rhs));
    return *this;
}


template<typename AgentID, typename Node>
template<typename T>
#if _WIN32
    inline typename collection_node<AgentID, Node>::message_port_proxy<T>& collection_node<AgentID, Node>::message_port_proxy<T>::operator=(const message_port_proxy& rhs)
#else
    inline typename collection_node<AgentID, Node>::template message_port_proxy<T>& collection_node<AgentID, Node>::message_port_proxy<T>::operator=(const message_port_proxy& rhs)
#endif
{
    const pointer& val = rhs.external_interface_.message_input_port_value(rhs.port_index_);
    external_interface_.set_message_input(port_index_, val);
    return *this;
}


template<typename AgentID, typename Node>
template<typename T>
inline collection_node<AgentID, Node>::message_port_proxy<T>::message_port_proxy(const port<message, input, T>& input_port)
    : external_interface_(const_cast<node_interface&>(input_port.external_interface()))
    , port_index_(input_port.port_index())
{
}


// const iterator members

template<typename AgentID, typename Node>
inline const AgentID& collection_node<AgentID, Node>::const_iterator::operator*() const
{
    return iter_->first;
}


template<typename AgentID, typename Node>
inline const AgentID* collection_node<AgentID, Node>::const_iterator::operator->() const
{
    return &iter_->first;
}
    

template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator& collection_node<AgentID, Node>::const_iterator::operator++()
{
    ++iter_;
    return *this;
}


template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator collection_node<AgentID, Node>::const_iterator::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}


template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator& collection_node<AgentID, Node>::const_iterator::operator--()
{
    --iter_;
    return *this;
}


template<typename AgentID, typename Node>
inline typename collection_node<AgentID, Node>::const_iterator collection_node<AgentID, Node>::const_iterator::operator--(int)
{
    auto old = *this;
    --(*this);
    return old;
}


template<typename AgentID, typename Node>
inline bool collection_node<AgentID, Node>::const_iterator::operator==(const const_iterator& rhs) const
{
    return iter_ == rhs.iter_;
}


template<typename AgentID, typename Node>
inline bool collection_node<AgentID, Node>::const_iterator::operator!=(const const_iterator& rhs) const
{
    return iter_ != rhs.iter_;
}


template<typename AgentID, typename Node>
inline collection_node<AgentID, Node>::const_iterator::const_iterator(const typename std::map<AgentID, int64>::const_iterator& iter)
    : iter_(iter)
{
}


}  // namespace
}  // namespace

#endif
