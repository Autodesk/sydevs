#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_CLOSED_SYSTEM_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_CLOSED_SYSTEM_H_

#include <examples/research/building7m_advanced/building_info_node.h>
#include <examples/research/building7m_advanced/initial_positions_node.h>
#include <examples/research/building7m_advanced/building_dynamics_node.h>
#include <examples/research/building7m_advanced/building_vis_node.h>
#include <sydevs/systems/composite_node.h>
#include <sydevs/systems/parameter_node.h>
#include <sydevs/systems/statistic_node.h>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


class building_closed_system : public composite_node
{
public:
    // Constructor/Destructor:
    building_closed_system(const std::string& node_name, const node_context& external_context);
    virtual ~building_closed_system() = default;

    // Components:
    parameter_node<duration> frame_duration;
    parameter_node<thermodynamic_temperature> outdoor_mean_temperature;
    parameter_node<duration> outdoor_temperature_period;
    parameter_node<duration> outdoor_temperature_time_step;
    parameter_node<duration> occupant_time_constant;
    parameter_node<thermodynamic_temperature> high_temperature;
    parameter_node<thermodynamic_temperature> initial_temperature;
    parameter_node<quantity<decltype(_K/_s)>> initial_temperature_rate;
    parameter_node<quantity<decltype(_m/_s)>> walking_speed;
    parameter_node<quantity<decltype(_g/_m/_s/_s)>> walking_sound;
    parameter_node<int64> occupant_count;
    building_info_node building_info;
    initial_positions_node initial_positions;
    building_dynamics_node building_dynamics;
    building_vis_node building_vis;
};


building_closed_system::building_closed_system(const std::string& node_name, const node_context& external_context)
    : composite_node(node_name, external_context)
    , frame_duration("frame_duration", internal_context(), 30_s)
    , outdoor_mean_temperature("outdoor_mean_temperature", internal_context(), 273150_mK + 20_K)
    , outdoor_temperature_period("outdoor_temperature_period", internal_context(), 7_min)
    , outdoor_temperature_time_step("outdoor_temperature_time_step", internal_context(), 15_s)
    , occupant_time_constant("occupant_time_constant", internal_context(), 5_s)
    , high_temperature("high_temperature", internal_context(), 273150_mK + 25_K)
    , initial_temperature("initial_temperature", internal_context(), 273150_mK + 20_K)
    , initial_temperature_rate("initial_temperature_rate", internal_context(), 80_mK/_s)
    , walking_speed("walking_speed", internal_context(), 1400_mm/_s)
    , walking_sound("walking_sound", internal_context(), 2_g/_m/_s/_s)
    , occupant_count("occupant_count", internal_context(), 5)
    , building_info("building_info", internal_context())
    , initial_positions("initial_positions", internal_context())
    , building_dynamics("building_dynamics", internal_context())
    , building_vis("building_vis", internal_context())
{
    // Initialization Links
    inner_link(frame_duration.parameter, building_vis.frame_duration_input);
    inner_link(outdoor_mean_temperature.parameter, building_dynamics.outdoor_mean_temperature_input);
    inner_link(outdoor_temperature_period.parameter, building_dynamics.outdoor_temperature_period_input);
    inner_link(outdoor_temperature_time_step.parameter, building_dynamics.outdoor_temperature_time_step_input);
    inner_link(occupant_time_constant.parameter, building_dynamics.occupant_time_constant_input);
    inner_link(high_temperature.parameter, building_dynamics.high_temperature_input);
    inner_link(initial_temperature.parameter, building_dynamics.initial_temperature_input);
    inner_link(initial_temperature_rate.parameter, building_dynamics.initial_temperature_rate_input);
    inner_link(walking_speed.parameter, building_dynamics.walking_speed_input);
    inner_link(walking_sound.parameter, building_dynamics.walking_sound_input);
    inner_link(occupant_count.parameter, initial_positions.occupant_count_input);
    inner_link(building_info.building_layout_output, building_dynamics.building_layout_input);
    inner_link(building_info.building_layout_output, initial_positions.building_layout_input);
    inner_link(building_info.wall_resistance_output, building_dynamics.wall_resistance_input);
    inner_link(building_info.wall_sound_absorption_output, building_dynamics.wall_sound_absorption_input);
    inner_link(building_info.floor_sound_absorption_output, building_dynamics.floor_sound_absorption_input);
    inner_link(building_info.ceiling_sound_absorption_output, building_dynamics.ceiling_sound_absorption_input);
    inner_link(initial_positions.initial_positions_output, building_dynamics.initial_positions_input);
    inner_link(building_info.building_layout_output, building_vis.building_layout_input);

    // Simulation Links
    inner_link(building_dynamics.temperature_field_output, building_vis.temperature_field_input);
    inner_link(building_dynamics.sound_field_output, building_vis.sound_field_input);
    inner_link(building_dynamics.occupant_position_output, building_vis.occupant_position_input);

    // Finalization Links
}


}  // namespace

#endif
