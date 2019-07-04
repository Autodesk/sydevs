#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_H_

#include <examples/research/building7m_advanced/building_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void building7m()
{
    try {
        auto total_dt = 15_s;  // 5_min

        timer sim_timer;
        sim_timer.start();
        simulation<building_closed_system> sim(total_dt, 1, std::cout);
        sim.top.frame_duration.set_value(200_ms);
        sim.process_remaining_events();
        sim_timer.stop();

        std::cout << std::endl;
        std::cout << "Timing Statistics" << std::endl;
        std::cout << "(overall simulation):                        " << sim_timer.cumulative_duration() << std::endl;
        std::cout << "sim:                                         " << sim.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_info:                       " << sim.top.building_info.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.initial_positions:                   " << sim.top.initial_positions.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_vis:                        " << sim.top.building_vis.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.weather:           " << sim.top.building_dynamics.weather.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.thermodynamics:    " << sim.top.building_dynamics.thermodynamics.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.heat_source:       " << sim.top.building_dynamics.heat_source.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.comfort:           " << sim.top.building_dynamics.comfort.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.acoustics:         " << sim.top.building_dynamics.acoustics.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.sound_source:      " << sim.top.building_dynamics.sound_source.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.hearing:           " << sim.top.building_dynamics.hearing.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.occupant_planning: " << sim.top.building_dynamics.occupant_planning.event_timer().cumulative_duration() << std::endl;
        std::cout << "sim.top.building_dynamics.occupant_steering: " << sim.top.building_dynamics.occupant_steering.event_timer().cumulative_duration() << std::endl;
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


}  // namespace

#endif
