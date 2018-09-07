#pragma once
#ifndef SYDEVS_EXAMPLES_CELLULAR_SYSTEMS_H_
#define SYDEVS_EXAMPLES_CELLULAR_SYSTEMS_H_

#include <examples/test_systems/cellular/upper_cellular_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void cellular_systems()
{
    try {
        simulation<upper_cellular_node> sim(1_s, 0, std::cout);
        sim.top.prototype.print_on_event();
        sim.top.prototype.coords_input.print_on_use();
        sim.top.prototype.cellspace.print_on_use();
        sim.top.prototype.prototype.print_on_event();
        sim.top.prototype.prototype.coords_input.print_on_use();
        sim.top.prototype.prototype.flow_output.print_on_use();
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
