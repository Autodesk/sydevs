#pragma once
#ifndef SYDEVS_SYSTEMS_ATOMIC_NODE_H_
#define SYDEVS_SYSTEMS_ATOMIC_NODE_H_

#include <sydevs/systems/system_node.h>

namespace sydevs {
namespace systems {


/**
 * @brief A base class for indivisible nodes in which simulation behavior is
 *        procedurally encoded.
 *
 * @details
 * The `atomic_node` abstract base class is inherited by classes that represent
 * system behavior using procedurally defined event handling functions. These
 * event handlers are specified by overriding the following pure virtual member
 * functions:
 *
 * - `initialization_event`, which is invoked during initialization and returns
 *   a planned duration;
 * - `unplanned_event`, which is invoked when a message input is received and
 *   returns a planned duration.
 * - `planned_event`, which is invoked when the previous planned duration
 *   elapses and returns a planned duration;
 * - `finalization`, which is invoked during finalization.
 */
class atomic_node : public system_node
{
public:
    /**
     * @brief Constructs an `atomic_node`.
     * 
     * @details
     * The atomic node is created along with its associated `node_interface`
     * object.
     *
     * @param node_name The name of the node within the encompassing context.
     * @param external_context The context in which the node is constructed.
     */
    atomic_node(const std::string& node_name, const node_context& external_context);

    virtual ~atomic_node() = default;  ///< Destructor

    void print_on_elapsed_duration(bool flag = true) const;  ///< If `flag` is `true`, all elapsed durations are printed for this node.
    void print_on_planned_duration(bool flag = true) const;  ///< If `flag` is `true`, all planned durations are printed for this node.

    data_mode node_dmode() const;  ///< Returns `message` for all atomic nodes.

    duration handle_initialization_event();                ///< Invoked at the beginning of a simulation; calls `initialization_event`.
    duration handle_unplanned_event(duration elapsed_dt);  ///< Invoked whenever a message is received; calls `unplanned_event`.
    duration handle_planned_event(duration elapsed_dt);    ///< Invoked when the planned duration elapses; calls `planned_event`.
    void handle_finalization_event(duration elapsed_dt);   ///< Invoked at the end of a simulation; calls `finalization_event`.

private:
    void adopt_component_print_flags(const system_node& node) const;

    virtual duration initialization_event() = 0;
    virtual duration unplanned_event(duration elapsed_dt) = 0;
    virtual duration planned_event(duration elapsed_dt) = 0;
    virtual void finalization_event(duration elapsed_dt) = 0;

    bool initialized_;
    bool finalized_;
};


inline atomic_node::atomic_node(const std::string& node_name, const node_context& external_context)
    : system_node(node_name, external_context)
    , initialized_(false)
    , finalized_(false)
{
}


inline void atomic_node::print_on_elapsed_duration(bool flag) const
{
    external_IO().print_on_elapsed_duration(flag);
}


inline void atomic_node::print_on_planned_duration(bool flag) const
{
    external_IO().print_on_planned_duration(flag);
}


inline data_mode atomic_node::node_dmode() const
{
    return message;
}


inline void atomic_node::adopt_component_print_flags(const system_node& node) const
{
}


}  // namespace
}  // namespace

#endif
