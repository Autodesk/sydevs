#pragma once
#ifndef CELLULAR_SYSTEMS_H_
#define CELLULAR_SYSTEMS_H_

#include <example/test_systems/cellular/upper_cellular_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

void cellular_systems()
{
    try {
        sydevs::systems::simulation<upper_cellular_node> sim(1_s, 0, std::cout);
        sim.top.prototype.print_on_event();
        sim.top.prototype.coords_input.print_on_use();
        sim.top.prototype.cellspace.print_on_use();
        sim.top.prototype.prototype.print_on_event();
        sim.top.prototype.prototype.coords_input.print_on_use();
        sim.top.prototype.prototype.flow_output.print_on_use();
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
