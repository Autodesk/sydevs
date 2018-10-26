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
    thermodynamic_temperature average_T = thermodynamic_temperature();
    try {
        simulation<building_closed_system> sim(5_min, 0, std::cout);
        sim.top.frame_duration.set_value(5_s);
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
