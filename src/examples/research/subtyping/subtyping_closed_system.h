#pragma once
#ifndef SYDEVS_EXAMPLES_SUBTYPING_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_SUBTYPING_CLOSED_SYSTEM_H_

#include <examples/research/subtyping/agent_derived_A_node.h>
#include <examples/research/subtyping/agent_derived_B_node.h>
#include <sydevs/systems/collection_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class subtyping_closed_system : public collection_node<int64, agent_base_node>
{
public:
    // Constructor/Destructor:
    subtyping_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~subtyping_closed_system() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

protected:
    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const int64& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);
};


inline subtyping_closed_system::subtyping_closed_system(const std::string& node_name, const node_context& external_context)
    : collection_node<int64, agent_base_node>(node_name, external_context)
{
}


inline duration subtyping_closed_system::macro_initialization_event()
{
    int64 agent_id = 0;
    for (float64 y0 = 0.0; y0 < 5.0; ++y0) {
        access(prototype.y0_input) = y0;
        create_agent<agent_derived_A_node>(agent_id);
        ++agent_id;
        create_agent<agent_derived_B_node>(agent_id);
        ++agent_id;
    }
    return 5_s;
}


inline duration subtyping_closed_system::macro_unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration subtyping_closed_system::micro_planned_event(const int64& agent_id, duration elapsed_dt)
{
    return 5_s;
}


inline duration subtyping_closed_system::macro_planned_event(duration elapsed_dt)
{
    for (int64 agent_id = 0; agent_id < 10; ++agent_id) {
        access(prototype.x_input) = 5.0;
        affect_agent(agent_id);
    }
    return 5_s;
}


inline void subtyping_closed_system::macro_finalization_event(duration elapsed_dt)
{
    for (int64 agent_id = 0; agent_id < 10; ++agent_id) {
        remove_agent(agent_id);
    }
}


}  // namespace

#endif
