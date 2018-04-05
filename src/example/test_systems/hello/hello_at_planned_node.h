#pragma once
#ifndef HELLO_AT_PLANNED_NODE_H_
#define HELLO_AT_PLANNED_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class hello_at_planned_node : public atomic_node
{
public:
    // Constructor/Destructor:
    hello_at_planned_node(const std::string& node_name, const node_context& external_context);
    virtual ~hello_at_planned_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:

protected:
    // State Variables:

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline hello_at_planned_node::hello_at_planned_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
{
}


inline duration hello_at_planned_node::initialization_event()
{
    return 0_s;
}


inline duration hello_at_planned_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration hello_at_planned_node::planned_event(duration elapsed_dt)
{
    print("Hello");

    return 1_s;
}


inline void hello_at_planned_node::finalization_event(duration elapsed_dt)
{
}


#endif
