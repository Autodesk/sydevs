#pragma once
#ifndef SYDEVS_EXAMPLES_BUILDING_SIMULATION_NODE_H_
#define SYDEVS_EXAMPLES_BUILDING_SIMULATION_NODE_H_

#include <examples/demo/building7m/building_info_node.h>
#include <examples/demo/building7m/initial_position_node.h>
#include <examples/demo/building7m/building_dynamics_node.h>
#include <examples/demo/building7m/building_vis_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class building_simulation_node : public composite_node
{
public:
    // Constructor/Destructor:
    building_simulation_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_simulation_node() = default;

    // Components:
    parameter_node<thermodynamic_temperature> initial_temperature;
    parameter_node<duration> frame_duration;
    parameter_node<quantity<decltype(_m/_s)>> walking_speed;
    building_info_node building_info;
    initial_position_node initial_position;
    building_dynamics_node building_dynamics;
    building_vis_node building_vis;
};


building_simulation_node::building_simulation_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , initial_temperature("initial_temperature", internal_context(), 293150_mK)
    , frame_duration("frame_duration", internal_context(), duration::inf())  // 30_s recommended for textual output
    , walking_speed("walking_speed", internal_context(), 1400_mm/_s)
    , building_info("building_info", internal_context())
    , initial_position("initial_position", internal_context())
    , building_dynamics("building_dynamics", internal_context())
    , building_vis("building_vis", internal_context())
{
    // Initialization Links
    inner_link(initial_temperature.parameter, building_dynamics.initial_temperature_input);
    inner_link(building_info.building_layout_output, building_dynamics.building_layout_input);
    inner_link(building_info.building_layout_output, initial_position.building_layout_input);
    inner_link(building_info.wall_resistance_output, building_dynamics.wall_resistance_input);
    inner_link(initial_position.initial_position_output, building_dynamics.initial_position_input);
    inner_link(walking_speed.parameter, building_dynamics.walking_speed_input);
    inner_link(frame_duration.parameter, building_vis.frame_duration_input);

    // Simulation Links
    inner_link(building_dynamics.temperature_field_output, building_vis.temperature_field_input);
    inner_link(building_dynamics.occupant_position_output, building_vis.occupant_position_input);
}


}  // namespace

#endif
