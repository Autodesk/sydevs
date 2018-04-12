#pragma once
#ifndef SYDEVS_EXAMPLES_HELLO_AT_INIT_NODE_H_
#define SYDEVS_EXAMPLES_HELLO_AT_INIT_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class hello_at_init_node : public atomic_node
{
public:
    // Constructor/Destructor:
    hello_at_init_node(const std::string& node_name, const node_context& external_context);
    virtual ~hello_at_init_node() = default;

    // Attributes:
    virtual scale time_precision() const { return no_scale; }

    // Ports:

protected:
    // State Variables:

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline hello_at_init_node::hello_at_init_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
{
}


inline duration hello_at_init_node::initialization_event()
{
    print("Hello");

    return duration::inf();
}


inline duration hello_at_init_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration hello_at_init_node::planned_event(duration elapsed_dt)
{
    return duration();
}


inline void hello_at_init_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
