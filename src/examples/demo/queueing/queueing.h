#pragma once
#ifndef SYDEVS_EXAMPLES_QUEUEING_H_
#define SYDEVS_EXAMPLES_QUEUEING_H_

#include <examples/demo/queueing/queueing_closed_system.h>
#include <examples/demo/queueing/two_stage_queueing_closed_system.h>
#include <examples/demo/queueing/parallel_queueing_closed_system.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void queueing()
{
    // Perform a simulation using the "queueing" atomic node.
    try {
        std::cout << "Testing queueing_node" << std::endl;
        simulation<queueing_closed_system> sim(2_min, 0, std::cout);
        sim.top.job_gen_dt.set_value(5_s);
        sim.top.serv_dt.set_value(14_s);
        sim.top.queue.job_id_input.print_on_use();
        sim.top.queue.job_id_output.print_on_use();
        sim.process_remaining_events();
        std::cout << "Idle time: " << sim.top.idle_dt.value().rescaled(unit) << std::endl;
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    // Perform a simulation using the "two-stage queueing" composite node.
    try {
        std::cout << "Testing two_stage_queueing_node" << std::endl;
        simulation<two_stage_queueing_closed_system> sim(2_min, 0, std::cout);
        sim.top.job_gen_dt.set_value(5_s);
        sim.top.serv_dt.set_value(14_s);
        sim.top.queue.job_id_input.print_on_use();
        sim.top.queue.queue_A.job_id_output.print_on_use();
        sim.top.queue.queue_B.job_id_output.print_on_use();
        sim.process_remaining_events();
        std::cout << "Idle time: " << sim.top.idle_dt.value().rescaled(unit) << std::endl;
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    // Perform a simulation using the "parallel queueing" collection node.
    try {
        std::cout << "Testing parallel_queueing_node" << std::endl;
        simulation<parallel_queueing_closed_system> sim(2_min, 0, std::cout);
        sim.top.job_gen_dt.set_value(5_s);
        sim.top.serv_dt.set_value(14_s);
        sim.top.max_n.set_value(3);
        sim.top.queue.prototype.print_on_event();
        sim.top.queue.prototype.job_id_input.print_on_use();
        sim.top.queue.prototype.job_id_output.print_on_use();
        sim.process_remaining_events();
        std::cout << "Idle time: " << sim.top.idle_dt.value().rescaled(unit) << std::endl;
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
