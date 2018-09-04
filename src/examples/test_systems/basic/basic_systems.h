#pragma once
#ifndef SYDEVS_EXAMPLES_BASIC_SYSTEMS_H_
#define SYDEVS_EXAMPLES_BASIC_SYSTEMS_H_

#include <examples/test_systems/basic/basic_generator_node.h>
#include <examples/test_systems/basic/basic_processor_node.h>
#include <examples/test_systems/basic/basic_double_processor_node.h>
#include <examples/test_systems/basic/basic_generator_double_processor_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void basic_systems()
{
    std::cout << "basic_generator_double_processor_node" << std::endl;
    simulation<basic_generator_double_processor_node> sim(10_s, 0, std::cout);
    sim.top.print_on_event();
    sim.top.mean_dt_parameter.print_on_event();
    sim.top.mean_dt_parameter.parameter.print_on_use();
    sim.top.response_dt_A_parameter.print_on_event();
    sim.top.response_dt_A_parameter.parameter.print_on_use();
    sim.top.response_dt_B_parameter.print_on_event();
    sim.top.response_dt_B_parameter.parameter.print_on_use();
    sim.top.generator.print_on_event();
    sim.top.generator.print_on_elapsed_duration();
    sim.top.generator.print_on_planned_duration();
    sim.top.generator.mean_dt_input.print_on_use();
    sim.top.generator.count_output.print_on_use();
    sim.top.processor.print_on_event();
    sim.top.processor.response_dt_A_input.print_on_use();
    sim.top.processor.response_dt_B_input.print_on_use();
    sim.top.processor.job_id_input.print_on_use();
    sim.top.processor.job_id_output.print_on_use();
    sim.top.processor.miss_count_output.print_on_use();
    sim.top.processor.processor_A.print_on_event();
    sim.top.processor.processor_A.print_on_elapsed_duration();
    sim.top.processor.processor_A.print_on_planned_duration();
    sim.top.processor.processor_A.response_dt_input.print_on_use();
    sim.top.processor.processor_A.job_id_input.print_on_use();
    sim.top.processor.processor_A.job_id_output.print_on_use();
    sim.top.processor.processor_A.miss_count_output.print_on_use();
    sim.top.processor.processor_B.print_on_event();
    sim.top.processor.processor_B.print_on_elapsed_duration();
    sim.top.processor.processor_B.print_on_planned_duration();
    sim.top.processor.processor_B.response_dt_input.print_on_use();
    sim.top.processor.processor_B.job_id_input.print_on_use();
    sim.top.processor.processor_B.job_id_output.print_on_use();
    sim.top.processor.processor_B.miss_count_output.print_on_use();
    sim.top.processor.adder.print_on_event();
    sim.top.processor.adder.miss_count_A_input.print_on_use();
    sim.top.processor.adder.miss_count_B_input.print_on_use();
    sim.top.processor.adder.miss_count_output.print_on_use();
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
