#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_DYNAMICS_NODE_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_DYNAMICS_NODE_H_

#include <examples/research/building7m_advanced/weather_node.h>
#include <examples/research/building7m_advanced/thermodynamics_node.h>
#include <examples/research/building7m_advanced/heat_source_node.h>
#include <examples/research/building7m_advanced/comfort_node.h>
#include <examples/research/building7m_advanced/acoustics_node.h>
#include <examples/research/building7m_advanced/sound_source_node.h>
#include <examples/research/building7m_advanced/hearing_node.h>
#include <examples/research/building7m_advanced/occupant_planning_node.h>
#include <examples/research/building7m_advanced/occupant_steering_node.h>
#include <sydevs/systems/composite_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class building_dynamics_node : public composite_node
{
public:
    // Constructor/Destructor:
    building_dynamics_node(const std::string& node_name, const node_context& external_context);
    virtual ~building_dynamics_node() = default;

    // Ports:
    port<flow, input, thermodynamic_temperature> outdoor_mean_temperature_input;
    port<flow, input, duration> outdoor_temperature_period_input;
    port<flow, input, duration> outdoor_temperature_time_step_input;
    port<flow, input, duration> occupant_time_constant_input;
    port<flow, input, thermodynamic_temperature> high_temperature_input;
    port<flow, input, thermodynamic_temperature> initial_temperature_input;
    port<flow, input, quantity<decltype(_K/_s)>> initial_temperature_rate_input;
    port<flow, input, std::pair<array2d<int64>, std::pair<distance, distance>>> building_layout_input;
    port<flow, input, float64> wall_resistance_input;
    port<flow, input, float64> wall_sound_absorption_input;
    port<flow, input, float64> floor_sound_absorption_input;
    port<flow, input, float64> ceiling_sound_absorption_input;
    port<flow, input, std::map<occupant_id, array1d<int64>>> initial_positions_input;
    port<flow, input, quantity<decltype(_m/_s)>> walking_speed_input;
    port<flow, input, quantity<decltype(_g/_m/_s/_s)>> walking_sound_input;
    port<message, output, array2d<thermodynamic_temperature>> temperature_field_output;
    port<message, output, array2d<quantity<decltype(_g/_m/_s/_s)>>> sound_field_output;
    port<message, output, std::pair<occupant_id, array1d<int64>>> occupant_position_output;

    // Components:
    weather_node weather;
    thermodynamics_node thermodynamics;
    heat_source_node heat_source;
    comfort_node comfort;
    acoustics_node acoustics;
    sound_source_node sound_source;
    hearing_node hearing;
    occupant_planning_node occupant_planning;
    occupant_steering_node occupant_steering;
};


building_dynamics_node::building_dynamics_node(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , outdoor_mean_temperature_input("outdoor_mean_temperature_input", external_interface())
    , outdoor_temperature_period_input("outdoor_temperature_period_input", external_interface())
    , outdoor_temperature_time_step_input("outdoor_temperature_time_step_input", external_interface())
    , occupant_time_constant_input("occupant_time_constant_input", external_interface())
    , high_temperature_input("high_temperature_input", external_interface())
    , initial_temperature_input("initial_temperature_input", external_interface())
    , initial_temperature_rate_input("initial_temperature_rate_input", external_interface())
    , building_layout_input("building_layout_input", external_interface())
    , wall_resistance_input("wall_resistance_input", external_interface())
    , wall_sound_absorption_input("wall_sound_absorption_input", external_interface())
    , floor_sound_absorption_input("floor_sound_absorption_input", external_interface())
    , ceiling_sound_absorption_input("ceiling_sound_absorption_input", external_interface())
    , initial_positions_input("initial_positions_input", external_interface())
    , walking_speed_input("walking_speed_input", external_interface())
    , walking_sound_input("walking_sound_input", external_interface())
    , temperature_field_output("temperature_field_output", external_interface())
    , sound_field_output("sound_field_output", external_interface())
    , occupant_position_output("occupant_position_output", external_interface())
    , weather("weather", internal_context())
    , thermodynamics("thermodynamics", internal_context())
    , heat_source("heat_source", internal_context())
    , comfort("comfort", internal_context())
    , acoustics("acoustics", internal_context())
    , sound_source("sound_source", internal_context())
    , hearing("hearing", internal_context())
    , occupant_planning("occupant_planning", internal_context())
    , occupant_steering("occupant_steering", internal_context())
{
    // Flow Input Links
    inward_link(outdoor_mean_temperature_input, weather.outdoor_mean_temperature_input);
    inward_link(outdoor_temperature_period_input, weather.outdoor_temperature_period_input);
    inward_link(outdoor_temperature_time_step_input, weather.outdoor_temperature_time_step_input);
    inward_link(occupant_time_constant_input, heat_source.occupant_time_constant_input);
    inward_link(high_temperature_input, occupant_planning.high_temperature_input);
    inward_link(initial_temperature_input, weather.initial_temperature_input);
    inward_link(initial_temperature_input, thermodynamics.initial_temperature_input);
    inward_link(initial_temperature_input, heat_source.initial_temperature_input);
    inward_link(initial_temperature_input, comfort.initial_temperature_input);
    inward_link(initial_temperature_input, occupant_planning.initial_temperature_input);
    inward_link(initial_temperature_rate_input, weather.initial_temperature_rate_input);
    inward_link(building_layout_input, thermodynamics.building_layout_input);
    inward_link(building_layout_input, acoustics.building_layout_input);
    inward_link(building_layout_input, occupant_planning.building_layout_input);
    inward_link(building_layout_input, occupant_steering.building_layout_input);
    inward_link(wall_resistance_input, thermodynamics.wall_resistance_input);
    inward_link(wall_sound_absorption_input, acoustics.wall_sound_absorption_input);
    inward_link(floor_sound_absorption_input, acoustics.floor_sound_absorption_input);
    inward_link(ceiling_sound_absorption_input, acoustics.ceiling_sound_absorption_input);
    inward_link(initial_positions_input, occupant_planning.initial_positions_input);
    inward_link(initial_positions_input, occupant_steering.initial_positions_input);
    inward_link(walking_speed_input, occupant_steering.walking_speed_input);
    inward_link(walking_sound_input, sound_source.walking_sound_input);

    // Message Links
    inner_link(weather.outdoor_temperature_output, thermodynamics.outdoor_temperature_input);
    inner_link(thermodynamics.temperature_field_output, heat_source.temperature_field_input);
    inner_link(thermodynamics.temperature_field_output, comfort.temperature_field_input);
    inner_link(heat_source.heat_source_output, thermodynamics.heat_source_input);
    inner_link(comfort.occupant_temperature_output, occupant_planning.occupant_temperature_input);
    inner_link(acoustics.sound_field_output, hearing.sound_field_input);
    inner_link(sound_source.sound_source_output, acoustics.sound_source_input);
    inner_link(occupant_planning.occupant_destination_output, occupant_steering.occupant_destination_input);
    inner_link(occupant_steering.occupant_position_output, heat_source.occupant_position_input);
    inner_link(occupant_steering.occupant_position_output, comfort.occupant_position_input);
    inner_link(occupant_steering.occupant_position_output, sound_source.occupant_position_input);
    inner_link(occupant_steering.occupant_position_output, hearing.occupant_position_input);
    inner_link(occupant_steering.occupant_position_output, occupant_planning.occupant_position_input);
    outward_link(thermodynamics.temperature_field_output, temperature_field_output);
    outward_link(acoustics.sound_field_output, sound_field_output);
    outward_link(occupant_steering.occupant_position_output, occupant_position_output);

    // Finalization Links
}


}  // namespace

#endif
