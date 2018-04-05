#pragma once
#ifndef BUILDING_DYNAMICS_NODE_H_
#define BUILDING_DYNAMICS_NODE_H_

#include <example/demo/building7m/weather_node.h>
#include <example/demo/building7m/thermodynamics_node.h>
#include <example/demo/building7m/occupant_node.h>
#include <sydevs/systems/composite_node.h>

using namespace sydevs;
using namespace sydevs::systems;


class building_dynamics_node : public composite_node
{
public:
    // Constructor/Destructor:
    building_dynamics_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_dynamics_node() = default;

    // Ports:
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, std::pair<array2d<int64>, distance>> building_layout_input;
    port<flow, input, float64> wall_resistance_input;
    port<flow, input, array1d<int64>> initial_position_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<message, output, array2d<thermodynamic_temperature>> temperature_field_output;
    port<message, output, array1d<int64>> occupant_position_output;

    // Components:
    weather_node weather;
    thermodynamics_node thermodynamics;
    occupant_node occupant;
};


building_dynamics_node::building_dynamics_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , initial_temperature_input("initial_temperature_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , wall_resistance_input("wall_resistance_input", external_interface())
    , initial_position_input("initial_position_input", external_interface())
    , walking_speed_input("walking_speed_input", external_interface())
    , temperature_field_output("temperature_field_output", external_interface())
    , occupant_position_output("occupant_position_output", external_interface())
    , weather("weather", internal_context())
    , thermodynamics("thermodynamics", internal_context())
    , occupant("occupant", internal_context())
{
    // Initialization Links
    inward_link(initial_temperature_input, weather.initial_temperature_input);
    inward_link(initial_temperature_input, thermodynamics.initial_temperature_input);
    inward_link(initial_temperature_input, occupant.initial_temperature_input);
    inward_link(building_layout_input, thermodynamics.building_layout_input);
    inward_link(building_layout_input, occupant.building_layout_input);
    inward_link(wall_resistance_input, thermodynamics.wall_resistance_input);
    inward_link(initial_position_input, occupant.initial_position_input);
    inward_link(walking_speed_input, occupant.walking_speed_input);

    // Simulation Links
    inner_link(weather.outdoor_temperature_output, thermodynamics.outdoor_temperature_input);
    outward_link(thermodynamics.temperature_field_output, temperature_field_output);
    outward_link(occupant.occupant_position_output, occupant_position_output);
}


#endif
