#pragma once
#ifndef SYDEVS_RESEARCH_AGENT_BASE_NODE_H_
#define SYDEVS_RESEARCH_AGENT_BASE_NODE_H_

#include <sydevs/systems/atomic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class agent_base_node : public atomic_node
{
public:
    // Constructor/Destructor:
    agent_base_node(const std::string& node_name, const node_context& external_context);
    virtual ~agent_base_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, float64> y0_input;
    port<message, input, float64> x_input;
    port<message, output, float64> y_output;
    port<flow, output, float64> y_final_output;

protected:
    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline agent_base_node::agent_base_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , y0_input("y0_input", external_interface())
    , x_input("x_input", external_interface())
    , y_output("y_output", external_interface())
    , y_final_output("y_final_output", external_interface())
{
}


inline duration agent_base_node::initialization_event()
{
    return duration();
}


inline duration agent_base_node::unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration agent_base_node::planned_event(duration elapsed_dt)
{
    return duration();
}


inline void agent_base_node::finalization_event(duration elapsed_dt)
{
}


}  // namespace

#endif
