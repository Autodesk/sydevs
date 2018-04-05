#pragma once
#ifndef CUSTOMS_H_
#define CUSTOMS_H_

#include <example/research/customs/customs_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>


void customs()
{
    try {
        sydevs::systems::simulation<customs_node> sim(12_hr, 0, std::cout);

        sim.top.source.item_output.print_on_use();
        sim.top.kiosk_queue.item_output.print_on_use();
        sim.top.kiosk_array.prototype.item_output.print_on_use();
        sim.top.central_queue.prototype.item_output.print_on_use();
        sim.top.booths.prototype.queue.item_output.print_on_use();
        sim.top.booths.prototype.server.item_output.print_on_use();
        sim.top.sink.item_output.print_on_use();

        sim.top.booths.lane_creation_output.print_on_use();
        sim.top.booths.lane_removal_output.print_on_use();

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
