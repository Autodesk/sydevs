#pragma once
#ifndef SYDEVS_EXAMPLES_WORKERS_H_
#define SYDEVS_EXAMPLES_WORKERS_H_

#include <examples/demo/workers/workplace_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>
#include <fstream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void workers()
{
    bool output_to_file = false;

    std::shared_ptr<std::fstream> os_ptr;
    if (output_to_file) {
        os_ptr = std::make_shared<std::fstream>();
        os_ptr->open("workers.csv", std::fstream::out);
    }

    try {
        simulation<workplace_closed_system> sim(7_hr, 0, std::cout);
        sim.top.worker_count.set_value(12);
        sim.top.arrival_dt.set_value(5_min);
        sim.top.work_dt.set_value(1_hr);
        sim.top.break_dt.set_value(10_min);
        sim.top.frame_dt.set_value(5_s);
        sim.top.ostream_ptr.set_value(os_ptr);
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

    if (output_to_file) {
        os_ptr->close();
    }
}


}  // namespace

#endif
