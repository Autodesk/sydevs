#pragma once
#ifndef SYDEVS_EXAMPLES_SIMPLE_COLLECTION_NODE_H_
#define SYDEVS_EXAMPLES_SIMPLE_COLLECTION_NODE_H_

#include <sydevs/systems/collection_node.h>
#include <examples/test_systems/agentbased/simple_agent_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class simple_collection_node : public collection_node<std::string, simple_agent_node>
{
public:
    // Constructor/Destructor:
    simple_collection_node(const std::string& node_name, const node_context& external_context);
    virtual ~simple_collection_node() = default;

    // Attributes:
    virtual scale time_precision() const { return micro; }

    // Ports:
    port<flow, input, int64> grid_extent_input;
    port<flow, input, duration> avg_delay_dt_input;
    port<flow, input, float64> invite_prob_input;
    port<flow, output, float64> avg_acquaintances_output;

protected:
    // State Variables:
    int64 grid_extent;
    duration avg_delay_dt;
    float64 invite_prob;
    array2d<std::set<std::string>> grid;
    std::map<std::string, array1d<int64>> agent_coords;
    int64 total_acquaintances;
    int64 departure_count;

    // Event Handlers:
    virtual duration macro_initialization_event();
    virtual duration macro_unplanned_event(duration elapsed_dt);
    virtual duration micro_planned_event(const std::string& agent_id, duration elapsed_dt);
    virtual duration macro_planned_event(duration elapsed_dt);
    virtual void macro_finalization_event(duration elapsed_dt);

    // Print Functions
    void print_grid();
};


inline simple_collection_node::simple_collection_node(const std::string& node_name, const node_context& external_context)
    : collection_node<std::string, simple_agent_node>(node_name, external_context)
    , grid_extent_input("grid_extent_input", external_interface())
    , avg_delay_dt_input("avg_delay_dt_input", external_interface())
    , invite_prob_input("invite_prob_input", external_interface())
    , avg_acquaintances_output("avg_acquaintances_output", external_interface())
{
}


inline duration simple_collection_node::macro_initialization_event()
{
    // Store parameter values.
    grid_extent = grid_extent_input.value();
    avg_delay_dt = avg_delay_dt_input.value();
    invite_prob = invite_prob_input.value();

    // Initialize the grid.
    int64 grid_length = 2*grid_extent + 1;
    grid = array2d<std::set<std::string>>({grid_length, grid_length}, std::set<std::string>());
    agent_coords = std::map<std::string, array1d<int64>>();

    // Initialize statistics.
    total_acquaintances = 0;
    departure_count = 0;

    // Create the first agent.
    access(prototype.avg_delay_dt_input) = avg_delay_dt;
    access(prototype.invite_prob_input) = invite_prob;
    create_agent("agent-0");

    // Print the grid.
    print_grid();

    // Wait indefinitely for agent activity.
    return duration::inf();
}


inline duration simple_collection_node::macro_unplanned_event(duration elapsed_dt)
{
    return duration();
}


inline duration simple_collection_node::micro_planned_event(const std::string& agent_id, duration elapsed_dt)
{
    // Respond according to the active prototype message output port.
    if (transmitted(prototype.move_output)) {
        // The agent has assumed a new position on the grid.
        const auto& new_pos = access(prototype.move_output);

        // The agent may have an outdated position on the grid; if so, erase it.
        auto agent_iter = agent_coords.find(agent_id);
        if (agent_iter != std::end(agent_coords)) {
            auto old_pos = agent_iter->second;
            grid(old_pos + grid_extent).erase(agent_id);
        }

        // If the agent's new position is within the grid, notify both this agent 
        // and all other agents at the same grid cell of the encounter.
        // Otherwise terminate this agent.
        if (std::abs(new_pos(0)) <= grid_extent && std::abs(new_pos(1)) <= grid_extent) {
            auto& neighbor_ids = grid(new_pos + grid_extent);
            for (const auto& neighbor_id : neighbor_ids) {
                access(prototype.encounter_input) = neighbor_id;
                affect_agent(agent_id);
                access(prototype.encounter_input) = agent_id;
                affect_agent(neighbor_id);
            }
            neighbor_ids.insert(agent_id);
            agent_coords[agent_id] = new_pos;
        }
        else {
            remove_agent(agent_id);
            int64 acquaintances = access(prototype.acquaintances_output);
            print(agent_id + " departed after encountering " + tostring(acquaintances) + " others");
            total_acquaintances += acquaintances;
            ++departure_count;
        }

        // Print the grid.
        print_grid();
    }
    else if (transmitted(prototype.invite_output)) {
        // The agent has invited a new agent.
        const auto& invitee_id = access(prototype.invite_output);
        access(prototype.avg_delay_dt_input) = avg_delay_dt;
        access(prototype.invite_prob_input) = invite_prob;
        create_agent(invitee_id);
    }

    // Wait indefinitely for agent activity.
    return duration::inf();
}


inline duration simple_collection_node::macro_planned_event(duration elapsed_dt)
{
    return duration();
}


inline void simple_collection_node::macro_finalization_event(duration elapsed_dt)
{
    // Calculate the average number of encountered acquaintances per agent.
    float64 avg_acquaintances = 0;
    if (departure_count > 0) {
        avg_acquaintances = total_acquaintances/float64(departure_count);
    }
    avg_acquaintances_output.assign(avg_acquaintances);
}


inline void simple_collection_node::print_grid()
{
    // Print the number of agents at each grid cell.
    for (int64 row = 0; row < grid.dims()[1]; ++row) {
        int64 j = grid.dims()[1] - grid_extent - row;
        auto line_str = std::string();
        for (int64 i = 0; i < grid.dims()[0]; ++i) {
            if (i > 0) {
                line_str += "|";
            }
            line_str += tostring(int64(grid(i, j).size()));
        }
        print(line_str);
    }
}


}  // namespace

#endif
