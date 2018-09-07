#pragma once
#ifndef SYDEVS_EXAMPLES_BUILDING7M_H_
#define SYDEVS_EXAMPLES_BUILDING7M_H_

#include <examples/demo/building7m/building_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void building7m()
{
    std::cout << "building_closed_system" << std::endl;
    thermodynamic_temperature average_T = thermodynamic_temperature();
    try {
        simulation<building_closed_system> sim(5_min, 0, std::cout);
        sim.top.initial_temperature.parameter.print_on_use();
        sim.top.building_dynamics.weather.outdoor_temperature_output.print_on_use();
        sim.process_remaining_events();
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
