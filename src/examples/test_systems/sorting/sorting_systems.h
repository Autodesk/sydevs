#pragma once
#ifndef SYDEVS_EXAMPLES_SORTING_SYSTEMS_H_
#define SYDEVS_EXAMPLES_SORTING_SYSTEMS_H_

#include <examples/test_systems/sorting/sorting_2_node.h>
#include <examples/test_systems/sorting/sorting_3_node.h>
#include <examples/test_systems/sorting/sorting_4_node.h>
#include <examples/test_systems/sorting/sorting_4_test_node.h>
#include <sydevs/systems/simulation.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


void sorting_systems()
{
    std::cout << "sorting_4_test_node" << std::endl;
    try {
        simulation<sorting_4_test_node> sim(0_s, 0, std::cout);
        sim.top.a_parameter.parameter.print_on_use();
        sim.top.b_parameter.parameter.print_on_use();
        sim.top.c_parameter.parameter.print_on_use();
        sim.top.d_parameter.parameter.print_on_use();
        sim.top.sorter.a_output.print_on_use();
        sim.top.sorter.b_output.print_on_use();
        sim.top.sorter.c_output.print_on_use();
        sim.top.sorter.d_output.print_on_use();
        sim.top.sorter.A.a_output.print_on_use();
        sim.top.sorter.A.b_output.print_on_use();
        sim.top.sorter.A.c_output.print_on_use();
        sim.top.sorter.A.A.a_output.print_on_use();
        sim.top.sorter.A.A.b_output.print_on_use();
        sim.top.sorter.A.B.a_output.print_on_use();
        sim.top.sorter.A.B.b_output.print_on_use();
        sim.top.sorter.A.C.a_output.print_on_use();
        sim.top.sorter.A.C.b_output.print_on_use();
        sim.top.sorter.B.a_output.print_on_use();
        sim.top.sorter.B.b_output.print_on_use();
        sim.top.sorter.C.a_output.print_on_use();
        sim.top.sorter.C.b_output.print_on_use();
        sim.top.sorter.D.a_output.print_on_use();
        sim.top.sorter.D.b_output.print_on_use();
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
