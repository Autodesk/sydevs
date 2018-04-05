#pragma once
#ifndef BUILDING7M_H_
#define BUILDING7M_H_

#include <example/demo/building7m/building_simulation_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>


void building7m()
{
    std::cout << "building_simulation_node" << std::endl;
    thermodynamic_temperature average_T = thermodynamic_temperature();
    try {
        sydevs::systems::simulation<building_simulation_node> sim(5_min, 0, std::cout);
        sim.top.initial_temperature.parameter.print_on_use();
        sim.top.building_dynamics.weather.outdoor_temperature_output.print_on_use();
        sim.process_remaining_events();
    }
    catch (const sydevs::systems::system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


#endif
