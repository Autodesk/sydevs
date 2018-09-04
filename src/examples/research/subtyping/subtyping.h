#pragma once
#ifndef SYDEVS_RESEARCH_SUBTYPING_H_
#define SYDEVS_RESEARCH_SUBTYPING_H_

#include <examples/research/subtyping/subtyping_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void subtyping()
{
    simulation<subtyping_closed_system> sim(30_s, 0, std::cout);
    sim.top.prototype.y0_input.print_on_use();
    sim.top.prototype.x_input.print_on_use();
    sim.top.prototype.y_output.print_on_use();
    sim.top.prototype.y_final_output.print_on_use();
    try {
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
