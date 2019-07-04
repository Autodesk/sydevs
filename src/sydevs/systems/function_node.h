#pragma once
#ifndef SYDEVS_SYSTEMS_FUNCTION_NODE_H_
#define SYDEVS_SYSTEMS_FUNCTION_NODE_H_

#include <sydevs/systems/system_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class for indivisible nodes in which function behavior is
 *        procedurally encoded.
 *
 * @details
 * The `function_node` abstract base class is inherited by classes that 
 * represent a computation using a single procedurally defined function. The
 * computation is specified by overriding the `flow_event` pure virtual member
 * function.
 */
class function_node : public system_node
{
public:
    /**
     * @brief Constructs a `function_node`.
     * 
     * @details
     * The function node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     */
    function_node(const std::string& node_name, const node_context& external_context);

    virtual ~function_node() = default;  ///< Destructor

    data_mode node_dmode() const;  ///< Returns `message` for all atomic nodes.
    scale time_precision() const;  ///< Returns `no_scale` for all function nodes.

    duration handle_initialization_event();                ///< Invoked when all flow input ports have values; calls `flow_event`.
    duration handle_unplanned_event(duration elapsed_dt);  ///< Never invoked for function nodes.
    duration handle_planned_event(duration elapsed_dt);    ///< Never invoked for function nodes.
    void handle_finalization_event(duration elapsed_dt);   ///< Never invoked for function nodes.

private:
    void adopt_component_print_flags(const system_node& node) const;

    virtual void flow_event() = 0;
};


inline function_node::function_node(const std::string& node_name, const node_context& external_context)
    : system_node(node_name, external_context)
{
}


inline data_mode function_node::node_dmode() const
{
    return flow;
}


inline scale function_node::time_precision() const
{
    return no_scale;
}


inline duration function_node::handle_initialization_event()
{
    ET().start();
    flow_event();
    ET().stop();
    return duration::inf();
}


inline duration function_node::handle_unplanned_event(duration elapsed_dt)
{
    throw std::logic_error("Unexpected unplanned event invoked for function node");
    return duration::inf();
}


inline duration function_node::handle_planned_event(duration elapsed_dt)
{
    throw std::logic_error("Unexpected planned event invoked for function node");
    return duration::inf();
}


inline void function_node::handle_finalization_event(duration elapsed_dt)
{
    throw std::logic_error("Unexpected finalization event invoked for function node");
}


inline void function_node::adopt_component_print_flags(const system_node& node) const
{
}


}  // namespace
}  // namespace

#endif
