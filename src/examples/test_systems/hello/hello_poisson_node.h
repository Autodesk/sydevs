#pragma once
#ifndef SYDEVS_EXAMPLES_HELLO_POISSON_NODE_H_
#define SYDEVS_EXAMPLES_HELLO_POISSON_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class hello_poisson_node : public atomic_node
{
public:
    // Constructor/Destructor:
    hello_poisson_node(const std::string& node_name, const node_context& external_context);
    virtual ~hello_poisson_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:

protected:
    // State Variables:
    int64 count;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline hello_poisson_node::hello_poisson_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
{
}


inline duration hello_poisson_node::initialization_event()
{
    count = 0;
    return 0_s;
}


inline duration hello_poisson_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration hello_poisson_node::planned_event(duration elapsed_dt)
{
    static auto dist = std::exponential_distribution<float64>(1.0);

    ++count;
    print("Hello " + tostring(count));

    return (dist(rng)*1_s).fixed_at(time_precision());
}


inline void hello_poisson_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
