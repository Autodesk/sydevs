#pragma once
#ifndef SIMPLE_AGENT_NODE_H_
#define SIMPLE_AGENT_NODE_H_

#include <sydevs/systems/atomic_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class simple_agent_node : public atomic_node
{
public:
    // Constructor/Destructor:
    simple_agent_node(const std::string& node_name, const node_context& external_context);
    virtual ~simple_agent_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, duration> avg_delay_dt_input;
    port<flow, input, float64> invite_prob_input;
    port<message, input, std::string> encounter_input;
    port<message, output, array1d<int64>> move_output;
    port<message, output, std::string> invite_output;
    port<flow, output, int64> acquaintances_output;

protected:
    // State Variables:
    duration avg_delay_dt;
    float64 invite_prob;
    array1d<int64> grid_pos;
    int64 invitee_count;
    std::set<std::string> unique_encounters;
    duration planned_dt;
    bool preparing_first_output;

    // Event Handlers:
    virtual duration initialization_event();
    virtual duration unplanned_event(duration elapsed_dt);
    virtual duration planned_event(duration elapsed_dt);
    virtual void finalization_event(duration elapsed_dt);

    // Probability Distributions:
    duration sample_delay_dt();
    bool sample_invite();
    int64 sample_direction();
};


inline simple_agent_node::simple_agent_node(const std::string& node_name, const node_context& external_context)
    : atomic_node(node_name, external_context)
    , avg_delay_dt_input("avg_delay_dt_input", external_interface())
    , invite_prob_input("invite_prob_input", external_interface())
    , encounter_input("encounter_input", external_interface())
    , move_output("move_output", external_interface())
    , invite_output("invite_output", external_interface())
    , acquaintances_output("acquaintances_output", external_interface())
{
}


inline duration simple_agent_node::initialization_event()
{
    avg_delay_dt = avg_delay_dt_input.value().fixed_at(time_precision());
    invite_prob = invite_prob_input.value();
    grid_pos = array1d<int64>({2}, {0, 0});
    invitee_count = 0;
    unique_encounters.clear();
    planned_dt = 0_s;
    preparing_first_output = true;
    return planned_dt;
}


inline duration simple_agent_node::unplanned_event(duration elapsed_dt)
{
    if (encounter_input.received()) {
        unique_encounters.insert(encounter_input.value());
        planned_dt -= elapsed_dt;
    }
    return planned_dt;
}


inline duration simple_agent_node::planned_event(duration elapsed_dt)
{
    if (preparing_first_output) {
        print("arriving at " + tostring(grid_pos));
        move_output.send(grid_pos);
        preparing_first_output = false;
    }
    else {
        auto invite = sample_invite();
        if (invite) {
            auto invitee_name = (string_builder() << node_name() << "%" << invitee_count).str();
            ++invitee_count;
            print("inviting " + invitee_name);
            invite_output.send(invitee_name);
        }
        else {
            int64 direction = sample_direction();
            switch (direction) {
            case 0:
                ++grid_pos({0});
                break;
            case 1:
                ++grid_pos({1});
                break;
            case 2:
                --grid_pos({0});
                break;
            case 3:
                --grid_pos({1});
                break;
            }
            print("moving to " + tostring(grid_pos));
            move_output.send(grid_pos);
        }
    }
    planned_dt = sample_delay_dt();
    return planned_dt;
}


inline void simple_agent_node::finalization_event(duration elapsed_dt)
{
    acquaintances_output.assign(unique_encounters.size());
}


inline duration simple_agent_node::sample_delay_dt()
{
    static std::exponential_distribution<float64> dist(1.0);
    return dist(rng)*avg_delay_dt;
}


inline bool simple_agent_node::sample_invite()
{
    static std::uniform_real_distribution<float64> dist(0, 1);
    return dist(rng) < invite_prob;
}


inline int64 simple_agent_node::sample_direction()
{
    static std::uniform_int_distribution<int64> dist(0, 3);
    return dist(rng);
}


#endif
