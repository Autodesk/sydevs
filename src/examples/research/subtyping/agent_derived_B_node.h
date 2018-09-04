#pragma once
#ifndef SYDEVS_RESEARCH_AGENT_DERIVED_B_NODE_H_
#define SYDEVS_RESEARCH_AGENT_DERIVED_B_NODE_H_

#include <examples/research/subtyping/agent_base_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class agent_derived_B_node : public agent_base_node
{
public:
    // Constructor/Destructor:
    agent_derived_B_node(const std::string& node_name, const node_context& external_context);
    virtual ~agent_derived_B_node() = default;

protected:
    // State Variables:
    float64 y;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);
};


inline agent_derived_B_node::agent_derived_B_node(const std::string& node_name, const node_context& external_context)
    : agent_base_node(node_name, external_context)
{
}


inline duration agent_derived_B_node::initialization_event()
{
    y = y0_input.value();
    return duration::inf();
}


inline duration agent_derived_B_node::unplanned_event(duration elapsed_dt)
{
    if (x_input.received()) {
        int64 x = x_input.value();
        y *= x;                             // SPECIFIC TO AGENT DERIVED B
    }
    return 0_s;
}


inline duration agent_derived_B_node::planned_event(duration elapsed_dt)
{
    // Note: if multiple inputs 'x' are received at the same time, the planned
    // event will only be invoked after the last one is received. Thus, not
    // every intermediate value of `y` will be output.
    y_output.send(y);
    return duration::inf();
}


inline void agent_derived_B_node::finalization_event(duration elapsed_dt)
{
    y_final_output.assign(y);
}


}  // namespace

#endif
