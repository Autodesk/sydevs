#pragma once
#ifndef SYDEVS_EXAMPLES_WORKERS_H_
#define SYDEVS_EXAMPLES_WORKERS_H_

#include <examples/demo/workers/workplace_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void workers()
{
    try {
        simulation<workplace_closed_system> sim(24_hr, 0, std::cout);
        sim.top.worker_count.set_value(12);
        sim.top.arrival_dt.set_value(5_min);
        sim.top.work_dt.set_value(1_hr);
        sim.top.break_dt.set_value(10_min);
        sim.top.workplace.arrival_input.print_on_use();
        sim.top.workplace.change_output.print_on_use();
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
