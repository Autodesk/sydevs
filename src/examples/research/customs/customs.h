#pragma once
#ifndef SYDEVS_EXAMPLES_CUSTOMS_H_
#define SYDEVS_EXAMPLES_CUSTOMS_H_

#include <examples/research/customs/customs_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void customs()
{
    simulation<customs_closed_system> sim(12_hr, 0, std::cout);

    sim.top.source.item_output.print_on_use();
    sim.top.kiosk_queue.item_output.print_on_use();
    sim.top.kiosk_array.prototype.item_output.print_on_use();
    sim.top.central_queue.prototype.item_output.print_on_use();
    sim.top.booths.prototype.queue.item_output.print_on_use();
    sim.top.booths.prototype.server.item_output.print_on_use();
    sim.top.sink.item_output.print_on_use();

    sim.top.booths.lane_creation_output.print_on_use();
    sim.top.booths.lane_removal_output.print_on_use();

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
